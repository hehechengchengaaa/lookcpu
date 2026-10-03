//cpu.cpp
//获取cpu信息

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <ctype.h>

#include "cpu.h"


void show_cpu(void) {
    printf("=== CPU ===\n");

    char buf[4096];
    if (read_file("/proc/cpuinfo", buf, sizeof(buf)) < 0) {
        printf("  /proc/cpuinfo 读不到(Can't read /proc/cpuinfo)\n\n");
        return;
    }

    /* 从 cpuinfo 里找 model name、vendor、cpu cores、flags */
    char *line = strtok(buf, "\n");
    int printed_model = 0, printed_vendor = 0;
    int cores = 0;
    while (line) {
        char *colon = strchr(line, ':');
        if (colon) {
            *colon = 0;
            char *key = trim(line);
            char *val = trim(colon + 1);

            if (!printed_model && strcmp(key, "model name") == 0) {
                printf("  型号(Model): %s\n", val);
                printed_model = 1;
            } else if (!printed_vendor && strcmp(key, "vendor_id") == 0) {
                printf("  厂商(Vendor): %s\n", val);
                printed_vendor = 1;
            } else if (strcmp(key, "cpu cores") == 0) {
                cores = atoi(val);
            }
        }
        line = strtok(NULL, "\n");
    }

    /* 逻辑核数：数 processor 行，或者直接用 sysconf 不行（要 unistd），
       这里数 /sys/devices/system/cpu 下的 cpuN 目录 */
    int logical = 0;
    DIR *d = opendir("/sys/devices/system/cpu");
    if (d) {
        struct dirent *e;
        while ((e = readdir(d))) {
            if (strncmp(e->d_name, "cpu", 3) == 0 && isdigit((unsigned char)e->d_name[3]))
                logical++;
        }
        closedir(d);
    }

    if (cores > 0)     printf("  物理核(Physical cores): %d\n", cores);
    if (logical > 0)   printf("  逻辑核(Logical cores): %d\n", logical);
    printf("\n");
}