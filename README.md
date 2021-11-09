# Smart-pot
Making a smart pot with Esp-32 

## Components Required
1. [ESP32 Wroom32 Module](https://www.amazon.in/dp/B08VJ2CKG1/ref=cm_sw_r_apan_glt_fabc_4EYKEFVJJVWBK3XV0ZNQ)  
2. [Wemos D1 Mini charging Shield](https://www.amazon.in/Battery-Shield-V1-2-0-Lithium-Charging/dp/B09FH2YW7L?dchild=1&keywords=wemos+d1+mini+charging&qid=1633947211&sr=8-1&linkCode=sl1&tag=wwwtechiesmsc-21&linkId=d8bbfacd1afa6fbaab014cb34b7724e8&language=en_IN&ref_=as_li_ss_tl)  
3. [8in TFT Touch Screen with SD card reader](https://www.amazon.in/dp/B08B1TH5RG/ref=cm_sw_r_apan_glt_fabc_3QB23C532V8MN6PVK6X2)  
4. [Soil Moisture Sensor](https://www.amazon.in/dp/B07FM41J4F/ref=cm_sw_r_apan_glt_fabc_T6S0DVZ65EVSWFGG9CHC?_encoding=UTF8&psc=1)  
5. [DHT11 Sensor](https://www.amazon.in/REES52-Digital-Temperature-Humidity-Compatible/dp/B01MXRT9DZ?crid=U57J52B0Y3IC&dchild=1&keywords=dht11+sensor&qid=1634983118&sprefix=DHT11,aps,895&sr=8-4&linkCode=sl1&tag=wwwtechiesmsc-21&linkId=2b4ea807d558d6a71ea31f8d428aaaba&language=en_IN&ref_=as_li_ss_tl)  
6. [LDR Sensor](https://www.amazon.in/SENSOR-GENUINE-DEPENDENT-RESISTOR-PHOTORESISTOR/dp/B07B8PH4Y2?crid=2O0V25RVUDZR3&dchild=1&keywords=ldr+sensor&qid=1634983183&sprefix=ldr+se,aps,286&sr=8-3&linkCode=sl1&tag=wwwtechiesmsc-21&linkId=43ef27e0520cb1e6130aef03d418756f&language=en_IN&ref_=as_li_ss_tl)  
7. On Off Switch  
8. Pushbuttons x 2    
9. Rechargeable LiPo Battery (1000 mAh)  
10. [4GB Micro SD Card](https://www.amazon.in/Strontium-Micro-SDHC-Class-6-Memory/dp/B00IMDYQPW?crid=1IINHCK7SNRPR&dchild=1&keywords=4gb+memory+card&qid=1634984322&sprefix=4gb+memo,aps,803&sr=8-3&linkCode=sl1&tag=wwwtechiesmsc-21&linkId=c72f8041e6d6bae89d48fe64efcd4d5f&language=en_IN&ref_=as_li_ss_tl)  
11. [SD card Adapter](https://www.amazon.in/Zeffcon-MicroSD-Memory-Adapter-Converter/dp/B0774VLSGM?dchild=1&keywords=SD+card+adapter&qid=1634984460&sr=8-3&linkCode=sl1&tag=wwwtechiesmsc-21&linkId=62cc4f2291b304373c0936faafcb69a1&language=en_IN&ref_=as_li_ss_tl)  
12. [3 pin Male-Female connectors x 2](https://www.amazon.in/dp/B0837WKFFH/ref=cm_sw_r_apan_glt_fabc_XHA6B9HS1JDZTY9KQ448)  
13. [2 pin Male-Female connectors x 2](https://www.amazon.in/dp/B0837W2VTN/ref=cm_sw_r_apan_glt_fabc_VW8JNQES21JE1QNBNAQH?_encoding=UTF8&psc=1)  
14. 10k Resistor x 3  
15. 47k Resistor x 2  
16. [HT7333 LDO](https://www.amazon.in/Chanzon-HT7333-1-SOT-89-Consumption-Transistor/dp/B08M3937KX?dchild=1&keywords=ht7333&qid=1634983612&sr=8-1&linkCode=sl1&tag=wwwtechiesmsc-21&linkId=5a65f79e5154f9dbc8790c68058598ad&language=en_IN&ref_=as_li_ss_tl)

## ESP32 WROOM32 DevKit Pinout
<p align="center"><img src="https://i.ibb.co/jz7wfMz/ESP32-Pinout.jpg"></p>

## Code requirments

Before you use the code, you first need to download & install several libraries whose links are mentioned below.

1. [TFT_eSPI](https://github.com/Bodmer/TFT_eSPI)  
2. [Pangodream_18650_CL](https://github.com/pangodream/18650CL)  
3. [DHT](https://github.com/adafruit/DHT-sensor-library)  
4. [JPEGDecoder](https://github.com/Bodmer/JPEGDecoder)  

After Downloading and installing all the libraries, you need to do one small change to support the libraries for our ESP32 module. For that, just open the folder containing all the installed libraries of Arduino.

In this just open up TFT_eSPI folder and delete User_Setup.h file

![file](https://i.ibb.co/B43jP04/Screenshot-2021-11-08-185507.jpg)
After deleting this file, just download the new User_Setup.h file by clicking here and paste this [new file](https://techiesms.com/wp-content/uploads/2021/10/User_Setup.h) in the same folder as shown above
## How to install ESP-32 boards

1. Attact below links in preferances.
```
https://dl.espressif.com/dl/package_esp32_index.json
http://arduino.esp8266.com/stable/package_esp8266com_index.json
```
2. Go to tools --> Board --> Board Manager --> serch ESP --> Download ESP related.
3. The installation process is finished.

## Board selection

![file](https://i.ibb.co/1XQLWZM/Screenshot-3.png)

**Hit the upload button**

## Circuit diagram
