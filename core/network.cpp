//network.cpp
//网络接口信息

#include <dirent.h>
#include <stdio.h>

#include "network.h"
#include "reading.h"


void show_net(void) {
    printf("=== 网络接口(Network Interfaces) ===\n");

    // 打开 /sys/class/net 目录，该目录包含系统中所有网络接口的信息
    DIR *d = opendir("/sys/class/net");
    if (!d) { printf("  /sys/class/net 读不到(can't read /sys/class/net)\n\n"); return; }

    struct dirent *e;
    while ((e = readdir(d))) {
        if (e->d_name[0] == '.') continue;

        char path[PATH_MAX_LEN];
        char buf[512];

        printf("  %s\n", e->d_name);

        /* MAC 地址 */
        snprintf(path, sizeof(path), "/sys/class/net/%s/address", e->d_name);
        if (read_line(path, buf, sizeof(buf)) == 0)
            printf("    MAC: %s\n", buf);

        /* 状态 */
        snprintf(path, sizeof(path), "/sys/class/net/%s/operstate", e->d_name);
        if (read_line(path, buf, sizeof(buf)) == 0)
            printf("    状态(State): %s\n", buf);

        /* 速率（有些虚拟接口没有这个文件） */
        snprintf(path, sizeof(path), "/sys/class/net/%s/speed", e->d_name);
        if (read_line(path, buf, sizeof(buf)) == 0)
            printf("    速率(Speed): %s Mbps\n", buf);
    }
    closedir(d);
    printf("\n");
}