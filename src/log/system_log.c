#include <time.h>
#include <stdlib.h>
#include "log/system_log.h"
#include "utile.h"
#include "config.h"
#define LOGO_PATH "/logo/logo.txt"


char dyj_logo[] =" ___   __   __      _            ___     ___     ___   __   __   ___     ___\r\n"
                 "|   \\  \\ \\ / /   _ | |          / __|   | __|   | _ \\  \\ \\ / /  | __|   | _ \\\r\n"
                 "| |) |  \\ V /   | || |   ___    \\__ \\   | _|    |   /   \\ V /   | _|    |   /\r\n"
                 "|___/   _|_|_   _\\__/   |___|   |___/   |___|   |_|_\\   _\\_/_   |___|   |_|_\\\r\n"
                 "_|\"\"\"\"\"|_| \"\"\" |_|\"\"\"\"\"|_|\"\"\"\"\"|_|\"\"\"\"\"|_|\"\"\"\"\"|_|\"\"\"\"\"|_| \"\"\"\"|_|\"\"\"\"\"|_|\"\"\"\"\"|\r\n"
                 "\"`-0-0-'\"`-0-0-'\"`-0-0-'\"`-0-0-'\"`-0-0-'\"`-0-0-'\"`-0-0-'\"`-0-0-'\"`-0-0-'\"`-0-0-'\r\n";

//可执行程序的路径
char localhost_path_buf[PATH_LEN];



/**
 * 打印logo
*/
static void print_log(void){
    LOGO_PRINT("\r\n");
    char logo_path_buf[512];
    int n = 0;
    memset(logo_path_buf,0,sizeof(logo_path_buf));
    sprintf(logo_path_buf,"%s/%s",localhost_path_buf,LOGO_PATH);

    int logofd = open(logo_path_buf,O_RDONLY);

    if(logofd == -1)
    {
        //如果没有logo.txt打印dyj_logo
        LOGO_PRINT("%s",dyj_logo); 
        goto logo;
    }
    memset(logo_path_buf,0,sizeof(logo_path_buf));
    while( (n = read(logofd,logo_path_buf,sizeof(logo_path_buf))) > 0)
    {
        LOGO_PRINT("%s",logo_path_buf);
        memset(logo_path_buf,0,sizeof(logo_path_buf));
    }
logo:
    LOGO_PRINT("\r\n");
    //打印软件版本
    COLOR_LOG("%s::     %s     ::%s\t\t(v%s)",yellow,SERVER_NAME,end_color,SERVER_VERSION);
    LOGO_PRINT("\r\n");
    LOGO_PRINT("\r\n");
    close(logofd);
    
}

/**
 * @brief 获取可执行文件绝对路径
*/
void get_localhost_path(void){
    time_t now = time(NULL); // 获取当前时间
    struct tm *t = localtime(&now); // 将时间转换为本地时间
    getcwd(localhost_path_buf,PATH_LEN);
    print_log();
    START_INFO_LOG("%d-%d-%d  %d:%d:%d  %sINFO%s %s%d%s --- [%s] %s%s%s\t:html path :%s%s/html%s\r\n",  \
                    t->tm_year + 1900,t->tm_mon + 1,t->tm_mday,t->tm_hour,t->tm_min,t->tm_sec,  \
                    yellow,end_color,\
                    green,getpid(),end_color, \
                    __func__ ,\
                    blue,__FILE__,end_color, \
                    grass,localhost_path_buf,end_color);
}

