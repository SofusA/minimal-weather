const ImageId = {
  IMAGE_QUESTION_DARK: 0,
  IMAGE_SUNNY_DARK: 1,
  IMAGE_CLEAR_NIGHT_DARK: 2,
  IMAGE_PARTLY_CLOUDY_DARK: 3,
  IMAGE_PARTLY_CLOUDY_NIGHT_DARK: 4,
  IMAGE_CLOUDY_DARK: 5,
  IMAGE_HEAVY_RAIN_DARK: 6,
  IMAGE_HEAVY_SNOW_DARK: 7,
  IMAGE_RAIN_SNOW_DARK: 8,
  IMAGE_THUNDERSTORM_DARK: 9,
};

const SYMBOL_TO_IMAGE_ID = {
  // Clear / fair
  clearsky_day: ImageId.IMAGE_SUNNY_DARK,
  clearsky_night: ImageId.IMAGE_CLEAR_NIGHT_DARK,
  clearsky_polartwilight: ImageId.IMAGE_CLEAR_NIGHT_DARK,

  fair_day: ImageId.IMAGE_PARTLY_CLOUDY_DARK,
  fair_night: ImageId.IMAGE_PARTLY_CLOUDY_NIGHT_DARK,
  fair_polartwilight: ImageId.IMAGE_PARTLY_CLOUDY_NIGHT_DARK,

  // Snow showers + thunder -> thunderstorm icon
  lightssnowshowersandthunder_day: ImageId.IMAGE_THUNDERSTORM_DARK,
  lightssnowshowersandthunder_night: ImageId.IMAGE_THUNDERSTORM_DARK,
  lightssnowshowersandthunder_polartwilight: ImageId.IMAGE_THUNDERSTORM_DARK,

  // Light snow showers -> snow
  lightsnowshowers_day: ImageId.IMAGE_HEAVY_SNOW_DARK,
  lightsnowshowers_night: ImageId.IMAGE_HEAVY_SNOW_DARK,
  lightsnowshowers_polartwilight: ImageId.IMAGE_HEAVY_SNOW_DARK,

  // Thunder mixes
  heavyrainandthunder: ImageId.IMAGE_THUNDERSTORM_DARK,
  heavysnowandthunder: ImageId.IMAGE_THUNDERSTORM_DARK,
  rainandthunder: ImageId.IMAGE_THUNDERSTORM_DARK,

  // Heavy sleet showers + thunder -> thunder
  heavysleetshowersandthunder_day: ImageId.IMAGE_THUNDERSTORM_DARK,
  heavysleetshowersandthunder_night: ImageId.IMAGE_THUNDERSTORM_DARK,
  heavysleetshowersandthunder_polartwilight: ImageId.IMAGE_THUNDERSTORM_DARK,

  // Straight weather types
  heavysnow: ImageId.IMAGE_HEAVY_SNOW_DARK,

  // Heavy rain showers -> heavy rain
  heavyrainshowers_day: ImageId.IMAGE_HEAVY_RAIN_DARK,
  heavyrainshowers_night: ImageId.IMAGE_HEAVY_RAIN_DARK,
  heavyrainshowers_polartwilight: ImageId.IMAGE_HEAVY_RAIN_DARK,

  // Sleet types -> mixed precipitation icon
  lightsleet: ImageId.IMAGE_RAIN_SNOW_DARK,
  heavyrain: ImageId.IMAGE_HEAVY_RAIN_DARK,

  // Light rain showers -> heavy rain
  lightrainshowers_day: ImageId.IMAGE_HEAVY_RAIN_DARK,
  lightrainshowers_night: ImageId.IMAGE_HEAVY_RAIN_DARK,
  lightrainshowers_polartwilight: ImageId.IMAGE_HEAVY_RAIN_DARK,

  // Sleet showers (heavy/light) -> mixed precipitation
  heavysleetshowers_day: ImageId.IMAGE_RAIN_SNOW_DARK,
  heavysleetshowers_night: ImageId.IMAGE_RAIN_SNOW_DARK,
  heavysleetshowers_polartwilight: ImageId.IMAGE_RAIN_SNOW_DARK,
  lightsleetshowers_day: ImageId.IMAGE_RAIN_SNOW_DARK,
  lightsleetshowers_night: ImageId.IMAGE_RAIN_SNOW_DARK,
  lightsleetshowers_polartwilight: ImageId.IMAGE_RAIN_SNOW_DARK,

  // Snow
  snow: ImageId.IMAGE_HEAVY_SNOW_DARK,

  // Heavy rain showers + thunder
  heavyrainshowersandthunder_day: ImageId.IMAGE_THUNDERSTORM_DARK,
  heavyrainshowersandthunder_night: ImageId.IMAGE_THUNDERSTORM_DARK,
  heavyrainshowersandthunder_polartwilight: ImageId.IMAGE_THUNDERSTORM_DARK,

  // Snow showers
  snowshowers_day: ImageId.IMAGE_HEAVY_SNOW_DARK,
  snowshowers_night: ImageId.IMAGE_HEAVY_SNOW_DARK,
  snowshowers_polartwilight: ImageId.IMAGE_HEAVY_SNOW_DARK,

  // Fog -> cloudy fallback (or add a dedicated fog icon later)
  fog: ImageId.IMAGE_CLOUDY_DARK,

  // Snow showers + thunder
  snowshowersandthunder_day: ImageId.IMAGE_THUNDERSTORM_DARK,
  snowshowersandthunder_night: ImageId.IMAGE_THUNDERSTORM_DARK,
  snowshowersandthunder_polartwilight: ImageId.IMAGE_THUNDERSTORM_DARK,

  // Light snow + thunder
  lightsnowandthunder: ImageId.IMAGE_THUNDERSTORM_DARK,

  // Heavy sleet + thunder
  heavysleetandthunder: ImageId.IMAGE_THUNDERSTORM_DARK,

  // Simple rain/snow
  lightrain: ImageId.IMAGE_HEAVY_RAIN_DARK,
  rainshowersandthunder_day: ImageId.IMAGE_THUNDERSTORM_DARK,
  rainshowersandthunder_night: ImageId.IMAGE_THUNDERSTORM_DARK,
  rainshowersandthunder_polartwilight: ImageId.IMAGE_THUNDERSTORM_DARK,
  rain: ImageId.IMAGE_HEAVY_RAIN_DARK,
  lightsnow: ImageId.IMAGE_HEAVY_SNOW_DARK,

  // Light rain showers + thunder
  lightrainshowersandthunder_day: ImageId.IMAGE_THUNDERSTORM_DARK,
  lightrainshowersandthunder_night: ImageId.IMAGE_THUNDERSTORM_DARK,
  lightrainshowersandthunder_polartwilight: ImageId.IMAGE_THUNDERSTORM_DARK,

  // More sleet variants
  heavysleet: ImageId.IMAGE_RAIN_SNOW_DARK,
  sleetandthunder: ImageId.IMAGE_THUNDERSTORM_DARK,
  lightrainandthunder: ImageId.IMAGE_THUNDERSTORM_DARK,
  sleet: ImageId.IMAGE_RAIN_SNOW_DARK,

  // Light sleet showers + thunder
  lightssleetshowersandthunder_day: ImageId.IMAGE_THUNDERSTORM_DARK,
  lightssleetshowersandthunder_night: ImageId.IMAGE_THUNDERSTORM_DARK,
  lightssleetshowersandthunder_polartwilight: ImageId.IMAGE_THUNDERSTORM_DARK,

  // Light sleet + thunder
  lightsleetandthunder: ImageId.IMAGE_THUNDERSTORM_DARK,

  // Partly cloudy
  partlycloudy_day: ImageId.IMAGE_PARTLY_CLOUDY_DARK,
  partlycloudy_night: ImageId.IMAGE_PARTLY_CLOUDY_NIGHT_DARK,
  partlycloudy_polartwilight: ImageId.IMAGE_PARTLY_CLOUDY_NIGHT_DARK,

  // Sleet showers + thunder
  sleetshowersandthunder_day: ImageId.IMAGE_THUNDERSTORM_DARK,
  sleetshowersandthunder_night: ImageId.IMAGE_THUNDERSTORM_DARK,
  sleetshowersandthunder_polartwilight: ImageId.IMAGE_THUNDERSTORM_DARK,

  // Rain showers
  rainshowers_day: ImageId.IMAGE_HEAVY_RAIN_DARK,
  rainshowers_night: ImageId.IMAGE_HEAVY_RAIN_DARK,
  rainshowers_polartwilight: ImageId.IMAGE_HEAVY_RAIN_DARK,

  // Snow + thunder
  snowandthunder: ImageId.IMAGE_THUNDERSTORM_DARK,

  // Sleet showers
  sleetshowers_day: ImageId.IMAGE_RAIN_SNOW_DARK,
  sleetshowers_night: ImageId.IMAGE_RAIN_SNOW_DARK,
  sleetshowers_polartwilight: ImageId.IMAGE_RAIN_SNOW_DARK,

  // Cloudy
  cloudy: ImageId.IMAGE_CLOUDY_DARK,

  // Heavy snow showers + thunder
  heavysnowshowersandthunder_day: ImageId.IMAGE_THUNDERSTORM_DARK,
  heavysnowshowersandthunder_night: ImageId.IMAGE_THUNDERSTORM_DARK,
  heavysnowshowersandthunder_polartwilight: ImageId.IMAGE_THUNDERSTORM_DARK,

  // Heavy snow showers
  heavysnowshowers_day: ImageId.IMAGE_HEAVY_SNOW_DARK,
  heavysnowshowers_night: ImageId.IMAGE_HEAVY_SNOW_DARK,
  heavysnowshowers_polartwilight: ImageId.IMAGE_HEAVY_SNOW_DARK,
};

// Public function: map API symbol -> numeric image id.
function getImageIdFromSymbol(symbol) {
  if (!symbol || typeof symbol !== 'string') {
    return ImageId.IMAGE_QUESTION_DARK;
  }

  const key = symbol.trim();
  return SYMBOL_TO_IMAGE_ID[key];
}

var latestData = {
  temp: null,
  icon_code: null,
  uv: null,
  precipitation: null,
  sunriseStr: null,
  sunsetStr: null
};

function resetLatestData() { 
  latestData.temp = null;
  latestData.icon_code = null;
  latestData.uv = null;
  latestData.precipitation = null;
  latestData.sunriseStr = null;
  latestData.sunsetStr = null;
}

function maybeSend() { 
  if (latestData.temp !== null &&
      latestData.icon_code !== null &&
      latestData.uv !== null &&
      latestData.precipitation !== null &&
      latestData.sunriseStr !== null &&
      latestData.sunsetStr !== null) {
    sendDataToPebble(
      latestData.temp,
      latestData.icon_code,
      latestData.uv,
      latestData.precipitation,
      latestData.sunriseStr,
      latestData.sunsetStr
    );
  }
}

// format ISO8601 UTC -> local "HH:MM" =====
function formatIsoToLocalHHMM(isoStr) {
  try {
    var d = new Date(isoStr); // respects +00:00 (UTC) and converts to local when reading H/M
    if (isNaN(d.getTime())) return '--:--';

    var h = d.getHours();
    var m = d.getMinutes();

    var hh = (h < 10 ? '0' : '') + h;
    var mm = (m < 10 ? '0' : '') + m;
    return hh + ':' + mm;
  } catch (e) {
    return '--:--';
  }
}

function sendDataToPebble(temperature, icon_code, uv, precipitation, sunriseStr, sunsetStr) { 
  console.log('Sending data to pebble.');

  var icon = getImageIdFromSymbol(icon_code);

  var message = {
    'WEATHER_TEMPERATURE': temperature,
    'WEATHER_ICON': icon,
    'WEATHER_UV': uv,
    'WEATHER_PRECIPITATION': precipitation,
    'SUNRISE': sunriseStr || '--:--', 
    'SUNSET':  sunsetStr || '--:--'   
  };

  Pebble.sendAppMessage(message,
    function(e) {
      console.log('Data sent successfully');
    },
    function(e) {
      console.log('Failed to send data: ' + e.error.message);
    }
  );
}

function fetchWeather(lat, lon) {
  const url =
    "https://api.met.no/weatherapi/locationforecast/2.0/complete" +
    "?lat=" + lat + "&lon=" + lon; 

  console.log("Fetching weather: " + url);

  var xhr = new XMLHttpRequest();
  xhr.open("GET", url, true);
  xhr.timeout = 15000;
  xhr.setRequestHeader("Accept", "application/json");
  xhr.setRequestHeader("User-Agent", "PebbleWatchface");

  xhr.onreadystatechange = function () {
    if (xhr.readyState === 4) {
      console.log("HTTP readyState=4, status=" + xhr.status);
      if (xhr.status >= 200 && xhr.status < 300) {
        try {
          var data = JSON.parse(xhr.responseText);

          var temp =
            data.properties.timeseries[0].data.instant.details.air_temperature;

          var uv = data.properties.timeseries[0].data.instant.details.ultraviolet_index_clear_sky;

          var image = data.properties.timeseries[0].data.next_1_hours.summary.symbol_code;
          var precipitation = data.properties.timeseries[0].data.next_6_hours.details.precipitation_amount;

          latestData.temp = Math.round(temp);
          latestData.icon_code = image;
          latestData.uv = Math.round(uv);
          latestData.precipitation = Math.round(precipitation);
          maybeSend(); 

        } catch (err) {
          console.log("JSON parse error: " + err);
        }
      } else {
        console.log("HTTP error, status=" + xhr.status + " body: " + xhr.responseText);
      }
    }
  };
  xhr.onerror = function () { console.log("XHR network error"); };
  xhr.ontimeout = function () { console.log("XHR timeout"); };
  xhr.send();
}

function fetchSunTimes(lat, lon) { 
  var url =
    "https://api.met.no/weatherapi/sunrise/3.0/sun?lat=" + lat + "&lon=" + lon;

  console.log("Fetching sunrise/sunset: " + url);

  var xhr = new XMLHttpRequest();
  xhr.open("GET", url, true);
  xhr.timeout = 15000;
  xhr.setRequestHeader("Accept", "application/json");
  xhr.setRequestHeader("User-Agent", "PebbleWatchface");

  xhr.onreadystatechange = function () {
    if (xhr.readyState === 4) {
      console.log("Sun API readyState=4, status=" + xhr.status);
      if (xhr.status >= 200 && xhr.status < 300) {
        try {
          var data = JSON.parse(xhr.responseText);

          // Guard for polar day/night or missing fields
          var sunriseIso = data && data.properties && data.properties.sunrise && data.properties.sunrise.time;
          var sunsetIso  = data && data.properties && data.properties.sunset  && data.properties.sunset.time;

          var sunriseStr = sunriseIso ? formatIsoToLocalHHMM(sunriseIso) : '—';
          var sunsetStr  = sunsetIso ? formatIsoToLocalHHMM(sunsetIso)   : '—';

          latestData.sunriseStr = sunriseStr;
          latestData.sunsetStr  = sunsetStr;
          maybeSend();

        } catch (err) {
          console.log("Sun API JSON parse error: " + err);
          latestData.sunriseStr = '--:--';
          latestData.sunsetStr = '--:--';
          maybeSend();
        }
      } else {
        console.log("Sun API HTTP error, status=" + xhr.status + " body: " + xhr.responseText);
        latestData.sunriseStr = '--:--';
        latestData.sunsetStr = '--:--';
        maybeSend();
      }
    }
  };
  xhr.onerror = function () {
    console.log("Sun API XHR network error");
    latestData.sunriseStr = '--:--';
    latestData.sunsetStr = '--:--';
    maybeSend();
  };
  xhr.ontimeout = function () {
    console.log("Sun API XHR timeout");
    latestData.sunriseStr = '--:--';
    latestData.sunsetStr = '--:--';
    maybeSend();
  };
  xhr.send();
}

function requestWeather() {
  console.log("Requesting geolocation...");
  navigator.geolocation.getCurrentPosition(
    function(pos) {
      console.log("Got location: " + pos.coords.latitude + ", " + pos.coords.longitude);
      resetLatestData(); 
      fetchWeather(pos.coords.latitude, pos.coords.longitude);  
      fetchSunTimes(pos.coords.latitude, pos.coords.longitude); 
    },
    function(err) {
      console.log("Location error: " + JSON.stringify(err));
    },
    { timeout: 15000, maximumAge: 600000 }
  );
}

Pebble.addEventListener("ready", function() {
  console.log("PKJS ready");
  requestWeather();
});

Pebble.addEventListener("appmessage", function(e) {
  console.log("AppMessage received from watch, refreshing weather");
  requestWeather();
});
