#include <pebble.h>
#include "weather.h"

static BitmapLayer *s_icon_layer  = NULL;
static GBitmap     *s_icon_bitmap = NULL;

static WeatherTempCallback s_on_temp    = NULL;
static WeatherIntCallback  s_on_uv      = NULL;
static WeatherIntCallback  s_on_precip  = NULL;
static WeatherSunCallback  s_on_sun     = NULL;

// Ordered must match your JS ImageId
static const uint32_t WEATHER_RES_IDS[] = {
  RESOURCE_ID_IMAGE_QUESTION_DARK,            
  RESOURCE_ID_IMAGE_SUNNY_DARK,               
  RESOURCE_ID_IMAGE_CLEAR_NIGHT_DARK,         
  RESOURCE_ID_IMAGE_PARTLY_CLOUDY_DARK,       
  RESOURCE_ID_IMAGE_PARTLY_CLOUDY_NIGHT_DARK, 
  RESOURCE_ID_IMAGE_CLOUDY_DARK,              
  RESOURCE_ID_IMAGE_HEAVY_RAIN_DARK,          
  RESOURCE_ID_IMAGE_HEAVY_SNOW_DARK,          
  RESOURCE_ID_IMAGE_RAIN_SNOW_DARK,           
  RESOURCE_ID_IMAGE_THUNDERSTORM_DARK         
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
    s_icon_bitmap = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_QUESTION_DARK);
  }

  bitmap_layer_set_bitmap(s_icon_layer, s_icon_bitmap);
}

void inbox_received_handler(DictionaryIterator *iter, void *context) {
  // Temperature: "12°"
  Tuple *temp_t = dict_find(iter, MESSAGE_KEY_WEATHER_TEMPERATURE);
  if (temp_t && s_on_temp) {
    static char buf[8];
    snprintf(buf, sizeof(buf), "%d°", (int)temp_t->value->int32);
    s_on_temp(buf);
  }

  // Icon
  Tuple *icon_t = dict_find(iter, MESSAGE_KEY_WEATHER_ICON);
  if (icon_t) {
    set_weather_icon(icon_t->value->int32);
  }

  // UV index
  Tuple *uv_t = dict_find(iter, MESSAGE_KEY_WEATHER_UV);
  if (uv_t && s_on_uv) {
    s_on_uv(uv_t->value->int32);
  }

  // Precip mm
  Tuple *precip_t = dict_find(iter, MESSAGE_KEY_WEATHER_PRECIPITATION);
  if (precip_t && s_on_precip) {
    s_on_precip(precip_t->value->int32);
  }

  // Sun
  Tuple *sun_t = dict_find(iter, MESSAGE_KEY_SUN_EVENT);
  if (sun_t && s_on_sun) {
    s_on_sun(sun_t->value->cstring);
  }
}

void weather_init(
  BitmapLayer *icon_layer,
  WeatherTempCallback   on_temp_cb,
  WeatherIntCallback    on_uv_cb,
  WeatherIntCallback    on_precip_cb,
  WeatherSunCallback    on_sun_cb
) {
  s_icon_layer = icon_layer;
  s_on_temp    = on_temp_cb;
  s_on_uv      = on_uv_cb;
  s_on_precip  = on_precip_cb;
  s_on_sun     = on_sun_cb;

  set_weather_icon(0);
}

void weather_deinit(void) {
  if (s_icon_bitmap) {
    gbitmap_destroy(s_icon_bitmap);
    s_icon_bitmap = NULL;
  }
  s_icon_layer = NULL;
  s_on_temp = NULL;
  s_on_uv = NULL;
  s_on_precip = NULL;
  s_on_sun = NULL;
}
