#ifndef AM2301B_H
#define AM2301B_H

class AM2301B
{
  private:
    int humidity ;   // 湿度
    float temperature; // 温度

  public:
    int getHumidity( ) { return humidity; }  // 湿度
    int getTemperature( ) { return (int)temperature ;}   // 温度

    void init( ) ;
    bool readSencer( ) ;
} ;

#endif
