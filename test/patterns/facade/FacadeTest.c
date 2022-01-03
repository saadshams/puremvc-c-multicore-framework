#include "FacadeTest.h"
#include "interfaces/Facade.h"
#include "FacadeTestCommand.h"
#include "FacadeTestVO.h"
#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

/**
 * Test the PureMVC Facade class.
 *
 * @see FacadeTestVO
 * @see FacadeTestCommand
 */
int main(int argc, char *argv[]) {
    testGetInstance();
    testRegisterCommandAndSendNotification();
    testRegisterAndRemoveCommandAndSendNotification();
    testRegisterAndRetrieveProxy();
    testRegisterAndRemoveProxy();
    testRegisterRetrieveAndRemoveMediator();
    testHasProxy();
    testHasMediator();
    testHasCommand();
    testHasCoreAndRemoveCore();
    testGetInstancesThreaded();
    puts("TestFacade: Success");
    return 0;
}

/**
 * Tests the Facade Multiton Factory Method
 */
void testGetInstance() {
    // Test Factory Method
    Facade *facade = $Facade.getInstance("FacadeTestKey1", $Facade.new);

    // test assertions
    assert(facade != NULL);
    $Facade.removeFacade("FacadeTestKey1");
}

/**
 * Tests Command registration and execution via the Facade.
 *
 * <P>This test gets a Multiton Facade instance
 * and registers the FacadeTestCommand class
 * to handle 'FacadeTest' Notifcations.<P>
 *
 * <P>It then sends a notification using the Facade.
 * Success is determined by evaluating
 * a property on an object placed in the body of
 * the Notification, which will be modified by the Command.</P>
 *
 */
void testRegisterCommandAndSendNotification() {
    // Create the Facade, register the FacadeTestCommand to
    // handle 'FacadeTest' notifications
    Facade *facade = $Facade.getInstance("FacadeTestKey2", $Facade.new);
    facade->registerCommand(facade, "FacadeTestNote", (SimpleCommand *(*)()) NewFacadeTestCommand);

    // Send notification. The Command associated with the event
    // (FacadeTestCommand) will be invoked, and will multiply
    // the vo.input value by 2 and set the result on vo.result
    FacadeTestVO vo = {32};
    facade->sendNotification(facade, "FacadeTestNote", &vo, NULL);

    // test assertions
    assert(vo.result == 64);
    facade->removeCommand(facade, "FacadeTestNote");
    $Facade.removeFacade("FacadeTestKey2");
}

/**
 * Tests Command removal via the Facade.
 *
 * <P>This test gets a Multiton Facade instance
 * and registers the FacadeTestCommand class
 * to handle 'FacadeTest' Notifcations. Then it removes the command.<P>
 *
 * <P>It then sends a Notification using the Facade.
 * Success is determined by evaluating
 * a property on an object placed in the body of
 * the Notification, which will NOT be modified by the Command.</P>
 *
 */
void testRegisterAndRemoveCommandAndSendNotification() {
    // Create the Facade, register the FacadeTestCommand to
    // handle 'FacadeTest' events
    Facade *facade = $Facade.getInstance("FacadeTestKey3", $Facade.new);
    facade->registerCommand(facade, "FacadeTestNote", (SimpleCommand *(*)()) NewFacadeTestCommand);
    facade->removeCommand(facade, "FacadeTestNote");

    // Send notification. The Command associated with the event
    // (FacadeTestCommand) will NOT be invoked, and will NOT multiply
    // the vo.input value by 2
    FacadeTestVO vo = {32};
    facade->sendNotification(facade, "FacadeTestNote", &vo, NULL);

    // test assertions
    assert(vo.result != 64);
    $Facade.removeFacade("FacadeTestKey3");
}

/**
 * Tests the regsitering and retrieving Model proxies via the Facade.
 *
 * <P>Tests <code>registerProxy</code> and <code>retrieveProxy</code> in the same test.
 * These methods cannot currently be tested separately
 * in any meaningful way other than to show that the
 * methods do not throw exception when called. </P>
 */
void testRegisterAndRetrieveProxy() {
    // register a new and retrieve it.
    Facade *facade = $Facade.getInstance("FacadeTestKey4", $Facade.new);
    facade->registerProxy(facade, $Proxy.new("colors", (char *[]) {"red", "green", "blue", NULL}));
    Proxy *proxy = facade->retrieveProxy(facade, "colors");

    // test assertions
    assert(proxy != NULL);

    // retrieve data from new
    const char **data = proxy->getData(proxy);

    // test assertions
    assert(data != NULL);
    assert(strcmp(data[0], "red") == 0);
    assert(strcmp(data[1], "green") == 0);
    assert(strcmp(data[2], "blue") == 0);

    $Proxy.delete(facade->removeProxy(facade, "colors"));
    $Facade.removeFacade("FacadeTestKey4");
}

/**
 * Tests the removing Proxies via the Facade.
 */
void testRegisterAndRemoveProxy() {
    // register a new, remove it, then try to retrieve it
    Facade *facade = $Facade.getInstance("FacadeTestKey5", $Facade.new);
    facade->registerProxy(facade, $Proxy.new("sizes", (char *[]) {"7", "13", "21"}));

    // remove the new
    Proxy *removedProxy = facade->removeProxy(facade, "sizes");

    // assert that we removed the appropriate new
    assert(strcmp(removedProxy->getProxyName(removedProxy), "sizes") == 0);

    // test assertions - make sure we can no longer retrieve the new from the model
    assert(facade->retrieveProxy(facade, "sizes") == NULL);

    $Proxy.delete(removedProxy);
    $Facade.removeFacade("FacadeTestKey5");
}

/**
 * Tests registering, retrieving and removing Mediators via the Facade.
 */
void testRegisterRetrieveAndRemoveMediator() {
    // register a mediator, remove it, then try to retrieve it
    struct Object {
    } object;
    Facade *facade = $Facade.getInstance("FacadeTestKey6", $Facade.new);
    facade->registerMediator(facade, $Mediator.new(MEDIATOR_NAME, &object));

    // retrieve the mediator
    assert(facade->retrieveMediator(facade, MEDIATOR_NAME) != NULL);

    // remove the mediator
    Mediator *removedMediator = facade->removeMediator(facade, MEDIATOR_NAME);

    // assert that we have removed the appropriate mediator
    assert(strcmp(removedMediator->getMediatorName(removedMediator), MEDIATOR_NAME) == 0);

    // assert that the mediator is no longer retrievable
    assert(facade->retrieveMediator(facade, MEDIATOR_NAME) == NULL);
    $Mediator.delete(removedMediator);
    $Facade.removeFacade("FacadeTestKey6");
}

void testHasProxy() {
    // register a Proxy
    Facade *facade = $Facade.getInstance("FacadeTestKey7", $Facade.new);
    facade->registerProxy(facade, $Proxy.new("hasProxyTest", (int[]) {1, 2, 3}));

    // assert that the model.hasProxy method returns true
    // for that new name
    assert(facade->hasProxy(facade, "hasProxyTest") == true);

    $Proxy.delete(facade->removeProxy(facade, "hasProxyTest"));
    assert(facade->hasProxy(facade, "hasProxyTest") == false);
    $Facade.removeFacade("FacadeTestKey7");
}

/**
   * Tests the hasMediator Method
   */
void testHasMediator() {
    // register a Mediator
    struct Object {
    } object;
    Facade *facade = $Facade.getInstance("FacadeTestKey8", $Facade.new);
    facade->registerMediator(facade, $Mediator.new("facadeHasMediatorTest", &object));

    // assert that the facade.hasMediator method returns true
    // for that mediator name
    assert(facade->hasMediator(facade, "facadeHasMediatorTest") == true);

    $Mediator.delete(facade->removeMediator(facade, "facadeHasMediatorTest"));

    // assert that the facade.hasMediator method returns false
    // for that mediator name
    assert(facade->hasMediator(facade, "facadeHasMediatorTest") == false);
    $Facade.removeFacade("FacadeTestKey8");
}

/**
 * Test hasCommand method.
 */
void testHasCommand() {
    // register the ControllerTestCommand to handle 'hasCommandTest' notes
    Facade *facade = $Facade.getInstance("FacadeTestKey10", $Facade.new);
    facade->registerCommand(facade, "facadeHasCommandTest", (SimpleCommand *(*)()) NewFacadeTestCommand);

    // test that hasCommand returns true for hasCommandTest notifications
    assert(facade->hasCommand(facade, "facadeHasCommandTest") == true);

    // Remove the Command from the Controller
    facade->removeCommand(facade, "facadeHasCommandTest");

    // test that hasCommand returns false for hasCommandTest notifications
    assert(facade->hasCommand(facade, "facadeHasCommandTest") == false);
    $Facade.removeFacade("FacadeTestKey10");
}

/**
 * Tests the hasCore and removeCore methods
 */
void testHasCoreAndRemoveCore() {
    // assert that the Facade.hasCore method returns false first
    assert($Facade.hasCore("FacadeTestKey11") == false);

    // register a Core
    $Facade.getInstance("FacadeTestKey11", $Facade.new);

    assert($Facade.hasCore("FacadeTestKey11") == true);

    // remove the Core
    $Facade.removeFacade("FacadeTestKey11");

    // assert that the Facade.hasCore method returns false now that the core has been removed.
    assert($Facade.hasCore("FacadeTestKey11") == false);
}

Facade *facade;

void *getFacadeInstances(void *arg) {
    assert(facade == $Facade.getInstance("FacadeTestKey11", $Facade.new));
    assert($Facade.hasCore("FacadeTestKey11") == true);
    return NULL;
}

/**
 * Tests getInstance in a multithreaded environment
 */
void testGetInstancesThreaded() {
    facade = $Facade.getInstance("FacadeTestKey11", $Facade.new);

    int total = 100;
    pthread_t *thread_group = malloc(sizeof(pthread_t) * total);

    // start all threads to begin work
    for (int i = 0; i < total; i++) {
        int error = pthread_create(&thread_group[i], NULL, getFacadeInstances, NULL);
        if (error != 0)
            printf("\nThread can't be created : [%s]", strerror(error));
    }

    // wait for all threads to finish
    for (int i = 0; i < total; i++) {
        pthread_join(thread_group[i], NULL);
    }

    free(thread_group);

    // cleanup
    $Facade.removeFacade("FacadeTestKey11");
}