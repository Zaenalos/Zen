cmake_minimum_required(VERSION 3.30)

file(GLOB SOURCES
	../essentials/imgui/*.cpp
	Scanner.cpp
    Gui.cpp
    Hook.cpp
    Main.cpp
)

add_library(${PROJECT_NAME} SHARED ${SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}/essentials/imgui
    ${CMAKE_SOURCE_DIR}/essentials/Logger
    ${CMAKE_SOURCE_DIR}/essentials/android-inline-hook/shadowhook/include
)

target_link_libraries(${PROJECT_NAME}
	android
	log
	GLESv3
	shadowhook
)