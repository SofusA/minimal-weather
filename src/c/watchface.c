#include <pebble.h>
#include <ctype.h>

#include "ui.h"
#include "messaging.h"
#include "weather.h"
#include "weather_graph.h"

static Window *s_window;
static Ui     *s_ui;

static bool    s_is_obstructed = false;
static int32_t s_last_uv       = -1;
static int32_t s_last_precip   = -1;
static bool    s_bt_connected  = true;
static int     s_batt_percent  = 100;

static char s_time_buf[6];
static char s_date_buf[20];

static void prv_update_time_date(void) {
  time_t now = time(NULL);
  struct tm *t = localtime(&now);

  strftime(s_time_buf, sizeof(s_time_buf), "%H:%M", t);
  ui_set_time(s_ui, s_time_buf);

  char wday[4] = "";
  char mon[4]  = "";

  strftime(wday, sizeof(wday), "%a", t);
  strftime(mon,  sizeof(mon),  "%b", t);

  for (size_t i = 0; wday[i] != '\0'; ++i) {
    wday[i] = (char)tolower((unsigned char)wday[i]);
  }

  for (size_t i = 0; mon[i] != '\0'; ++i) {
    mon[i] = (char)tolower((unsigned char)mon[i]);
  }

  snprintf(s_date_buf, sizeof(s_date_buf), "%s %d %s", wday, t->tm_mday, mon);
  ui_set_date(s_ui, s_date_buf);
}

static void prv_relayout(void) {
  if (!s_window || !s_ui) return;

  Layer *root = window_get_root_layer(s_window);
  GRect full  = layer_get_bounds(root);
  GRect unob  = layer_get_unobstructed_bounds(root);

  s_is_obstructed = (unob.size.h < full.size.h);

  ui_relayout(s_ui, s_is_obstructed, s_last_uv, s_last_precip);
}

static void on_weather_update(int32_t uv, int32_t precip) {
  s_last_uv = uv;
  s_last_precip = precip;

  ui_set_precip(s_ui, precip);
  weather_graph_mark_dirty();
  prv_relayout();
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  (void)units_changed;

  prv_update_time_date();

  if (tick_time->tm_min % 15 == 0) {
    messaging_request_weather_refresh();
  }
}

static void bt_handler(bool connected) {
  s_bt_connected = connected;

  ui_show_bt_icon(s_ui, !connected);

  if (connected) {
    messaging_request_weather_refresh();
  }
}

static void battery_handler(BatteryChargeState state) {
  s_batt_percent = state.charge_percent;

  bool low = (state.charge_percent <= 25) && !state.is_charging;
  ui_show_batt_low_icon(s_ui, low);
}

static void on_unobstructed_will_change(GRect final_unobstructed_screen_area, void *ctx) {
  (void)final_unobstructed_screen_area;
  (void)ctx;
}

static void on_unobstructed_change(AnimationProgress progress, void *ctx) {
  (void)progress;
  (void)ctx;

  prv_relayout();
}

static void on_unobstructed_did_change(void *ctx) {
  (void)ctx;

  prv_relayout();
}

static void window_load(Window *window) {
  s_ui = ui_create(window);

  weather_graph_init(ui_get_weather_graph_layer(s_ui));

  weather_init(
    ui_get_main_icon_layer(s_ui),
    on_weather_update
  );

  prv_update_time_date();
  prv_relayout();
}

static void window_unload(Window *window) {
  (void)window;

  weather_deinit();
  weather_graph_deinit();

  ui_destroy(s_ui);
  s_ui = NULL;
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
  battery_state_service_subscribe(battery_handler);
  battery_handler(battery_state_service_peek());

  messaging_open();
  messaging_request_weather_refresh();
}

static void deinit(void) {
  unobstructed_area_service_unsubscribe();
  tick_timer_service_unsubscribe();
  connection_service_unsubscribe();
  battery_state_service_unsubscribe();

  window_destroy(s_window);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}
