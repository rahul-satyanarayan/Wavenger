#include <WiFi.h>
#include <esp_now.h>
#include <WebServer.h>

// Motor driver pins M1
const int ENA = 13;
const int m1r = 25;
const int m1b = 32;
const int m2r = 14;
const int m2b = 26;

// Motor driver pins M2
const int ENB = 15;
const int m3r = 19;
const int m3b = 18;
const int m4r = 16;
const int m4b = 4;

// WiFi credentials
const char* ssid = "Rahul";
const char* password = "m66ntugh";

// Web server
WebServer server(80);

// LED for light
const int ledPin = 2;
bool isLightOn = false;

// ================= MOTOR FUNCTIONS =================
void Forward() {
  analogWrite(ENA, 255);
  analogWrite(ENB, 255);

  digitalWrite(m1r, HIGH); digitalWrite(m1b, LOW);
  digitalWrite(m2r, HIGH); digitalWrite(m2b, LOW);
  digitalWrite(m3r, HIGH); digitalWrite(m3b, LOW);
  digitalWrite(m4r, HIGH); digitalWrite(m4b, LOW);

  Serial.println("Moving Forward");
}

void Backward() {
  analogWrite(ENA, 255);
  analogWrite(ENB, 255);

  digitalWrite(m1r, LOW); digitalWrite(m1b, HIGH);
  digitalWrite(m2r, LOW); digitalWrite(m2b, HIGH);
  digitalWrite(m3r, LOW); digitalWrite(m3b, HIGH);
  digitalWrite(m4r, LOW); digitalWrite(m4b, HIGH);

  Serial.println("Moving Backward");
}

void Rotate_Left() {
  analogWrite(ENA, 255);
  analogWrite(ENB, 255);

  digitalWrite(m1r, LOW); digitalWrite(m1b, HIGH);
  digitalWrite(m2r, LOW); digitalWrite(m2b, HIGH);
  digitalWrite(m3r, HIGH); digitalWrite(m3b, LOW);
  digitalWrite(m4r, HIGH); digitalWrite(m4b, LOW);

  Serial.println("Rotating Left");
}

void Rotate_Right() {
  analogWrite(ENA, 255);
  analogWrite(ENB, 255);

  digitalWrite(m1r, HIGH); digitalWrite(m1b, LOW);
  digitalWrite(m2r, HIGH); digitalWrite(m2b, LOW);
  digitalWrite(m3r, LOW); digitalWrite(m3b, HIGH);
  digitalWrite(m4r, LOW); digitalWrite(m4b, HIGH);

  Serial.println("Rotating Right");
}

void Left_Side() {
  analogWrite(ENA, 255);
  analogWrite(ENB, 255);

  digitalWrite(m1r, LOW); digitalWrite(m1b, HIGH);
  digitalWrite(m2r, HIGH); digitalWrite(m2b, LOW);
  digitalWrite(m3r, HIGH); digitalWrite(m3b, LOW);
  digitalWrite(m4r, LOW); digitalWrite(m4b, HIGH);

  Serial.println("Strafing Left");
}

void Right_Side() {
  analogWrite(ENA, 255);
  analogWrite(ENB, 255);

  digitalWrite(m1r, HIGH); digitalWrite(m1b, LOW);
  digitalWrite(m2r, LOW); digitalWrite(m2b, HIGH);
  digitalWrite(m3r, LOW); digitalWrite(m3b, HIGH);
  digitalWrite(m4r, HIGH); digitalWrite(m4b, LOW);

  Serial.println("Strafing Right");
}

void For_Left() {
  analogWrite(ENA, 255);
  analogWrite(ENB, 255);

  digitalWrite(m1r, HIGH); digitalWrite(m1b, LOW);
  digitalWrite(m2r, LOW); digitalWrite(m2b, LOW);
  digitalWrite(m3r, LOW); digitalWrite(m3b, LOW);
  digitalWrite(m4r, HIGH); digitalWrite(m4b, LOW);

  Serial.println("Forward-Left");
}

void For_Right() {
  analogWrite(ENA, 255);
  analogWrite(ENB, 255);

  digitalWrite(m1r, LOW); digitalWrite(m1b, LOW);
  digitalWrite(m2r, HIGH); digitalWrite(m2b, LOW);
  digitalWrite(m3r, HIGH); digitalWrite(m3b, LOW);
  digitalWrite(m4r, LOW); digitalWrite(m4b, LOW);

  Serial.println("Forward-Right");
}

void Back_Left() {
  analogWrite(ENA, 255);
  analogWrite(ENB, 255);

  digitalWrite(m1r, LOW); digitalWrite(m1b, LOW);
  digitalWrite(m2r, LOW); digitalWrite(m2b, HIGH);
  digitalWrite(m3r, LOW); digitalWrite(m3b, HIGH);
  digitalWrite(m4r, LOW); digitalWrite(m4b, LOW);

  Serial.println("Backward-Left");
}

void Back_Right() {
  analogWrite(ENA, 255);
  analogWrite(ENB, 255);

  digitalWrite(m1r, LOW); digitalWrite(m1b, HIGH);
  digitalWrite(m2r, LOW); digitalWrite(m2b, LOW);
  digitalWrite(m3r, LOW); digitalWrite(m3b, LOW);
  digitalWrite(m4r, LOW); digitalWrite(m4b, HIGH);

  Serial.println("Backward-Right");
}

void Lat_FL() {
  analogWrite(ENA, 255);
  analogWrite(ENB, 255);

  digitalWrite(m1r, HIGH); digitalWrite(m1b, LOW);
  digitalWrite(m2r, LOW); digitalWrite(m2b, LOW);
  digitalWrite(m3r, LOW); digitalWrite(m3b, HIGH);
  digitalWrite(m4r, LOW); digitalWrite(m4b, LOW);

  Serial.println("Lateral Front-Left");
}

void Lat_FR() {
  analogWrite(ENA, 255);
  analogWrite(ENB, 255);

  digitalWrite(m1r, LOW); digitalWrite(m1b, HIGH);
  digitalWrite(m2r, LOW); digitalWrite(m2b, LOW);
  digitalWrite(m3r, HIGH); digitalWrite(m3b, LOW);
  digitalWrite(m4r, LOW); digitalWrite(m4b, LOW);

  Serial.println("Lateral Front-Right");
}

void Lat_BL() {
  analogWrite(ENA, 255);
  analogWrite(ENB, 255);

  digitalWrite(m1r, LOW); digitalWrite(m1b, LOW);
  digitalWrite(m2r, HIGH); digitalWrite(m2b, LOW);
  digitalWrite(m3r, LOW); digitalWrite(m3b, LOW);
  digitalWrite(m4r, LOW); digitalWrite(m4b, HIGH);

  Serial.println("Lateral Back-Left");
}

void Lat_BR() {
  analogWrite(ENA, 255);
  analogWrite(ENB, 255);

  digitalWrite(m1r, LOW); digitalWrite(m1b, LOW);
  digitalWrite(m2r, LOW); digitalWrite(m2b, HIGH);
  digitalWrite(m3r, LOW); digitalWrite(m3b, LOW);
  digitalWrite(m4r, HIGH); digitalWrite(m4b, LOW);

  Serial.println("Lateral Back-Right");
}

void Stop() {
  digitalWrite(m1r, LOW); digitalWrite(m1b, LOW);
  digitalWrite(m2r, LOW); digitalWrite(m2b, LOW);
  digitalWrite(m3r, LOW); digitalWrite(m3b, LOW);
  digitalWrite(m4r, LOW); digitalWrite(m4b, LOW);

  Serial.println("Stopping");
}

void Horn() {
  Serial.println("Horn Activated");
}

void Light() {
  isLightOn = !isLightOn;
  digitalWrite(ledPin, isLightOn ? HIGH : LOW);
  Serial.println(isLightOn ? "Light ON" : "Light OFF");
}

// ================= COMMAND HANDLER =================
void handleCommand(char cmd) {
  switch(cmd){
    case 'F': Forward(); break;
    case 'B': Backward(); break;
    case 'L': Rotate_Left(); break;
    case 'R': Rotate_Right(); break;
    case 'G': For_Left(); break;
    case 'H': For_Right(); break;
    case 'I': Back_Left(); break;
    case 'J': Back_Right(); break;
    case 'D': Lat_FL(); break;
    case 'K': Lat_FR(); break;
    case 'N': Lat_BL(); break;
    case 'V': Lat_BR(); break;
    case 'S': Stop(); break;
    case 'Y': Horn(); break;
    case 'Z': Light(); break;
    default: Serial.println("Unknown Command"); break;
  }
}

// ================= WEB SERVER =================
void handleWebControl() {
  if(server.hasArg("command")) {
    String cmdStr = server.arg("command");
    if(cmdStr.length() == 1){
      char cmd = cmdStr.charAt(0);
      Serial.print("Web command: "); Serial.println(cmd);
      handleCommand(cmd);
      server.send(200,"text/plain","Command received: "+cmdStr);
    } else server.send(400,"text/plain","Invalid command");
  } else server.send(400,"text/plain","No command specified");
}

// ================= ESP-NOW CALLBACK =================
void OnDataRecv(const esp_now_recv_info_t *info, const uint8_t *incomingData, int len) {
  if(len > 0){
    char cmd = (char)incomingData[0];
    Serial.print("ESP-NOW command: "); Serial.println(cmd);
    handleCommand(cmd);
  }
}

// ================= SETUP =================
void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);

  int motorPins[] = {ENA, ENB, m1r,m1b,m2r,m2b,m3r,m3b,m4r,m4b};
  for(int i=0;i<10;i++) pinMode(motorPins[i], OUTPUT);

  // WiFi + ESP-NOW
  WiFi.mode(WIFI_STA);
  if(esp_now_init() != ESP_OK){
    Serial.println("ESP-NOW init failed");
  } else {
    Serial.println("ESP-NOW initialized");
  }
  esp_now_register_recv_cb(OnDataRecv);

  // Connect to WiFi
  WiFi.begin(ssid,password);
  int attempts = 0;
  Serial.print("Connecting WiFi");
  while(WiFi.status() != WL_CONNECTED && attempts < 30){
    delay(500);
    Serial.print(".");
    attempts++;
  }
  if(WiFi.status()==WL_CONNECTED){
    Serial.println("\nWiFi connected! IP:");
    Serial.println(WiFi.localIP());
  } else Serial.println("\nWiFi failed");

  // Web server
  server.on("/control", HTTP_GET, handleWebControl);
  server.begin();
  Serial.println("WebServer started");
}

// ================= LOOP =================
void loop() {
  server.handleClient(); // Handle HTTP requests
}
