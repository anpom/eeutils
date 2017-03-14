/*
 * 
 */

#include "cli.h"

microrl_t rl;
microrl_t * prl = &rl;

FIFO(64) rx_fifo;


void cli_init() {
    Serial.begin(9600);
    microrl_init(prl, cli_print);
    microrl_set_execute_callback(prl, cli_execute);
}


void cli_insert_char() {
    if (Serial.available()) {
        if(!FIFO_IS_FULL(rx_fifo)) {
            FIFO_PUSH(rx_fifo, (char)Serial.peek());
        }
        microrl_insert_char(prl, (char)Serial.read());
    }
}


void cli_print(const char * str) {
    Serial.write(str);
}


int cli_execute (int argc, const char * const * argv) {
    int i = 0;
    Serial.print("\033[2K");
    while (i < argc) {
        if (strcmp(argv[i], "help") == 0) {
            cli_print_help();
        } else if (strcmp(argv[i], "clear") == 0) {
            Serial.print("\033[2J\033[H");
        } else if (strcmp(argv[i], "rx") == 0) {
            cli_print_rx_buffer();
        } else {
            Serial.print("command: '");
            Serial.print((char*)argv[i]);
            Serial.print("' not found.\n\r");
        }
        i++;
    }
    return 0;
}


void cli_print_help() {
    Serial.print("microrl v");
    Serial.println(MICRORL_LIB_VER);    
}


void cli_print_rx_buffer() {
    Serial.println("rx buffer:");
    for (byte k = 0; k < FIFO_SIZE(rx_fifo); k++) {
        if (rx_fifo.buf[k] < 16) Serial.print("0");
        Serial.print(rx_fifo.buf[k], HEX);
        ((k + 1) & 0x0F)? Serial.print(" ") : Serial.println("");
    }
}
