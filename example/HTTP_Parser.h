#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <cstring>
#include <map>
#include "../llhttp.h"

#define MAX_LEN 2048

using namespace std;

struct MapStringComparator {
   bool operator()(const char* left, const char* right) const {
      return ((left != nullptr) && (right != nullptr) && (strcmp(left, right) < 0));
   }
};

class HTTP_Parser {
    public:
        HTTP_Parser(llhttp_type type) {
            llhttp_init(&parser, type, &HTTP_Parser::settings);
            parser.data = this;
        }
    
        int on_message_begin(llhttp_t* parser)
        {
            printf("parse start\n");
            return 0;
        }

        int on_status(llhttp_t* parser, const char* at, size_t length)
        {
            char status[MAX_LEN];
            strncpy(status, at, length);
            status[length] = '\0';
            printf("on_status: %s\n", status);
            return 0;
        }

        int on_url(llhttp_t* parser, const char* at, size_t length)
        { 
           url.append(at, length);
           cout<<url;
           return 0;
        }

        int on_header_field(llhttp_t* parser, const char* at, size_t length)
        {
            char header_field[MAX_LEN];
            strncpy(header_field, at, length);
            header_field[length] = '\0';
            printf("head field: %s\n", header_field);
            return 0;
        }

        int on_header_value(llhttp_t* parser, const char* at, size_t length)
        {
            char header_value[MAX_LEN];
            strncpy(header_value, at, length);
            header_value[length] = '\0';
            printf("head value: %s\n", header_value);
            return 0;
        }

        int on_headers_complete(llhttp_t* parser)
        {
            printf("on_headers_complete, major: %d, major: %d, keep-alive: %d, upgrade: %d\n", parser->http_major, parser->http_minor, llhttp_should_keep_alive(parser), parser->upgrade);
            return 0;
        }

        int on_body(llhttp_t* parser, const char* at, size_t length)
        {
            char body[MAX_LEN];
            strncpy(body, at, length);
            body[length] = '\0';
            printf("on_body: %s\n", body);
            return 0;
        }

        int on_message_complete(llhttp_t* parser)
        {
            printf("on_message_complete\n");
            return 0;
        }

        int parse(const char* data, int len) {
            enum llhttp_errno err = llhttp_execute(&parser, data, len);

            if (err != HPE_OK) {
                fprintf(stderr, "Parse error: %s %s\n", llhttp_errno_name(err), parser.reason);
                return 1;
            }
            return 0;
        }
        
    private: 
        string url;
        string status;
        map<char*, char*, MapStringComparator> headers;
        string body;
        llhttp_t parser;
        static llhttp_settings_t settings;
};


llhttp_settings_t HTTP_Parser::settings = {
    [](llhttp_t * parser) {
        return ((HTTP_Parser *)parser->data)->on_message_begin(parser);
    },
    [](llhttp_t * parser, const char * data, size_t len) {
        return ((HTTP_Parser *)parser->data)->on_url(parser, data, len);
    },
    [](llhttp_t * parser, const char * data, size_t len) {
        return ((HTTP_Parser *)parser->data)->on_status(parser, data, len);
    },
    [](llhttp_t * parser, const char * data, size_t len) {
        return ((HTTP_Parser *)parser->data)->on_header_field(parser, data, len);
    },
    [](llhttp_t * parser, const char * data, size_t len) {
        return ((HTTP_Parser *)parser->data)->on_header_value(parser, data, len);
    },
    [](llhttp_t * parser) {
        return ((HTTP_Parser *)parser->data)->on_headers_complete(parser);
    },
    [](llhttp_t * parser, const char * data, size_t len) {
        return ((HTTP_Parser *)parser->data)->on_body(parser, data, len);
    },
    [](llhttp_t * parser) {
        return ((HTTP_Parser *)parser->data)->on_message_complete(parser);
    }
};
