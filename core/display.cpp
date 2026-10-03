//display.cpp
//显卡检测


#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>

#include "display.h"
#include "reading.h"


void show_gpu(void) {
    printf("=== 显卡(GPU) ===\n");

    DIR *d = opendir("/sys/class/drm");
    if (!d) {
        printf("  /sys/class/drm 读不到(Can't read /sys/class/drm)\n\n");
        return;
    }

    struct dirent *e;
    while ((e = readdir(d))) {
        if (strncmp(e->d_name, "card", 4) != 0) continue;
        if (strchr(e->d_name, '-')) continue;

        char path[PATH_MAX_LEN];
        char link[PATH_MAX_LEN];
        ssize_t len;

        // 读厂商 ID 和设备 ID
        char vendor[64] = "?";
        char device[64] = "?";

        snprintf(path, sizeof(path), "/sys/class/drm/%s/device/vendor", e->d_name);
        read_line(path, vendor, sizeof(vendor));

        snprintf(path, sizeof(path), "/sys/class/drm/%s/device/device", e->d_name);
        read_line(path, device, sizeof(device));

        // 读驱动名
        char driver[64] = "未知";
        snprintf(path, sizeof(path), "/sys/class/drm/%s/device/driver", e->d_name);
        len = readlink(path, link, sizeof(link) - 1);
        if (len > 0) {
            link[len] = '\0';
            const char *p = strrchr(link, '/');
            if (p) strncpy(driver, p + 1, sizeof(driver) - 1);
        }


        // 跳过临时帧缓冲（不是真正的显卡）
        if (strcmp(driver, "simple-framebuffer") == 0) continue;
        // ---------- 查 pci.ids 翻译 ----------
        char vendor_name[128] = "未知";
        char device_name[256] = "未知";

        // 去掉 "0x" 前缀
        const char *vid = trim(vendor);
        const char *did = trim(device);
        if (strncmp(vid, "0x", 2) == 0) vid += 2;
        if (strncmp(did, "0x", 2) == 0) did += 2;

        lookup_pci_id(vid, did,
                      vendor_name, sizeof(vendor_name),
                      device_name, sizeof(device_name));

        // ---------- 输出 ----------
        printf("  %s\n", e->d_name);
        printf("    厂商(Vendor): %s\n", vendor_name);
        printf("    型号(Device): %s\n", device_name);
        printf("    驱动(Driver): %s\n", driver);
        printf("\n");
    }

    closedir(d);
    printf("\n");
}