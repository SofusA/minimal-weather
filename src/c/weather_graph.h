#pragma once

#include <pebble.h>

#define WEATHER_GRAPH_POINTS 13

void weather_graph_init(Layer *graph_layer);
void weather_graph_deinit(void);

void weather_graph_mark_dirty(void);
void weather_graph_parse_forecast(const char *forecast);
