#include <avr/sleep.h>
#include <avr/wdt.h>
#include <SPI.h>
#include <RF24.h>
#include <RF24Network.h>
#include "DHTesp.h"

#define led 6
#define data_dht 3
#define power_sensor 2
RF24 radio(7, 10);                    // nRF24L01(+) radio attached using Getting Started board

RF24Network network(radio);          // Network uses that radio

DHTesp dht;
float humidity,temperature;
const uint16_t this_node = 01;       // Address of our node in Octal format
const uint16_t other_node = 00;      // Address of the other node in Octal format

const unsigned long interval = 2000; // How often (in ms) to send 'hello world' to the other unit

uint8_t iteration=3; 
unsigned long last_sent;             // When did we last send?
unsigned long packets_sent;          // How many have we sent already
float buffer_soil,buffer_bat, buffer_solar;

struct payload_t {                   // Structure of our payload
  unsigned long ms;
  unsigned long counter;
  float bat;
  float solar;
  float soil_val;
  float hum;
  float temp;
};
volatile int counter=0;

void setup(void) {
  pinMode(led,OUTPUT);
  pinMode(power_sensor,OUTPUT);
  digitalWrite(9,HIGH);
  dht.setup(data_dht, DHTesp::DHT11);
  Serial.begin(115200);
  if (!Serial) {
    // some boards need this because of native USB capability
  }
  Serial.println(F("RF24Network/examples/helloworld_tx/"));

  SPI.begin();
  if (!radio.begin()) {
    Serial.println(F("Radio hardware not responding!"));
    bt_sleep();
  }
  network.begin(/*channel*/ 90, /*node address*/ this_node);
}

void loop() {
  ADCSRA = 0xC7;
  digitalWrite(power_sensor,HIGH);
  delay(1500);
  network.update(); // Check the network regularly
  
  read_analog();  //fungsi baca A0-A3
  
  read_dht11();
  
  send_data();
  
  digitalWrite(led,LOW);
  digitalWrite(power_sensor,LOW);
  buffer_bat=0;
  buffer_solar=0;
  buffer_soil=0;
  bt_sleep(); //back to sleep
  
}

void read_analog()
{ 
  //for(uint8_t i=0;i<iteration;i++){
    buffer_bat+=map(analogRead(A0),0,430,0,100); //nilai max = (4.2 / 2) / 5 * 1024
    delay(30);
    buffer_solar+=map(analogRead(A1),615,0,0,100);; //nilai max = (6 / 2) / 5 * 1024 
    delay(30);
    buffer_soil+=map(analogRead(A2),0,676,0,100);; //nilai max = 3.3 / 5 * 1024
    delay(30);
  //}
  /*buffer_bat=buffer_bat/iteration;
  buffer_solar=buffer_solar/iteration;
  buffer_soil=buffer_soil/iteration;*/
}
void read_dht11()
{
  humidity = dht.getHumidity();
  temperature = dht.getTemperature();
 
  Serial.print(dht.getHumidity(), 1);
  Serial.print(",\t");
  Serial.println(dht.getTemperature(), 1);
}

void send_data()
{
  last_sent = millis();
    Serial.print("Sending...");
    payload_t payload = { millis(), packets_sent++,buffer_bat,buffer_solar,buffer_soil,humidity,temperature };
    RF24NetworkHeader header(/*to node*/ other_node);
    bool ok = network.write(header, &payload, sizeof(payload));
    if (ok){
      Serial.println("ok.");
      digitalWrite(led,HIGH);
      //delay(300);
    }
    else
      Serial.println("failed.");
}
void bt_sleep()
{ 
  radio.powerDown();
  // disable ADC
  ADCSRA = 0;
  // clear various "reset" flags
  MCUSR = 0;
  // allow changes, disable reset
  WDTCSR = bit (WDCE) | bit (WDE);
  // set interrupt mode and an interval
  WDTCSR = bit (WDIE) | bit (WDP3) | bit (WDP0);    // set WDIE, and 8 seconds delay
  wdt_reset();  // pat the dog
  
  set_sleep_mode (SLEEP_MODE_PWR_DOWN);
  noInterrupts ();           // timed sequence follows
  sleep_enable();

  // turn off brown-out enable in software
  MCUCR = bit (BODS) | bit (BODSE);
  MCUCR = bit (BODS);
  interrupts ();             // guarantees next instruction executed
  sleep_cpu ();
  // cancel sleep as a precaution
  sleep_disable();
}


ISR (WDT_vect)
{
  counter++;
  wdt_disable();  // disable watchdog
}  // end of WDT_vect
