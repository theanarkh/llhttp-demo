request: request.cc
	gcc request.cc llhttp.c api.c http.c -o request && ./request

response: response.cc
	gcc response.cc llhttp.c api.c http.c -o reponse && ./reponse

request-with-static: request.cc
	gcc request.cc -o request -lllhttp -L. && ./request

response-with-static: response.cc
	gcc response.cc -o reponse -lllhttp -L. && ./reponse