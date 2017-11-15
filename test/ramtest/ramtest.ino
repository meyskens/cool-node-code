#line 2 "test2.ino"
#include <ArduinoUnit.h>

typedef struct {
    int    senderID;
    char message[5]; 
} MessagePayload;

// Used from https://learn.adafruit.com/memories-of-an-arduino/measuring-free-memory
int freeRam () {
    extern int __heap_start, *__brkval; 
    int v; 
    return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval); 
}

MessagePayload getTestStuct() {
    MessagePayload payload;

    payload.senderID = 1;
    strcpy(payload.message, "test");

    return payload;
} 

test(usagePerStruct) {
    Serial.print("Initial free RAM ");
    Serial.println(freeRam());
    MessagePayload test = getTestStuct();
    Serial.print("Free RAM after struct");
    Serial.println(freeRam());
}

test(usageInIfScope) {
    Serial.print("Initial free RAM ");
    Serial.println(freeRam());
    if (true) {
        MessagePayload test = getTestStuct();
        Serial.print("Free RAM after struct ");
        Serial.println(freeRam());
    }
    Serial.print("Free RAM after if ");
    Serial.println(freeRam());
}

test(usageInFor) {
    Serial.print("Initial free RAM  ");
    Serial.println(freeRam());
    for (int i = 0; i <30; i++) {
        MessagePayload test = getTestStuct();
        test.senderID = i;
        Serial.print("Free RAM after struct ");
        Serial.println(freeRam());
    }
    Serial.print("Free RAM after for ");
    Serial.println(freeRam());
}

void setup() {
    Serial.begin(9600);
    while(!Serial);
  }
  
void loop() {
    Test::run();
}
