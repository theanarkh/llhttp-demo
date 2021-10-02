#include <stdio.h>
#include <string.h>
#include "llhttp.h"

#define URL_MAX_LEN 2048

int handle_url(llhttp_t* parser, const char* at, size_t length)
{
    char url[URL_MAX_LEN];
    strncpy_s(url, URL_MAX_LEN, at, length);
    printf("URL: %s", url);
    return 0;
}

int main()
{
    llhttp_t parser;
    llhttp_settings_t settings;
    llhttp_settings_init(&settings);
    llhttp_init(&parser, HTTP_BOTH, &settings);

    settings.on_url = handle_url;

    const char* request = "GET /index.html HTTP/1.1\r\n\r\n";
    int request_len = strlen(request);

    enum llhttp_errno err = llhttp_execute(&parser, request, request_len);
    if (err == HPE_OK) {
        /* Successfully parsed! */
    }
    else {
        fprintf(stderr, "Parse error: %s %s\n", llhttp_errno_name(err),
            parser.reason);
    }

    return 0;
}