#line 2 "test2.ino"
#include <ArduinoUnit.h>

bool compare(char data[], char compare[], int len) {
    for (int i = 0; i < len; i++){
        if (data[i] != compare[i]) {
            return false;
        }
    }
    return true;
}

test(compareSucceed) {
    char test1[] = "KEY1234";
    char test2[] = "KEY";
    bool ok = compare(test1, test2, 3);
    assertTrue(ok);
}

test(compareFalse) {
    char test1[] = "KOY1234";
    char test2[] = "KEY";
    bool ok = compare(test1, test2, 3);
    assertFalse(ok);
}

test(concatStrint) {
    char test1[] = "OK";
    char out[5];
    sprintf(out, "%s%", "KEY", publicKey);

    assertEqual(out, "KEYOK");
}

void setup() {
    Serial.begin(9600);
    while(!Serial);
}
  
void loop() {
    Test::run();
}
