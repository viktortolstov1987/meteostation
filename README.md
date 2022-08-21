# meteostation
Arduino-based device for air temprature, pressure and humidity tracking with Python application for visualisation

В представленной конструкции использованы следующие комплектующие:
-	контроллер Arduino Uno 
-	датчик MW33 (можно заменить на оригинал DHT11 без изменения программы) https://aliexpress.ru/item/583643121.html
-	дисплей 128х32, 0.91” https://aliexpress.ru/item/32672229793.html
-	макетный щит https://ru.aliexpress.com/item/32787572265.html 

Устройство для измерения температуры. влажности и давления воздуха.
Вариант с датчиком DHT11/MW33 измеряет только температуру и влажность.
Для программирования в среде Arduino IDE необходимы библиотеки:
-	DHT11 https://github.com/adafruit/DHT-sensor-library 
-	Adafruit Sensor https://github.com/adafruit/Adafruit_Sensor 
-	Adafruit BME_280_lib https://github.com/adafruit/Adafruit_BME280_Library
-	библиотека дисплея U8g2 + инструкция по подключению
https://www.instructables.com/id/Tutorial-to-Interface-OLED-091inch-128x32-With-Ard/ 

Приложение на Python может принимать данные из Arduino и строить график.
Для Python 3 необходимо установить:
-	библиотека COM-порта pyserial https://pyserial.readthedocs.io/en/latest/pyserial.html#installation 
-	библиотека графиков matplotlib https://matplotlib.org/index.html 
