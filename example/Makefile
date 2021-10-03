request: request.cc
	gcc request.cc ../llhttp.c ../api.c ../http.c -o request -g && ./request && rm request

response: response.cc
	gcc response.cc ../llhttp.c ../api.c ../http.c -o reponse -g && ./reponse && rm reponse

request-with-static: request.cc
	gcc request.cc -o request -lllhttp -L.. && ./request -g && rm request

response-with-static: response.cc
	gcc response.cc -o reponse -lllhttp -L.. && ./reponse -g  && rm reponse

request-with-oop: request-oop.cc
	g++ request-oop.cc -o request -lllhttp -L.. --std=c++14 -g && ./request

response-with-oop: response-oop.cc
	g++ response-oop.cc -o response -lllhttp -L.. --std=c++14 -g && ./response && rm response

