#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "puremvc/puremvc.h"

#include "ModelTest.h"
#include "ModelTestProxy.h"
#include "ModelTestProxy2.h"

int main() {
    testGetInstance();
    testRegisterAndRetrieveProxy();
    testRegisterAndRemoveProxy();
    testRegisterAndReplaceProxy();
    testHasProxy();
    testOnRegisterAndOnRemove();
    testRemoveModel();
    testMultipleModels();
    testEmbeddedProxy();
    return 0;
}

void testGetInstance() {
    // Test Factory Method
    const char *error = NULL;
    struct IModel *model = puremvc_model_getInstance("ModelTestKey1", puremvc_model_new, &error);
    assert(model != NULL);
    model->initializeModel(model, &error);

    // test assertions
    assert(model == puremvc_model_getInstance("ModelTestKey1", puremvc_model_new, &error));
    puremvc_model_removeModel("ModelTestKey1");
    model = NULL;
}

void testRegisterAndRetrieveProxy() {
    // register a new and retrieve it.
    const char *error = NULL;
    struct IModel *model = puremvc_model_getInstance("ModelTestKey2", puremvc_model_new, &error);
    model->initializeModel(model, &error);

    const char **colors = malloc(sizeof(char*) * 4);
    memset(colors, 0, sizeof(char*) * 4);
    for(const char **data = (const char *[]) {"red", "green", "blue", NULL}, **cursor = colors; *data; data++, cursor++) {
        *cursor = strdup(*data);
    }

    model->registerProxy(model, puremvc_proxy_new("colors", colors, &error), &error);
    const struct IProxy *proxy = model->retrieveProxy(model, "colors");

    assert(proxy != NULL);
    const char **data = proxy->getData(proxy);

    // test assertions
    assert(strcmp(*data, "red") == 0);
    assert(strcmp(*(data + 1), "green") == 0);
    assert(strcmp(*(data + 2), "blue") == 0);

    struct IProxy *removedProxy = model->removeProxy(model, "colors");
    assert(strcmp(removedProxy->getName(removedProxy), "colors") == 0);
    puremvc_proxy_free(&removedProxy);

    assert(model->retrieveProxy(model, "colors") == NULL);
    puremvc_model_removeModel("ModelTestKey2");
    model = NULL;
}

void testRegisterAndRemoveProxy() {
    // register a new, remove it, then try to retrieve it
    const char *error = NULL;
    struct IModel *model = puremvc_model_getInstance("ModelTestKey4", puremvc_model_new, &error);
    model->initializeModel(model, &error);

    int *sizes = malloc(sizeof(int) * 4), *cursor = sizes;
    memset(sizes, 0, sizeof(int) * 4);
    for(const int *data = (int []) {1, 2, 3, 0}; *data != 0; data++, cursor++) // 0 as terminator, or use -1
        *cursor = *data;

    model->registerProxy(model, puremvc_proxy_new("sizes", sizes, &error), &error);

    // remove the new
    struct IProxy *removedProxy = model->removeProxy(model, "sizes");

    // assert that we removed the appropriate new
    assert(strcmp(removedProxy->getName(removedProxy), "sizes") == 0);

    // ensure that the new is no longer retrievable from the model
    assert(model->retrieveProxy(model, "sizes") == NULL);

    puremvc_proxy_free(&removedProxy);
    puremvc_model_removeModel("ModelTestKey4");
    model = NULL;
}

void testHasProxy() {
    // register a new
    const char *error = NULL;
    struct IModel *model = puremvc_model_getInstance("ModelTestKey5", puremvc_model_new, &error);
    model->initializeModel(model, &error);

    const char **aces = malloc(sizeof(char*) * 5), **cursor = aces;
    memset(aces, 0, sizeof(char*) * 5);
    for(const char **data = (const char *[]) {"clubs", "spades", "hearts", "diamonds", NULL}; *data; data++, cursor++) {
        *cursor = strdup(*data);
    }

    model->registerProxy(model, puremvc_proxy_new("aces", aces, &error), &error);

    // assert that the model.hasProxy method returns true
    // for that new name
    assert(model->hasProxy(model, "aces") == true);

    // remove the new
    struct IProxy *proxy = model->removeProxy(model, "aces");

    assert(proxy != NULL);
    puremvc_proxy_free(&proxy);

    // assert that the model.hasProxy method returns false
    // for that new name
    assert(model->hasProxy(model, "aces") == false);

    puremvc_model_removeModel("ModelTestKey5");
    model = NULL;
}

void testOnRegisterAndOnRemove() {
    // Get a Multiton Model instance
    const char *error = NULL;
    struct IModel *model = puremvc_model_getInstance("ModelTestKey6", puremvc_model_new, &error);
    model->initializeModel(model, &error);

    // Create and register the test proxy
    struct IProxy *proxy = model_test_proxy_new("ModelTestProxy", NULL, &error);
    model->registerProxy(model, proxy, &error);

    // assert that onRegister was called, and the new responded by setting its data accordingly
    assert(strcmp(proxy->getData(proxy), ON_REGISTER_CALLED) == 0);

    // Remove the proxy
    model->removeProxy(model, "ModelTestProxy");

    // assert that onRemove was called, and the new responded by setting its data accordingly
    assert(strcmp(proxy->getData(proxy), ON_REMOVE_CALLED) == 0);
    puremvc_proxy_free(&proxy);

    puremvc_model_removeModel("ModelTestKey6");
    model = NULL;
}

void testRemoveModel() {
    // Get a Multiton Model instance
    const char *error = NULL;
    struct IModel *model = puremvc_model_getInstance("ModelTestKey6", puremvc_model_new, &error);
    model->initializeModel(model, &error);

    // remove the model
    puremvc_model_removeModel("ModelTestKey6");

    // re-create the model without throwing an exception
    puremvc_model_new("ModelTestKey6", &error);

    // cleanup
    puremvc_model_removeModel("ModelTestKey6");
}

void testMultipleModels() {
    // Get a Multiton Model instance
    const char *error = NULL;
    struct IModel *model1 = puremvc_model_getInstance("ModelTestKey7", puremvc_model_new, &error);
    model1->initializeModel(model1, &error);
    struct IModel *model2 = puremvc_model_getInstance("ModelTestKey8", puremvc_model_new, &error);
    model2->initializeModel(model2, &error);

    const char **colors = malloc(sizeof(char*) * 4);
    memset(colors, 0, sizeof(char*) * 4);
    for(const char **data = (const char *[]) {"red", "green", "blue", NULL}, **cursor = colors; *data; data++, cursor++) {
        *cursor = strdup(*data);
    }

    const char **aces = malloc(sizeof(char*) * 5);
    memset(aces, 0, sizeof(char*) * 5);
    for(const char **data = (const char *[]) {"clubs", "spades", "hearts", "diamonds", NULL}, **cursor = aces; *data; data++, cursor++) {
        *cursor = strdup(*data);
    }

    model1->registerProxy(model1, puremvc_proxy_new("colors", colors, &error), &error);
    model2->registerProxy(model2, puremvc_proxy_new("aces", aces, &error), &error);

    assert(model1->hasProxy(model1, "colors"));
    assert(!model2->hasProxy(model2, "colors"));

    assert(!model1->hasProxy(model1, "aces"));
    assert(model2->hasProxy(model2, "aces"));

    struct IProxy *proxy = model1->removeProxy(model1, "colors");
    if (proxy != NULL) puremvc_proxy_free(&proxy);
    struct IProxy *proxy2 = model2->removeProxy(model2, "aces");
    if (proxy2 != NULL) puremvc_proxy_free(&proxy2);

    puremvc_model_removeModel("ModelTestKey7");
    puremvc_model_removeModel("ModelTestKey8");
}

void testRegisterAndReplaceProxy() {
    const char *error = NULL;
    struct IModel *model = puremvc_model_getInstance("ModelTestKey8", puremvc_model_new, &error);
    model->initializeModel(model, &error);

    int *sizes = malloc(sizeof(int) * 2);
    memset(sizes, 0, sizeof(int) * 2);
    for(int *data = (int []) {1, 0}, *cursor = sizes; *data != 0; data++, cursor++) // 0 as terminator, or use -1
        *cursor = *data;

    model->registerProxy(model, puremvc_proxy_new("sizes", sizes, &error), &error);

    const char **colors = malloc(sizeof(char*) * 4);
    memset(colors, 0, sizeof(char*) * 4);
    for(const char **data = (const char *[]) {"red", "green", "blue", NULL}, **cursor = colors; *data; data++, cursor++) {
        *cursor = strdup(*data);
    }

    model->registerProxy(model, puremvc_proxy_new("sizes", colors, &error), &error);

    const struct IProxy *proxy = model->retrieveProxy(model, "sizes");

    assert(proxy != NULL);
    const char **data = proxy->getData(proxy);

    // test assertions
    assert(strcmp(*data, "red") == 0);
    assert(strcmp(*(data + 1), "green") == 0);
    assert(strcmp(*(data + 2), "blue") == 0);

    struct IProxy *removedProxy = model->removeProxy(model, "sizes");
    assert(strcmp(removedProxy->getName(removedProxy), "sizes") == 0);
    puremvc_proxy_free(&removedProxy);

    assert(model->retrieveProxy(model, "sizes") == NULL);
    puremvc_model_removeModel("ModelTestKey8");
    model = NULL;
}

void testEmbeddedProxy() {
    // Get a Multiton Model instance
    const char *error = NULL;
    struct IModel *model = puremvc_model_getInstance("ModelTestKey8", puremvc_model_new, &error);
    model->initializeModel(model, &error);

    // Create and register the test proxy
    struct IProxy *proxy = model_test_proxy2_new("ModelTestProxy2", NULL, &error);
    model->registerProxy(model, (struct IProxy *)proxy, &error); // breaks on release

    // assert that onRegister was called, and the new responded by setting its data accordingly
    assert(strcmp(proxy->getData(proxy), ON_REGISTER_CALLED2) == 0);

    // Remove the proxy
    model->removeProxy(model, "ModelTestProxy2");

    // assert that onRemove was called, and the new responded by setting its data accordingly
    assert(strcmp(proxy->getData(proxy), ON_REMOVE_CALLED2) == 0);

    puremvc_proxy_free(&proxy);

    free(proxy);
    proxy = NULL;
}