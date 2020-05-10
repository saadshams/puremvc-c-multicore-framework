#include "FacadeTest.h"
#include "interfaces/Facade.h"
#include "FacadeTestCommand.h"
#include "FacadeTestVO.h"
#include <stdio.h>
#include <string.h>
#include <assert.h>

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

void testGetInstance() {
    Facade *facade = getFacadeInstance("FacadeTestKey1", NewFacade);
    assert(facade != NULL);
    RemoveFacadeCore("FacadeTestKey1");
}

void testRegisterCommandAndSendNotification() {
    Facade *facade = getFacadeInstance("FacadeTestKey2", NewFacade);
    facade->registerCommand(facade, "FacadeTestNote", (SimpleCommand *(*)()) NewFacadeTestCommand);

    FacadeTestVO vo = {32};
    facade->sendNotification(facade, "FacadeTestNote", &vo, NULL);

    assert(vo.result == 64);
    facade->removeCommand(facade, "FacadeTestNote");
    RemoveFacadeCore("FacadeTestKey2");
}

void testRegisterAndRemoveCommandAndSendNotification() {
    Facade *facade = getFacadeInstance("FacadeTestKey3", NewFacade);
    facade->registerCommand(facade, "FacadeTestNote", (SimpleCommand *(*)()) NewFacadeTestCommand);
    facade->removeCommand(facade, "FacadeTestNote");

    FacadeTestVO vo = {32};
    facade->sendNotification(facade, "FacadeTestNote", &vo, NULL);

    assert(vo.result != 64);
    RemoveFacadeCore("FacadeTestKey3");
}

void testRegisterAndRetrieveProxy() {
    Facade *facade = getFacadeInstance("FacadeTestKey4", NewFacade);
    facade->registerProxy(facade, NewProxy("colors", (char *[]) {"red", "green", "blue"}));

    Proxy *proxy = facade->retrieveProxy(facade, "colors");

    assert(proxy != NULL);

    const char **data = proxy->getData(proxy);

    assert(data != NULL);
    assert(strcmp(data[0], "red") == 0);
    assert(strcmp(data[1], "green") == 0);
    assert(strcmp(data[2], "blue") == 0);

    DeleteProxy(facade->removeProxy(facade, "colors"));
    RemoveFacadeCore("FacadeTestKey4");
}

void testRegisterAndRemoveProxy() {
    Facade *facade = getFacadeInstance("FacadeTestKey5", NewFacade);
    facade->registerProxy(facade, NewProxy("sizes", (char *[]) {"7", "13", "21"}));

    Proxy *removedProxy = facade->removeProxy(facade, "sizes");

    assert(removedProxy != NULL);

    assert(strcmp(removedProxy->getProxyName(removedProxy), "sizes") == 0);

    assert(facade->retrieveProxy(facade, "sizes") == NULL);

    DeleteProxy(removedProxy);
    RemoveFacadeCore("FacadeTestKey5");
}

void testRegisterRetrieveAndRemoveMediator() {
    struct Object {} object;
    Facade *facade = getFacadeInstance("FacadeTestKey6", NewFacade);
    facade->registerMediator(facade, NewMediator(MEDIATOR_NAME, &object));

    assert(facade->retrieveMediator(facade, MEDIATOR_NAME) != NULL);

    Mediator *removedMediator = facade->removeMediator(facade, MEDIATOR_NAME);

    assert(strcmp(removedMediator->getMediatorName(removedMediator), MEDIATOR_NAME) == 0);
    assert(facade->retrieveMediator(facade, MEDIATOR_NAME) == NULL);
    DeleteMediator(removedMediator);
    RemoveFacadeCore("FacadeTestKey6");
}

void testHasProxy() {
    Facade *facade = getFacadeInstance("FacadeTestKey7", NewFacade);
    facade->registerProxy(facade, NewProxy("hasProxyTest", (int []) {1, 2, 3}));

    assert(facade->hasProxy(facade, "hasProxyTest") == true);

    DeleteProxy(facade->removeProxy(facade, "hasProxyTest"));

    assert(facade->hasProxy(facade, "hasProxyTest") == false);
    RemoveFacadeCore("FacadeTestKey7");
}

void testHasMediator() {
    struct Object {} object;
    Facade *facade = getFacadeInstance("FacadeTestKey8", NewFacade);
    facade->registerMediator(facade, NewMediator("facadeHasMediatorTest", &object));

    assert(facade->hasMediator(facade, "facadeHasMediatorTest") == true);

    DeleteMediator(facade->removeMediator(facade, "facadeHasMediatorTest"));

    assert(facade->hasMediator(facade, "facadeHasMediatorTest") == false);
    RemoveFacadeCore("FacadeTestKey8");
}

void testHasCommand() {
    Facade *facade = getFacadeInstance("FacadeTestKey10", NewFacade);
    facade->registerCommand(facade, "facadeHasCommandTest", (SimpleCommand *(*)()) NewFacadeTestCommand);

    assert(facade->hasCommand(facade, "facadeHasCommandTest") == true);

    facade->removeCommand(facade, "facadeHasCommandTest");

    assert(facade->hasCommand(facade, "facadeHasCommandTest") == false);
    RemoveFacadeCore("FacadeTestKey10");
}

void testHasCoreAndRemoveCore() {
    assert(HasFacadeCore("FacadeTestKey11") == false);

    Facade *facade = getFacadeInstance("FacadeTestKey11", NewFacade);

    assert(HasFacadeCore("FacadeTestKey11") == true);

    RemoveFacadeCore("FacadeTestKey11");

    assert(HasFacadeCore("FacadeTestKey11") == false);

}