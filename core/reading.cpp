//reading.cpp
//读取文件

#include "reading.h"
#include <ctype.h>
#include <stdio.h>
#include <string.h>

/* 读一行，去掉尾部换行 */
int read_line(const char *path, char *buf, size_t n) {
    FILE *f = fopen(path, "r");
    if (!f) return -1;
    if (!fgets(buf, (int)n, f)) { fclose(f); return -1; }
    fclose(f);

    size_t len = strlen(buf);
    while (len && (buf[len-1] == '\n' || buf[len-1] == '\r')) buf[--len] = 0;
    return 0;
}

/* 读整个文件到一个缓冲 */
long read_file(const char *path, char *buf, size_t n) {
    FILE *f = fopen(path, "r");
    if (!f) return -1;
    size_t r = fread(buf, 1, n - 1, f);
    fclose(f);
    buf[r] = 0;
    return (long)r;
}

/* 去掉字符串首尾空白 */
char *trim(char *s) {
    while (*s && isspace((unsigned char)*s)) s++;
    char *e = s + strlen(s);
    while (e > s && isspace((unsigned char)e[-1])) *--e = 0;
    return s;
}

// 去掉字符串首尾空白，返回新起点

// 打开 pci.ids 文件，返回 FILE*，找不到返回 NULL
FILE* open_pci_ids(void) {
    const char *paths = PCI_IDS_PATHS;
    char buf[512];
    const char *p = paths;

    while (*p) {
        const char *colon = strchr(p, ':');
        size_t len = colon ? (size_t)(colon - p) : strlen(p);
        if (len >= sizeof(buf)) len = sizeof(buf) - 1;

        strncpy(buf, p, len);
        buf[len] = '\0';

        FILE *f = fopen(buf, "r");
        if (f) return f;

        if (!colon) break;
        p = colon + 1;
    }
    return NULL;
}

int lookup_pci_id(const char *vendor_id, const char *device_id,
                  char *out_vendor, size_t vendor_size,
                  char *out_device, size_t device_size) {
    FILE *f = open_pci_ids();
    if (!f) return -1;

    char line[512];
    int in_vendor = 0;   // 是否已进入目标厂商

    while (fgets(line, sizeof(line), f)) {
        // 跳过注释和空行
        if (line[0] == '#' || line[0] == '\n') continue;

        // ---------- 厂商行（顶格，不以 Tab 开头） ----------
        if (!isspace((unsigned char)line[0])) {
            // 厂商 ID 是前 4 个字符
            if (strncmp(line, vendor_id, 4) == 0) {
                in_vendor = 1;

                // 提取厂商名（第 4 个字符之后，去掉空白）
                char *name = trim(line + 4);
                if (out_vendor && vendor_size > 0) {
                    strncpy(out_vendor, name, vendor_size - 1);
                    out_vendor[vendor_size - 1] = '\0';
                }
            } else {
                // 不是目标厂商，且已经找到过目标厂商 → 退出
                if (in_vendor) break;
                in_vendor = 0;
            }
            continue;
        }

        // ---------- 设备行（以 Tab 开头） ----------
        if (in_vendor && line[0] == '\t') {
            // 跳过子设备（双 Tab 开头）
            if (line[1] == '\t') continue;

            // 设备 ID 是 Tab 之后的 4 个字符
            const char *id = line + 1;
            if (strncmp(id, device_id, 4) == 0) {
                char *name = trim((char *)id + 4);
                if (out_device && device_size > 0) {
                    strncpy(out_device, name, device_size - 1);
                    out_device[device_size - 1] = '\0';
                }
                fclose(f);
                return 0;
            }
        }
    }

    fclose(f);
    return -1;
}




void help(void){
    printf("help, bang_zhu     显示此帮助信息\n");
    printf("all, quan_bu       显示所有硬件信息\n");
    printf("cpu                显示 CPU 信息\n");
    printf("gpu, xian_ka       显示显卡信息\n");
    printf("memory, nei_cun    显示内存信息\n");
    printf("block, yin_pan     显示硬盘和分区信息\n");
    printf("board, zhu_ban     显示主板和 BIOS 信息\n");
    printf("net, wang_luo      显示网络接口信息\n");
    printf("power, dian_yuan   显示电池和电源信息\n");
    printf("vital, jian_duan   显示关键硬件信息(CPU/显卡/内存/硬盘/主板)\n");
    printf("用法: sudo lookcpu [选项]\n例如查看所有硬件信息: lookcpu all\n\n");

    printf("help, bang_zhu     xian_shi_ci_bang_zhu_xin_xi\n");
    printf("all, quan_bu       xian_shi_suo_you_ying_jian_xin_xi\n");
    printf("cpu                xian_shi_CPU_xin_xi\n");
    printf("gpu, xian_ka       xian_shi_xian_ka_xin_xi\n");
    printf("memory, nei_cun    xian_shi_nei_cun_xin_xi\n");
    printf("block, yin_pan     xian_shi_ying_pan_he_fen_qu_xin_xi\n");
    printf("board, zhu_ban     xian_shi_zhu_ban_he_BIOS_xin_xi\n");
    printf("net, wang_luo      xian_shi_wang_luo_jie_kou_xin_xi\n");
    printf("power, dian_yuan   xian_shi_dian_chi_he_dian_yuan_xin_xi\n");
    printf("vital, jian_duan   xian_shi_guan_jian_ying_jian_xin_xi(CPU/xian_ka/nei_cun/ying_pan/zhu_ban)\n");
    printf("yong_fa: sudo lookcpu [xuan_xiang]\nli_ru_cha_kan_suo_you_ying_jian xin_xi: lookcpu all\n\n");

    printf("help, bang_zhu     Show this help message\n");
    printf("all, quan_bu       Show all hardware information\n");
    printf("cpu                Show CPU information\n");
    printf("gpu, xian_ka       Show GPU information\n");
    printf("memory, nei_cun    Show memory information\n");
    printf("block, yin_pan     Show disks and partitions\n");
    printf("board, zhu_ban     Show motherboard and BIOS info\n");
    printf("net, wang_luo      Show network interfaces\n");
    printf("power, dian_yuan   Show battery and power info\n");
    printf("vital, jian_duan   Show key hardware (CPU/GPU/RAM/Disk/Board)\n");
    printf("usage: sudo lookcpu [option]\nExample: lookcpu all\n");

}