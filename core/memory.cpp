//memory.cpp
//内存读取

#include "memory.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>



const char* dmi_string(const uint8_t *data, uint8_t index) {
    if (index == 0) return "Data unknown";

    size_t offset = data[1];
    const uint8_t *str = data + offset;

    for (uint8_t i = 1; i < index; i++) {
        while (*str) str++;
        str++;
    }
    return (const char*)str;
}



// 内存类型映射
const char* mem_type_name(uint8_t type) {
    switch (type) {
        case 0x01: return "Other";
        case 0x02: return "Unknown";
        case 0x03: return "DRAM";
        case 0x04: return "EDRAM";
        case 0x05: return "VRAM";
        case 0x06: return "SRAM";
        case 0x07: return "RAM";
        case 0x08: return "ROM";
        case 0x09: return "FLASH";
        case 0x0A: return "EEPROM";
        case 0x0B: return "FEPROM";
        case 0x0C: return "EPROM";
        case 0x0D: return "CDRAM";
        case 0x0E: return "3DRAM";
        case 0x0F: return "SDRAM";
        case 0x10: return "SGRAM";
        case 0x11: return "RDRAM";
        case 0x12: return "DDR";
        case 0x13: return "DDR2";
        case 0x14: return "DDR2 FB-DIMM";
        case 0x18: return "DDR3";
        case 0x1A: return "DDR4";
        case 0x1E: return "LPDDR4";
        case 0x22: return "DDR5";
        case 0x23: return "LPDDR5";
        default:   return "Unknown";
    }
}



void show_mem(void) {
    printf("=== 内存使用(Memory Usage) ===\n");

    char buf[8192];
    if (read_file("/proc/meminfo", buf, sizeof(buf)) < 0) {
        printf("  /proc/meminfo 读不到(Can't read /proc/meminfo)\n\n");
        return;
    }

    char *line = strtok(buf, "\n");
    while (line) {
        char *colon = strchr(line, ':');
        if (colon) {
            *colon = 0;
            char *key = trim(line);
            char *val = trim(colon + 1);

            if (strcmp(key, "MemTotal") == 0 ||
                strcmp(key, "MemFree") == 0 ||
                strcmp(key, "MemAvailable") == 0 ||
                strcmp(key, "SwapTotal") == 0 ||
                strcmp(key, "SwapFree") == 0) {

                unsigned long long value = strtoll(val, NULL, 10);
                printf("  %-14s %s ( %.2f GiB)\n", key, val, (double)value / (1024.0 * 1024.0));
            }
        }
        line = strtok(NULL, "\n");
    }
    printf("\n");
}

void show_memory(void) {
    printf("=== 内存条(Memory Modules) ===\n");


    FILE *f = fopen("/sys/firmware/dmi/tables/DMI", "rb");
    if (!f) {
        printf("  读不到 DMI 表(Can't read DMI)\n试试sudo或者root(try \"sudo\" or \"root\")\n\n");
        return;
    }

    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    fseek(f, 0, SEEK_SET);
    
    if(size <= 0) { fclose(f); return; }


    uint8_t *buf = (uint8_t*)malloc(size);
    if (!buf) { fclose(f); return; }
    fread(buf, 1, size, f);
    fclose(f);

    size_t pos = 0;
    while (pos + 4 <= (size_t)size) {
        uint8_t type = buf[pos];
        uint8_t len = buf[pos + 1];

        if (type == 17) {   // Type 17 = Memory Device
            const uint8_t *s = buf + pos;

            uint16_t size_mb = *(uint16_t *)(s + 0x0C);
            uint8_t mem_type = s[0x12];
            uint16_t speed = *(uint16_t *)(s + 0x15);
            uint8_t manuf_idx = s[0x17];
            uint8_t part_idx = s[0x1A];
            const char *part = dmi_string(s, part_idx);
            if (strcmp(part, "Unknown") == 0 || strlen(part) == 0) {
                part_idx = s[0x2B];
                part = dmi_string(s, part_idx);
            }
            uint8_t loc_idx = s[0x10];

            // 跳过空槽位
            

            if (size_mb == 0 || size_mb == 0xFFFF) {
                // 继续下一个结构
            } else {
                printf("  插槽(Slot): %s\n", dmi_string(s, loc_idx));
                printf("  容量(Size): %u MB\n", size_mb & 0x7FFF);
                printf("  类型(Type): %s\n", mem_type_name(mem_type));
                printf("  速度(Speed): %u MT/s\n", speed);
                printf("  厂商(Manufacturer): %s\n", dmi_string(s, manuf_idx));
                printf("  型号(Part Number): %s\n", dmi_string(s, part_idx));
                printf("\n");
            }
        }



        // 跳到下一个结构
        size_t next = pos + len;
        while (next + 1 < (size_t)size &&
               !(buf[next] == 0 && buf[next + 1] == 0)) {
            next++;
        }
        next += 2;
        pos = next;
    }

    free(buf);
    printf("\n");

}