cmake_minimum_required (VERSION 3.15.0)

set(PROJECT_NAME "libMicroNetwork")
set(LFRAMEWORK_LIB "true")
project(${PROJECT_NAME} LANGUAGES CXX)

set(API_DIR "${CMAKE_CURRENT_SOURCE_DIR}/Interfaces")
set(LFRAMEWORK_CONFIG_FILE_DIR ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(libMicroNetwork)
#add_library(libMicroNetwork SHARED)


set_property(TARGET libMicroNetwork PROPERTY CXX_STANDARD 17)

target_sources(libMicroNetwork 
	PRIVATE 
		main.cpp
		Library.h
		Library.cpp
		LFrameworkConfig.h
)

target_include_directories(libMicroNetwork PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/Interfaces")
target_include_directories(libMicroNetwork PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")

add_subdirectory(Dependencies)

target_compile_definitions(LFramework PUBLIC LF_USB_HOST)



target_link_libraries(libMicroNetwork PRIVATE LFramework)
target_link_libraries(libMicroNetwork PRIVATE MicroNetworkCommon)

target_link_libraries(MicroNetworkHost PRIVATE LFramework)
target_link_libraries(MicroNetworkHost PRIVATE MicroNetworkCommon)

target_link_libraries(libMicroNetwork PRIVATE MicroNetworkHost)
target_link_libraries(libMicroNetwork PRIVATE MicroNetworkUser)

target_link_libraries(libMicroNetwork PRIVATE User32 Advapi32 Ole32 Winusb SetupAPI)