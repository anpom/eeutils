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
    Serial.println("\033[0m");
    while (i < argc) {
        if (strcmp(argv[i], "help") == 0) {
            cli_help();
        } else if (strcmp(argv[i], "clear") == 0) {
            cli_clear();
        } else if (strcmp(argv[i], "diag") == 0) {
            cli_diag();
        } else {
            Serial.print("Command '\033[33m");
            Serial.print((char*)argv[i]);
            Serial.print("\033[0m' not found. ");
            Serial.println("Use '\033[33mhelp\033[0m' to see command list");
            return 0;
        }
        i++;
    }
    return 0;
}


void cli_help() {
    Serial.println("Available commands:");
    Serial.println("  \033[33mhelp\033[0m    - show command list");
    Serial.println("  \033[33mclear\033[0m   - clear screen");
    Serial.println("  \033[33mdiag\033[0m    - diagnostic function");
}


void cli_clear(){
    Serial.print("\033[2J\033[H");
}


void cli_diag() {
    char ch;
    Serial.print("uart RX buffer (");
    Serial.print(FIFO_SIZE(rx_fifo));
    Serial.println("bytes):");
    for (byte i = 0; i < FIFO_SIZE(rx_fifo) / 16; i++) {
        // вывод hex представления буфера
        Serial.print("  ");
        for (byte k = 0; k < 16; k++) {
            ch = rx_fifo.buf[i*16+k];
            if (ch < 16) Serial.print("0");
            Serial.print(ch, HEX);
            Serial.print(" ");
            if (k == 7) Serial.print(" ");
        }
        // вывод символьного представления буфера
        Serial.print(" | ");
        for (byte k = 0; k < 16; k++) {
            ch = rx_fifo.buf[i*16+k];
            if (ch < 32 || ch > 126) ch = '.';
            Serial.print(ch);
            if (k == 7) Serial.print(" ");
        }
        Serial.println(" |");
    }
}
