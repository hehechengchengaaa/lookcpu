#ifndef READING_H
#define READING_H


#include <stdio.h>
#include <dirent.h>

#define PATH_MAX_LEN 512
#define PCI_IDS_PATHS \
    "/usr/share/hwdata/pci.ids:" \
    "/usr/share/misc/pci.ids:" \
    "/usr/local/share/pci.ids:" \
    "/usr/share/pci.ids:" \
    "/etc/pci.ids"

int read_line(const char *path, char *buf, size_t n);

/* 读整个文件到一个缓冲 */
long read_file(const char *path, char *buf, size_t n);

/* 去掉字符串首尾空白 */
char *trim(char *s);



FILE* open_pci_ids(void);

int lookup_pci_id(const char *vendor_id, const char *device_id,
                  char *out_vendor, size_t vendor_size,
                  char *out_device, size_t device_size);



void help(void);

#endif //READING_H