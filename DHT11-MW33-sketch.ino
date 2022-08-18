#include "DHT.h"          //библиотека датчика
#include <Wire.h>         //для дисплея
#include <U8g2lib.h>
#include <SPI.h>

#define DHTPIN 2                            // номер пина, к которому подключен data датчика
DHT dht(DHTPIN, DHT11);                                 //Инициализация датчика
U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C u8g2(U8G2_R0);   //дисплей 

void setup() {
  Serial.begin(9600);   // обмен данными с компьютером
  dht.begin();          // запуск датчика
  u8g2.begin();;        // запуск дисплея
  u8g2.clearBuffer();                     // очистка дисплея
  u8g2.setFont(u8g2_font_helvB12_tf);     //один из шрифтов 12px высотой
  u8g2.setCursor(6, 16);
  u8g2.print("Waiting data");
  u8g2.setCursor(6, 32);
  u8g2.print("from sensor");  
  u8g2.sendBuffer();                      // отображение растра
}
void loop() {
  delay(5000);                            // 5 секунд задержки
  float h = dht.readHumidity();           //Измеряем влажность
  float t = dht.readTemperature();        //Измеряем температуру
  if (isnan(h) || isnan(t)) {              // Проверка. Если не удается считать показания, выводится «Ошибка считывания», и программа завершает работу
    u8g2.clearBuffer();              
    u8g2.setCursor(6, 16);
    u8g2.print(" ERROR");
    u8g2.setCursor(6, 32);
    u8g2.print("no data"); 
    u8g2.sendBuffer();               
    return;
  }
  // отображение на дисплее
  u8g2.clearBuffer();              
  u8g2.setCursor(1, 16);
  u8g2.print("Temp : " + String(t) + " *C");    // первая строка
  u8g2.setCursor(1, 32);
  u8g2.print("Humid: " + String(h) + " %");    // вторая строка 
  u8g2.sendBuffer();               
  //передача 4-х чисел: целая и дробная части h и t
  //дробные части умножим на 100 и +1
  byte tz = byte(t);
  byte tf = byte((t-tz)*100+1);
  byte hz = byte(h);
  byte hf = byte((h-hz)*100+1);
  //!!! пока что предполагается использование системы 
  //!!! только при положительных температурах
  //разделяем значения пробелами
  Serial.print(String(tz) + " ");
  Serial.print(String(tf) + " ");
  Serial.print(String(hz) + " ");
  Serial.println(String(hf)); 
}
