
#include <Arduino.h>
#include "cli.h"


void setup() {
    cli_init();
}


void loop() {
    
}


void serialEvent() {
    cli_insert_char();
}
