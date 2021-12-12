#include "http.h"

#include <string.h>
#include <stdio.h>

#include "http.h"
// parse header of input HTTP-packet from client:
enum HTTP_REQUEST_TYPE http_parse_request (char *request) {
	const char http_index[] = "GET / HTTP/1.1";
	const char http_readdata[] = "GET /readdata HTTP/1.1";
	const char http_writedata[] = "POST /writedata HTTP/1.1";
	if (strncmp(request, http_index, strlen(http_index)) == 0) 
		return HTTP_REQ_INDEX_HTML;
	if (strncmp(request, http_readdata, strlen(http_readdata)) == 0)
		return HTTP_REQ_READDATA;
	if (strncmp(request, http_writedata, strlen(http_writedata)) == 0) 
		return HTTP_REQ_WRITEDATA;
	return HTTP_REQ_OTHER;
}
// find data in input HTTP-packet from client:
int http_find_data (char *request, char **indata) {
// find start of input data:
	for (int i = 0; i < (strlen(request)-4); i++) {
		if (strncmp(request+i, "\r\n\r\n", 4) == 0) {
			*indata = request+i+4;
		break;
		}
	}
	// check data:
	int len = strlen(*indata) - 4;
	if (strncmp(*indata+len, "\r\n\r\n", 4) == 0) {
		return len;
	}
	return -1;
}
// create output HTTP-packet to client:
void http_200OK (char *response, char *data) {
	strcpy(response, "HTTP/1.1 200 OK\r\n");
	strcat(response, "Status: 200 OK\r\n");
	strcat(response, "Content-Length: ");
	if (strlen(data) == 0) {
		strcat(response, "0\r\n\r\n");
	} else {
		sprintf(response+strlen(response), "%lu\r\n\r\n%s", strlen(data), data);
	}
}
