#ifndef UTILE_H_
#define UTILE_H_

#ifdef __cplusplus
extern "C" {
#endif

#if DEBUG
    #define DEBUG_LOG printf
#else
    #define DEBUG_LOG
#endif



#define COLOR_LOG       printf          //颜色输出
#define red             "\033[31m"      //红的
#define green           "\033[32m"      //绿色
#define yellow          "\033[33m"      //黄色
#define blue            "\033[34m"      //蓝色
#define magenta         "\033[35m"      //品红
#define grass           "\033[36m"      //青色
#define end_color       "\033[0m"       //结束


typedef unsigned char  uint8_t; 
typedef unsigned short uint16_t; 
typedef unsigned int   uint32_t; 
typedef unsigned long  uint64_t; 

typedef uint8_t bool;

#define true     1
#define false    0

#define LOGO_PRINT                  printf
#define START_INFO_LOG              printf
#define FLOG                        fprintf

#ifdef __cplusplus
}
#endif
#endif