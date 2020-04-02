/*!
 * @file DFRobot_MY9221SS.h
 * @brief Define the basic structure of class DFRobot_MY9221SS
 *
 * @copyright   Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @licence     The MIT License (MIT)
 * @author [YeHangYu](hangyu.ye@dfrobot.com)
 * @version  V1.0
 * @date  2020-03-30
 * @https://github.com/DFRobot/DFRobot_MY9221SS
 */
#include "DFRobot_MY9221SS.h"

DFRobot_MY9221SS::DFRobot_MY9221SS(void)
{

}
void DFRobot_MY9221SS::begin(uint32_t clockPin, uint32_t dataPin)//初始化
{
  _dataPin = dataPin;
  _clockPin = clockPin;
  pinMode(_clockPin, OUTPUT);
  pinMode(_dataPin, OUTPUT);
  //设置模式
  setMode();
}
void DFRobot_MY9221SS::sendCmd(uint16_t bits)//发送16位CMD命令
{
  for (uint8_t i = 0, clk = 1; i < 16; i++) {
    digitalWrite(_dataPin, bits & 0x8000 ? HIGH : LOW);//数据发送
    digitalWrite(_clockPin, clk);//时钟上升沿下降沿，数据有效
    clk = 1-clk;
    bits <<= 1;
  }
}
void DFRobot_MY9221SS::sendData(uint16_t bits)//每次调用发送16位数据
{
  for (uint8_t i = 0, clk = 1; i < 16; i++) {
    digitalWrite(_dataPin, bits & 0x8000 ? HIGH : LOW);//数据发送
    digitalWrite(_clockPin, clk);//时钟上升沿下降沿，数据有效
    clk = 1-clk;
    bits <<= 1;
  }
  
}

void DFRobot_MY9221SS::setMode(uint8_t temp, uint8_t hspd, uint8_t bs, uint8_t gck, uint8_t sep, uint8_t osc, uint8_t pol, uint8_t cntset, uint8_t onest)//模式设置
{
  sMode_t  mode;
  mode.temp = temp;
  mode.hspd = hspd;//输出电流设置为慢速可能会出现问题
  mode.bs = bs;
  mode.gck = gck;
  mode.sep = sep;
  mode.osc = osc;
  mode.pol = pol;
  mode.cntset = cntset;
  mode.onest = onest;
  _mode = *((uint16_t*)&mode);
}


void DFRobot_MY9221SS::write(uint16_t* buf)//向芯片发送所有数据
{
  sendCmd(_mode); //发送命令，选择模式
  for (uint8_t i = 0; i < 12; i++) { //发送灰阶数据，从A3引脚的buf开始发
    sendData(buf[i]);
  }
  //所有数据发送完后发送锁存信号，灰阶资料和命令锁存后自动发给驱动器使LED灯工作
  digitalWrite(_dataPin, LOW);
  digitalWrite(_clockPin, HIGH);
  digitalWrite(_clockPin, LOW);
  digitalWrite(_clockPin, HIGH);
  digitalWrite(_clockPin, LOW);
  delayMicroseconds(240);
  digitalWrite(_dataPin, HIGH);
  digitalWrite(_dataPin, LOW);
  digitalWrite(_dataPin, HIGH);
  digitalWrite(_dataPin, LOW);
  digitalWrite(_dataPin, HIGH);
  digitalWrite(_dataPin, LOW);
  digitalWrite(_dataPin, HIGH);
  digitalWrite(_dataPin, LOW);
  delayMicroseconds(1);
  digitalWrite(_clockPin, HIGH);
  digitalWrite(_clockPin, LOW);
  return;
}

void DFRobot_MY9221SS::setLed(uint8_t ledNo, uint16_t R, uint16_t G, uint16_t B)//用RGB888设置某一个RGB灯的颜色
{
  uint16_t  buf[LED_PIN_COUNT];
  ledNo = 5 - ledNo;
  if(ledNo <= 4 || ledNo >= 1) {
    for(uint8_t i = 0; i < LED_PIN_COUNT; i++) {
      if(i == ledNo*3-3) {
        buf[i] = G ;
      } else if(i == ledNo*3-2) {
        buf[i] = R ;
      } else if(i == ledNo*3-1) {
        buf[i] = B ;
      } else {
        buf[i] = LED_TURN_OFF;
      }
    }
    write(buf);//向芯片写入数据，12个引脚，每个16bit
  } else {
    Serial.println("ledNo error");
  }
}

void DFRobot_MY9221SS::setAllLed(uint16_t R, uint16_t G, uint16_t B)//用RGB888设置所有RGB灯的颜色
{
  uint16_t  buf[LED_PIN_COUNT];
  for(uint8_t i = 0; i < LED_PIN_COUNT; i++) {
    if(i%3 == 0) {
      buf[i] = G ;
    } else if(i%3 == 1) {
      buf[i] = R ;
    } else if(i%3 == 2) {
      buf[i] = B ;
    }
  }
  write(buf);//向芯片写入数据，12个引脚，每个16bit
}

void DFRobot_MY9221SS::autoColorChange(void)//所有RGB灯，随机颜色
{
  uint16_t  buf[LED_PIN_COUNT];
  uint16_t R = rand()%255; 
  uint16_t G = rand()%255; 
  uint16_t B = 255 - (R + G)/2;
  for(uint8_t i = 0; i < LED_PIN_COUNT; i++) {
    if(i%3 == 0) {
      buf[i] = G ;
    } else if(i%3 == 1) {
      buf[i] = R ;
    } else if(i%3 == 2) {
      buf[i] = B ;
    }
  }
  write(buf);//向芯片写入数据，12个引脚，每个16bit
}

void DFRobot_MY9221SS::setSinglePin(uint8_t pinNo, uint16_t brightness)//设置单个引脚的亮度
{
  uint16_t  buf[LED_PIN_COUNT];
  pinNo = 11 - pinNo;
  if(pinNo <= 11 || pinNo >= 0) {
    for(uint8_t i = 0; i < LED_PIN_COUNT; i++) {
      if(i == pinNo) {
        buf[i] = brightness ;
      } else {
        buf[i] = LED_TURN_OFF;
      }
    }
    write(buf);//向芯片写入数据，12个引脚，每个16bit
  } else {
    Serial.println("pinNo error");
  }
}

void DFRobot_MY9221SS::setPins(uint16_t bits, uint16_t brightness)//改用12位二进制指定引脚，控制对应引脚亮度，最高位是C0，最低位是A11
{
  uint16_t  buf[LED_PIN_COUNT];
  for(uint8_t i = 0; i < LED_PIN_COUNT; i++) {
    if(bits & 0x0001) {
      buf[i] = brightness;
    } else {
      buf[i] = LED_TURN_OFF;
    }
    bits>>=1;
  }
  write(buf);//向芯片写入数据，12个引脚，每个16bit
}