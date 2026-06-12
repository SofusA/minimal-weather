#pragma once

#include <pebble.h>

typedef void (*WeatherUpdateCallback)(int32_t uv, int32_t precip);

void weather_init(BitmapLayer *icon_layer, WeatherUpdateCallback on_update_cb);
void weather_deinit(void);
void weather_inbox_parse(DictionaryIterator *iter);
void set_weather_icon(int32_t idx);
