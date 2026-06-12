#pragma once

#include <pebble.h>

typedef struct Ui Ui;

Ui* ui_create(Window *window);
void ui_destroy(Ui *ui);

void ui_set_time(Ui *ui, const char *hhmm);
void ui_set_date(Ui *ui, const char *date_text);
void ui_set_precip(Ui *ui, int32_t mm);

void ui_show_bt_icon(Ui *ui, bool show);
void ui_show_batt_low_icon(Ui *ui, bool show);

void ui_relayout(Ui *ui, bool is_obstructed, int32_t last_uv, int32_t last_precip);

BitmapLayer* ui_get_main_icon_layer(Ui *ui);
Layer* ui_get_weather_graph_layer(Ui *ui);
