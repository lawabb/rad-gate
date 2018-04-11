/*****************************************
 *  RemoteArDuino (RAD) Gate
 *  (c) Danny Frencham 2017
 *****************************************/

#include <stdint.h>
#include <Arduino.h>

void serial_print(const char* print_string);
void serial_print_val(const char* print_string, unsigned long print_val);
int aprintf(char *str, ...);
