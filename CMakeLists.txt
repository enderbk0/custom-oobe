cmake_minimum_required(VERSION 3.20)
project(custom-oobe
    VERSION 1.0.0
    DESCRIPTION "Custom Windows OOBE Experience"
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_AUTOMOC OFF)
set(CMAKE_AUTORCC OFF)
set(CMAKE_AUTOUIC OFF)

set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded")

set(SOURCES
    src/main.cpp
    src/app.cpp
    src/mainwindow.cpp
    src/webviewhost.cpp
    src/backend.cpp
    src/config.cpp
    src/logging.cpp
    src/statemanager.cpp
    src/bridgemessage.cpp
    src/jsonutils.cpp
)

set(HEADERS
    src/app.h
    src/mainwindow.h
    src/webviewhost.h
    src/backend.h
    src/config.h
    src/logging.h
    src/statemanager.h
    src/bridgemessage.h
    src/jsonutils.h
    src/resource.h
)

add_executable(custom-oobe WIN32
    ${SOURCES}
    ${HEADERS}
    resources/app.rc
)

target_include_directories(custom-oobe PRIVATE src)
target_include_directories(custom-oobe PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

find_package(unofficial-webview2 CONFIG REQUIRED)
find_package(wil CONFIG REQUIRED)

target_link_libraries(custom-oobe PRIVATE
    unofficial::webview2::webview2
    WIL::WIL
    kernel32.lib
    user32.lib
    gdi32.lib
    comctl32.lib
    ole32.lib
    oleaut32.lib
    shlwapi.lib
    advapi32.lib
    shell32.lib
)

set_target_properties(custom-oobe PROPERTIES
    WIN32_EXECUTABLE TRUE
    LINKER_LANGUAGE CXX
)

add_custom_command(TARGET custom-oobe POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory
        "$<TARGET_FILE_DIR:custom-oobe>/frontend/css"
    COMMAND ${CMAKE_COMMAND} -E make_directory
        "$<TARGET_FILE_DIR:custom-oobe>/frontend/js/pages"
    COMMAND ${CMAKE_COMMAND} -E make_directory
        "$<TARGET_FILE_DIR:custom-oobe>/frontend/assets"
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/frontend"
        "$<TARGET_FILE_DIR:custom-oobe>/frontend"
    COMMENT "Copying frontend assets to output directory"
)
