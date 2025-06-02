# OpenStill

Project for controlling still. Developed for ESP32.

[!["Buy Me A Coffee"](https://www.buymeacoffee.com/assets/img/custom_images/orange_img.png)](https://buycoffee.to/stefunk)


## Functions

* Controlling power of the heater via SSR (0-100%) 
* Shows temperature from 4 sensors (DS18B20) on 20x4 I2C LCD screen
* Show if temperature is rising or dropping
* Show ABV of vapour in head
* Show amount of alcohol left in tank
* Sensor temperature limit - turns off heater when temperature on sensor is reached
* CSV data export with temperatures
* Initial Wifi access point mode for wifi configuration
* Web server with:
  * live temperatures (1s refresh)
  * heater control
  * live charts
  * notifications
  * heater off temperature limit
  * temperature of the day for 10 shelf notifications
  * sensor assignment
  * csv log download
  * csv file reset

## Hardware

* Project is developed on ESP32
* 4 sensors DS18B20 in parasite mode connected to pin D7 with 4.7K resistor
* SSR connected to D4
* I2C 20x4 LCD - [how to connect](https://www.google.com/search?q=i2c+lcd+esp32&tbm=isch)

## Dependencies

* Developed using [PlatformIO IDE](https://platformio.org/)
* [TaskManagerIO](https://github.com/davetcc/TaskManagerIO)
* [LiquidCrystalIO](https://github.com/davetcc/LiquidCrystalIO)
* [OneWire](https://github.com/PaulStoffregen/OneWire)
* [DallasTemperature](https://github.com/milesburton/Arduino-Temperature-Control-Library)
* [IoAbstraction](https://github.com/davetcc/IoAbstraction)
* [ESPAsyncWebServer](https://github.com/me-no-dev/ESPAsyncWebServer/)
* ArduinoJson

## License
[MIT](https://choosealicense.com/licenses/mit/)

![Screen shot](https://i.ibb.co/StTxN0J/termo.png)



