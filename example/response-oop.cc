#include "HTTP_Parser.h"

int main()
{
    
    
    const char* reponse = "HTTP/1.1 200 OK\r\nServer: nginx\r\ncontent-length: 11\r\n\r\nhello:world\r\n\r\n";
    int reponse_len = strlen(reponse);
    HTTP_Parser parser(HTTP_RESPONSE);
    parser.parse(reponse, reponse_len);
    return 0;
}