var ImageId = require('./imageIds');

var SYMBOL_TO_IMAGE_ID = {
  clearsky_day: ImageId.IMAGE_CLEAR_SKY_DAY,
  clearsky_night: ImageId.IMAGE_CLEAR_SKY_NIGHT,
  clearsky_polartwilight: ImageId.IMAGE_CLEAR_SKY_NIGHT,

  fair_day: ImageId.IMAGE_PARTLY_CLOUDED_DAY,
  fair_night: ImageId.IMAGE_PARTLY_CLOUDED_NIGHT,
  fair_polartwilight: ImageId.IMAGE_PARTLY_CLOUDED_NIGHT,

  lightssnowshowersandthunder_day: ImageId.IMAGE_THUNDER_AND_SNOW_DAY,
  lightssnowshowersandthunder_night: ImageId.IMAGE_THUNDER_AND_SNOW_NIGHT,
  lightssnowshowersandthunder_polartwilight: ImageId.IMAGE_THUNDER_AND_SNOW_NIGHT,

  lightsnowshowers_day: ImageId.IMAGE_SNOW_SHOWERS_DAY,
  lightsnowshowers_night: ImageId.IMAGE_SNOW_SHOWERS_NIGHT,
  lightsnowshowers_polartwilight: ImageId.IMAGE_SNOW_SHOWERS_NIGHT,

  heavyrainandthunder: ImageId.IMAGE_RAIN_AND_THUNDER,

  heavysnowandthunder: ImageId.IMAGE_RAIN_AND_THUNDER,
  rainandthunder: ImageId.IMAGE_RAIN_AND_THUNDER,

  heavysleetshowersandthunder_day: ImageId.IMAGE_THUNDER_AND_SNOW_DAY, 
  heavysleetshowersandthunder_night: ImageId.IMAGE_THUNDER_AND_SNOW_NIGHT, 
  heavysleetshowersandthunder_polartwilight: ImageId.IMAGE_THUNDER_AND_SNOW_NIGHT, 

  heavysnow: ImageId.IMAGE_SNOW,

  heavyrainshowers_day: ImageId.IMAGE_HEAVY_RAIN_SHOWERS_DAY,
  heavyrainshowers_night: ImageId.IMAGE_HEAVY_RAIN_SHOWERS_NIGHT,
  heavyrainshowers_polartwilight: ImageId.IMAGE_HEAVY_RAIN_SHOWERS_NIGHT,

  lightsleet: ImageId.IMAGE_SLEET,
  heavyrain: ImageId.IMAGE_HEAVY_RAIN,

  lightrainshowers_day: ImageId.IMAGE_LIGHT_RAIN_SHOWERS_DAY,
  lightrainshowers_night: ImageId.IMAGE_LIGHT_RAIN_SHOWERS_NIGHT,
  lightrainshowers_polartwilight: ImageId.IMAGE_LIGHT_RAIN_SHOWERS_NIGHT,

  heavysleetshowers_day: ImageId.IMAGE_SLEET_SHOWERS_DAY,
  heavysleetshowers_night: ImageId.IMAGE_SLEET_SHOWERS_NIGHT,
  heavysleetshowers_polartwilight: ImageId.IMAGE_SLEET_SHOWERS_NIGHT,

  lightsleetshowers_day: ImageId.IMAGE_LIGHT_SLEET_SHOWERS_DAY,
  lightsleetshowers_night: ImageId.IMAGE_LIGHT_SLEET_SHOWERS_NIGHT,
  lightsleetshowers_polartwilight: ImageId.IMAGE_LIGHT_SLEET_SHOWERS_NIGHT,

  snow: ImageId.IMAGE_SNOW,

  heavyrainshowersandthunder_day: ImageId.IMAGE_RAIN_SHOWERS_AND_THUNDER_DAY,
  heavyrainshowersandthunder_night: ImageId.IMAGE_RAIN_SHOWERS_AND_THUNDER_NIGHT,
  heavyrainshowersandthunder_polartwilight: ImageId.IMAGE_RAIN_SHOWERS_AND_THUNDER_NIGHT,

  snowshowers_day: ImageId.IMAGE_SNOW_SHOWERS_DAY,
  snowshowers_night: ImageId.IMAGE_SNOW_SHOWERS_NIGHT,
  snowshowers_polartwilight: ImageId.IMAGE_SNOW_SHOWERS_NIGHT,

  fog: ImageId.IMAGE_FOG,

  snowshowersandthunder_day: ImageId.IMAGE_THUNDER_AND_SNOW_DAY,
  snowshowersandthunder_night: ImageId.IMAGE_THUNDER_AND_SNOW_NIGHT,
  snowshowersandthunder_polartwilight: ImageId.IMAGE_THUNDER_AND_SNOW_NIGHT,

  lightsnowandthunder: ImageId.IMAGE_SNOW_AND_THUNDER,

  heavysleetandthunder: ImageId.IMAGE_SNOW_AND_THUNDER, 

  lightrain: ImageId.IMAGE_LIGHT_RAIN,

  rainshowersandthunder_day: ImageId.IMAGE_RAIN_SHOWERS_AND_THUNDER_DAY,
  rainshowersandthunder_night: ImageId.IMAGE_RAIN_SHOWERS_AND_THUNDER_NIGHT,
  rainshowersandthunder_polartwilight: ImageId.IMAGE_RAIN_SHOWERS_AND_THUNDER_NIGHT,

  rain: ImageId.IMAGE_LIGHT_RAIN,

  lightsnow: ImageId.IMAGE_LIGHT_SNOW,

  lightrainshowersandthunder_day: ImageId.IMAGE_RAIN_SHOWERS_AND_THUNDER_DAY,
  lightrainshowersandthunder_night: ImageId.IMAGE_RAIN_SHOWERS_AND_THUNDER_NIGHT,
  lightrainshowersandthunder_polartwilight: ImageId.IMAGE_RAIN_SHOWERS_AND_THUNDER_NIGHT,

  heavysleet: ImageId.IMAGE_SLEET,
  sleetandthunder: ImageId.IMAGE_SNOW_AND_THUNDER, 
  lightrainandthunder: ImageId.IMAGE_RAIN_AND_THUNDER,
  sleet: ImageId.IMAGE_SLEET,

  lightssleetshowersandthunder_day: ImageId.IMAGE_THUNDER_AND_SNOW_DAY,
  lightssleetshowersandthunder_night: ImageId.IMAGE_THUNDER_AND_SNOW_NIGHT,
  lightssleetshowersandthunder_polartwilight: ImageId.IMAGE_THUNDER_AND_SNOW_NIGHT, 

  lightsleetandthunder: ImageId.IMAGE_SNOW_AND_THUNDER, 

  partlycloudy_day: ImageId.IMAGE_PARTLY_CLOUDED_DAY,
  partlycloudy_night: ImageId.IMAGE_PARTLY_CLOUDED_NIGHT,
  partlycloudy_polartwilight: ImageId.IMAGE_PARTLY_CLOUDED_NIGHT,

  sleetshowersandthunder_day: ImageId.IMAGE_THUNDER_AND_SNOW_DAY,
  sleetshowersandthunder_night: ImageId.IMAGE_THUNDER_AND_SNOW_NIGHT, 
  sleetshowersandthunder_polartwilight: ImageId.IMAGE_THUNDER_AND_SNOW_NIGHT,

  rainshowers_day: ImageId.IMAGE_RAIN_SHOWERS_DAY,
  rainshowers_night: ImageId.IMAGE_RAIN_SHOWERS_NIGHT,
  rainshowers_polartwilight: ImageId.IMAGE_RAIN_SHOWERS_NIGHT,

  snowandthunder: ImageId.IMAGE_SNOW_AND_THUNDER,

  sleetshowers_day: ImageId.IMAGE_SLEET_SHOWERS_DAY,
  sleetshowers_night: ImageId.IMAGE_SLEET_SHOWERS_NIGHT,
  sleetshowers_polartwilight: ImageId.IMAGE_SLEET_SHOWERS_NIGHT,

  cloudy: ImageId.IMAGE_CLOUDY,

  heavysnowshowersandthunder_day: ImageId.IMAGE_THUNDER_AND_SNOW_DAY,
  heavysnowshowersandthunder_night: ImageId.IMAGE_THUNDER_AND_SNOW_NIGHT,
  heavysnowshowersandthunder_polartwilight: ImageId.IMAGE_THUNDER_AND_SNOW_NIGHT,

  heavysnowshowers_day: ImageId.IMAGE_SNOW_SHOWERS_DAY,
  heavysnowshowers_night: ImageId.IMAGE_SNOW_SHOWERS_NIGHT,
  heavysnowshowers_polartwilight: ImageId.IMAGE_SNOW_SHOWERS_NIGHT
};

function getImageIdFromSymbol(symbol) {
  if (!symbol || typeof symbol !== 'string') {
    return ImageId.IMAGE_EMPTY;
  }
  var key = symbol.trim();
  return SYMBOL_TO_IMAGE_ID[key];
}

module.exports = getImageIdFromSymbol;
