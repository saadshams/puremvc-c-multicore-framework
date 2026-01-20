#add_executable(ControllerTest core/ControllerTest.c
#        core/ControllerTestCommand.c
#        core/ControllerTestCommand2.c)
#target_link_libraries(ControllerTest PRIVATE puremvc)
#add_test(NAME PureMVC.ControllerTest COMMAND ControllerTest)

#add_executable(ModelTest core/ModelTest.c core/ModelTestProxy.c)
#target_link_libraries(ModelTest PRIVATE puremvc)
#add_test(NAME PureMVC.ModelTest COMMAND ModelTest)

#add_executable(ViewTest core/ViewTest.c
#        core/ViewTestMediator.c core/ViewTestMediator2.c
#        core/ViewTestMediator3.c core/ViewTestMediator4.c
#        core/ViewTestMediator5.c core/ViewTestMediator6.c)
#target_link_libraries(ViewTest PRIVATE puremvc)
#add_test(NAME PureMVC.ViewTest COMMAND ViewTest)

#add_executable(SimpleCommandTest
#        patterns/command/SimpleCommandTest.c
#        patterns/command/SimpleCommandTestCommand.c)
#target_link_libraries(SimpleCommandTest PRIVATE puremvc)
#add_test(NAME PureMVC.SimpleCommandTest COMMAND SimpleCommandTest)

#add_executable(MacroCommandTest patterns/command/MacroCommandTest.c
#        patterns/command/MacroCommandTestCommand.c
#        patterns/command/MacroCommandTestSub1Command.c
#        patterns/command/MacroCommandTestSub2Command.c)
#target_link_libraries(MacroCommandTest PRIVATE puremvc)
#add_test(NAME PureMVC.MacroCommandTest COMMAND MacroCommandTest)

#add_executable(FacadeTest patterns/facade/FacadeTest.c
#        patterns/facade/FacadeTestCommand.c)
#target_link_libraries(FacadeTest PRIVATE puremvc)
#add_test(NAME PureMVC.FacadeTest COMMAND FacadeTest)

#add_executable(MediatorTest patterns/mediator/MediatorTest.c)
#target_link_libraries(MediatorTest PRIVATE puremvc)
#add_test(NAME PureMVC.MediatorTest COMMAND MediatorTest)

#add_executable(NotificationTest patterns/observer/NotificationTest.c)
#target_link_libraries(NotificationTest PRIVATE puremvc)
#add_test(NAME PureMVC.NotificationTest COMMAND NotificationTest)

add_executable(NotifierTest patterns/observer/NotifierTest.c)
target_link_libraries(NotifierTest PRIVATE puremvc)
add_test(NAME PureMVC.NotifierTest COMMAND NotifierTest)

#add_executable(ObserverTest patterns/observer/ObserverTest.c)
#target_link_libraries(ObserverTest PRIVATE puremvc)
#add_test(NAME PureMVC.ObserverTest COMMAND ObserverTest)

add_executable(ProxyTest patterns/proxy/ProxyTest.c)
target_link_libraries(ProxyTest PRIVATE puremvc)
add_test(NAME PureMVC.ProxyTest COMMAND ProxyTest)
