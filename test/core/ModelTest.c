#include "ModelTest.h"
#include "interfaces/Model.h"
#include "ModelTestProxy.h"
#include <assert.h>
#include <string.h>
#include <stdio.h>

/**
 * Test the PureMVC Model class.
 */
int main() {
    testGetInstance();
    testRegisterAndRetrieveProxy();
    testRegisterAndRemoveProxy();
    testHasProxy();
    testOnRegisterAndOnRemove();
    testRemoveModel();
    puts("ModelTest: Success");
    return 0;
}

/**
 * Tests the Model Multiton Factory Method
 */
void testGetInstance() {
    // Test Factory Method
    Model *model = getModelInstance("ModelTestKey1", NewModel);
    assert(model != NULL);

    // test assertions
    assert(model == getModelInstance("ModelTestKey1", NewModel));
    RemoveModel("ModelTestKey1");
    model = NULL;
}

/**
 * Tests the proxy registration and retrieval methods.
 *
 * <P>Tests <code>registerProxy</code> and <code>retrieveProxy</code> in the same test.
 * These methods cannot currently be tested separately
 * in any meaningful way other than to show that the
 * methods do not throw exception when called. </P>
 */
void testRegisterAndRetrieveProxy() {
    // register a proxy and retrieve it.
    Model *model = getModelInstance("ModelTestKey2", NewModel);
    model->registerProxy(model, NewProxy("colors", (char *[]) {"red", "green", "blue", NULL}));
    Proxy *proxy = model->retrieveProxy(model, "colors");

    assert(proxy != NULL);
    const char **data = proxy->getData(proxy);

    // test assertions
    assert(strcmp(*data, "red") == 0);
    assert(strcmp(*(data + 1), "green") == 0);
    assert(strcmp(*(data + 2), "blue") == 0);

    Proxy *removedProxy = model->removeProxy(model, "colors");
    assert(strcmp(removedProxy->proxyName, "colors") == 0);
    DeleteProxy(removedProxy);
    assert(model->retrieveProxy(model, "colors") == NULL);
    RemoveModel("ModelTestKey2");
    model = NULL;
}

/**
 * Tests the proxy removal method.
 */
void testRegisterAndRemoveProxy() {
    // register a proxy, remove it, then try to retrieve it
    Model *model = getModelInstance("ModelTestKey4", NewModel);
    model->registerProxy(model, NewProxy("sizes", (char *[]) {"7", "13", "21"}));

    // remove the proxy
    Proxy *removedProxy = model->removeProxy(model, "sizes");

    // assert that we removed the appropriate proxy
    assert(strcmp(removedProxy->proxyName, "sizes") == 0);

    // ensure that the proxy is no longer retrievable from the model
    assert(model->retrieveProxy(model, "sizes") == NULL);

    DeleteProxy(removedProxy);
    RemoveModel("ModelTestKey4");
    model = NULL;
}

/**
 * Tests the hasProxy Method
 */
void testHasProxy() {
    // register a proxy
    Model *model = getModelInstance("ModelTestKey5", NewModel);
    model->registerProxy(model, NewProxy("aces", (char *[]) {"clubs", "spades", "hearts", "diamonds"}));

    // assert that the model.hasProxy method returns true
    // for that proxy name
    assert(model->hasProxy(model, "aces") == true);

    // remove the proxy
    Proxy *proxy = model->removeProxy(model, "aces");

    assert(proxy != NULL);
    DeleteProxy(proxy);

    // assert that the model.hasProxy method returns false
    // for that proxy name
    assert(model->hasProxy(model, "aces") == false);

    RemoveModel("ModelTestKey5");
    model = NULL;
}

/**
 * Tests that the Model calls the onRegister and onRemove methods
 */
void testOnRegisterAndOnRemove() {
    // Get a Multiton Model instance
    Model *model = getModelInstance("ModelTestKey6", NewModel);

    // Create and register the test mediator
    ModelTestProxy *modelTestProxy = NewModelTestProxy("ModelTestProxy", NULL);
    model->registerProxy(model, (Proxy *)modelTestProxy);

    // assert that onRegister was called, and the proxy responded by setting its data accordingly
    char *data = ON_REGISTER_CALLED;
    assert(strcmp(modelTestProxy->proxy.getData(&modelTestProxy->proxy), data) == 0);

    // Remove the component
    model->removeProxy(model, "ModelTestProxy");

    // assert that onRemove was called, and the proxy responded by setting its data accordingly
    data = ON_REMOVE_CALLED;
    assert(strcmp(modelTestProxy->proxy.data, data) == 0);
    DeleteProxy((Proxy *) modelTestProxy);

    RemoveModel("ModelTestKey6");
    model = NULL;
}

void testRemoveModel() {
    // Get a Multiton Model instance
    getModelInstance("ModelTestKey6", NewModel);

    // remove the model
    RemoveModel("ModelTestKey6");

    // re-create the model without throwing an exception
    NewModel("ModelTestKey6");

    // cleanup
    RemoveModel("ModelTestKey6");
}