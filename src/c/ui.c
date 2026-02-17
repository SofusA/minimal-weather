#include <pebble.h>
#include "ui.h"

// ---------- Tunables / constants ----------
#define TOP_MARGIN_NORMAL        20
#define TOP_MARGIN_OBSTRUCTED     5

#define TIME_NUDGE 5

#define GAP_TIME_TO_DATE_NORMAL  40
#define GAP_TIME_TO_DATE_OBS     43

// We hide the weather row entirely when obstructed
#define GAP_DATE_TO_WEATHER      34

#define ICON_W 40
#define ICON_H 40

// ---------- Internal state ----------
struct Ui {
  Window *window;

  Layer       *frame_layer;

  TextLayer   *time_layer;
  TextLayer   *date_layer;
  TextLayer   *weather_row_layer;

  BitmapLayer *icon_layer;    
  BitmapLayer *uv_icon_layer; 
  GBitmap     *uv_icon_bitmap;

  TextLayer   *precip_layer;  // Optional "mm" column (two-line)

  BitmapLayer *bt_icon_layer;
  GBitmap     *bt_icon_bitmap;

  BitmapLayer *batt_icon_layer;
  GBitmap     *batt_icon_bitmap;

  char weather_row_buf[40];
  char precip_buf[16];
};

// ---------- Frame (visual border) ----------
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

// Helper: root/unobstructed
static inline Layer* prv_root(Ui *ui) {
  return window_get_root_layer(ui->window);
}

static inline GRect prv_bounds(Ui *ui) {
  return layer_get_bounds(prv_root(ui));
}

static inline GRect prv_unob_bounds(Ui *ui) {
  return layer_get_unobstructed_bounds(prv_root(ui));
}

static void prv_layout_top_row(Ui *ui, int32_t last_uv, int32_t last_precip, bool is_obstructed) {
  Layer *root = prv_root(ui);
  GRect unob = prv_unob_bounds(ui);

  const int GAP = 6;

  bool show_uv = (last_uv > 3);
  bool show_precip = (last_precip > 1);

  // Precip column width matches an icon for simpler centering
  const int precip_w = show_precip ? ICON_W : 0;

  int total_w = ICON_W; // main icon always
  if (show_uv)     total_w += GAP + ICON_W;
  if (show_precip) total_w += GAP + precip_w;

  int x = (unob.size.w - total_w) / 2;
  int y = unob.origin.y + (is_obstructed ? TOP_MARGIN_OBSTRUCTED : TOP_MARGIN_NORMAL);

  // Main icon
  layer_set_frame(bitmap_layer_get_layer(ui->icon_layer), GRect(x, y, ICON_W, ICON_H));
  x += ICON_W;

  // UV icon
  if (show_uv) {
    x += GAP;
    if (!ui->uv_icon_bitmap) {
      ui->uv_icon_bitmap = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_CLEAR_SKY_DAY);
      bitmap_layer_set_bitmap(ui->uv_icon_layer, ui->uv_icon_bitmap);
      bitmap_layer_set_background_color(ui->uv_icon_layer, GColorClear);
      bitmap_layer_set_compositing_mode(ui->uv_icon_layer, GCompOpSet);
    }
    layer_set_hidden(bitmap_layer_get_layer(ui->uv_icon_layer), false);
    layer_set_frame(bitmap_layer_get_layer(ui->uv_icon_layer), GRect(x, y, ICON_W, ICON_H));
    x += ICON_W;
  } else {
    layer_set_hidden(bitmap_layer_get_layer(ui->uv_icon_layer), true);
  }

  // Precip text (two lines: "<n>\nmm")
  if (show_precip) {
    x += GAP;
    layer_set_hidden(text_layer_get_layer(ui->precip_layer), false);
    layer_set_frame(text_layer_get_layer(ui->precip_layer), GRect(x, y, precip_w, ICON_H));
  } else {
    layer_set_hidden(text_layer_get_layer(ui->precip_layer), true);
  }
}

static void prv_relayout_all(Ui *ui, bool is_obstructed, int32_t last_uv, int32_t last_precip) {
  GRect unob = prv_unob_bounds(ui);
  GRect full = prv_bounds(ui);

  layer_set_frame(ui->frame_layer, unob);

  const int margin_top = is_obstructed ? TOP_MARGIN_OBSTRUCTED : TOP_MARGIN_NORMAL;
  int y = unob.origin.y + margin_top;

  // Time layer
  const int time_y = y + ICON_H - TIME_NUDGE;
  layer_set_frame(text_layer_get_layer(ui->time_layer),
                  GRect(unob.origin.x, time_y, unob.size.w, 46));

  // Date layer
  const int gap_time_to_date = is_obstructed ? GAP_TIME_TO_DATE_OBS : GAP_TIME_TO_DATE_NORMAL;
  const int date_y = time_y + gap_time_to_date;
  layer_set_frame(text_layer_get_layer(ui->date_layer),
                  GRect(unob.origin.x, date_y, unob.size.w, 28));

  // Weather row
  if (is_obstructed) {
    layer_set_hidden(text_layer_get_layer(ui->weather_row_layer), true);
  } else {
    layer_set_hidden(text_layer_get_layer(ui->weather_row_layer), false);
    const int weather_y = date_y + GAP_DATE_TO_WEATHER;
    layer_set_frame(text_layer_get_layer(ui->weather_row_layer),
                    GRect(unob.origin.x, weather_y, unob.size.w, 24));
  }

  prv_layout_top_row(ui, last_uv, last_precip, is_obstructed);

  // Status icons (TL: BT, TR: battery)
  const int x_pad = 8;
  const int y_pad = 5;
  layer_set_frame(bitmap_layer_get_layer(ui->bt_icon_layer),
                  GRect(unob.origin.x + x_pad, unob.origin.y + y_pad, 24, 24));

  layer_set_frame(bitmap_layer_get_layer(ui->batt_icon_layer),
                  GRect(unob.origin.x + unob.size.w - x_pad - 24, unob.origin.y + y_pad, 24, 24));

  layer_mark_dirty(ui->frame_layer);
  layer_mark_dirty(prv_root(ui));
}

// ---------- Public API ----------
Ui* ui_create(Window *window) {
  Ui *ui = (Ui*)malloc(sizeof(Ui));
  memset(ui, 0, sizeof(*ui));
  ui->window = window;

  Layer *root = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(root);

  window_set_background_color(window, GColorBlack);

  // Frame
  ui->frame_layer = layer_create(bounds);
  layer_set_update_proc(ui->frame_layer, frame_update_proc);
  layer_add_child(root, ui->frame_layer);

  // Main icon
  ui->icon_layer = bitmap_layer_create(GRect(0, 0, ICON_W, ICON_H));
  bitmap_layer_set_background_color(ui->icon_layer, GColorClear);
  bitmap_layer_set_compositing_mode(ui->icon_layer, GCompOpSet);
  layer_add_child(root, bitmap_layer_get_layer(ui->icon_layer));

  // UV icon (hidden initially)
  ui->uv_icon_layer = bitmap_layer_create(GRect(0, 0, ICON_W, ICON_H));
  bitmap_layer_set_background_color(ui->uv_icon_layer, GColorClear);
  bitmap_layer_set_compositing_mode(ui->uv_icon_layer, GCompOpSet);
  layer_set_hidden(bitmap_layer_get_layer(ui->uv_icon_layer), true);
  layer_add_child(root, bitmap_layer_get_layer(ui->uv_icon_layer));

  // Precip (hidden initially)
  ui->precip_layer = text_layer_create(GRect(0, 0, ICON_W, ICON_H));
  text_layer_set_background_color(ui->precip_layer, GColorClear);
  text_layer_set_text_color(ui->precip_layer, GColorWhite);
  text_layer_set_font(ui->precip_layer, fonts_get_system_font(FONT_KEY_GOTHIC_14_BOLD));
  text_layer_set_text_alignment(ui->precip_layer, GTextAlignmentCenter);
  text_layer_set_text(ui->precip_layer, "");
  layer_set_hidden(text_layer_get_layer(ui->precip_layer), true);
  layer_add_child(root, text_layer_get_layer(ui->precip_layer));

  // Time
  ui->time_layer = text_layer_create(GRect(0, 0, bounds.size.w, 46));
  text_layer_set_background_color(ui->time_layer, GColorClear);
  text_layer_set_text_color(ui->time_layer, GColorWhite);
  text_layer_set_font(ui->time_layer, fonts_get_system_font(FONT_KEY_BITHAM_42_BOLD));
  text_layer_set_text_alignment(ui->time_layer, GTextAlignmentCenter);
  layer_add_child(root, text_layer_get_layer(ui->time_layer));

  // Date
  ui->date_layer = text_layer_create(GRect(0, 0, bounds.size.w, 28));
  text_layer_set_background_color(ui->date_layer, GColorClear);
  text_layer_set_text_color(ui->date_layer, GColorWhite);
  text_layer_set_font(ui->date_layer, fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD));
  text_layer_set_text_alignment(ui->date_layer, GTextAlignmentCenter);
  text_layer_set_text(ui->date_layer, "");
  layer_add_child(root, text_layer_get_layer(ui->date_layer));

  // Weather row
  ui->weather_row_layer = text_layer_create(GRect(0, 0, bounds.size.w, 24));
  text_layer_set_background_color(ui->weather_row_layer, GColorClear);
  text_layer_set_text_color(ui->weather_row_layer, GColorWhite);
  text_layer_set_font(ui->weather_row_layer, fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD));
  text_layer_set_text_alignment(ui->weather_row_layer, GTextAlignmentCenter);
  text_layer_set_text(ui->weather_row_layer, "--- | --:--");
  layer_add_child(root, text_layer_get_layer(ui->weather_row_layer));

  // BT "No Signal" icon (hidden by default)
  ui->bt_icon_bitmap = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_NO_SIGNAL);
  ui->bt_icon_layer = bitmap_layer_create(GRect(0, 0, 24, 24));
  bitmap_layer_set_background_color(ui->bt_icon_layer, GColorClear);
  bitmap_layer_set_compositing_mode(ui->bt_icon_layer, GCompOpSet);
  bitmap_layer_set_bitmap(ui->bt_icon_layer, ui->bt_icon_bitmap);
  layer_set_hidden(bitmap_layer_get_layer(ui->bt_icon_layer), true);
  layer_add_child(root, bitmap_layer_get_layer(ui->bt_icon_layer));

  // Battery "Low" icon (hidden by default)
  ui->batt_icon_bitmap = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BATTERY_LOW);
  ui->batt_icon_layer = bitmap_layer_create(GRect(0, 0, 24, 24));
  bitmap_layer_set_background_color(ui->batt_icon_layer, GColorClear);
  bitmap_layer_set_compositing_mode(ui->batt_icon_layer, GCompOpSet);
  bitmap_layer_set_bitmap(ui->batt_icon_layer, ui->batt_icon_bitmap);
  layer_set_hidden(bitmap_layer_get_layer(ui->batt_icon_layer), true);
  layer_add_child(root, bitmap_layer_get_layer(ui->batt_icon_layer));

  // Initial layout will be done by ui_relayout() from caller
  return ui;
}

void ui_destroy(Ui *ui) {
  if (!ui) return;

  text_layer_destroy(ui->time_layer);
  text_layer_destroy(ui->date_layer);
  text_layer_destroy(ui->weather_row_layer);

  layer_destroy(ui->frame_layer);

  if (ui->bt_icon_bitmap) gbitmap_destroy(ui->bt_icon_bitmap);
  if (ui->batt_icon_bitmap) gbitmap_destroy(ui->batt_icon_bitmap);
  bitmap_layer_destroy(ui->bt_icon_layer);
  bitmap_layer_destroy(ui->batt_icon_layer);

  if (ui->uv_icon_bitmap) {
    gbitmap_destroy(ui->uv_icon_bitmap);
    ui->uv_icon_bitmap = NULL;
  }
  bitmap_layer_destroy(ui->uv_icon_layer);

  text_layer_destroy(ui->precip_layer);

  bitmap_layer_destroy(ui->icon_layer);

  free(ui);
}

void ui_set_time(Ui *ui, const char *hhmm) {
  text_layer_set_text(ui->time_layer, hhmm ? hhmm : "");
}

void ui_set_date(Ui *ui, const char *date_text) {
  text_layer_set_text(ui->date_layer, date_text ? date_text : "");
}

void ui_set_weather_row(Ui *ui, const char *temp, const char *sun_text) {
  snprintf(ui->weather_row_buf, sizeof(ui->weather_row_buf),
           "%s | %s", temp ? temp : "---", sun_text ? sun_text : "--:--");
  text_layer_set_text(ui->weather_row_layer, ui->weather_row_buf);
}

void ui_set_precip(Ui *ui, int32_t mm) {
  snprintf(ui->precip_buf, sizeof(ui->precip_buf), "%ld\nmm", (long)mm);
  text_layer_set_text(ui->precip_layer, ui->precip_buf);
}

void ui_show_bt_icon(Ui *ui, bool show) {
  layer_set_hidden(bitmap_layer_get_layer(ui->bt_icon_layer), !show);
}

void ui_show_batt_low_icon(Ui *ui, bool show) {
  layer_set_hidden(bitmap_layer_get_layer(ui->batt_icon_layer), !show);
}

void ui_relayout(Ui *ui, bool is_obstructed, int32_t last_uv, int32_t last_precip) {
  prv_relayout_all(ui, is_obstructed, last_uv, last_precip);
}

BitmapLayer* ui_get_main_icon_layer(Ui *ui) {
  return ui->icon_layer;
}
