# OpenStill

Project for controlling still. Developed for ESP8266 (but should work for other devices). 


## Functions

* Controlling power of the heater via SSR (0-100%) 
* Shows temperature from 4 sensors (DS18B20) on 20x4 I2C LCD screen
* Show if temperature is rising or dropping
* Show ABV of vapour in head
* Show amount of alcohol left in tank

## Dependencies

* Developed using [PlatformIO IDE](https://platformio.org/)
* [TaskManagerIO](https://github.com/davetcc/TaskManagerIO)
* [LiquidCrystalIO](https://github.com/davetcc/LiquidCrystalIO)
* [OneWire](https://github.com/PaulStoffregen/OneWire)
* [DallasTemperature](https://github.com/milesburton/Arduino-Temperature-Control-Library)
* [IoAbstraction](https://github.com/davetcc/IoAbstraction)

## License
[MIT](https://choosealicense.com/licenses/mit/)
