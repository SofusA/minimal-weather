#include <pebble.h>
#include <ctype.h>
#include "weather.h"

static Window *s_window;

static TextLayer   *time_layer;
static TextLayer   *date_temp_layer;
static Layer       *battery_layer;
static Layer       *weekday_layer;  
static Layer       *frame_layer;   
static BitmapLayer *icon_layer;
static BitmapLayer *uv_icon_layer;     
static GBitmap     *uv_icon_bitmap;    
static TextLayer   *precip_layer;      

static char time_buffer[6];         
static char date_temp_buffer[24];   
static char latest_temp[8] = "---"; 
static int32_t s_last_uv = -1;         
static int32_t s_last_precip = -1;     

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

// ---------- Weekday ----------
static void weekday_update_proc(Layer *layer, GContext *ctx) {
  time_t now = time(NULL);
  struct tm *t = localtime(&now);

  int wday_pebble = t->tm_wday;
  int wday_mon_first = (wday_pebble == 0) ? 6 : (wday_pebble - 1);

  const char *letters[7] = {"M", "T", "W", "T", "F", "S", "S"};

  GRect bounds = layer_get_bounds(layer);

  GFont font = fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD);
  const int gap_px = 6;          
  const int underline_thickness = 3; 
  const int underline_inset = 1; 
  const int underline_offset = 2; 

  GSize glyph_sizes[7];
  int total_width = 0;
  for (int i = 0; i < 7; ++i) {
    glyph_sizes[i] = graphics_text_layout_get_content_size(
        letters[i],
        font,
        GRect(0, 0, bounds.size.w, bounds.size.h),
        GTextOverflowModeTrailingEllipsis,
        GTextAlignmentLeft);
    total_width += glyph_sizes[i].w;
    if (i < 6) total_width += gap_px;
  }

  // Center the entire strip horizontally
  int start_x = bounds.origin.x + (bounds.size.w - total_width) / 2;
  int center_y = bounds.origin.y + bounds.size.h / 2;

  graphics_context_set_text_color(ctx, GColorWhite);
  graphics_context_set_fill_color(ctx, GColorWhite);
  graphics_context_set_stroke_color(ctx, GColorWhite);

  // Position and draw each letter
  int x = start_x;
  for (int i = 0; i < 7; ++i) {
    GSize sz = glyph_sizes[i];

    // Vertically center the letter's frame
    int text_y = center_y - sz.h / 2;
    GRect text_frame = GRect(x, text_y, sz.w, sz.h);

    // Draw the letter
    graphics_draw_text(ctx,
                       letters[i],
                       font,
                       text_frame,
                       GTextOverflowModeWordWrap,
                       GTextAlignmentLeft,
                       NULL);

    // Underline the current day: draw a filled rectangle (thicker and more visible)
    if (i == wday_mon_first) {
      int ul_x = x + underline_inset;
      int ul_w = sz.w - 2 * underline_inset;
      if (ul_w < 3) ul_w = sz.w; // safety: don't collapse on very narrow glyphs

      int ul_y = text_frame.origin.y + text_frame.size.h + underline_offset;

      // Clamp underline within layer vertically
      graphics_fill_rect(ctx, GRect(ul_x, ul_y, ul_w, underline_thickness), 0, GCornerNone);
    }

    // Advance X with tight gap
    x += sz.w + gap_px;
  }
}
 
// ---------- Battery ----------

static void battery_update_proc(Layer *layer, GContext *ctx) {
  BatteryChargeState state = battery_state_service_peek();
  if (state.charge_percent >= 25) return;
  graphics_context_set_stroke_color(ctx, GColorWhite);
  graphics_draw_rect(ctx, GRect(0, 0, 14, 8));
  graphics_draw_rect(ctx, GRect(14, 2, 2, 4));
}

// ---------- Time & Combined Date/Temp ----------

static void update_display(void) {
  time_t now = time(NULL);
  struct tm *t = localtime(&now);

  strftime(time_buffer, sizeof(time_buffer), "%H:%M", t);
  text_layer_set_text(time_layer, time_buffer);

  char mon[4] = "";
  strftime(mon, sizeof(mon), "%b", t);
  for (size_t i = 0; mon[i] != '\0'; ++i) {
    mon[i] = (char)tolower((unsigned char)mon[i]);
  }

  // Build "15 feb"
  char date_part[16];
  snprintf(date_part, sizeof(date_part), "%d %s", t->tm_mday, mon);

  // Combine: "15 feb, 5°"  (or "15 feb, ---" until weather updates)
  snprintf(date_temp_buffer, sizeof(date_temp_buffer), "%s | %s", date_part, latest_temp);
  text_layer_set_text(date_temp_layer, date_temp_buffer);
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  update_display();
}

static void weather_on_temp(const char *temp_text) {
  snprintf(latest_temp, sizeof(latest_temp), "%s", temp_text ? temp_text : "---");
  update_display();
}

// Lay out the top row: [main icon] [optional UV icon] [optional "mm" column]
static void layout_top_row(void) {
  if (!s_window) return;

  Layer *root = window_get_root_layer(s_window);
  GRect bounds = layer_get_bounds(root);

  const int ICON_W = 32, ICON_H = 32;
  const int MARGIN_TOP = 20;
  const int GAP = 6;

  bool show_uv = (s_last_uv > 3);
  bool show_precip = (s_last_precip > 1);

  // Precip column will be same width as an icon for simple centering
  const int precip_w = show_precip ? ICON_W : 0;

  int total_w = ICON_W; // always show main icon
  if (show_uv)     total_w += GAP + ICON_W;
  if (show_precip) total_w += GAP + precip_w;

  int x = (bounds.size.w - total_w) / 2;
  int y = MARGIN_TOP;

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

  frame_layer = layer_create(bounds);
  layer_set_update_proc(frame_layer, frame_update_proc);
  layer_add_child(root, frame_layer);  

  const int ICON_W = 32, ICON_H = 32;
  const int MARGIN_TOP = 20;

  const int icon_x = (bounds.size.w - ICON_W) / 2;
  const int icon_y = MARGIN_TOP;
  icon_layer = bitmap_layer_create(GRect(icon_x, icon_y, ICON_W, ICON_H));
  bitmap_layer_set_background_color(icon_layer, GColorClear);
  bitmap_layer_set_compositing_mode(icon_layer, GCompOpSet);

  uv_icon_layer = bitmap_layer_create(GRect(0, 0, ICON_W, ICON_H));
  bitmap_layer_set_background_color(uv_icon_layer, GColorClear);
  bitmap_layer_set_compositing_mode(uv_icon_layer, GCompOpSet);
  layer_set_hidden(bitmap_layer_get_layer(uv_icon_layer), true);

  const int time_y = icon_y + ICON_H;
  time_layer = text_layer_create(GRect(0, time_y, bounds.size.w, 46));
  text_layer_set_background_color(time_layer, GColorClear);
  text_layer_set_text_color(time_layer, GColorWhite);
  text_layer_set_font(time_layer, fonts_get_system_font(FONT_KEY_BITHAM_42_BOLD));
  text_layer_set_text_alignment(time_layer, GTextAlignmentCenter);

  const int row3_y = time_y + 40;
  date_temp_layer = text_layer_create(GRect(0, row3_y, bounds.size.w, 28));
  text_layer_set_background_color(date_temp_layer, GColorClear);
  text_layer_set_text_color(date_temp_layer, GColorWhite);
  text_layer_set_font(date_temp_layer, fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD));
  text_layer_set_text_alignment(date_temp_layer, GTextAlignmentCenter);
  text_layer_set_text(date_temp_layer, ""); 

  precip_layer = text_layer_create(GRect(0, 0, ICON_W, ICON_H));
  text_layer_set_background_color(precip_layer, GColorClear);
  text_layer_set_text_color(precip_layer, GColorWhite);
  text_layer_set_font(precip_layer, fonts_get_system_font(FONT_KEY_GOTHIC_14_BOLD));
  text_layer_set_text_alignment(precip_layer, GTextAlignmentCenter);
  text_layer_set_text(precip_layer, "");
  layer_set_hidden(text_layer_get_layer(precip_layer), true);

  const int weekday_y = row3_y + 35;
  weekday_layer = layer_create(GRect(0, weekday_y, bounds.size.w, 25));
  layer_set_update_proc(weekday_layer, weekday_update_proc);

  battery_layer = layer_create(GRect(bounds.size.w - 20, 6, 18, 10));
  layer_set_update_proc(battery_layer, battery_update_proc);

  layer_add_child(root, bitmap_layer_get_layer(icon_layer));
  layer_add_child(root, bitmap_layer_get_layer(uv_icon_layer));   
  layer_add_child(root, text_layer_get_layer(precip_layer));      

  layer_add_child(root, text_layer_get_layer(time_layer));
  layer_add_child(root, text_layer_get_layer(date_temp_layer));
  layer_add_child(root, weekday_layer);
  layer_add_child(root, battery_layer);

  weather_init(icon_layer, weather_on_temp, weather_on_uv, weather_on_precip);

  update_display();
  layout_top_row(); 
}

static void window_unload(Window *window) {
  text_layer_destroy(time_layer);
  text_layer_destroy(date_temp_layer);
 
  layer_destroy(frame_layer);
  layer_destroy(weekday_layer);
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

static void init(void) {
  s_window = window_create();
  window_set_window_handlers(s_window, (WindowHandlers){
    .load = window_load,
    .unload = window_unload
  });
  window_stack_push(s_window, true);

  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);

  app_message_register_inbox_received(inbox_received_handler);
  app_message_open(64, 64);
}

static void deinit(void) {
  window_destroy(s_window);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}
