#ifndef HTTP_H
#define HTTP_H

enum	HTTP_REQUEST_TYPE
{
	HTTP_REQ_INDEX_HTML,
	HTTP_REQ_READDATA,
	HTTP_REQ_WRITEDATA,
	HTTP_REQ_OTHER
};

enum	HTTP_REQUEST_TYPE	http_parse_request(char *request);
void						http_200OK(char *response, char *data);
int							http_find_data(char *request, char **indata);

#endif
