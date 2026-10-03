//power.h
//电源信息


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>


#include "power.h"
#include "reading.h"




void show_power(void) {
    printf("=== 电源(Power) ===\n");

    DIR *d = opendir("/sys/class/power_supply");
    if (!d) {
        printf("  /sys/class/power_supply 读不到(Can't read /sys/class/power_supply)\n\n");
        return;
    }

    struct dirent *e;
    while ((e = readdir(d))) {
        if (e->d_name[0] == '.') continue;

        char path[PATH_MAX_LEN];
        char buf[512];

        // 读类型（Battery / Mains）
        snprintf(path, sizeof(path), "/sys/class/power_supply/%s/type", e->d_name);
        char type[64] = "未知";
        if (read_line(path, buf, sizeof(buf)) == 0) {
            strncpy(type, trim(buf), sizeof(type) - 1);
            type[sizeof(type) - 1] = '\0';
        }

        printf("  %s (%s)\n", e->d_name, type);

        // ---------- 电源适配器 ----------
        if (strcmp(type, "Mains") == 0) {
            snprintf(path, sizeof(path),
                     "/sys/class/power_supply/%s/online", e->d_name);
            if (read_line(path, buf, sizeof(buf)) == 0) {
                printf("    状态(Status): %s\n",
                       strcmp(trim(buf), "1") == 0 ? "已插入( Plugged in)" : "未插入( Unplugged)");
            }
        }

        // ---------- 电池 ----------
        if (strcmp(type, "Battery") == 0) {
            // 电量
            snprintf(path, sizeof(path),
                     "/sys/class/power_supply/%s/capacity", e->d_name);
            if (read_line(path, buf, sizeof(buf)) == 0)
                printf("  电量(Capacity): %s%%\n", trim(buf));

            // 状态
            snprintf(path, sizeof(path),
                     "/sys/class/power_supply/%s/status", e->d_name);
            if (read_line(path, buf, sizeof(buf)) == 0)
                printf("  状态(Status): %s\n", trim(buf));

            // 制造商
            snprintf(path, sizeof(path),
                     "/sys/class/power_supply/%s/manufacturer", e->d_name);
            if (read_line(path, buf, sizeof(buf)) == 0)
                printf("  厂商(Manufacturer): %s\n", trim(buf));

            // 型号
            snprintf(path, sizeof(path),
                     "/sys/class/power_supply/%s/model_name", e->d_name);
            if (read_line(path, buf, sizeof(buf)) == 0)
                printf("  型号(Model): %s\n", trim(buf));

            // 循环次数
            snprintf(path, sizeof(path),
                     "/sys/class/power_supply/%s/cycle_count", e->d_name);
            if (read_line(path, buf, sizeof(buf)) == 0)
                printf("  循环次数(Cycle Count): %s\n", trim(buf));

            // 设计容量和当前满电容量（算损耗）
            unsigned long long design = 0, full = 0;

            snprintf(path, sizeof(path),
                     "/sys/class/power_supply/%s/energy_full_design", e->d_name);
            if (read_line(path, buf, sizeof(buf)) == 0)
                design = strtoull(buf, NULL, 10);

            snprintf(path, sizeof(path),
                     "/sys/class/power_supply/%s/energy_full", e->d_name);
            if (read_line(path, buf, sizeof(buf)) == 0)
                full = strtoull(buf, NULL, 10);

            if (design > 0 && full > 0) {
                double health = (double)full / design * 100.0;
                if(health > 100.0) health = 100.0;
                printf("  健康度(Health): %.1f%%\n", health);
            }
        }

        printf("\n");
    }

    closedir(d);
    printf("\n");
}