# 1️⃣ Project setup
cmake_minimum_required(VERSION 3.23)
project(puremvc VERSION 1.0.0 LANGUAGES C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# 2️⃣ Find dependencies
find_package(collection REQUIRED CONFIG)

# 3️⃣ Define library target
add_library(puremvc STATIC
        src/core/Controller.c
        src/core/Model.c
        src/core/View.c
        src/patterns/command/MacroCommand.c
        src/patterns/command/SimpleCommand.c
        src/patterns/facade/Facade.c
        src/patterns/mediator/Mediator.c
        src/patterns/observer/Notification.c
        src/patterns/observer/Notifier.c
        src/patterns/observer/Observer.c
        src/patterns/proxy/Proxy.c)

add_library(puremvc::puremvc ALIAS puremvc)

# 4️⃣ Target include paths
target_include_directories(puremvc
        PUBLIC
            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
            $<INSTALL_INTERFACE:include>)

target_link_libraries(puremvc PUBLIC collection::collection)

# 5️⃣ Install rules
include(GNUInstallDirs)

install(TARGETS puremvc
        EXPORT puremvcTargets
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT PureMVCDevelopment)

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

# 6️⃣ Export targets for find_package()
install(EXPORT puremvcTargets
        FILE puremvcTargets.cmake
        NAMESPACE puremvc::
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/puremvc)

# 7️⃣ Package configuration files
include(CMakePackageConfigHelpers)

configure_package_config_file(${CMAKE_CURRENT_LIST_DIR}/cmake/puremvcConfig.cmake.in
        ${CMAKE_CURRENT_BINARY_DIR}/puremvcConfig.cmake
        INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/puremvc)

# 8️⃣ Package version file
write_basic_package_version_file(${CMAKE_CURRENT_BINARY_DIR}/puremvcConfigVersion.cmake
        VERSION ${PROJECT_VERSION}
        COMPATIBILITY SameMajorVersion)

install(FILES
        ${CMAKE_CURRENT_BINARY_DIR}/puremvcConfig.cmake
        ${CMAKE_CURRENT_BINARY_DIR}/puremvcConfigVersion.cmake
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/puremvc)

# 9️⃣ Tests
option(BUILD_TESTS "Build Tests" ON)
if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(test)
endif()
