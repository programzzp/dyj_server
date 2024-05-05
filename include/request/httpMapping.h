//
// Created by redmi on 2024/4/4.
//

#ifndef CDEMOS_HTTPMAPPING_H
#define CDEMOS_HTTPMAPPING_H

#ifdef __cplusplus
extern "C" {
#endif

#include "string.h"
#include "httpHeadInformation.h"
typedef struct {
    int RequestMethod;
    char method[50];
}HttpMethodMapping;


typedef struct {
    int RequestVersion;
    char version[50];
}HttpVersionMapping;

extern HttpMethodMapping httpMethodMapping[];
extern HttpVersionMapping httpVersionMapping[];
#ifdef __cplusplus
}
#endif
#endif //CDEMOS_HTTPMAPPING_H
