/*!
 * @file blink.ino
 * @brief 所有灯闪烁，基色B对应引脚A0~A3、基色R对应引脚B0~B3、基色G对应引脚C0~C3，请将RGB灯的引脚按GRB顺序连接Ax、Bx、Cx
 * @n 本示例支持的主板有ESP8266、FireBeetle-M0、UNO、ESP32、Leonardo 、Mega2560
 * @copyright  Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @licence     The MIT License (MIT)
 * @author [YeHangYu](hangyu.ye@dfrobot.com)
 * @version  V0.1
 * @date  2020-03-20
 * @url https://github.com/DFRobot/DFRobot_MY9221SS
 */
#include <DFRobot_MY9221SS.h>

//自定义通信引脚
/*FireBeetle-M0*/
#if defined ARDUINO_SAM_ZERO
#define DATA_PIN   7
#define CLK_PIN    5
/*ESP32 and ESP8266*/
#elif defined(ESP32) || defined(ESP8266)
#define DATA_PIN   D3
#define CLK_PIN    D4
/*AVR系列主板*/
#else
#define DATA_PIN   2
#define CLK_PIN    3
#endif

/**
 * @brief Constructor LED驱动构造函数
 */
DFRobot_MY9221SS rgbDriver; 

/*
 *供用户使用的宏定义LED灯名
 *LED0 LED1 LED2 LED3
 */
void setup() {
  //初始化串口
  Serial.begin(115200);
  /**
  * @brief  初始化LED驱动
  * @param clockPin 时钟引脚
  * @param dataPin  数据引脚
  */
  rgbDriver.begin(/*clockPin=*/CLK_PIN, /*dataPin=*/DATA_PIN);
}

void loop() {
  /**
   * @brief 指定LED灯，并通过RGB各分量控制颜色  
   * @param ledNo 宏定义灯名，一共四路/颗灯，LED0~LED3
   * @param R     设置RGB红色分量，硬件应连接引脚B，8位灰阶数据模式取值范围为0~255，16位时取值范围0~65535
   * @param G     设置RGB绿色分量，硬件应连接引脚A，8位灰阶数据模式取值范围为0~255，16位时取值范围0~65535
   * @param B     设置RGB蓝色分量，硬件应连接引脚C，8位灰阶数据模式取值范围为0~255，16位时取值范围0~65535
  */
  rgbDriver.setRgbLeds(/*ledNo=*/LED0|LED1|LED2|LED3,/*R=*/0xff,/*G=*/0xff,/*B=*/0xff);
  delay(200);
  //关灯
  rgbDriver.setRgbLeds(/*ledNo=*/LED0|LED1|LED2|LED3,/*R=*/0,/*G=*/0,/*B=*/0);
  delay(200);
}






  
