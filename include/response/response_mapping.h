#ifndef CDEMOS_RESPONSE_MAPPING_H
#define CDEMOS_RESPONSE_MAPPING_H
#ifdef __cplusplus
extern "C" {
#endif

enum ContextType{
    HTML, CSS,JS, ICO, XML, PLAIN, GIF, JPEG,PNG,JSON,JPG,MP4
};

enum StateCode{
    OK,Continue,NotFound,InternalServerError
};

typedef struct {
    int file_index;
    char file_type[50];
    char res_type[50];
}HttpResponseContextType;


typedef struct {
    int StateIndex;
    char Code[10];
    char StateExplain[40];
}HttpStateCode;

extern HttpResponseContextType httpResponseContextType[];

extern HttpStateCode httpStateCode[];

extern int len;

#ifdef __cplusplus
}
#endif
#endif //CDEMOS_RESPONSE_MAPPING_H
