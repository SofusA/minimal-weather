#include <pebble.h>
#include "weather.h"

static BitmapLayer *s_icon_layer  = NULL;
static GBitmap     *s_icon_bitmap = NULL;

static WeatherUpdateCallback s_on_update = NULL;

static const uint32_t WEATHER_RES_IDS[] = {
  RESOURCE_ID_IMAGE_EMPTY,
  RESOURCE_ID_IMAGE_CLEAR_SKY_DAY,
  RESOURCE_ID_IMAGE_CLEAR_SKY_NIGHT,
  RESOURCE_ID_IMAGE_CLOUDY,
  RESOURCE_ID_IMAGE_FOG,
  RESOURCE_ID_IMAGE_HEAVY_RAIN,
  RESOURCE_ID_IMAGE_HEAVY_RAIN_SHOWERS_DAY,
  RESOURCE_ID_IMAGE_HEAVY_RAIN_SHOWERS_NIGHT,
  RESOURCE_ID_IMAGE_LIGHT_RAIN,
  RESOURCE_ID_IMAGE_LIGHT_RAIN_SHOWERS_DAY,
  RESOURCE_ID_IMAGE_LIGHT_RAIN_SHOWERS_NIGHT,
  RESOURCE_ID_IMAGE_LIGHT_SLEET_SHOWERS_DAY,
  RESOURCE_ID_IMAGE_LIGHT_SLEET_SHOWERS_NIGHT,
  RESOURCE_ID_IMAGE_LIGHT_SNOW,
  RESOURCE_ID_IMAGE_LIGHT_SNOW_SHOWERS_DAY,
  RESOURCE_ID_IMAGE_NIGHT_SNOW_SHOWERS_NIGHT,
  RESOURCE_ID_IMAGE_PARTLY_CLOUDED_DAY,
  RESOURCE_ID_IMAGE_PARTLY_CLOUDED_NIGHT,
  RESOURCE_ID_IMAGE_RAIN_AND_THUNDER,
  RESOURCE_ID_IMAGE_RAIN_SHOWERS_AND_THUNDER_DAY,
  RESOURCE_ID_IMAGE_RAIN_SHOWERS_AND_THUNDER_NIGHT,
  RESOURCE_ID_IMAGE_RAIN_SHOWERS_DAY,
  RESOURCE_ID_IMAGE_RAIN_SHOWERS_NIGHT,
  RESOURCE_ID_IMAGE_SNOW_AND_THUNDER,
  RESOURCE_ID_IMAGE_SLEET,
  RESOURCE_ID_IMAGE_SLEET_SHOWERS_DAY,
  RESOURCE_ID_IMAGE_SLEET_SHOWERS_NIGHT,
  RESOURCE_ID_IMAGE_SNOW,
  RESOURCE_ID_IMAGE_SNOW_SHOWERS_DAY,
  RESOURCE_ID_IMAGE_SNOW_SHOWERS_NIGHT,
  RESOURCE_ID_IMAGE_THUNDER,
  RESOURCE_ID_IMAGE_THUNDER_AND_SNOW_DAY,
  RESOURCE_ID_IMAGE_THUNDER_AND_SNOW_NIGHT,
  RESOURCE_ID_IMAGE_THUNDER_DAY,
  RESOURCE_ID_IMAGE_THUNDER_NIGHT
};

static inline int32_t prv_res_count(void) {
  return (int32_t)(sizeof(WEATHER_RES_IDS) / sizeof(WEATHER_RES_IDS[0]));
}

void set_weather_icon(int32_t idx) {
  if (!s_icon_layer) return;

  if (idx < 0 || idx >= prv_res_count()) idx = 0;

  if (s_icon_bitmap) {
    gbitmap_destroy(s_icon_bitmap);
    s_icon_bitmap = NULL;
  }

  s_icon_bitmap = gbitmap_create_with_resource(WEATHER_RES_IDS[idx]);
  if (!s_icon_bitmap) {
    s_icon_bitmap = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_EMPTY);
  }

  bitmap_layer_set_bitmap(s_icon_layer, s_icon_bitmap);
}

void weather_inbox_parse(DictionaryIterator *iter) {
  Tuple *uv_t   = dict_find(iter, MESSAGE_KEY_WEATHER_UV);
  Tuple *pr_t   = dict_find(iter, MESSAGE_KEY_WEATHER_PRECIPITATION);
  Tuple *icon_t = dict_find(iter, MESSAGE_KEY_WEATHER_ICON);

  if (icon_t) {
    set_weather_icon(icon_t->value->int32);
  }

  if (s_on_update) {
    int32_t uv = uv_t ? uv_t->value->int32 : 0;
    int32_t pr = pr_t ? pr_t->value->int32 : 0;
    s_on_update(uv, pr);
  }
}

void weather_init(BitmapLayer *icon_layer, WeatherUpdateCallback on_update_cb) {
  s_icon_layer = icon_layer;
  s_on_update  = on_update_cb;

  set_weather_icon(0);
}

void weather_deinit(void) {
  if (s_icon_bitmap) {
    gbitmap_destroy(s_icon_bitmap);
    s_icon_bitmap = NULL;
  }

  s_icon_layer = NULL;
  s_on_update = NULL;
}
