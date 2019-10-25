#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include "RoboClaw.h"

RoboClaw roboclaw(&Serial, 10000);

const char pw[] = "basicmicro";
const char ap_name[] = "esp8266_thing_test";

ESP8266WebServer server(80);

const char home_page[] = R"=====(
<!DOCTYPE html>
<html>
<head>
<script>
  function sendCommand(chan,state) {
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function() {
      if (this.readyState == 4 && this.status == 200) {
 
      }
    };
    xhttp.open("GET", "handleMotor?motorChannel="+chan+"&motorState="+state, true);
    xhttp.send();
  }

</script>

<style>
      .round {
        border-radius: 25px;
        background-color: #cde6f5;
        width: 25%;
        height: 25%;
        margin: 20px;
        padding: 20px;
      }

      h2 {
        color: #707078;
      }

      .button {
        background-color: #87919e;
        display: inline-block;
        border-radius: 10px;
        border: none;
        color: white;
        font-size: 15px;
        padding: 25px;
      }

      span {
        color: #707078;
      }

      p {
        color: #707078;
        font-size: 25px;
      }
    </style>
</head
<body>

    <div class="round">
      <h2>Motor Channel 1</h2>
      <button type="button" class="button" onclick="sendCommand('1','1')">Motor On</button>
        <button type="button" class="button" onclick="sendCommand('1','0')">Motor Off</button>
        <br />
        <br />
        <p>Encoder Count: <span id="encoder_1_value">0</span></p>
    </div>

    <div class="round">
      <h2>Motor Channel 2</h2>
      <button type="button" class="button" onclick="sendCommand('2','1')">Motor On</button>
        <button type="button" class="button" onclick="sendCommand('2','0')">Motor Off</button>
        <br />
        <br />
        <p>Encoder Count: <span id="encoder_2_value">0</span></p>
    </div>
</body>
</html>

)=====";

void handleHome(){
  String page = home_page;
  server.send(200, "text/html",page);
}

void handleMotor(){
  String motor_channel = server.arg("motorChannel");
  String motor_state = server.arg("motorState");
  if(motor_channel == "1") {
    if(motor_state == "1") {
      roboclaw.ForwardM1(0x80, 64);
    }
    else if(motor_state == "0") {
      roboclaw.ForwardM1(0x80, 0);
    }
  }

  else if(motor_channel == "2") {
     if(motor_state == "1") {
      roboclaw.ForwardM2(0x80, 64);
    }
    else if(motor_state == "0") {
      roboclaw.ForwardM2(0x80, 0);
    }
  }
  
}

void setup() {
  roboclaw.begin(38400);
  WiFi.softAP(ap_name, pw);

  server.on("/", handleHome);
  server.on("/handleMotor", handleMotor);

  server.begin();

}

void loop() {
  server.handleClient();
}
