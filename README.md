# Farm1 Universal Firmware

เฟิร์มแวร์บอร์ด Farm1 ที่รองรับการเปลี่ยนเซ็นเซอร์และผู้ให้บริการ Cloud ได้หลากหลาย ใช้งานร่วมกับแอพฯ Farm Configs รวมทั้งใช้งานแบบไม่มีอินเตอร์เน็ตหรือใช้งานภายในวงแลน

## ฟีเจอร์

 - [x] ตั้งค่าเชื่อมต่อ WiFi ผ่าน Smart Configs
 - [x] REST API
 - [ ] ใช้งานแบบ Offline
 - [ ] Event Triger

## อุปกรณ์ที่รองรับ

### เซ็นเซอร์อุณหภูมิอากาศ

 - [x] SHT20 (I2C)
 - [ ] SHT30 (I2C)
 - [x] XY-MD02 (RS485/Modbus RTU)
 - [ ] DHT22 (IO)
 - [x] AM2301 (IO)
 - [x] DS18B20 (IO)

### เซ็นเซอร์ความชื้นอากาศ

 - [x] SHT20 (I2C)
 - [ ] SHT30 (I2C)
 - [x] XY-MD02 (RS485/Modbus RTU)
 - [ ] DHT22 (IO)
 - [x] AM2301 (IO)

### เซ็นเซอร์ความชื้นดิน

 - [ ] Analog Soil (Analog)
 - [ ] JXBS-3001-TR (RS485/Modbus RTU)
 - [x] RS485 Soil (RS485/Modbus RTU)

### เซ็นเซอร์แสง

 - [x] BH1750 (I2C)

### อุปกรณ์เอาต์พุต

 - [ ] On board (Farm1)

### จอแสดงผล

 - [x] Farm1-LCD3.2 (TFT LCD 3.2 inch, SPI)
 - [ ] 20x4 Character LCD (I2C)

## Cloud Service

 - [ ] HandySense
 - [ ] Blynk IoT
 - [ ] Azure IoT Central
 - [ ] AWS IoT
