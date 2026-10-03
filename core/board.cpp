//board.cpp
// 主板和 BIOS 信息

#include <stdio.h>
#include <stdlib.h>

#include "board.h"
#include "reading.h"








void show_board(void) {
    printf("=== 主板(Motherboard) ===\n");

    char buf[512];

    // 主板厂商
    if (read_line("/sys/class/dmi/id/board_vendor", buf, sizeof(buf)) == 0)
        printf("  厂商(Vendor): %s\n", trim(buf));

    // 主板型号
    if (read_line("/sys/class/dmi/id/board_name", buf, sizeof(buf)) == 0)
        printf("  型号(Model): %s\n", trim(buf));

    // 主板版本
    if (read_line("/sys/class/dmi/id/board_version", buf, sizeof(buf)) == 0)
        printf("  版本(Version): %s\n", trim(buf));

    printf("\n");
}

void show_bios(void) {
    printf("=== BIOS ===\n");

    char buf[512];

    // BIOS 厂商
    if (read_line("/sys/class/dmi/id/bios_vendor", buf, sizeof(buf)) == 0)
        printf("  厂商(Vendor): %s\n", trim(buf));

    // BIOS 版本
    if (read_line("/sys/class/dmi/id/bios_version", buf, sizeof(buf)) == 0)
        printf("  版本(Version): %s\n", trim(buf));

    // BIOS 日期
    if (read_line("/sys/class/dmi/id/bios_date", buf, sizeof(buf)) == 0)
        printf("  日期(Date): %s\n", trim(buf));

    printf("\n");
}