// 温湿度センサー AM2301B I2C アクセステスト
#include <TWELITE>
#include <NWK_SIMPLE>// ネットワークサポート

#include "DISP7SEG.h"
#include "AM2301B.h"

#define DO3_PIN 9
#define DO4_PIN 12

DISP7SEG display ;
AM2301B Senser ;

void setup() {
  Wire.begin(); // ココ、きちんと初期化しないと無線モジュールがハングアップする。
  Serial.begin() ;
  delay(1000) ; // I2C デバイスが立ち上がるまでの待ち時間

  display.init() ;
  Senser.init() ;

  display.disp7SEG(0,0,11) ;
  display.disp7SEG(72,0,12) ;
  display.disp7SEG(0,4,13) ;
  display.disp7SEG(72,4,14) ;
}

void loop() {
  if(Senser.readSencer( )) {
    int dispBuff[3] ;
    int data ;
    // --- temperature
    data = Senser.getTemperature( ) ;
    dispBuff[2] = data % 10 ; data /= 10 ;
    dispBuff[1] = data % 10 ;
    if (dispBuff[1] == 0) { dispBuff[1] = 16 ; }
    dispBuff[0] = (data < 0) ? 17 : 16 ; 
    for (int i=0;i<3;i++) {
      display.disp7SEG(20+i*16,0,dispBuff[i]) ;
    }
    // --- humidity
    data = Senser.getHumidity( ) ;
    dispBuff[2] = data % 10 ; data /= 10 ; 
    dispBuff[1] = data % 10 ; data /= 10 ; 
    dispBuff[0] = data % 10 ; data /= 10 ;
    if (dispBuff[0] == 0) {
      dispBuff[0] = 16 ;
      if (dispBuff[1] == 0) {
        dispBuff[1] = 16 ;
      }
    }
    for (int i=0;i<3;i++) {
      display.disp7SEG(20+i*16,4,dispBuff[i]) ;
    }
  }
  delay(5000) ;
}
