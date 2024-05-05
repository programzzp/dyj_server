#ifndef SYSTEM_LOG_H
#define SYSTEM_LOG_H

#ifdef __cplusplus
extern "C" {
#endif

#include <unistd.h>
#include <fcntl.h>
#include <malloc.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "utile.h"

#define PATH_LEN 255

extern char localhost_path_buf[PATH_LEN];

extern FILE *success_log;

void get_localhost_path(void);
long judgeFileWhetherExist(const char* path);
void Init_log(void);
void by_pass_file_suffix(const char *file_name,char* suffix);
bool indexHtmlfile(const char *file_name);
int getFileFd(char *path);

#ifdef __cplusplus
}
#endif

#endif