#include "request/httpMapping.h"

//GET, HEAD, POST, PUT, PATCH, DELETE, OPTIONS, TRACE
HttpMethodMapping httpMethodMapping[] = {
        {
                .method = "GET",
                .RequestMethod = GET
        },
        {
                .method = "HEAD",
                .RequestMethod = HEAD
        },
        {
                .method = "POST",
                .RequestMethod = POST
        },
        {
                .method = "PUT",
                .RequestMethod = PUT
        },
        {
                .method = "PATCH",
                .RequestMethod = PATCH
        },
        {
                .method = "DELETE",
                .RequestMethod = DELETE
        },
        {
                .method = "OPTIONS",
                .RequestMethod = OPTIONS
        },
        {
                .method = "TRACE",
                .RequestMethod = TRACE
        }
};

HttpVersionMapping httpVersionMapping[] ={
        {
            .RequestVersion = HTTP_1_0,
            .version = "HTTP_1_0"
        },
        {
                .RequestVersion = HTTP_1_1,
                .version = "HTTP_1_1"
        },
        {
                .RequestVersion = HTTP_2_0,
                .version = "HTTP_2_0"
        }
};