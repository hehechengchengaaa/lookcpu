//os.cpp
//操作系统的信息


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "os.h"
#include "reading.h"


void show_os(void) {
    printf("=== 操作系统Operating System ===\n");

    char buf[1024];

    // ---------- 发行版信息 ----------
    if (read_file("/etc/os-release", buf, sizeof(buf)) > 0) {
        char *line = strtok(buf, "\n");
        while (line) {
            char *eq = strchr(line, '=');
            if (eq) {
                *eq = 0;
                char *key = trim(line);
                char *val = trim(eq + 1);

                // 去掉值两边的引号
                size_t vlen = strlen(val);
                if (vlen >= 2 && ((val[0] == '"' && val[vlen-1] == '"') ||(val[0] == '\'' && val[vlen-1] == '\''))) {
                    val[vlen - 1] = 0;
                    val++;
                }

                if (strcmp(key, "NAME") == 0)
                    printf("  发行版Distro: %s\n", val);
                else if (strcmp(key, "VERSION") == 0)
                    printf("  版本(Version): %s\n", val);
                else if (strcmp(key, "ID") == 0)
                    printf("  ID: %s\n", val);
            }
            line = strtok(NULL, "\n");
        }
    }

    // ---------- 内核版本 ----------
    if (read_line("/proc/version", buf, sizeof(buf)) == 0) {
        // /proc/version 格式: Linux version 6.x.x-arch1-1 (gcc ...) ...
        char *p = strstr(buf, "version ");
        if (p) {
            p += 8;   // 跳过 "version "
            char *end = strchr(p, ' ');
            if (end) *end = 0;
            printf("  内核(Kernel): %s\n", p);
        }
    }

    // ---------- 主机名 ----------
    if (read_line("/etc/hostname", buf, sizeof(buf)) == 0)
        printf("  主机名(Hostname): %s\n", trim(buf));

    // ---------- 开机时长 ----------
    if (read_line("/proc/uptime", buf, sizeof(buf)) == 0) {
        double seconds = atof(buf);
        int days = (int)(seconds / 86400);
        int hours = (int)((seconds - days * 86400) / 3600);
        int mins = (int)((seconds - days * 86400 - hours * 3600) / 60);

        printf("  开机时长(Uptime): ");
        if (days > 0) printf("%d 天(days) ", days);
        if (hours > 0) printf("%d 小时(hours) ", hours);
        printf("%d 分钟(minutes)\n", mins);
    }

    printf("\n");
}