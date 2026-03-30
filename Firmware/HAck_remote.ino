#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <IRremoteESP8266.h>
#include <IRsend.h>

const char* ssid = "Airtel_XXXX";
const char* password = "MohXXXXX09";

ESP8266WebServer server(80);

// IR LED pin (D2 = GPIO4)
const uint16_t kIrLed = 4;
IRsend irsend(kIrLed);

// ✅ TRUE ON command
void sendON() {
  Serial.println("Sending COOLIX ON...");
  irsend.sendCOOLIX(0xB29FD0, 24);
}

// ✅ TRUE OFF command
void sendOFF() {
  Serial.println("Sending COOLIX OFF...");
  irsend.sendCOOLIX(0xB27BE0, 24);
}

void sendtempdown() {
  Serial.println("Sending COOLIIX Decrease temparature...");
  irsend.sendCOOLIX(0xB25F00, 24);
}

void sendtempinc() {
  Serial.println("Sending COOLIIX Increase Temparature...");
  irsend.sendCOOLIX(0xB25F10, 24);
}

void sendmode() {
  Serial.println("Sending COOLIIX set mode...");
  irsend.sendCOOLIX(0xB29FE4, 24);
}

void sendswing() {
  Serial.println("Sending COOLIIX set swing...");
  irsend.sendCOOLIX(0xB26BE0, 24);
}

void sendsleep() {
  Serial.println("Sending COOLIIX set sleep...");
  irsend.sendCOOLIX(0xB2E003, 24);
}

void sendtimerON() {
  Serial.println("Sending COOLIIX set timer ON...");
  irsend.sendCOOLIX(0xB24D1FE00B01, 48);
}

void sendLEDdisplay() {
  Serial.println("Sending COOLIIX set Display LED...");
  irsend.sendCOOLIX(0xB5F5A5, 24);
}

// Web handlers
void handleOn() {
  Serial.println("ON Triggered");
  sendON();
  server.send(200, "text/plain", "AC ON");
}

void handleOff() {
  Serial.println("OFF Triggered");
  sendOFF();
  server.send(200, "text/plain", "AC OFF");
}

void handletempdown() {
  Serial.println("Temparature decreased!");
  sendtempdown();
  server.send(200, "text/plain", "AC Temparature decreased");
}

void handletempinc() {
  Serial.println("Temparature increased!");
  sendtempinc();
  server.send(200, "text/plain", "AC Temparature increased");
}

void handlemode() {
  Serial.println("AC mode changed!");
  sendmode();
  server.send(200, "text/plain", "Mode changed");
}

void handleswing() {
  Serial.println("AC on swing!");
  sendswing();
  server.send(200, "text/plain", "AC on swing");
}

void handlesleep() {
  Serial.println("AC on sleep!");
  sendsleep();
  server.send(200, "text/plain", "AC on sleep");
}

void handletimeron() {
  Serial.println("AC timer begun");
  sendtimerON();
  server.send(200, "text/plain", "Timer begun");
}

void handleleddisplay() {
  Serial.println("AC LED displayed!");
  sendLEDdisplay();
  server.send(200, "text/plain", "LED displayed");
}

// Web page
void handleRoot() {
  String html = R"rawliteral(
    <!DOCTYPE html>
    <html>
    <head>
      <title>AC Remote</title>
      <style>
        body {
          font-family: Arial;
          text-align: center;
          margin-top: 50px;
        }
        button {
          padding: 20px;
          font-size: 20px;
          margin: 10px;
          border: none;
          border-radius: 10px;
          color: white;
        }
        .on { background-color: green; }
        .off { background-color: red; }
        .tempdown { background-color: yellow; }
        .tempinc { backgorund-color: blue }
        .mode { background-color: pink }
        .swing { background-color: aqua }
        .sleep { background-color: coral }
        .timeron { background-color: purple }
        .leddisplay { background-color: amber }
      </style>
    </head>
    <body>
      <h1>AC Remote Control</h1>

      <button class="on" onclick="sendOn()">Turn ON</button>
      <button class="off" onclick="sendOff()">Turn OFF</button>
      <button class="tempdown" onclick="sendtempdown()">Decrease temparature</button>
      <button class="tempinc" onclick="sendtempinc()">Increase temparature</button>
      <button class="mode" onclick="sendmode()">Mode</button>
      <button class="swing" onclick="sendswing()">Swing</button>
      <button class="sleep" onclick="sendsleep()">Sleep</button>
      <button class="timeron" onclick="sendtimerOn()">Timer ON</button>
      <button class="leddisplay" onclick="sendleddisplay()">LED Display</button>

      <script>
        function sendOn() {
          fetch('/on')
            .then(res => res.text())
            .then(data => alert(data));
        }

        function sendOff() {
          fetch('/off')
            .then(res => res.text())
            .then(data => alert(data));
        }

        function sendtempdown() {
          fetch('/tempdown')
            .then(res => res.text())
            .then(data => alert(data));
        }

        function sendtempinc() {
          fetch('/tempinc')
            .then(res => res.text())
            .then(data => alert(data));
        }

        function sendmode() {
          fetch('/mode')
            .then(res => res.text())
            .then(data => alert(data));
        }

        function sendswing() {
          fetch('/swing')
            .then(res => res.text())
            .then(data => alert(data));
        }

        function sendsleep() {
          fetch('/sleep')
            .then(res => res.text())
            .then(data => alert(data));
        }

        function sendtimerOn() {
          fetch('/timerON')
            .then(res => res.text())
            .then(data => alert(data));
        }

        function sendLEDdisplay() {
          fetch('/leddisplay')
            .then(res => res.text())
            .then(data => alert(data));
        }
      </script>
    </body>
    </html>
  )rawliteral";

  server.send(200, "text/html", html);
}

void setup() {
  Serial.begin(115200);
  irsend.begin();

  WiFi.begin(ssid, password);
  Serial.print("Connecting");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nConnected!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);
  server.on("/on", handleOn);
  server.on("/off", handleOff);
  server.on("/tempdown", handletempdown);
  server.on("/tempinc", handletempinc);
  server.on("/mode", handlemode);
  server.on("/swing", handlemode);
  server.on("/sleep", handlemode);
  server.on("/timerON", handlemode);
  server.on("/LEDdisplay", handlemode);

  server.begin();
  Serial.println("Server started");
}

void loop() {
  server.handleClient();
}
