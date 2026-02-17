#include <pebble.h>
#include <ctype.h>
#include "weather.h"

static Window *s_window;

static TextLayer   *time_layer;
static TextLayer   *date_layer;         
static TextLayer   *weather_row_layer;  
static Layer       *battery_layer;
static Layer       *frame_layer;
static BitmapLayer *icon_layer;
static BitmapLayer *uv_icon_layer;
static GBitmap     *uv_icon_bitmap;
static TextLayer   *precip_layer;

static char time_buffer[6];
static char date_buffer[20];           
static char latest_temp[8] = "---";    
static char s_sun_next[8];
static char s_weather_row[40];         

static int32_t s_last_uv = -1;
static int32_t s_last_precip = -1;

static bool s_is_obstructed = false;

#define TOP_MARGIN_NORMAL        20
#define TOP_MARGIN_OBSTRUCTED    5

#define TIME_NUDGE_OBSTRUCTED    5

#define GAP_TIME_TO_DATE_NORMAL  40
#define GAP_TIME_TO_DATE_OBS     43

// We hide the weather row entirely when obstructed
#define GAP_DATE_TO_WEATHER      34 

#define ICON_W 32
#define ICON_H 32

// ---------- Frame ----------
static void frame_update_proc(Layer *layer, GContext *ctx) {
  GRect bounds = layer_get_bounds(layer);

  GRect r1 = grect_inset(bounds, GEdgeInsets(1));
  GRect r2 = grect_inset(bounds, GEdgeInsets(2));
  GRect r3 = grect_inset(bounds, GEdgeInsets(3));

  graphics_context_set_stroke_color(ctx, GColorWhite);

  graphics_draw_round_rect(ctx, r1, 2);
  graphics_draw_round_rect(ctx, r2, 2);
  graphics_draw_round_rect(ctx, r3, 2);
}

// ---------- Battery ----------
static void battery_update_proc(Layer *layer, GContext *ctx) {
  BatteryChargeState state = battery_state_service_peek();
  if (state.charge_percent >= 25) return;
  graphics_context_set_stroke_color(ctx, GColorWhite);
  graphics_draw_rect(ctx, GRect(0, 0, 14, 8));
  graphics_draw_rect(ctx, GRect(14, 2, 2, 4));
}

// ---------- Weather Row (temp + sun) ----------
static void update_weather_row(void) {
  snprintf(s_weather_row, sizeof(s_weather_row),
         "%s | %s",
         latest_temp,
         s_sun_next);
  text_layer_set_text(weather_row_layer, s_weather_row);
}

static void weather_on_temp(const char *temp_text) {
  snprintf(latest_temp, sizeof(latest_temp), "%s", temp_text ? temp_text : "---");
  update_weather_row();
}

static void weather_on_sun_event(const char *t) {
  snprintf(s_sun_next, sizeof(s_sun_next), "%s", t ? t : "--:--");
  update_weather_row();
}

// ---------- Time & Date ----------
static void update_display(void) {
  time_t now = time(NULL);
  struct tm *t = localtime(&now);

  strftime(time_buffer, sizeof(time_buffer), "%H:%M", t);
  text_layer_set_text(time_layer, time_buffer);

  // Get 3-letter weekday and month, then lowercase both
  char wday[4] = "";
  char mon[4]  = "";
  strftime(wday, sizeof(wday), "%a", t); // e.g., "Sun"
  strftime(mon,  sizeof(mon),  "%b", t); // e.g., "Feb"

  for (size_t i = 0; wday[i] != '\0'; ++i) {
    wday[i] = (char)tolower((unsigned char)wday[i]);
  }
  for (size_t i = 0; mon[i] != '\0'; ++i) {
    mon[i] = (char)tolower((unsigned char)mon[i]);
  }

  // Build "sun, 15 feb"
  snprintf(date_buffer, sizeof(date_buffer), "%s %d %s", wday, t->tm_mday, mon);
  text_layer_set_text(date_layer, date_buffer);
}

// Lay out the top row: [main icon] [optional UV icon] [optional "mm" column]
static void layout_top_row(void) {
  if (!s_window) return;

  Layer *root = window_get_root_layer(s_window);
  GRect bounds = layer_get_unobstructed_bounds(root);
  
  const int GAP = 6;

  bool show_uv = (s_last_uv > 3);
  bool show_precip = (s_last_precip > 1);

  // Precip column will be same width as an icon for simple centering
  const int precip_w = show_precip ? ICON_W : 0;

  int total_w = ICON_W; // always show main icon
  if (show_uv)     total_w += GAP + ICON_W;
  if (show_precip) total_w += GAP + precip_w;

  // int x = bounds.origin.x + (bounds.size.w - total_w) / 2;
  int x = (bounds.size.w - total_w) / 2;
  int y = bounds.origin.y +
      (s_is_obstructed ? TOP_MARGIN_OBSTRUCTED : TOP_MARGIN_NORMAL);

  layer_set_frame(bitmap_layer_get_layer(icon_layer), GRect(x, y, ICON_W, ICON_H));
  x += ICON_W;

  // UV icon
  if (show_uv) {
    x += GAP;
    if (!uv_icon_bitmap) {
      uv_icon_bitmap = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_SUNNY_DARK);
      bitmap_layer_set_bitmap(uv_icon_layer, uv_icon_bitmap);
      bitmap_layer_set_background_color(uv_icon_layer, GColorClear);
      bitmap_layer_set_compositing_mode(uv_icon_layer, GCompOpSet);
    }
    layer_set_hidden(bitmap_layer_get_layer(uv_icon_layer), false);
    layer_set_frame(bitmap_layer_get_layer(uv_icon_layer), GRect(x, y, ICON_W, ICON_H));
    x += ICON_W;
  } else {
    layer_set_hidden(bitmap_layer_get_layer(uv_icon_layer), true);
  }

  // Precip text (two lines: "<n>\nmm")
  if (show_precip) {
    x += GAP;
    layer_set_hidden(text_layer_get_layer(precip_layer), false);
    layer_set_frame(text_layer_get_layer(precip_layer), GRect(x, y, precip_w, ICON_H));
  } else {
    layer_set_hidden(text_layer_get_layer(precip_layer), true);
  }
}

static void relayout_all(void) {
  if (!s_window) return;

  Layer *root = window_get_root_layer(s_window);
  GRect full   = layer_get_bounds(root);
  GRect unob   = layer_get_unobstructed_bounds(root);

  // Are we obstructed? (e.g., Timeline Quick View showing)
  s_is_obstructed = (unob.size.h < full.size.h);

  layer_set_frame(frame_layer, unob);

  const int margin_top = s_is_obstructed ? TOP_MARGIN_OBSTRUCTED : TOP_MARGIN_NORMAL;

  int y = unob.origin.y + margin_top;

  // Time layer
  const int time_y = (y + ICON_H) - (s_is_obstructed ? TIME_NUDGE_OBSTRUCTED : 0);
  layer_set_frame(text_layer_get_layer(time_layer),
                  GRect(unob.origin.x, time_y, unob.size.w, 46));

  // Date layer
  const int gap_time_to_date = s_is_obstructed ? GAP_TIME_TO_DATE_OBS : GAP_TIME_TO_DATE_NORMAL;
  const int date_y = time_y + gap_time_to_date;
  layer_set_frame(text_layer_get_layer(date_layer),
                  GRect(unob.origin.x, date_y, unob.size.w, 28));

  // Weather row: hide when obstructed
  if (s_is_obstructed) {
    layer_set_hidden(text_layer_get_layer(weather_row_layer), true);
  } else {
    layer_set_hidden(text_layer_get_layer(weather_row_layer), false);
    const int weather_y = date_y + GAP_DATE_TO_WEATHER;
    layer_set_frame(text_layer_get_layer(weather_row_layer),
                    GRect(unob.origin.x, weather_y, unob.size.w, 24));
  }

  // Battery 
  layer_set_frame(battery_layer,
                  GRect(unob.origin.x + unob.size.w - 20, unob.origin.y + 6, 18, 10));

  layout_top_row();

  layer_mark_dirty(frame_layer);
  layer_mark_dirty(window_get_root_layer(s_window));
}

static void weather_on_uv(int32_t uv) {
  s_last_uv = uv;
  layout_top_row();
}

static void weather_on_precip(int32_t mm) {
  s_last_precip = mm;

  // Show number above "mm"
  static char s_precip_buf[16];
  snprintf(s_precip_buf, sizeof(s_precip_buf), "%ld\nmm", (long)mm);
  text_layer_set_text(precip_layer, s_precip_buf);

  layout_top_row();
}

// ---------- Window ----------
static void window_load(Window *window) {
  Layer *root = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(root);

  window_set_background_color(window, GColorBlack);

  // Frame layer starts as full window; relayout_all() will shrink it if needed
  frame_layer = layer_create(bounds);
  layer_set_update_proc(frame_layer, frame_update_proc);
  layer_add_child(root, frame_layer);

  icon_layer = bitmap_layer_create(GRect(0, 0, ICON_W, ICON_H));
  bitmap_layer_set_background_color(icon_layer, GColorClear);
  bitmap_layer_set_compositing_mode(icon_layer, GCompOpSet);

  uv_icon_layer = bitmap_layer_create(GRect(0, 0, ICON_W, ICON_H));
  bitmap_layer_set_background_color(uv_icon_layer, GColorClear);
  bitmap_layer_set_compositing_mode(uv_icon_layer, GCompOpSet);
  layer_set_hidden(bitmap_layer_get_layer(uv_icon_layer), true);

  time_layer = text_layer_create(GRect(0, 0, bounds.size.w, 46));
  text_layer_set_background_color(time_layer, GColorClear);
  text_layer_set_text_color(time_layer, GColorWhite);
  text_layer_set_font(time_layer, fonts_get_system_font(FONT_KEY_BITHAM_42_BOLD));
  text_layer_set_text_alignment(time_layer, GTextAlignmentCenter);

  date_layer = text_layer_create(GRect(0, 0, bounds.size.w, 28));
  text_layer_set_background_color(date_layer, GColorClear);
  text_layer_set_text_color(date_layer, GColorWhite);
  text_layer_set_font(date_layer, fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD));
  text_layer_set_text_alignment(date_layer, GTextAlignmentCenter);
  text_layer_set_text(date_layer, "");

  weather_row_layer = text_layer_create(GRect(0, 0, bounds.size.w, 24));
  text_layer_set_background_color(weather_row_layer, GColorClear);
  text_layer_set_text_color(weather_row_layer, GColorWhite);
  text_layer_set_font(weather_row_layer, fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD));
  text_layer_set_text_alignment(weather_row_layer, GTextAlignmentCenter);
  text_layer_set_text(weather_row_layer, "--- | --:--");

  precip_layer = text_layer_create(GRect(0, 0, ICON_W, ICON_H));
  text_layer_set_background_color(precip_layer, GColorClear);
  text_layer_set_text_color(precip_layer, GColorWhite);
  text_layer_set_font(precip_layer, fonts_get_system_font(FONT_KEY_GOTHIC_14_BOLD));
  text_layer_set_text_alignment(precip_layer, GTextAlignmentCenter);
  text_layer_set_text(precip_layer, "");
  layer_set_hidden(text_layer_get_layer(precip_layer), true);

  battery_layer = layer_create(GRect(0, 0, 18, 10)); // position set in relayout_all
  layer_set_update_proc(battery_layer, battery_update_proc);

  layer_add_child(root, bitmap_layer_get_layer(icon_layer));
  layer_add_child(root, bitmap_layer_get_layer(uv_icon_layer));
  layer_add_child(root, text_layer_get_layer(precip_layer));
  layer_add_child(root, text_layer_get_layer(time_layer));
  layer_add_child(root, text_layer_get_layer(date_layer));
  layer_add_child(root, text_layer_get_layer(weather_row_layer));
  layer_add_child(root, battery_layer);

  weather_init(
    icon_layer,
    weather_on_temp,
    weather_on_uv,
    weather_on_precip,
    weather_on_sun_event
  );

  update_display();
  update_weather_row();

  relayout_all();
}

static void window_unload(Window *window) {
  text_layer_destroy(time_layer);
  text_layer_destroy(date_layer);
  text_layer_destroy(weather_row_layer);

  layer_destroy(frame_layer);
  layer_destroy(battery_layer);

  if (uv_icon_bitmap) {
    gbitmap_destroy(uv_icon_bitmap);
    uv_icon_bitmap = NULL;
  }
  bitmap_layer_destroy(uv_icon_layer);
  text_layer_destroy(precip_layer);

  weather_deinit();
  bitmap_layer_destroy(icon_layer);
}

// ---------- Init ----------
static void send_weather_refresh(void) {
  DictionaryIterator *iter;
  AppMessageResult res = app_message_outbox_begin(&iter);
  if (res != APP_MSG_OK) {
    return;
  }

  dict_write_uint8(iter, MESSAGE_KEY_WEATHER_REFRESH, 1);
  dict_write_end(iter);
  app_message_outbox_send();
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  update_display();

  if (tick_time->tm_min % 15 == 0) {
    send_weather_refresh();
  }
}

static void bt_handler(bool connected) {
  if (connected) {
    send_weather_refresh();
  }
}

static void on_unobstructed_will_change(GRect final_unobstructed_screen_area, void *ctx) {
}

static void on_unobstructed_change(AnimationProgress progress, void *ctx) {
  relayout_all();
}

static void on_unobstructed_did_change(void *ctx) {
  relayout_all();
}

static void init(void) {
  s_window = window_create();
  window_set_window_handlers(s_window, (WindowHandlers){
    .load = window_load,
    .unload = window_unload
  });
  window_stack_push(s_window, true);

  UnobstructedAreaHandlers ua = {
    .will_change = on_unobstructed_will_change,
    .change      = on_unobstructed_change,
    .did_change  = on_unobstructed_did_change
  };
  unobstructed_area_service_subscribe(ua, NULL);

  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
  connection_service_subscribe((ConnectionHandlers){ .pebble_app_connection_handler = bt_handler });

  app_message_register_inbox_received(inbox_received_handler);
  app_message_open(256, 256);
}

static void deinit(void) {
  unobstructed_area_service_unsubscribe();
  window_destroy(s_window);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}
