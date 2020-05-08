#include "interfaces/Proxy.h"
#include "interfaces/Notifier.h"
#include "ProxyTest.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

int main() {
    testNameAccessor();
    testDataAccessors();
    testConstructor();
    puts("ProxyTest: Success");
    return 0;
}

typedef struct Array Array;
struct Array {
    int length;
    void **values;
};

void testNameAccessor() {
    Proxy *proxy = NewProxy("TestProxy", NULL);
    assert(proxy != NULL);
    assert(proxy->notifier != NULL);
    assert(strcmp(proxy->getProxyName(proxy), "TestProxy") == 0);
    DeleteProxy(proxy);
}

void testDataAccessors() {
    Proxy *proxy = NewProxy("colors", NULL);
    assert(proxy != NULL);
    proxy->setData(proxy, &(Array) {3, (void **) (char *[]) {"red", "green", "blue"}});

    Array *data = proxy->getData(proxy);
    assert(data->length == 3);
    assert(strcmp(data->values[0], "red") == 0);
    assert(strcmp(data->values[1], "green") == 0);
    assert(strcmp(data->values[2], "blue") == 0);

    DeleteProxy(proxy);
}

void testConstructor() {
    Proxy *proxy = NewProxy("colors", &(Array) {3, (void **) (char *[]) {"red", "green", "blue"}});
    Array *data = proxy->getData(proxy);

    assert(proxy != NULL);
    assert(strcmp(proxy->getProxyName(proxy), "colors") == 0);

    assert(data->length == 3);
    assert(strcmp(data->values[0], "red") == 0);
    assert(strcmp(data->values[1], "green") == 0);
    assert(strcmp(data->values[2], "blue") == 0);

    DeleteProxy(proxy);
}

#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

// https://jameshfisher.com/2016/12/20/http-hello-world/
// https://rosettacode.org/wiki/Hello_world/Web_server#C
int service() {
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_port = htons(8080);
    server.sin_addr.s_addr = htonl(INADDR_ANY);
    bind(server_fd, (struct sockaddr *) &server, sizeof(server));
    listen(server_fd, 128);
    for (;;) {
        int client_fd = accept(server_fd, NULL, NULL);
        char response[] = "HTTP/1.1 200 OK\r\nContent-Length: 13\r\nConnection: close\r\n\r\nHello, world!";
        for (int sent = 0;
             sent < sizeof(response); sent += send(client_fd, response + sent, sizeof(response) - sent, 0));
        close(client_fd);
    }
    return 0;
}
