#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include <stdbool.h>

bool filesystem_open(void);
void filesystem_close(void);

bool filesystem_init(const char* arg0);
bool filesystem_set_identity(const char* identity, bool append_to_path);

#endif
