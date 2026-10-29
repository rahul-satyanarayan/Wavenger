#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <WiFi.h>
#include <esp_now.h>

#define PI 3.14159265358979323846

Adafruit_MPU6050 mpu;

// Replace with your receiver MAC address
uint8_t receiverAddress[] = {0x24, 0x6F, 0x28, 0xAA, 0xBB, 0xCC};

// Callback when data is sent
void OnDataSent(const wifi_tx_info_t *tx_info, esp_now_send_status_t status) {
  Serial.print("Last Packet Send Status: ");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Success" : "Fail");
}

// Structure to send data
typedef struct struct_motion_data {
  char motion;      // F, B, L, R, N
  float pitch;
  float roll;
} motion_data_t;

motion_data_t motionData;

void setup() {
  Serial.begin(115200);

  // Initialize MPU6050
  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) delay(10);
  }
  Serial.println("MPU6050 Found!");
  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);

  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Register send callback
  esp_now_register_send_cb(OnDataSent);

  // Add peer
  esp_now_peer_info_t peerInfo = {};
  memcpy(peerInfo.peer_addr, receiverAddress, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer");
    return;
  }
}

void loop() {
  // ---- Read MPU6050 ----
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  float roll  = atan2(a.acceleration.y, a.acceleration.z) * 180 / PI;
  float pitch = atan(-a.acceleration.x / 
               sqrt(a.acceleration.y * a.acceleration.y + 
                    a.acceleration.z * a.acceleration.z)) * 180 / PI;

  char motion = 'N'; // Default: None
  float threshold = 15.0;

  if (pitch > threshold) motion = 'F';
  else if (pitch < -threshold) motion = 'B';
  else if (roll > threshold) motion = 'R';
  else if (roll < -threshold) motion = 'L';

  // Fill data struct
  motionData.motion = motion;
  motionData.pitch = pitch;
  motionData.roll = roll;

  // Send only if motion is detected
  if (motion != 'N') {
    esp_err_t result = esp_now_send(receiverAddress, (uint8_t *)&motionData, sizeof(motionData));
    if (result == ESP_OK) {
      Serial.print("Sent motion: "); Serial.print(motion);
      Serial.print(" | Pitch: "); Serial.print(pitch);
      Serial.print(" | Roll: "); Serial.println(roll);
    } else {
      Serial.println("Error sending motion");
    }
  }

  delay(50); // Adjustable small delay
}
