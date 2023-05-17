#include <winsock2.h>
#include <stdio.h>
#include "server.h"
#include <unistd.h>

char *get_full_path(char *name)
{
    char filename[1024] = {0};
    getcwd(filename, 1024);

    if ((filename[strlen(filename)] != '\\') && 
        (name[strlen(name)] != '/') &&
        (name[strlen(name)] != '\\'))
    {
        strcat(filename, "\\");
    }

    int i;
    for (i = 0; name[i]; i++)
        if (name[i] == '/')
            name[i] = '\\';

    strcat(filename, name);
    return strdup(filename);
}

RESPONSE *GetResponse(REQUEST *request)
{
    RESPONSE *response;

    response = malloc(sizeof(RESPONSE));
    response->error    = 0;
    response->filename = request->value;
    response->filepath = get_full_path(request->value);
    response->header   = get_header(response);

    return response;
}

int SendResponse(SOCKET sock, RESPONSE* response)
{
    if (response->error) {
        send(sock, DEFAULT_ERROR_404, strlen(DEFAULT_ERROR_404), 0);
        return 1;
    }

    char response_msg[1024];
    snprintf(response_msg, sizeof(response_msg),
        "HTTP/1.1 200 OK\r\nContent-Length: 2\r\n\r\nOK");

    send(sock, response_msg, strlen(response_msg), 0);

    return 1;
}

