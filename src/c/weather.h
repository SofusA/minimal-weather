#pragma once
#include <pebble.h>

void set_weather_icon(int32_t idx);

void inbox_received_handler(DictionaryIterator *iter, void *context);

void weather_init(
  BitmapLayer *icon_layer,
  void (*on_temp_cb)(const char *temp_text),
  void (*on_uv_cb)(int32_t uv),
  void (*on_precip_cb)(int32_t mm),
  void (*on_sunrise_cb)(const char *sunrise_hhmm),
  void (*on_sunset_cb)(const char *sunset_hhmm)
);

void weather_deinit(void);
