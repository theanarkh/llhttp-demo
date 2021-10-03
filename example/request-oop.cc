#include "HTTP_Parser.h"

int main()
{
    
    {
        const char* request = "POST /index.html HTTP/1.1\r\nconnection:close\r\ncontent-length: 1\r\n\r\n1\r\n\r\n";
        int request_len = strlen(request);
        HTTP_Parser parser(HTTP_REQUEST);
        parser.parse(request, request_len);
    }
    // {
    //     const char* request = "GET /hello HTTP/1.1\r\nconnection: keep-alive\r\ncontent-length: 2\r\n\r\n13\r\n\r\n";
    //     int request_len = strlen(request);
    //     HTTP_Parser parser(HTTP_REQUEST);
    //     parser.parse(request, request_len);
    // }
    return 0;
}