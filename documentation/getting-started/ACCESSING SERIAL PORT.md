####ACCESSING BOARD THROUGH SERIAL PORT
---

#####MAC OSX YOSEMITE 10.10.4

* Install [USB-Serial driver](https://www.silabs.com/products/mcu/Pages/USBtoUARTBridgeVCPDrivers.aspx)

* `ls /dev/tty.*` -> Now check if a new device is visible after plugging nodemcu. This is your device of interest

* `screen /dev/tty.foo`

* For an IDE, you can use [ESPlorer](http://esp8266.ru/esplorer/)


#####LINUX UBUNTU 14.04

* No need to install any driver. Just follow steps 2 and 3 of mac osx section. ESPlorer seems to be not able to detect USB-Serial


#####WINDOWS 8

* To be updated
