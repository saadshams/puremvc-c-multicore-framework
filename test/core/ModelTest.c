#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "puremvc/puremvc.h"

#include "ModelTest.h"
#include "ModelTestProxy.h"

int main() {
    testGetInstance();
    testRegisterAndRetrieveProxy();
    testRegisterAndRemoveProxy();
    testHasProxy();
    testOnRegisterAndOnRemove();
    testRemoveModel();
    testMultipleModels();
    return 0;
}

void testGetInstance() {
    // Test Factory Method
    const struct IModel *model = puremvc_model_getInstance("ModelTestKey1", puremvc_model_new);
    assert(model != NULL);

    // test assertions
    assert(model == puremvc_model_getInstance("ModelTestKey1", puremvc_model_new));
    puremvc_model_removeModel("ModelTestKey1");
    model = NULL;
}

void testRegisterAndRetrieveProxy() {
    // register a new and retrieve it.
    const struct IModel *model = puremvc_model_getInstance("ModelTestKey2", puremvc_model_new);

    const char **colors = malloc(sizeof(char*) * 4);
    memset(colors, 0, sizeof(char*) * 4);
    for(const char **data = (const char *[]) {"red", "green", "blue", NULL}, **cursor = colors; *data; data++, cursor++) {
        *cursor = strdup(*data);
    }

    model->registerProxy(model, puremvc_proxy_new("colors", colors));
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
    const struct IModel *model = puremvc_model_getInstance("ModelTestKey4", puremvc_model_new);

    int *sizes = malloc(sizeof(int) * 4), *cursor = sizes;
    memset(sizes, 0, sizeof(int) * 4);
    for(const int *data = (int []) {1, 2, 3, 0}; *data != 0; data++, cursor++) // 0 as terminator, or use -1
        *cursor = *data;

    model->registerProxy(model, puremvc_proxy_new("sizes", sizes));

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
    const struct IModel *model = puremvc_model_getInstance("ModelTestKey5", puremvc_model_new);

    const char **aces = malloc(sizeof(char*) * 5), **cursor = aces;
    memset(aces, 0, sizeof(char*) * 5);
    for(const char **data = (const char *[]) {"clubs", "spades", "hearts", "diamonds", NULL}; *data; data++, cursor++) {
        *cursor = strdup(*data);
    }

    model->registerProxy(model, puremvc_proxy_new("aces", aces));

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
    const struct IModel *model = puremvc_model_getInstance("ModelTestKey6", puremvc_model_new);

    // Create and register the test mediator
    struct IProxy *modelTestProxy = model_test_proxy_new("ModelTestProxy", NULL);
    model->registerProxy(model, modelTestProxy);

    // assert that onRegister was called, and the new responded by setting its data accordingly
    assert(strcmp(modelTestProxy->getData(modelTestProxy), ON_REGISTER_CALLED) == 0);

    // Remove the component
    model->removeProxy(model, "ModelTestProxy");

    // assert that onRemove was called, and the new responded by setting its data accordingly
    assert(strcmp(modelTestProxy->getData(modelTestProxy), ON_REMOVE_CALLED) == 0);
    puremvc_proxy_free(&modelTestProxy);

    puremvc_model_removeModel("ModelTestKey6");
    model = NULL;
}

void testRemoveModel() {
    // Get a Multiton Model instance
    puremvc_model_getInstance("ModelTestKey6", puremvc_model_new);

    // remove the model
    puremvc_model_removeModel("ModelTestKey6");

    // re-create the model without throwing an exception
    puremvc_model_new("ModelTestKey6");

    // cleanup
    puremvc_model_removeModel("ModelTestKey6");
}

void testMultipleModels() {
    // Get a Multiton Model instance
    const struct IModel *model1 = puremvc_model_getInstance("ModelTestKey7", puremvc_model_new);
    const struct IModel *model2 = puremvc_model_getInstance("ModelTestKey8", puremvc_model_new);

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

    model1->registerProxy(model1, puremvc_proxy_new("colors", colors));
    model2->registerProxy(model2, puremvc_proxy_new("aces", aces));

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
