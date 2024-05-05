//
// Created by redmi on 2024/4/8.
//
#include "response/response_mapping.h"

int len = 0;
//HTML, CSS,JS, ICO, XML, PLAIN, GIF, JPEG,PNG,JSON

HttpResponseContextType httpResponseContextType[] = {
        {
            .file_index = HTML,
            .file_type = "html",
            .res_type = "text/html"
        },
        {
            .file_index = CSS,
            .file_type = "css",
            .res_type = "text/css"
        },
        {
            .file_index = JS,
            .file_type = "js",
            .res_type = "application/javascript"
        },
        {
            .file_index = ICO,
            .file_type = "ico",
            .res_type = "image/ico"
        },
        {
            .file_index = XML,
            .file_type = "html",
            .res_type = "text/html"
        },
        {
            .file_index = PLAIN,
            .file_type = "plain",
            .res_type = "text/plain"
        },
        {
            .file_index = GIF,
            .file_type = "gif",
            .res_type = "image/gif"
        },
        {
            .file_index = JPEG,
            .file_type = "jpeg",
            .res_type = "image/jpeg"
        },
        {
            .file_index = PNG,
            .file_type = "png",
            .res_type = "image/png"
        },
        {
            .file_index = JSON,
            .file_type = "json",
            .res_type = "application/json"
        },
        {
            .file_index = JPG,
            .file_type = "jpg",
            .res_type = "image/jpg"
        },{
            .file_index = MP4,
            .file_type = "mp4",
            .res_type = "video/mp4"
        }
};

//OK,Continue,NotFound,InternalServerError
HttpStateCode httpStateCode[] = {
        {
            .StateIndex = OK,
            .Code = "200",
            .StateExplain = "OK"
        },
        {
            .StateIndex = Continue,
            .Code = "100",
            .StateExplain = "Continue"
        },
        {
            .StateIndex = NotFound,
            .Code = "404",
            .StateExplain = "Not Found"
        },
        {
            .StateIndex = InternalServerError,
            .Code = "500",
            .StateExplain = "Internal Server Error"
        }
};