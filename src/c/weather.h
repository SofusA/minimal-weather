#pragma once
#include <pebble.h>

void set_weather_icon(int32_t idx);
void inbox_received_handler(DictionaryIterator *iter, void *context);

void weather_init(
  BitmapLayer *icon_layer,
  void (*on_temp_cb)(const char *temp_text),
  void (*on_uv_cb)(int32_t uv),
  void (*on_precip_cb)(int32_t mm)
);

void weather_deinit(void);
