#pragma once
#include <pebble.h>

typedef void (*WeatherTempCallback)(const char *temp_text);
typedef void (*WeatherIntCallback)(int32_t value);
typedef void (*WeatherSunCallback)(const char *hhmm);

void weather_init(
  BitmapLayer *icon_layer,
  WeatherTempCallback    on_temp_cb,
  WeatherIntCallback     on_uv_cb,
  WeatherIntCallback     on_precip_cb,
  WeatherSunCallback     on_sun_cb     
);

void weather_deinit(void);
void inbox_received_handler(DictionaryIterator *iter, void *context);
