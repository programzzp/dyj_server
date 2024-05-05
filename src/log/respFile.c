#include <stdlib.h>
#include "log/system_log.h"
#include "config.h"
#include <time.h>

FILE *success_log;
/**
 * 判断文件是否存在,存在返回文件大小
*/
static void Init_log_print_Info(const char *file_path,const char *fun_name,const char* log_path);

long judgeFileWhetherExist(const char* path){
    char resp_path_buf[512];
    int access_flag;
    memset(resp_path_buf,0,sizeof(resp_path_buf));


    sprintf(resp_path_buf,"%s/%s%s",localhost_path_buf,HTML_RESOURCE,path);
    access_flag = access(resp_path_buf,R_OK);

    if(access_flag == 0)
    {
        struct stat resp_info;
        stat(resp_path_buf,&resp_info);
        return resp_info.st_size;   
    }else if(access_flag == -1)
    {
        return -1;
    }
    return -1;
}

/**
 * 打印log信息
*/
static void Init_log_print_Info(const char *file_path,const char *fun_name,const char* log_path){
    time_t now = time(NULL); // 获取当前时间
    struct tm *t = localtime(&now); // 将时间转换为本地时间


    START_INFO_LOG("%d-%d-%d  %d:%d:%d  %sINFO%s %s%d%s --- [\t\t  %s] %s%s%s\t:log path :%s%s%s\r\n", \
                   t->tm_year + 1900,t->tm_mon + 1,t->tm_mday,t->tm_hour,t->tm_min,t->tm_sec,  \
                    yellow,end_color,\
                    green,getpid(),end_color, \
                    fun_name, \
                    blue,file_path,end_color, \
                    grass,log_path,end_color);

}

/**
 * log初始化
*/
void Init_log(void){
    char resp_path_buf[512];
    memset(resp_path_buf,0,sizeof(resp_path_buf));
    sprintf(resp_path_buf,"%s/%s",localhost_path_buf,SUCCESS_LOG);
    DEBUG_LOG("%s\r\n",resp_path_buf);

    success_log = fopen(resp_path_buf,"a");
    if(success_log == NULL)
    {
        COLOR_LOG("%sthe file : [ %s ] not found !%s\r\n",red,resp_path_buf,end_color);
        exit(1);
    }else
    {
        Init_log_print_Info(__FILE__,__func__,resp_path_buf);
    }
}




/**
 * @brief 通过文件名字得到后缀
*/
void by_pass_file_suffix(const char *file_name,char* suffix){
    char file_name_buf[255];
    memset(file_name_buf,0,sizeof(file_name_buf));
    memset(suffix,0,strlen(suffix));
    strcpy(file_name_buf,file_name);
    char *file_name_point = file_name_buf;
    char *suffix_point = suffix;
    while(*file_name_point != '.'){
        ++file_name_point;
    }
    ++file_name_point;
    while(*file_name_point != '\0'){
        *suffix_point = *file_name_point;
        ++file_name_point;
        ++suffix_point;
    }
    *suffix_point = '\0';
}



/**
 * @brief  判断路径是否为/ 或者 index
 * @param  file_name  文件路径 /index.html这样的路径
 * @return bool: true 路径是指定根路径获取 /
*/
bool indexHtmlfile(const char *file_name){
    if(strcmp(ROOT_PATH,file_name) == 0){
        return true;
    }else if(strcmp("/",file_name) == 0){
        return true;
    }else{
        return false;
    }
}



int getFileFd(char *path){
    char path_buf[1024];
    memset(path_buf,0,sizeof(path_buf));

    sprintf(path_buf,"%s/%s%s",localhost_path_buf,HTML_RESOURCE,path);

    return open(path_buf,O_RDWR);
}