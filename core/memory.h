#ifndef MEMORY_H
#define MEMORY_H


#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <stdint.h>
#include "reading.h"


void show_mem(void);
void show_memory(void);
const char* dmi_string(const uint8_t *data, uint8_t index);
const char* mem_type_name(uint8_t type);

#endif //MEMORY_H