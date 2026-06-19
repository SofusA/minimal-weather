#include <pebble.h>
#include "weather_graph.h"

#ifndef WEATHER_GRAPH_POINTS
#define WEATHER_GRAPH_POINTS 13
#endif

#ifndef PERSIST_KEY_WEATHER_GRAPH_TEMPS
#define PERSIST_KEY_WEATHER_GRAPH_TEMPS 7101
#endif

#ifndef PERSIST_KEY_WEATHER_GRAPH_PRECIP
#define PERSIST_KEY_WEATHER_GRAPH_PRECIP 7102
#endif

#ifndef PERSIST_KEY_WEATHER_GRAPH_COUNT
#define PERSIST_KEY_WEATHER_GRAPH_COUNT 7103
#endif

#ifndef PERSIST_KEY_WEATHER_GRAPH_AVAILABLE
#define PERSIST_KEY_WEATHER_GRAPH_AVAILABLE 7104
#endif

static Layer *s_graph_layer = NULL;

static int s_temps[WEATHER_GRAPH_POINTS];
static int s_precip[WEATHER_GRAPH_POINTS];
static int s_count = 0;
static bool s_available = false;

static int prv_parse_int(const char **p) {
  bool neg = false;
  int val = 0;

  if (**p == '-') {
    neg = true;
    (*p)++;
  }

  while (**p >= '0' && **p <= '9') {
    val = (val * 10) + (**p - '0');
    (*p)++;
  }

  return neg ? -val : val;
}

static int prv_nice_precip_max(int max_mm) {
  if (max_mm <= 0) return 1;
  if (max_mm <= 1) return 1;
  if (max_mm <= 2) return 2;
  if (max_mm <= 5) return 5;
  if (max_mm <= 10) return 10;
  if (max_mm <= 20) return 20;
  return ((max_mm + 9) / 10) * 10;
}

static void prv_save(void) {
  persist_write_data(PERSIST_KEY_WEATHER_GRAPH_TEMPS, s_temps, sizeof(s_temps));
  persist_write_data(PERSIST_KEY_WEATHER_GRAPH_PRECIP, s_precip, sizeof(s_precip));
  persist_write_int(PERSIST_KEY_WEATHER_GRAPH_COUNT, s_count);
  persist_write_bool(PERSIST_KEY_WEATHER_GRAPH_AVAILABLE, s_available);
}

static void prv_load(void) {
  if (persist_exists(PERSIST_KEY_WEATHER_GRAPH_TEMPS)) {
    persist_read_data(PERSIST_KEY_WEATHER_GRAPH_TEMPS, s_temps, sizeof(s_temps));
  }

  if (persist_exists(PERSIST_KEY_WEATHER_GRAPH_PRECIP)) {
    persist_read_data(PERSIST_KEY_WEATHER_GRAPH_PRECIP, s_precip, sizeof(s_precip));
  }

  if (persist_exists(PERSIST_KEY_WEATHER_GRAPH_COUNT)) {
    s_count = persist_read_int(PERSIST_KEY_WEATHER_GRAPH_COUNT);
    if (s_count < 0) s_count = 0;
    if (s_count > WEATHER_GRAPH_POINTS) s_count = WEATHER_GRAPH_POINTS;
  }

  if (persist_exists(PERSIST_KEY_WEATHER_GRAPH_AVAILABLE)) {
    s_available = persist_read_bool(PERSIST_KEY_WEATHER_GRAPH_AVAILABLE);
  }
}

static void prv_draw_outlined_text(GContext *ctx, const char *text, GFont font, GRect rect, GTextAlignment alignment) {
  graphics_context_set_text_color(ctx, GColorBlack);
  graphics_draw_text(ctx, text, font, GRect(rect.origin.x - 1, rect.origin.y, rect.size.w, rect.size.h), GTextOverflowModeTrailingEllipsis, alignment, NULL);
  graphics_draw_text(ctx, text, font, GRect(rect.origin.x + 1, rect.origin.y, rect.size.w, rect.size.h), GTextOverflowModeTrailingEllipsis, alignment, NULL);
  graphics_draw_text(ctx, text, font, GRect(rect.origin.x, rect.origin.y - 1, rect.size.w, rect.size.h), GTextOverflowModeTrailingEllipsis, alignment, NULL);
  graphics_draw_text(ctx, text, font, GRect(rect.origin.x, rect.origin.y + 1, rect.size.w, rect.size.h), GTextOverflowModeTrailingEllipsis, alignment, NULL);

  graphics_context_set_text_color(ctx, GColorWhite);
  graphics_draw_text(ctx, text, font, rect, GTextOverflowModeTrailingEllipsis, alignment, NULL);
}

static void prv_update_proc(Layer *layer, GContext *ctx) {
  GRect bounds = layer_get_bounds(layer);

  graphics_context_set_fill_color(ctx, GColorBlack);
  graphics_fill_rect(ctx, bounds, 0, GCornerNone);

  graphics_context_set_stroke_color(ctx, GColorWhite);
  graphics_context_set_stroke_width(ctx, 1);

  if (!s_available || s_count < 2) {
    GFont font = fonts_get_system_font(FONT_KEY_GOTHIC_14);
    prv_draw_outlined_text(ctx, "--", font, bounds, GTextAlignmentCenter);
    return;
  }

  const int label_w = 26;
  const int margin_left = 2;
  const int margin_right = 2;
  const int margin_top = 2;
  const int margin_bottom = 2;

  const int graph_x = margin_left;
  const int graph_y = margin_top;
  const int graph_w = bounds.size.w - margin_left - margin_right;
  const int graph_h = bounds.size.h - margin_top - margin_bottom;

  if (graph_w <= 4 || graph_h <= 4) return;

  int temp_min = s_temps[0];
  int temp_max = s_temps[0];
  int precip_max = s_precip[0];

  for (int i = 1; i < s_count; i++) {
    if (s_temps[i] < temp_min) temp_min = s_temps[i];
    if (s_temps[i] > temp_max) temp_max = s_temps[i];
    if (s_precip[i] > precip_max) precip_max = s_precip[i];
  }

  if (temp_max == temp_min) {
    temp_max++;
    temp_min--;
  }

  int precip_scale = prv_nice_precip_max(precip_max);

  int col_w = graph_w / s_count;
  if (col_w < 1) col_w = 1;

  int used_w = col_w * s_count;
  int gx = graph_x + ((graph_w - used_w) / 2);

  for (int i = 0; i < s_count; i++) {
    if (s_precip[i] <= 0) continue;

    int bar_h = (s_precip[i] * graph_h) / precip_scale;
    if (bar_h < 1) bar_h = 1;
    if (bar_h > graph_h) bar_h = graph_h;

    int bar_x = gx + (i * col_w);
    int bar_y = graph_y + graph_h - bar_h;
    int bar_w = col_w - 1;
    if (bar_w < 1) bar_w = 1;

    graphics_context_set_fill_color(ctx, GColorDarkGray);
    graphics_fill_rect(ctx, GRect(bar_x, bar_y, bar_w, bar_h), 0, GCornerNone);
  }

  graphics_context_set_stroke_color(ctx, GColorWhite);
  graphics_context_set_stroke_width(ctx, 2);

  for (int i = 0; i < s_count - 1; i++) {
    int x1 = gx + (i * col_w) + (col_w / 2);
    int x2 = gx + ((i + 1) * col_w) + (col_w / 2);

    int y1 = graph_y + graph_h - ((s_temps[i] - temp_min) * graph_h / (temp_max - temp_min));
    int y2 = graph_y + graph_h - ((s_temps[i + 1] - temp_min) * graph_h / (temp_max - temp_min));

    graphics_draw_line(ctx, GPoint(x1, y1), GPoint(x2, y2));
  }

  graphics_context_set_fill_color(ctx, GColorWhite);

  for (int i = 0; i < s_count; i++) {
    int x = gx + (i * col_w) + (col_w / 2);
    int y = graph_y + graph_h - ((s_temps[i] - temp_min) * graph_h / (temp_max - temp_min));
    graphics_fill_circle(ctx, GPoint(x, y), 1);
  }

  GFont label_font = fonts_get_system_font(FONT_KEY_GOTHIC_14);
  char temp_max_buf[16];
  char temp_min_buf[16];
  char current_temp_buf[16];
  char precip_buf[16];

  snprintf(temp_max_buf, sizeof(temp_max_buf), "%d°", temp_max);
  snprintf(temp_min_buf, sizeof(temp_min_buf), "%d°", temp_min);
  snprintf(current_temp_buf, sizeof(current_temp_buf), "%d°", s_temps[0]);
  snprintf(precip_buf, sizeof(precip_buf), "%d", precip_scale);

  prv_draw_outlined_text(ctx, temp_max_buf, label_font,
                         GRect(bounds.size.w - label_w, graph_y - 3, label_w, 16),
                         GTextAlignmentRight);

  prv_draw_outlined_text(ctx, temp_min_buf, label_font,
                         GRect(bounds.size.w - label_w, graph_y + graph_h - 13, label_w, 16),
                         GTextAlignmentRight);

  if (precip_max > 0) {
    prv_draw_outlined_text(ctx, precip_buf, label_font,
                           GRect(0, graph_y - 3, label_w, 16),
                           GTextAlignmentLeft);

    prv_draw_outlined_text(ctx, "mm", label_font,
                           GRect(0, graph_y + graph_h - 13, label_w, 16),
                           GTextAlignmentLeft);
  }

  prv_draw_outlined_text(ctx, current_temp_buf, label_font,
                         GRect(0, graph_y + 15, label_w, 16),
                         GTextAlignmentLeft);
}

void weather_graph_init(Layer *graph_layer) {
  s_graph_layer = graph_layer;
  prv_load();

  if (s_graph_layer) {
    layer_set_update_proc(s_graph_layer, prv_update_proc);
    layer_mark_dirty(s_graph_layer);
  }
}

void weather_graph_deinit(void) {
  s_graph_layer = NULL;
}

void weather_graph_mark_dirty(void) {
  if (s_graph_layer) {
    layer_mark_dirty(s_graph_layer);
  }
}

void weather_graph_parse_forecast(const char *forecast) {
  memset(s_temps, 0, sizeof(s_temps));
  memset(s_precip, 0, sizeof(s_precip));

  s_count = 0;
  s_available = false;

  if (!forecast || !forecast[0]) {
    prv_save();
    weather_graph_mark_dirty();
    return;
  }

  const char *p = forecast;

  while (*p && s_count < WEATHER_GRAPH_POINTS) {
    s_temps[s_count] = prv_parse_int(&p);

    if (*p != ',') break;
    p++;

    s_precip[s_count] = prv_parse_int(&p);
    if (s_precip[s_count] < 0) s_precip[s_count] = 0;

    s_count++;

    if (*p == '|') {
      p++;
    } else if (*p != '\0') {
      break;
    }
  }

  s_available = (s_count >= 2);

  prv_save();
  weather_graph_mark_dirty();
}
