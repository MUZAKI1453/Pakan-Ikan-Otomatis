#include <Servo.h>
#include <Wire.h>
#include <hd44780.h>
#include <hd44780ioClass/hd44780_I2Cexp.h>
#include "RTClib.h"

// Objek
Servo mekanik;
hd44780_I2Cexp lcd;
RTC_DS1307 rtc;

char daysOfTheWeek[7][12] = {"Ahad", "Senin", "Selasa", "Rabu", "Kamis", "Jum'at", "Sabtu"};
int jam, menit, detik;
int tanggal, bulan, tahun;
String hari;

void setup() {
  Serial.begin(9600);

  // RTC
  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }
  if (! rtc.isrunning()) {
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__))); // set sekali waktu upload
  }

  // LCD
  lcd.begin(16, 2);

  // SERVO
  mekanik.attach(4);
  mekanik.write(0);
}

void loop() {
  // RTC
  DateTime now = rtc.now();
  jam     = now.hour();
  menit   = now.minute();
  detik   = now.second();
  tanggal = now.day();
  bulan   = now.month();
  tahun   = now.year();
  hari    = daysOfTheWeek[now.dayOfTheWeek()];

  // Serial monitor
  Serial.print(hari); Serial.print(", ");
  Serial.print(tanggal); Serial.print("-");
  Serial.print(bulan); Serial.print("-");
  Serial.println(tahun);

  Serial.print(jam); Serial.print(":");
  Serial.print(menit); Serial.print(":");
  Serial.println(detik);
  Serial.println();

  // LCD
  lcd.setCursor(0, 0);
  lcd.print("                "); // clear baris pertama
  lcd.setCursor(0, 0);
  lcd.print(hari);
  lcd.print(",");
  lcd.print(tanggal);
  lcd.print("-");
  lcd.print(bulan);
  lcd.print("-");
  lcd.print(tahun);

  lcd.setCursor(0, 1);
  lcd.print("                "); // clear baris kedua
  lcd.setCursor(0, 1);
  lcd.print(jam);
  lcd.print(":");
  lcd.print(menit);
  lcd.print(":");
  lcd.print(detik);

  // KONDISI (atur jadwal kasih pakan)
  if ((jam == 17) && (menit == 20) && (detik == 1)) {
    kasih_pakan(10);
  }
  if ((jam == 17) && (menit == 22) && (detik == 1)) {
    kasih_pakan(10);
  }

  delay(500); // biar LCD tidak flicker
}

void kasih_pakan(int jumlah) {
  for (int i = 1; i <= jumlah; i++) {
    mekanik.write(150);
    delay(100);
    mekanik.write(0);
    delay(100);
  }
}
