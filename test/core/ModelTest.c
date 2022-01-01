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
    Model *model = $Model.getInstance("ModelTestKey1", $Model.new);
    assert(model != NULL);

    // test assertions
    assert(model == $Model.getInstance("ModelTestKey1", $Model.new));
    $Model.removeModel("ModelTestKey1");
    model = NULL;
}

/**
 * Tests the new registration and retrieval methods.
 *
 * <P>Tests <code>registerProxy</code> and <code>retrieveProxy</code> in the same test.
 * These methods cannot currently be tested separately
 * in any meaningful way other than to show that the
 * methods do not throw exception when called. </P>
 */
void testRegisterAndRetrieveProxy() {
    // register a new and retrieve it.
    Model *model = $Model.getInstance("ModelTestKey2", $Model.new);
    model->registerProxy(model, $Proxy.new("colors", (char *[]) {"red", "green", "blue", NULL}));
    Proxy *proxy = model->retrieveProxy(model, "colors");

    assert(proxy != NULL);
    const char **data = proxy->getData(proxy);

    // test assertions
    assert(strcmp(*data, "red") == 0);
    assert(strcmp(*(data + 1), "green") == 0);
    assert(strcmp(*(data + 2), "blue") == 0);

    Proxy *removedProxy = model->removeProxy(model, "colors");
    assert(strcmp(removedProxy->proxyName, "colors") == 0);
    $Proxy.delete(removedProxy);
    assert(model->retrieveProxy(model, "colors") == NULL);
    $Model.removeModel("ModelTestKey2");
    model = NULL;
}

/**
 * Tests the new removal method.
 */
void testRegisterAndRemoveProxy() {
    // register a new, remove it, then try to retrieve it
    Model *model = $Model.getInstance("ModelTestKey4", $Model.new);
    model->registerProxy(model, $Proxy.new("sizes", (char *[]) {"7", "13", "21"}));

    // remove the new
    Proxy *removedProxy = model->removeProxy(model, "sizes");

    // assert that we removed the appropriate new
    assert(strcmp(removedProxy->proxyName, "sizes") == 0);

    // ensure that the new is no longer retrievable from the model
    assert(model->retrieveProxy(model, "sizes") == NULL);

    $Proxy.delete(removedProxy);
    $Model.removeModel("ModelTestKey4");
    model = NULL;
}

/**
 * Tests the hasProxy Method
 */
void testHasProxy() {
    // register a new
    Model *model = $Model.getInstance("ModelTestKey5", $Model.new);
    model->registerProxy(model, $Proxy.new("aces", (char *[]) {"clubs", "spades", "hearts", "diamonds"}));

    // assert that the model.hasProxy method returns true
    // for that new name
    assert(model->hasProxy(model, "aces") == true);

    // remove the new
    Proxy *proxy = model->removeProxy(model, "aces");

    assert(proxy != NULL);
    $Proxy.delete(proxy);

    // assert that the model.hasProxy method returns false
    // for that new name
    assert(model->hasProxy(model, "aces") == false);

    $Model.removeModel("ModelTestKey5");
    model = NULL;
}

/**
 * Tests that the Model calls the onRegister and onRemove methods
 */
void testOnRegisterAndOnRemove() {
    // Get a Multiton Model instance
    Model *model = $Model.getInstance("ModelTestKey6", $Model.new);

    // Create and register the test mediator
    ModelTestProxy *modelTestProxy = NewModelTestProxy("ModelTestProxy", NULL);
    model->registerProxy(model, (Proxy *)modelTestProxy);

    // assert that onRegister was called, and the new responded by setting its data accordingly
    char *data = ON_REGISTER_CALLED;
    assert(strcmp(modelTestProxy->proxy.getData(&modelTestProxy->proxy), data) == 0);

    // Remove the component
    model->removeProxy(model, "ModelTestProxy");

    // assert that onRemove was called, and the new responded by setting its data accordingly
    data = ON_REMOVE_CALLED;
    assert(strcmp(modelTestProxy->proxy.data, data) == 0);
    $Proxy.delete((Proxy *) modelTestProxy);

    $Model.removeModel("ModelTestKey6");
    model = NULL;
}

void testRemoveModel() {
    // Get a Multiton Model instance
    $Model.getInstance("ModelTestKey6", $Model.new);

    // remove the model
    $Model.removeModel("ModelTestKey6");

    // re-create the model without throwing an exception
    $Model.new("ModelTestKey6");

    // cleanup
    $Model.removeModel("ModelTestKey6");
}
