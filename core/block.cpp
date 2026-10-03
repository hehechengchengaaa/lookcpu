//block.cpp

#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <ctype.h>

#include "block.h"
#include "reading.h"


#define MAX_PARTS 128

typedef struct {
    char name[64];
    double gib;
    int is_partition;
} PartInfo;

// 按名字排序的比较函数
static int cmp_part(const void *a, const void *b) {
    return strcmp(((const PartInfo *)a)->name, ((const PartInfo *)b)->name);
}


void show_block(void) {
    printf("=== 硬盘信息(Disk Information) ===\n");

    DIR *d = opendir("/sys/block");
    if (!d) { printf("  /sys/block 读不到(Can't read /sys/block)\n\n"); return; }

    struct dirent *e;
    while ((e = readdir(d))) {
        if (e->d_name[0] == '.') continue;

        char path[PATH_MAX_LEN];
        char buf[512];

        printf("  %s\n", e->d_name);

        /* 大小：以 512 字节扇区为单位 */
        snprintf(path, sizeof(path), "/sys/block/%s/size", e->d_name);
        if (read_line(path, buf, sizeof(buf)) == 0) {
            unsigned long long sectors = strtoull(buf, NULL, 10);
            unsigned long long bytes = sectors * 512ULL;
            printf("    容量(Size): %llu 字节(bytes) (%.1f GiB)\n",
                   bytes, (double)bytes / (1024.0 * 1024.0 * 1024.0));
        }

        /* 型号 */
        snprintf(path, sizeof(path), "/sys/block/%s/device/model", e->d_name);
        if (read_line(path, buf, sizeof(buf)) == 0) {
            printf("    型号(Model): %s\n", trim(buf));
        }

        /* 是否可移动 */
        snprintf(path, sizeof(path), "/sys/block/%s/removable", e->d_name);
        if (read_line(path, buf, sizeof(buf)) == 0) {
            printf("    可移动(Removable): %s\n", strcmp(buf, "1") == 0 ? "是(Yes)" : "否(No)");
        }
    }
    closedir(d);
    printf("\n");
}


void show_partition(void) {
    printf("=== 硬盘分区(Partitions) ===\n");

    FILE *f = fopen("/proc/partitions", "r");
    if (!f) { printf("  /proc/partitions 读不到(Can't read /proc/partitions)\n\n"); return; }

    char line[512];
    fgets(line, sizeof(line), f);   // 跳过标题
    fgets(line, sizeof(line), f);   // 跳过空行

    PartInfo parts[MAX_PARTS];
    int count = 0;

    while (fgets(line, sizeof(line), f) && count < MAX_PARTS) {
        unsigned int major, minor;
        unsigned long long blocks;
        char name[64];

        if (sscanf(line, "%u %u %llu %63s",
                   &major, &minor, &blocks, name) == 4) {
            unsigned long long bytes = blocks * 1024ULL;
            double gib = (double)bytes / (1024.0 * 1024.0 * 1024.0);

            size_t len = strlen(name);
            int is_part = (len > 0 && isdigit((unsigned char)name[len - 1]));

            strncpy(parts[count].name, name, sizeof(parts[count].name) - 1);
            parts[count].name[sizeof(parts[count].name) - 1] = '\0';
            parts[count].gib = gib;
            parts[count].is_partition = is_part;
            count++;
        }
    }

    fclose(f);

    // 按名字排序
    qsort(parts, count, sizeof(PartInfo), cmp_part);

    // 输出
    for (int i = 0; i < count; i++) {
        if(strncmp(parts[i].name, "ram", 3) == 0)
            continue;
        printf("  %-20s %8.2f GiB  %s\n",
               parts[i].name, parts[i].gib,
               parts[i].is_partition ? "[分区](Partition)" : "[块设备](Block Device)");
    }

    printf("\n");
}