/* 
 * File:   cli.h
 * Author: ANPom
 *
 * Created on 14 марта 2017 г., 14:20
 */

#ifndef CLI_H
#define CLI_H

#include <Arduino.h>
#include <microrl.h>
#include <fifo.h>

void cli_init();

void cli_insert_char();

void cli_print(const char * str);

int cli_execute (int argc, const char * const * argv);

void cli_help();

void cli_clear();

void cli_diag();

#endif /* CLI_H */

