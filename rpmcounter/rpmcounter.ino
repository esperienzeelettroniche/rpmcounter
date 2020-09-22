#include <U8g2lib.h>

// Change DISPLAY_TYPE according to your model
// See full list at https://github.com/olikraus/u8g2/wiki/u8g2setupcpp
#define DISPLAY_TYPE U8G2_SH1106_128X64_NONAME_1_HW_I2C

DISPLAY_TYPE u8g2(U8G2_R0);

#define TCCR1B_CAPTURE_RISING 0x07
#define TCCR1B_CAPTURE_FALLING 0x06
#define TCCR1B_OFF 0x00

// To obtain RPM we have to multiply the sensor reading by 60
// and divide by the number of teeth of the sensor wheel.
// For a 15 teeth wheel use TCNT1_MULTIPLIER = (60 / 15) = 4
#define TCNT1_MULTIPLIER 4

void setup() {
  // Set TIMER1 normal mode operation
  TCCR1A = 0x00;
  TCCR1B = 0x00;

  pinMode(5, INPUT);

  u8g2.begin();
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_logisoso28_tr);
  
  // This is for testing purpose. Connecting pin 3 to pin 5 you should read 1960
  analogWrite(3, 127);
}

void loop() {
  long speed = 0;
  
  // Reset TIMER1 count
  TCCR1B = TCCR1B_OFF;
  TCNT1 = 0;

  // Enable TIMER1 external clock input (D5)
  TCCR1B = TCCR1B_CAPTURE_RISING;
  // Wait 1 second
  delay(1000);
  // Disable TIMER1 external clock input
  TCCR1B = TCCR1B_OFF;

  // Calculate speed
  speed = ( TCNT1 * TCNT1_MULTIPLIER );
  
  // Display the result
  u8g2.firstPage();
  do {
    u8g2.setCursor(0, 28);
    u8g2.print("RPM");
    u8g2.setCursor(0, 64);
    u8g2.print(speed);
  } while( u8g2.nextPage() );
}
