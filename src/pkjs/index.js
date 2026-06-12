var getImageId = require("./symbolMap");

function sendDataToPebble(icon_code, uv, prec, forecast) {
  var icon = getImageId(icon_code);

  Pebble.sendAppMessage({
    WEATHER_ICON: icon,
    WEATHER_UV: uv,
    WEATHER_PRECIPITATION: prec,
    WEATHER_FORECAST: forecast
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
  console.log("Featching weather");
  var url = `https://api.met.no/weatherapi/locationforecast/2.0/complete?lat=${lat}&lon=${lon}`;

  var xhr = new XMLHttpRequest();
  xhr.open("GET", url, true);
  xhr.timeout = 15000;
  xhr.setRequestHeader("Accept", "application/json");

  xhr.onreadystatechange = function () {
    if (xhr.readyState === 4) {
      if (xhr.status >= 200 && xhr.status < 300) {
        console.log("Got resonse");
        try {
          var data = JSON.parse(xhr.responseText);
          var data_object = data.properties.timeseries[0].data;

          var t = data_object.instant.details.air_temperature;
          var uv = data_object.instant.details.ultraviolet_index_clear_sky;
          var icon = data_object.next_1_hours.summary.symbol_code;
          var prec = data_object.next_6_hours.details.precipitation_amount;

          var uv_round = Math.round(uv);
          var precipitation = Math.round(prec);

          console.log("Sending data to pebble");
          console.log(icon);
          console.log(uv_round);
          console.log(precipitation);

          var forecastParts = [];

          for (var i = 0; i <= 12; i++) {
            var data_i = data.properties.timeseries[i].data;

            var temp_i = Math.round(data_i.instant.details.air_temperature);

            var precip_i = 0;

            if (data_i.next_1_hours) {
              precip_i = Math.round(
                data_i.next_1_hours.details.precipitation_amount
              );
            }

            forecastParts.push(temp_i + "," + precip_i);
          }

          var forecastString = forecastParts.join("|");
          console.log(forecastString);

          sendDataToPebble(icon, uv_round, precipitation, forecastString);
        } catch (err) {
          console.log("JSON parse error: " + err);
        }
      } else {
        console.log("Got bad status code: " + xhr.status);
        var icon = "rain";
        var uv_round = 2;
        var precipitation = 4;
        var forecastString = "18,0|19,0|19,0|18,0|18,0|18,0|17,1|17,5|16,2|15,1|15,0|15,0|15,0";

        sendDataToPebble(icon, uv_round, precipitation, forecastString);

      }
    }
  };

  xhr.send();
}
