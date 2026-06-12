var getImageId = require("./symbolMap");

function sendDataToPebble(temp, icon_code, uv, prec, min, max) {
  var icon = getImageId(icon_code);

  Pebble.sendAppMessage({
    WEATHER_TEMPERATURE: temp,
    WEATHER_ICON: icon,
    WEATHER_UV: uv,
    WEATHER_PRECIPITATION: prec,
    WEATHER_MIN: min,
    WEATHER_MAX: max,
  });
}

function requestWeather() {
  navigator.geolocation.getCurrentPosition(function (pos) {
    var lat = pos.coords.latitude;
    var lon = pos.coords.longitude;

    fetchWeather(lat, lon);
  });
}

Pebble.addEventListener("ready", function () {
  console.log("Ready");
  requestWeather();
});

Pebble.addEventListener("appmessage", function () {
  console.log("appmessage");
  requestWeather();
});

function fetchWeather(lat, lon) {
  var url = `https://api.met.no/weatherapi/locationforecast/2.0/complete?lat=${lat}&lon=${lon}`;

  var xhr = new XMLHttpRequest();
  xhr.open("GET", url, true);
  xhr.timeout = 15000;
  xhr.setRequestHeader("Accept", "application/json");

  xhr.onreadystatechange = function () {
    if (xhr.readyState === 4) {
      if (xhr.status >= 200 && xhr.status < 300) {
        try {
          var data = JSON.parse(xhr.responseText);
          var data_object = data.properties.timeseries[0].data;

          var t = data_object.instant.details.air_temperature;
          var uv = data_object.instant.details.ultraviolet_index_clear_sky;
          var icon = data_object.next_1_hours.summary.symbol_code;
          var prec = data_object.next_6_hours.details.precipitation_amount;

          var min = data_object.next_6_hours.details.air_temperature_min;
          var max = data_object.next_6_hours.details.air_temperature_max;

          var temp = Math.round(t);
          var uv_round = Math.round(uv);
          var precipitation = Math.round(prec);
          var min_round = Math.round(min);
          var max_round = Math.round(max);

          console.log("Sending data to pebble");
          console.log(temp);
          console.log(icon);
          console.log(uv_round);
          console.log(precipitation);
          console.log(min_round);
          console.log(max_round);

          sendDataToPebble(
            temp,
            icon,
            uv_round,
            precipitation,
            min_round,
            max_round
          );
        } catch (err) {
          console.log("JSON parse error: " + err);
        }
      }
    }
  };

  xhr.send();
}
