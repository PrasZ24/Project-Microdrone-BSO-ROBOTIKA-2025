#include <Preferences.h>
#include <MPU6050_light.h>

MPU6050 sensor_mpu(Wire);
Preferences pref;

void setup() {
  Serial.begin(9600);
  delay(1000);  // Biar serial sempat siap
  Serial.println("Inisialisasi...");

  // Mulai I2C (gunakan pin sesuai dengan wiring kamu)
  Wire.begin(7, 6);
  delay(500);  // Delay tambahan agar stabil

  // Mulai sensor MPU6050
  byte status = sensor_mpu.begin(3,3);
  if (status != 0) {
    Serial.print("Gagal mulai MPU6050! Kode error: ");
    Serial.println(status);
    while (1);
  }
  Serial.println("MPU6050 berhasil dimulai.");

  Serial.println("Kalibrasi... Jangan gerakkan sensor!");
  delay(2000);
  sensor_mpu.calcOffsets(true, true);
  Serial.println("Kalibrasi selesai.");

  // Simpan offset ke variabel
  float ax = sensor_mpu.getAccXoffset();
  float ay = sensor_mpu.getAccYoffset();
  float az = sensor_mpu.getAccZoffset();
  float gx = sensor_mpu.getGyroXoffset();
  float gy = sensor_mpu.getGyroYoffset();
  float gz = sensor_mpu.getGyroZoffset();

  Serial.println("Offset yang didapat:");
  Serial.print("AccX: "); Serial.println(ax);
  Serial.print("AccY: "); Serial.println(ay);
  Serial.print("AccZ: "); Serial.println(az);
  Serial.print("GyroX: "); Serial.println(gx);
  Serial.print("GyroY: "); Serial.println(gy);
  Serial.print("GyroZ: "); Serial.println(gz);

  // Simpan offset ke flash
  if (pref.begin("kalibrasiMPU", false)) {
    pref.putFloat("ax", ax);
    pref.putFloat("ay", ay);
    pref.putFloat("az", az);
    pref.putFloat("gx", gx);
    pref.putFloat("gy", gy);
    pref.putFloat("gz", gz);
    Serial.println("Offset berhasil disimpan ke flash.");
    pref.end();

    sensor_mpu.setAccOffsets(ax, ay, az);
    sensor_mpu.setGyroOffsets(gx, gy, gz);
  } else {
    Serial.println("Gagal membuka Preferences untuk menyimpan!");
  }

  // Update sensor untuk menghitung sudut
  sensor_mpu.update();

  // Tampilkan sudut X dan Y
  Serial.print("Sudut X (pitch): ");
  Serial.println(sensor_mpu.getAngleX());
  Serial.print("Sudut Y (roll): ");
  Serial.println(sensor_mpu.getAngleY());
}

void loop() {
  // Kosong, hanya kalibrasi dan tampilkan sekali saat setup
}
