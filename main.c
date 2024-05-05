#include "dyj_server.h"
#include "WebSocket/WebSocketConnect.h"


char src[] = "{\"name\":\"springboot\",\"age\": 12}";


void json_test(void){
    COLOR_LOG("%s%s%s\r\n",blue,src,end_color);

    cJSON *json = cJSON_Parse(src);

    cJSON *item = cJSON_GetObjectItem(json,"name");
    DEBUG_LOG("name: %s\r\n",item->valuestring);
    item = cJSON_GetObjectItem(json,"age");
    DEBUG_LOG("name: %d\r\n",item->valueint);


    cJSON_Delete(json);
}


RequestMapping(name,GET,/user{user}){
    DEBUG_LOG("data = %s\r\n",data);
    char *ret = (char *)malloc(sizeof(char)*100);
    
    strcpy(ret,"springboot\r\n");

    return ret;
}



#define TEST 1
#define BUFF_LENGTH SHA_DIGEST_LENGTH * 2
char * sha1_to_hex(unsigned char *sha1)
{
	static char buffer[BUFF_LENGTH];
	static const char hex[] = "0123456789abcdef";
	char *buf = buffer;
	int i;

	for (i = 0; i < SHA_DIGEST_LENGTH; i++) {
		unsigned int val = *sha1++;
		*buf++ = hex[val >> 4];
		*buf++ = hex[val & 0xf];
	}
	return buffer;
}


int main(int argc, char const *argv[])
{


#if TEST
    get_localhost_path();
    webServer_Init();
    webServer_Loop();
#else 
    DEBUG_LOG("start\r\n");
    char data_once[] = "CA39qveNu8p6rGHUh+NgUg=="; 
    
    char* code = WebSocketResponse_head(data_once);
    printf("%s",code);

    free(code);



#endif
    return 0;
}
