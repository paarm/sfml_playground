cmake_minimum_required(VERSION 3.0)
set(CMAKE_CXX_STANDARD 11) # C++11...
set(CMAKE_CXX_STANDARD_REQUIRED ON) #...is required...
set(CMAKE_CXX_EXTENSIONS OFF) #...without compiler extensions like gnu++11
#set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "cmake")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

project(mysdlproject)

set (CMAKE_BUILD_TYPE Debug)


find_package(SFML 2 REQUIRED system window graphics network audio)
message(STATUS "SFML is in " ${SFML_INCLUDE_DIR})

set (SOURCE 
src/director.cpp 
src/director.h 
src/utils/clock.h 
src/main.cpp 
src/node.cpp 
src/node2d.cpp 
src/nodetext.cpp
src/node.h 
src/texturemanager.cpp
src/texturemanager.h 
src/fontmanager.h
src/fontmanager.cpp
src/game/gameconst.h
src/game/leveldesc/levelloader.h
src/game/leveldesc/level.h
src/game/leveldesc/row.h
src/game/leveldesc/column.h
src/game/leveldesc/block.h    
src/game/leveldesc/blocktype.h
)

add_executable(${PROJECT_NAME} ${SOURCE})
include_directories(${SFML_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME} ${SFML_LIBRARIES})

add_custom_command(TARGET ${PROJECT_NAME} PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory
                       ${CMAKE_SOURCE_DIR}/assets $<TARGET_FILE_DIR:${PROJECT_NAME}>/assets)