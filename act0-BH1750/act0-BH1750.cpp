#include <TWELITE>
#include <NWK_SIMPLE>   // ネットワークサポート

#include "DISP7SEG.h"
DISP7SEG disp ; // SSD1306表示クラスのインスタンス

#define BH1750_ADDR 0x23  // Ambient Light Sensor BH1750のI2Cアドレス 0x23 or 0x5C
#define BH1750_MODE 0x10  // Continuously H-resolution mode

/**
 * Ambient Light Sensor BH1750 から照度を読み取る
 */
uint16_t readAmbienLight( ) {
  uint16_t sensorData = 0;
  
  // -- SEND COMMAND --
  if (auto&& wrt = Wire.get_writer(BH1750_ADDR)) {
    wrt  << BH1750_MODE ;
  } else {
    return 0 ;
  }
  delay (180) ;

  // -- Result --
  if (auto&& rdr = Wire.get_reader(BH1750_ADDR, 2)) {
    uint8_t d1 , d2 ;
    rdr >> d1;
    rdr >> d2;
    sensorData = d1 << 8 | d2 ;
  } else {
    return 0;
  }
  sensorData = sensorData / 1.2 ;
  return sensorData ;
}

/*** the setup procedure (called on boot) */
void setup() {
  Wire.begin(); // ココ、きちんと初期化しないと無線モジュールがハングアップする。
  delay(1000) ; // I2C デバイスが立ち上がるまでの待ち時間

  disp.init() ;

  // --- 初期表示 88888Lx
  disp.disp7SEG(0,2,13) ;
  disp.disp7SEG(24,0,12) ;
  disp.disp7SEG(40,0,12) ;
  disp.disp7SEG(56,0,12) ;
  disp.disp7SEG(72,0,12) ;
  disp.disp7SEG(88,0,12) ;
  disp.dispFont16(112,4,'L') ;
  disp.dispFont16(120,4,'x') ;
  delay(1000) ;
}

/*** the loop procedure (called every event) */
void loop() {
  // --- 明るさ
  uint16_t sensorData = readAmbienLight( ) ;

  // --- 表示用データに変換
  uint8_t diapData[5] ;
  for (int i=0;i<5;i++) {
    diapData[i] = sensorData % 10 ;
    sensorData /= 10 ;
  }

  // --- 照度表示
  disp.disp7SEG(24,0,diapData[4]) ;
  disp.disp7SEG(40,0,diapData[3]) ;
  disp.disp7SEG(56,0,diapData[2]) ;
  disp.disp7SEG(72,0,diapData[1]) ;
  disp.disp7SEG(88,0,diapData[0]) ;

  delay(1000);
}