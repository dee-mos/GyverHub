// CANVAS DEMO

// WiFi
#define AP_SSID ""
#define AP_PASS ""

#include <Arduino.h>
#include <GyverHUB.h>
GyverHUB hub("MyDevices", "ESP8266", "");

void build() {
  hub.BeginWidgets();

  // пустой холст, 400x300px
  hub.Canvas(F("cv"));

  hub.WidgetSize(50);

  // холст с рисунком. Обязательный порядок вызова: создать, начать, рисовать, закончить
  GHcanvas cv1;                                 // создать холст
  hub.BeginCanvas(F("cv1"), 200, 200, &cv1);    // начать рисование, холст 200x200
  cv1.stroke(0xff0000);
  cv1.strokeWeight(5);
  cv1.line(0, 0, -1, -1);
  cv1.line(0, -1, -1, 0);
  hub.EndCanvas();                        // закончить

  // и ещё один
  GHcanvas cv2;
  hub.BeginCanvas(F("cv2"), 300, 300, &cv2, F("circle")); // + название
  cv2.fill(0x00ff00);
  cv2.noStroke();
  cv2.circle(150, 150, 50);
  hub.EndCanvas();
}

void setup() {
  Serial.begin(115200);

#ifdef GH_ESP_BUILD
  WiFi.mode(WIFI_STA);
  WiFi.begin(AP_SSID, AP_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.println(WiFi.localIP());

  hub.setupMQTT("test.mosquitto.org", 1883);
#endif

  hub.onBuild(build);
  hub.begin();
}

void loop() {
  hub.tick();

  // обновление холста по таймеру
  static GHtimer tmr(300);
  if (tmr.ready()) {
    // способ 1
    GHcanvas cv;                        // создать холст
    cv.noStroke();
    cv.fill(GHcolor(random(255), random(255), random(255)), random(100, 255));
    cv.circle(random(0, 400), random(0, 300), random(5, 30));
    hub.sendCanvas(F("cv"), cv);        // отправить холст

    // способ 2 (использует меньше оперативной памяти!)
    GHcanvas cv2;                       // создать холст
    hub.sendCanvasBegin(F("cv2"), cv2); // начать отправку
    cv2.stroke(0xffffff, 100);
    cv2.strokeWeight(5);
    cv2.line(random(0, 300), random(0, 300), random(0, 300), random(0, 300));
    hub.sendCanvasEnd(cv2);             // закончить отправку
  }
}