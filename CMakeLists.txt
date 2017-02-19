cmake_minimum_required(VERSION 3.0)
set(CMAKE_CXX_STANDARD 11) # C++11...
set(CMAKE_CXX_STANDARD_REQUIRED ON) #...is required...
set(CMAKE_CXX_EXTENSIONS OFF) #...without compiler extensions like gnu++11
#set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "cmake")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

project(mysdlproject)

set (CMAKE_BUILD_TYPE Debug)

if(WIN32)
    set(SFML_STATIC_LIBRARIES TRUE)
    set(SFML_ROOT "C:/Users/Martin/Programmierung/SFML-2.4.2")
endif()

find_package(SFML 2 REQUIRED system window graphics network audio)
message(STATUS "SFML is in " ${SFML_INCLUDE_DIR})

set (SOURCE 
thirdparty/simplejson/JSON.h thirdparty/simplejson/JSON.cpp 
thirdparty/simplejson/JSONValue.h thirdparty/simplejson/JSONValue.cpp 
src/engine/director.h src/engine/director.cpp
src/engine/clock.h 
src/engine/node.h src/engine/node.cpp 
src/engine/node2d.cpp 
src/engine/nodetext.cpp
src/engine/texturemanager.h src/engine/texturemanager.cpp
src/engine/fontmanager.h src/engine/fontmanager.cpp
src/game/leveldesc/levelloader.h src/game/leveldesc/levelloader.cpp
src/game/leveldesc/level.h src/game/leveldesc/level.cpp
src/game/leveldesc/row.h
src/game/leveldesc/column.h
src/game/leveldesc/objectdesc.h src/game/leveldesc/objectdesc.cpp
src/game/gamedirector.h src/game/gamedirector.cpp
src/game/gamescene.h src/game/gamescene.cpp
src/main.cpp 
)

add_executable(${PROJECT_NAME} ${SOURCE})
include_directories(${SFML_INCLUDE_DIR})

if(WIN32)
    target_link_libraries(${PROJECT_NAME} ${SFML_LIBRARIES} ${SFML_DEPENDENCIES})
else()
    target_link_libraries(${PROJECT_NAME} ${SFML_LIBRARIES})
endif()

add_custom_command(TARGET ${PROJECT_NAME} PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory
                       ${CMAKE_SOURCE_DIR}/assets $<TARGET_FILE_DIR:${PROJECT_NAME}>/assets)