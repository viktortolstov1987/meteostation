#include <Adafruit_BME280.h>         //библиотека датчика
#include <Wire.h>                     //для дисплея
#include <U8g2lib.h>
#include <SPI.h>

#define BME_SCK 13
#define BME_MISO 12
#define BME_MOSI 11
#define BME_CS 10
Adafruit_BME280 bme(BME_CS, BME_MOSI, BME_MISO, BME_SCK); // software SPI

U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C u8g2(U8G2_R0);   //дисплей 

void setup() {
  bme.begin();      // запуск датчика
  u8g2.begin();;    // запуск дисплея
  u8g2.clearBuffer();             // очистка дисплея
  u8g2.setFont(u8g2_font_helvB12_tf);   //один из шрифтов 12px высотой
  u8g2.setCursor(6, 16);
  u8g2.print("Waiting data");
  u8g2.setCursor(6, 32);
  u8g2.print("from sensor");  
  u8g2.sendBuffer();              // отображение растра
  if(Serial){
    Serial.end();
  }
  Serial.begin(9600);   // обмен данными с компьютером
}

int numToSend = 0;  // количество измерений для отправки в компьютер
boolean displayPressure = false;       //показывать давление
long flashTime = 0;                   //метка времени смены цвета
#define FLASH_DURATION 1000            //интервал смены содержимого

void loop() {
  delay(5000); // 5 секунд задержки
  //обрабатывать запрос на количество измерений только в том случае,
  //если система не в режиме передачи данных
  if(Serial.available()){
    if(numToSend==0){       // система не передаёт данные
      char inChar = Serial.read();
      switch(inChar){
      case 'a':
        numToSend = 10;
        break;
      case 'b':
        numToSend = 20;
        break;
      case 'c':
        numToSend = 50;
        break;
      }
    } else {                // система передаёт данные
      if(Serial.read()=='x'){
        numToSend = 0;      // передача остановится
      }
    }
  }
  float h = bme.readHumidity();     //Измеряем влажность
  float t = bme.readTemperature();  //Измеряем температуру
  float p = bme.readPressure() / 133.3223684F;     //Измеряем давление в mmHg
  if (isnan(h) || isnan(t)) {  // Проверка. Если не удается считать показания, выводится «Ошибка считывания», и программа завершает работу
    //Serial.println("Ошибка считывания");
    u8g2.clearBuffer();             // 
    u8g2.setCursor(6, 16);
    u8g2.print(" ERROR");
    u8g2.setCursor(6, 32);
    u8g2.print("no data"); 
    u8g2.sendBuffer();              // 
    return;
  }

  // отображение на дисплее
  u8g2.clearBuffer();             // 
  if(numToSend > 0){              // отображение на дисплее при передаче данных
    u8g2.setCursor(1, 16);
    u8g2.print("Sending data");   
    u8g2.setCursor(1, 32);
    u8g2.print(String(numToSend) + " datasets left");   
  } else {                        // отображение на дисплее в автономном режиме
    if(displayPressure){
      u8g2.setCursor(1, 16);
      u8g2.print("Pressure:");    
      u8g2.setCursor(1, 32);
      u8g2.print(String(p) + " mmHg");      
    } else {
      u8g2.setCursor(1, 16);
      u8g2.print("Temp : " + String(t) + " *C");    
      u8g2.setCursor(1, 32);
      u8g2.print("Humid: " + String(h) + " %");    
    }
    if(millis() > (flashTime + FLASH_DURATION)){
      displayPressure = !displayPressure;
      flashTime = millis() + FLASH_DURATION; 
    }
  }
  u8g2.sendBuffer();              // 
  
  if(numToSend > 0){
    //передача 4-х чисел: целая и дробная части h и t
    //дробные части умнож на 100 и +1
    byte tz = byte(t);
    byte tf = byte((t-tz)*100+1);
    byte hz = byte(h);
    byte hf = byte((h-hz)*100+1);
    byte pm = byte(p/255);            //множитель
    byte pz = byte(p-pm*255);         //целая часть остатка
    byte pf = byte((pm*255-pz)*100+1); //дробная часть остатка
    //!!! пока что предполагается использование системы 
    //!!! только при положительных температурах
    //разделяем значения пробелами
    Serial.print(String(tz) + " ");
    Serial.print(String(tf) + " ");
    Serial.print(String(hz) + " ");
    Serial.print(String(hf) + " ");
    Serial.print(String(pm) + " ");
    Serial.print(String(pz) + " ");
    Serial.println(String(pf)); 
    numToSend--;
  }
}
