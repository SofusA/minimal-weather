#include <pebble.h>
#include "weather.h"

static BitmapLayer *s_icon_layer  = NULL;
static GBitmap     *s_icon_bitmap = NULL;
static void        (*s_on_temp)(const char *temp_text) = NULL;

static void        (*s_on_uv)(int32_t uv) = NULL;
static void        (*s_on_precip)(int32_t mm) = NULL;

static const uint32_t WEATHER_RES_IDS[] = {
  RESOURCE_ID_IMAGE_QUESTION_DARK,            // 0
  RESOURCE_ID_IMAGE_SUNNY_DARK,               // 1
  RESOURCE_ID_IMAGE_CLEAR_NIGHT_DARK,         // 2
  RESOURCE_ID_IMAGE_PARTLY_CLOUDY_DARK,       // 3
  RESOURCE_ID_IMAGE_PARTLY_CLOUDY_NIGHT_DARK, // 4
  RESOURCE_ID_IMAGE_CLOUDY_DARK,              // 5
  RESOURCE_ID_IMAGE_HEAVY_RAIN_DARK,          // 6
  RESOURCE_ID_IMAGE_HEAVY_SNOW_DARK,          // 7
  RESOURCE_ID_IMAGE_RAIN_SNOW_DARK,           // 8
  RESOURCE_ID_IMAGE_THUNDERSTORM_DARK         // 9
};

static inline int32_t prv_countof_res(void) {
  return (int32_t)(sizeof(WEATHER_RES_IDS) / sizeof(WEATHER_RES_IDS[0]));
}

void set_weather_icon(int32_t idx) {
  if (!s_icon_layer) {
    return;
  }

  if (idx < 0 || idx >= prv_countof_res()) {
    idx = 0; // fallback "?"
  }

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
  Tuple *t = dict_find(iter, MESSAGE_KEY_WEATHER_TEMPERATURE);
  if (t) {
    static char s_temp_buffer[8];
    snprintf(s_temp_buffer, sizeof(s_temp_buffer), "%d°", (int)t->value->int32);
    if (s_on_temp) s_on_temp(s_temp_buffer);
  }

  Tuple *icon_t = dict_find(iter, MESSAGE_KEY_WEATHER_ICON);
  if (icon_t) {
    set_weather_icon(icon_t->value->int32);
  }

  Tuple *uv_t = dict_find(iter, MESSAGE_KEY_WEATHER_UV);
  if (uv_t && s_on_uv) {
    s_on_uv(uv_t->value->int32);
  }

  Tuple *precip_t = dict_find(iter, MESSAGE_KEY_WEATHER_PRECIPITATION);
  if (precip_t && s_on_precip) {
    s_on_precip(precip_t->value->int32);
  }
}

void weather_init(
  BitmapLayer *icon_layer,
  void (*on_temp_cb)(const char *temp_text),
  void (*on_uv_cb)(int32_t uv),
  void (*on_precip_cb)(int32_t mm)
) {
  s_icon_layer = icon_layer;
  s_on_temp    = on_temp_cb;
  s_on_uv      = on_uv_cb;      
  s_on_precip  = on_precip_cb;  

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
}
