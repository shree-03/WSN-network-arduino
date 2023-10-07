#include <SPI.h>
#include <RF24.h>
#include <RF24Network.h>

RF24 radio(7, 8);  // nRF24L01(+) radio attached using Getting Started board

RF24Network network(radio);  // Network uses that radio

const uint16_t this_node = 00;   // Address of our node in Octal format
const uint16_t other_node_1 = 01;  // Address of the other node in Octal format
const uint16_t other_node_2= 02;
const uint16_t other_node_3= 03;

const unsigned long interval = 60000;  // How often (in ms) to send 'hello world' to the other unit

unsigned long last_sent;     // When did we last send?
unsigned long packets_sent[4]={0,0,0,0};  // How many have we sent already


struct payload_t {  // Structure of our payload
  unsigned long ms;
  unsigned long counter;
};

void setup(void) {
  Serial.begin(115200);
  while (!Serial) {
    // some boards need this because of native USB capability
  }
  Serial.println(F("COORDINATOR\n"));

  if (!radio.begin()) {
    Serial.println(F("Radio hardware not responding!"));
    while (1) {
      // hold in infinite loop
    }
  }
  radio.setChannel(90);
  network.begin(/*node address*/ this_node);
}

void loop() {

  network.update();  // Check the network regularly

  unsigned long now = millis();
  unsigned long t;
  
  // If it's time to send a message, send it!
  if (now - last_sent >= interval) {
    last_sent = now;
  
    payload_t payload_1 = { millis(),++packets_sent[1]};
    Serial.print(F("\nHELLO FROM COORDINATOR to NODE 1,packet number="));
    Serial.print(payload_1.counter);
    RF24NetworkHeader header_1(/*to node*/ other_node_1);
    bool ok_1 = network.write(header_1, &payload_1, sizeof(payload_1));
    Serial.println(ok_1 ? F("\nACK RECEIVED from Node 1") : F("\nACK NOT RECEIVED from Node 1"));

    delay(15000);
    payload_t payload_2 = { millis(), ++packets_sent[2] };
    Serial.print(F("\nHELLO FROM COORDINATOR to NODE 2,packet number="));
    Serial.print(payload_2.counter);
    RF24NetworkHeader header_2(/*to node*/ other_node_2);
    bool ok_2 = network.write(header_2, &payload_2, sizeof(payload_2));
    Serial.println(ok_2 ? F("\nACK RECEIVED from Node 2") : F("\nACK NOT RECEIVED from Node 2"));

    delay(30000);
    payload_t payload_3 = { millis(), ++packets_sent[3] };
    Serial.print(F("\nHELLO FROM COORDINATOR to NODE 3,packet number="));
    Serial.print(payload_3.counter);
    RF24NetworkHeader header_3(/*to node*/ other_node_3);
    bool ok_3 = network.write(header_3, &payload_3, sizeof(payload_3));
    Serial.println(ok_3 ? F("\nACK RECEIVED from Node 3") : F("\nACK NOT RECEIVED from Node 3"));

    Serial.print(F("\n****************Set of transmission done="));
    Serial.print(++packets_sent[0]);
    Serial.print(F("************************\n\n"));
  }
}
