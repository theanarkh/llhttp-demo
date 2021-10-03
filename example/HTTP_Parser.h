#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <cstring>
#include <ctime>
#include "../llhttp.h"

#define MAX_LEN 2048

using namespace std;

class HTTP_Parser {
    public:
        HTTP_Parser(llhttp_type type) {
            llhttp_init(&parser, type, &HTTP_Parser::settings);
            // set data after llhttp_init, because llhttp_init will call memset to fill zero to memory 
            parser.data = this;
        }
    
        int on_message_begin(llhttp_t* parser)
        {
            major_version = 0;
            minor_version = 0;
            upgrade = 0;
            keepalive = 0;
            parse_start_time = 0;
            header_end_time = 0;
            message_end_time = 0;
            url.clear();
            status.clear();
            keys.clear();
            values.clear();
            body.clear();
            
            parse_start_time = time(NULL);
            return 0;
        }

        int on_status(llhttp_t* parser, const char* at, size_t length)
        {
            status.append(at, length);
            return 0;
        }

        int on_url(llhttp_t* parser, const char* at, size_t length)
        { 
           url.append(at, length);
           return 0;
        }

        int on_header_field(llhttp_t* parser, const char* at, size_t length)
        {   
            keys.push_back(string(at, length));
            return 0;
        }

        int on_header_value(llhttp_t* parser, const char* at, size_t length)
        {
            values.push_back(string(at, length));
            return 0;
        }

        int on_headers_complete(llhttp_t* parser)
        {
            header_end_time = time(NULL);
            major_version = parser->http_major;
            minor_version = parser->http_minor;
            upgrade = parser->upgrade;
            keepalive = llhttp_should_keep_alive(parser);
            return 0;
        }

        int on_body(llhttp_t* parser, const char* at, size_t length)
        {
            body.append(at, length);
            return 0;
        }

        int on_message_complete(llhttp_t* parser)
        {
            message_end_time = time(NULL);
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
        void print() {
            cout<<"parse start: "<<ctime(&parse_start_time);
            cout<<"url: "<<url<<endl;
            cout<<"headers: "<<endl;
            for (int i = 0; i < keys.size(); i++) {
                cout <<"    "<<keys[i]<<": "<<values[i]<< endl;
            }
            cout<<"header on complete: "<<ctime(&header_end_time);
            cout<<"body: "<<body<<endl;
            cout<<"message on complete: "<<ctime(&message_end_time);
            printf("major_version: %d major_version: %d keppalive: %d upgrade: %d", major_version, minor_version, keepalive, upgrade);
        }
    private: 
        unsigned char major_version;
        unsigned char minor_version;
        unsigned char upgrade;
        unsigned char keepalive;
        time_t parse_start_time;
        time_t header_end_time;
        time_t message_end_time;
        string url;
        string status;
        vector<string> keys;
        vector<string> values;
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
