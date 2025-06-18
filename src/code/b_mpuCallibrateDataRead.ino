#include <Preferences.h>
Preferences pref;

void setup() {
  Serial.begin(9600);
  delay(1000); // beri waktu serial siap

  Serial.println("Mulai baca preferences...");

  pref.begin("kalibrasiMPU", false);

  if (pref.isKey("ax")) {
    Serial.println("Offset AX: " + String(pref.getFloat("ax")));
    Serial.println("Offset AY: " + String(pref.getFloat("ay")));
    Serial.println("Offset AZ: " + String(pref.getFloat("az")));
    Serial.println("Offset GX: " + String(pref.getFloat("gx")));
    Serial.println("Offset GY: " + String(pref.getFloat("gy")));
    Serial.println("Offset GZ: " + String(pref.getFloat("gz")));
  } else {
    Serial.println("Data kalibrasi tidak ditemukan.");
  }

  pref.end();
}

void loop() {
}
