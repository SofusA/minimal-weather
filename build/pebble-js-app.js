/******/ (function(modules) { // webpackBootstrap
/******/ 	// The module cache
/******/ 	var installedModules = {};
/******/
/******/ 	// The require function
/******/ 	function __webpack_require__(moduleId) {
/******/
/******/ 		// Check if module is in cache
/******/ 		if(installedModules[moduleId])
/******/ 			return installedModules[moduleId].exports;
/******/
/******/ 		// Create a new module (and put it into the cache)
/******/ 		var module = installedModules[moduleId] = {
/******/ 			exports: {},
/******/ 			id: moduleId,
/******/ 			loaded: false
/******/ 		};
/******/
/******/ 		// Execute the module function
/******/ 		modules[moduleId].call(module.exports, module, module.exports, __webpack_require__);
/******/
/******/ 		// Flag the module as loaded
/******/ 		module.loaded = true;
/******/
/******/ 		// Return the exports of the module
/******/ 		return module.exports;
/******/ 	}
/******/
/******/
/******/ 	// expose the modules object (__webpack_modules__)
/******/ 	__webpack_require__.m = modules;
/******/
/******/ 	// expose the module cache
/******/ 	__webpack_require__.c = installedModules;
/******/
/******/ 	// __webpack_public_path__
/******/ 	__webpack_require__.p = "";
/******/
/******/ 	// Load entry module and return exports
/******/ 	return __webpack_require__(0);
/******/ })
/************************************************************************/
/******/ ([
/* 0 */
/***/ (function(module, exports, __webpack_require__) {

	__webpack_require__(1);
	module.exports = __webpack_require__(2);


/***/ }),
/* 1 */
/***/ (function(module, exports) {

	/**
	 * Copyright 2024 Google LLC
	 *
	 * Licensed under the Apache License, Version 2.0 (the "License");
	 * you may not use this file except in compliance with the License.
	 * You may obtain a copy of the License at
	 *
	 *     http://www.apache.org/licenses/LICENSE-2.0
	 *
	 * Unless required by applicable law or agreed to in writing, software
	 * distributed under the License is distributed on an "AS IS" BASIS,
	 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
	 * See the License for the specific language governing permissions and
	 * limitations under the License.
	 */
	
	(function(p) {
	  if (!p === undefined) {
	    console.error('Pebble object not found!?');
	    return;
	  }
	
	  // Aliases:
	  p.on = p.addEventListener;
	  p.off = p.removeEventListener;
	
	  // For Android (WebView-based) pkjs, print stacktrace for uncaught errors:
	  if (typeof window !== 'undefined' && window.addEventListener) {
	    window.addEventListener('error', function(event) {
	      if (event.error && event.error.stack) {
	        console.error('' + event.error + '\n' + event.error.stack);
	      }
	    });
	  }
	
	})(Pebble);


/***/ }),
/* 2 */
/***/ (function(module, exports) {

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
	
	  // Light rain showers -> heavy rain (you can switch to a lighter rain icon if you add one)
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
	// Provides a safe fallback if the symbol is missing/unknown.
	function getImageIdFromSymbol(symbol) {
	  if (!symbol || typeof symbol !== 'string') {
	    return ImageId.IMAGE_QUESTION_DARK;
	  }
	
	  const key = symbol.trim();
	  return SYMBOL_TO_IMAGE_ID[key];
	}
	
	function sendDataToPebble(temperature, icon_code, uv, precipitation) {
	  console.log('Sending data to pebble.');
	
	  var icon = getImageIdFromSymbol(icon_code);
	
	  var message = {
	    'WEATHER_TEMPERATURE': temperature,
	    'WEATHER_ICON': icon,
	    'WEATHER_UV': uv,
	    'WEATHER_PRECIPITATION': precipitation
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
	          
	          const temp =
	            data.properties.timeseries[0].data.instant.details.air_temperature;
	
	          const uv = data.properties.timeseries[0].data.instant.details.ultraviolet_index_clear_sky;
	
	          const image = data.properties.timeseries[0].data.next_1_hours.summary.symbol_code;
	          const precipitation = data.properties.timeseries[0].data.next_6_hours.details.precipitation_amount;
	
	          sendDataToPebble(Math.round(temp), image, Math.round(uv), Math.round(precipitation));
	          
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
	
	
	function requestWeather() {
	  console.log("Requesting geolocation...");
	  navigator.geolocation.getCurrentPosition(
	    function(pos) {
	      console.log("Got location: " + pos.coords.latitude + ", " + pos.coords.longitude);
	      fetchWeather(pos.coords.latitude, pos.coords.longitude);
	    },
	    function(err) {
	      console.log("Location error, using fallback (Copenhagen): " + JSON.stringify(err));
	      // Fallback for emulator/dev
	      fetchWeather(55.6761, 12.5683);
	    },
	    { timeout: 15000, maximumAge: 600000 }
	  );
	}
	
	Pebble.addEventListener("ready", function() {
	  console.log("PKJS ready");
	  requestWeather();
	  // Refresh every 30 minutes
	  setInterval(requestWeather, 30 * 60 * 1000);
	});
	
	// Optional: let the watchface ping pkjs to force refresh
	Pebble.addEventListener("appmessage", function(e) {
	  console.log("AppMessage received from watch, refreshing weather");
	  requestWeather();
	});


/***/ })
/******/ ]);
//# sourceMappingURL=pebble-js-app.js.map