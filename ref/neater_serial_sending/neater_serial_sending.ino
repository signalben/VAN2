#include <message.h>
#include <ringbuff.h>

Message msg;


struct vanPort {
  private:
  public:
  //int a;
  HardwareSerial(uint8_t);// refSer;
//    void func( uint8_t d)
//    {
//      refSer.write(d);
//    }
};
typedef struct vanPort VanPort;

VanPort port0;




void setup() {
  Serial.begin(115200);
}

void loop() {
  port0.a = 0;
  uint8_t c = 'q';
  delay(1000);
  port0.func(c);
}
