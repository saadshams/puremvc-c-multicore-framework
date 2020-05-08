#include "ModelTest.h"
#include "interfaces/Model.h"
#include "ModelTestProxy.h"
#include <assert.h>
#include <string.h>
#include <stdio.h>

typedef struct Array Array;
struct Array {
    int length;
    char **values;
};

int main() {
    testGetInstance();
    testRegisterAndRetrieveProxy();
    testRegisterAndRemoveProxy();
    testReRegisterAndRetrieveProxy();
    testHasProxy();
    testOnRegisterAndOnRemove();
    testRemoveModel();
    puts("ModelTest: Success");
    return 0;
}

void testGetInstance() {
    Model *model = getModelInstance("ModelTestKey1", NewModel);
    assert(model != NULL);
    assert(model == getModelInstance("ModelTestKey1", NewModel));
}

void testRegisterAndRetrieveProxy() {
    Model *model = getModelInstance("ModelTestKey2", NewModel);
    model->registerProxy(model, NewProxy("colors", &(Array) {3, (char *[]) {"red", "green", "blue"}}));
    Proxy *proxy = model->retrieveProxy(model, "colors");

    assert(proxy != NULL);
    Array *data = proxy->getData(proxy);
    assert(data->length == 3);
    assert(strcmp(data->values[0], "red") == 0);
    assert(strcmp(data->values[1], "green") == 0);
    assert(strcmp(data->values[2], "blue") == 0);

    Proxy *removedProxy = model->removeProxy(model, "colors");
    assert(strcmp(removedProxy->proxyName, "colors") == 0);

    assert(model->retrieveProxy(model, "colors") == NULL);
}

void testRegisterAndRemoveProxy() {
    Model *model = getModelInstance("ModelTestKey4", NewModel);
    model->registerProxy(model, NewProxy("colors", &(Array) {3, (char *[]) {"red", "green", "blue"}}));
    model->registerProxy(model, NewProxy("aces", &(Array) {4, (char *[]) {"clubs", "spades", "hearts", "diamonds"}}));
    model->registerProxy(model, NewProxy("stooges", &(Array) {3, (char *[]) {"moe", "larry", "curly"}}));
    model->registerProxy(model, NewProxy("sizes", &(Array) {3, (char *[]) {"7", "13", "21"}}));

    // removal order FIFO
    assert(strcmp(model->removeProxy(model, "colors")->proxyName, "colors") == 0);
    assert(strcmp(model->removeProxy(model, "aces")->proxyName, "aces") == 0);
    assert(strcmp(model->removeProxy(model, "stooges")->proxyName, "stooges") == 0);
    assert(strcmp(model->removeProxy(model, "sizes")->proxyName, "sizes") == 0);

    assert(model->retrieveProxy(model, "colors") == NULL);
    assert(model->retrieveProxy(model, "aces") == NULL);
    assert(model->retrieveProxy(model, "stooges") == NULL);
    assert(model->retrieveProxy(model, "sizes") == NULL);

    // change removal order - random
    model->registerProxy(model, NewProxy("colors", &(Array) {3, (char *[]) {"red", "green", "blue"}}));
    model->registerProxy(model, NewProxy("aces", &(Array) {4, (char *[]) {"clubs", "spades", "hearts", "diamonds"}}));
    model->registerProxy(model, NewProxy("stooges", &(Array) {3, (char *[]) {"moe", "larry", "curly"}}));
    model->registerProxy(model, NewProxy("sizes", &(Array) {3, (char *[]) {"7", "13", "21"}}));

    assert(strcmp(model->removeProxy(model, "stooges")->proxyName, "stooges") == 0);
    assert(strcmp(model->removeProxy(model, "aces")->proxyName, "aces") == 0);
    assert(strcmp(model->removeProxy(model, "sizes")->proxyName, "sizes") == 0);
    assert(strcmp(model->removeProxy(model, "colors")->proxyName, "colors") == 0);

    assert(model->retrieveProxy(model, "colors") == NULL);
    assert(model->retrieveProxy(model, "aces") == NULL);
    assert(model->retrieveProxy(model, "stooges") == NULL);
    assert(model->retrieveProxy(model, "sizes") == NULL);

    // change removal order - LIFO
    model->registerProxy(model, NewProxy("colors", &(Array) {3, (char *[]) {"red", "green", "blue"}}));
    model->registerProxy(model, NewProxy("aces", &(Array) {4, (char *[]) {"clubs", "spades", "hearts", "diamonds"}}));
    model->registerProxy(model, NewProxy("stooges", &(Array) {3, (char *[]) {"moe", "larry", "curly"}}));
    model->registerProxy(model, NewProxy("sizes", &(Array) {3, (char *[]) {"7", "13", "21"}}));

    assert(model->removeProxy(model, "unknown") == NULL);
    assert(strcmp(model->removeProxy(model, "stooges")->proxyName, "stooges") == 0);
    assert(strcmp(model->removeProxy(model, "colors")->proxyName, "colors") == 0);
    assert(strcmp(model->removeProxy(model, "aces")->proxyName, "aces") == 0);
    assert(strcmp(model->removeProxy(model, "sizes")->proxyName, "sizes") == 0);
    assert(model->removeProxy(model, "unknown") == NULL);

    assert(model->retrieveProxy(model, "unknown") == NULL);
    assert(model->retrieveProxy(model, "colors") == NULL);
    assert(model->retrieveProxy(model, "aces") == NULL);
    assert(model->retrieveProxy(model, "stooges") == NULL);
    assert(model->retrieveProxy(model, "sizes") == NULL);
    assert(model->retrieveProxy(model, "unknown") == NULL);
}

void testReRegisterAndRetrieveProxy() {
    Model *model = getModelInstance("ModelTestKey4_1", NewModel);
    model->registerProxy(model, NewProxy("colors", &(Array) {3, (char *[]) {"red", "green", "blue"}}));
    model->registerProxy(model, NewProxy("colors", &(Array) {2, (char *[]) {"yellow", "orange"}})); // re-register
    Proxy *proxy = model->retrieveProxy(model, "colors");

    assert(proxy != NULL);
    Array *data = proxy->getData(proxy);
    assert(data != NULL);
    assert(data->length == 2);
    assert(strcmp(data->values[0], "yellow") == 0);
    assert(strcmp(data->values[1], "orange") == 0);

    assert(strcmp(model->removeProxy(model, "colors")->proxyName, "colors") == 0);
    assert(model->retrieveProxy(model, "colors") == NULL);
}

void testHasProxy() {
    Model *model = getModelInstance("ModelTestKey5", NewModel);
    model->registerProxy(model, NewProxy("aces", &(Array) {4, (char *[]) {"clubs", "spades", "hearts", "diamonds"}}));
    assert(model->hasProxy(model, "aces") == true);
    Proxy *proxy = model->removeProxy(model, "aces");

    assert(proxy != NULL);
    DeleteProxy(proxy);

    assert(model->hasProxy(model, "aces") == false);

    model->registerProxy(model, NewProxy("colors", &(Array) {3, (char *[]) {"red", "green", "blue"}}));
    model->registerProxy(model, NewProxy("aces", &(Array) {4, (char *[]) {"clubs", "spades", "hearts", "diamonds"}}));
    model->registerProxy(model, NewProxy("stooges", &(Array) {3, (char *[]) {"moe", "larry", "curly"}}));
    model->registerProxy(model, NewProxy("sizes", &(Array) {3, (char *[]) {"7", "13", "21"}}));

    assert(model->hasProxy(model, "stooges") == true);
    assert(model->hasProxy(model, "aces") == true);
    assert(model->hasProxy(model, "colors") == true);
    assert(model->hasProxy(model, "sizes") == true);

    assert(strcmp(model->removeProxy(model, "stooges")->proxyName, "stooges") == 0);
    assert(strcmp(model->removeProxy(model, "colors")->proxyName, "colors") == 0);
    assert(strcmp(model->removeProxy(model, "aces")->proxyName, "aces") == 0);
    assert(strcmp(model->removeProxy(model, "sizes")->proxyName, "sizes") == 0);

    assert(model->retrieveProxy(model, "colors") == NULL);
    assert(model->retrieveProxy(model, "aces") == NULL);
    assert(model->retrieveProxy(model, "stooges") == NULL);
    assert(model->retrieveProxy(model, "sizes") == NULL);
}

void testOnRegisterAndOnRemove() {
    Model *model = getModelInstance("ModelTestKey6", NewModel);

    ModelTestProxy *modelTestProxy = NewModelTestProxy("ModelTestProxy", NULL);
    model->registerProxy(model, (Proxy *)modelTestProxy);

    char *data = ON_REGISTER_CALLED;
    assert(strcmp(modelTestProxy->proxy.getData(&modelTestProxy->proxy), data) == 0);

    model->removeProxy(model, "ModelTestProxy");

    data = ON_REMOVE_CALLED;
    assert(modelTestProxy->proxy.data == data);
}

void testRemoveModel() {

}