#include <pebble.h>
#include "messaging.h"
#include "weather.h"
#include "weather_graph.h"

static void prv_inbox_received(DictionaryIterator *iter, void *context) {
  (void)context;

  weather_inbox_parse(iter);

  Tuple *forecast_tuple = dict_find(iter, MESSAGE_KEY_WEATHER_FORECAST);
  if (forecast_tuple && forecast_tuple->type == TUPLE_CSTRING) {
    weather_graph_parse_forecast(forecast_tuple->value->cstring);
  }
}

void messaging_open(void) {
  app_message_register_inbox_received(prv_inbox_received);
  app_message_open(256, 256);
}

void messaging_request_weather_refresh(void) {
  DictionaryIterator *iter;
  AppMessageResult res = app_message_outbox_begin(&iter);

  if (res != APP_MSG_OK) {
    APP_LOG(APP_LOG_LEVEL_WARNING, "Outbox begin failed: %d", res);
    return;
  }

  dict_write_uint8(iter, MESSAGE_KEY_WEATHER_REFRESH, 1);
  dict_write_end(iter);
  app_message_outbox_send();
}
