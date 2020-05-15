#include "FacadeTest.h"
#include "interfaces/Facade.h"
#include "FacadeTestCommand.h"
#include "FacadeTestVO.h"
#include <stdio.h>
#include <string.h>
#include <assert.h>

/**
 * Test the PureMVC Facade class.
 *
 * @see org.puremvc.c.multicore.patterns.facade.FacadeTestVO FacadeTestVO
 * @see org.puremvc.c.multicore.patterns.facade.FacadeTestCommand FacadeTestCommand
 */
int main() {
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
    puts("TestFacade: Success");
    return 0;
}

/**
 * Tests the Facade Multiton Factory Method
 */
void testGetInstance() {
    // Test Factory Method
    Facade *facade = getFacadeInstance("FacadeTestKey1", NewFacade);

    // test assertions
    assert(facade != NULL);
    RemoveFacadeCore("FacadeTestKey1");
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
    Facade *facade = getFacadeInstance("FacadeTestKey2", NewFacade);
    facade->registerCommand(facade, "FacadeTestNote", (SimpleCommand *(*)()) NewFacadeTestCommand);

    // Send notification. The Command associated with the event
    // (FacadeTestCommand) will be invoked, and will multiply
    // the vo.input value by 2 and set the result on vo.result
    FacadeTestVO vo = {32};
    facade->sendNotification(facade, "FacadeTestNote", &vo, NULL);

    // test assertions
    assert(vo.result == 64);
    facade->removeCommand(facade, "FacadeTestNote");
    RemoveFacadeCore("FacadeTestKey2");
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
    Facade *facade = getFacadeInstance("FacadeTestKey3", NewFacade);
    facade->registerCommand(facade, "FacadeTestNote", (SimpleCommand *(*)()) NewFacadeTestCommand);
    facade->removeCommand(facade, "FacadeTestNote");

    // Send notification. The Command associated with the event
    // (FacadeTestCommand) will NOT be invoked, and will NOT multiply
    // the vo.input value by 2
    FacadeTestVO vo = {32};
    facade->sendNotification(facade, "FacadeTestNote", &vo, NULL);

    // test assertions
    assert(vo.result != 64);
    RemoveFacadeCore("FacadeTestKey3");
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
    // register a proxy and retrieve it.
    Facade *facade = getFacadeInstance("FacadeTestKey4", NewFacade);
    facade->registerProxy(facade, NewProxy("colors", (char *[]) {"red", "green", "blue", NULL}));
    Proxy *proxy = facade->retrieveProxy(facade, "colors");

    // test assertions
    assert(proxy != NULL);

    // retrieve data from proxy
    const char **data = proxy->getData(proxy);

    // test assertions
    assert(data != NULL);
    assert(strcmp(data[0], "red") == 0);
    assert(strcmp(data[1], "green") == 0);
    assert(strcmp(data[2], "blue") == 0);

    DeleteProxy(facade->removeProxy(facade, "colors"));
    RemoveFacadeCore("FacadeTestKey4");
}

/**
 * Tests the removing Proxies via the Facade.
 */
void testRegisterAndRemoveProxy() {
    // register a proxy, remove it, then try to retrieve it
    Facade *facade = getFacadeInstance("FacadeTestKey5", NewFacade);
    facade->registerProxy(facade, NewProxy("sizes", (char *[]) {"7", "13", "21"}));

    // remove the proxy
    Proxy *removedProxy = facade->removeProxy(facade, "sizes");

    // assert that we removed the appropriate proxy
    assert(strcmp(removedProxy->getProxyName(removedProxy), "sizes") == 0);

    // test assertions - make sure we can no longer retrieve the proxy from the model
    assert(facade->retrieveProxy(facade, "sizes") == NULL);

    DeleteProxy(removedProxy);
    RemoveFacadeCore("FacadeTestKey5");
}

/**
 * Tests registering, retrieving and removing Mediators via the Facade.
 */
void testRegisterRetrieveAndRemoveMediator() {
    // register a mediator, remove it, then try to retrieve it
    struct Object {} object;
    Facade *facade = getFacadeInstance("FacadeTestKey6", NewFacade);
    facade->registerMediator(facade, NewMediator(MEDIATOR_NAME, &object));

    // retrieve the mediator
    assert(facade->retrieveMediator(facade, MEDIATOR_NAME) != NULL);

    // remove the mediator
    Mediator *removedMediator = facade->removeMediator(facade, MEDIATOR_NAME);

    // assert that we have removed the appropriate mediator
    assert(strcmp(removedMediator->getMediatorName(removedMediator), MEDIATOR_NAME) == 0);

    // assert that the mediator is no longer retrievable
    assert(facade->retrieveMediator(facade, MEDIATOR_NAME) == NULL);
    DeleteMediator(removedMediator);
    RemoveFacadeCore("FacadeTestKey6");
}

void testHasProxy() {
    // register a Proxy
    Facade *facade = getFacadeInstance("FacadeTestKey7", NewFacade);
    facade->registerProxy(facade, NewProxy("hasProxyTest", (int []) {1, 2, 3}));

    // assert that the model.hasProxy method returns true
    // for that proxy name
    assert(facade->hasProxy(facade, "hasProxyTest") == true);

    DeleteProxy(facade->removeProxy(facade, "hasProxyTest"));
    assert(facade->hasProxy(facade, "hasProxyTest") == false);
    RemoveFacadeCore("FacadeTestKey7");
}

/**
   * Tests the hasMediator Method
   */
void testHasMediator() {
    // register a Mediator
    struct Object {} object;
    Facade *facade = getFacadeInstance("FacadeTestKey8", NewFacade);
    facade->registerMediator(facade, NewMediator("facadeHasMediatorTest", &object));

    // assert that the facade.hasMediator method returns true
    // for that mediator name
    assert(facade->hasMediator(facade, "facadeHasMediatorTest") == true);

    DeleteMediator(facade->removeMediator(facade, "facadeHasMediatorTest"));

    // assert that the facade.hasMediator method returns false
    // for that mediator name
    assert(facade->hasMediator(facade, "facadeHasMediatorTest") == false);
    RemoveFacadeCore("FacadeTestKey8");
}

/**
 * Test hasCommand method.
 */
void testHasCommand() {
    // register the ControllerTestCommand to handle 'hasCommandTest' notes
    Facade *facade = getFacadeInstance("FacadeTestKey10", NewFacade);
    facade->registerCommand(facade, "facadeHasCommandTest", (SimpleCommand *(*)()) NewFacadeTestCommand);

    // test that hasCommand returns true for hasCommandTest notifications
    assert(facade->hasCommand(facade, "facadeHasCommandTest") == true);

    // Remove the Command from the Controller
    facade->removeCommand(facade, "facadeHasCommandTest");

    // test that hasCommand returns false for hasCommandTest notifications
    assert(facade->hasCommand(facade, "facadeHasCommandTest") == false);
    RemoveFacadeCore("FacadeTestKey10");
}

/**
 * Tests the hasCore and removeCore methods
 */
void testHasCoreAndRemoveCore() {
    // assert that the Facade.hasCore method returns false first
    assert(HasFacadeCore("FacadeTestKey11") == false);

    // register a Core
    getFacadeInstance("FacadeTestKey11", NewFacade);

    assert(HasFacadeCore("FacadeTestKey11") == true);

    // remove the Core
    RemoveFacadeCore("FacadeTestKey11");

    // assert that the Facade.hasCore method returns false now that the core has been removed.
    assert(HasFacadeCore("FacadeTestKey11") == false);
}
