cmake_minimum_required (VERSION 3.15.0)

set(PROJECT_NAME "libMicroNetwork")
set(LFRAMEWORK_LIB "true")
project(${PROJECT_NAME} LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD 11)

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
	add_link_options(-static-libgcc)
	add_link_options(-static-libstdc++)
endif()

set(API_DIR "${CMAKE_CURRENT_SOURCE_DIR}/Interfaces")
set(LFRAMEWORK_CONFIG_FILE_DIR ${CMAKE_CURRENT_SOURCE_DIR})

#add_compile_options(-gdwarf-4)

message("!!   ${CMAKE_SYSTEM_NAME}")

add_library(libMicroNetworkSources INTERFACE )

target_sources(libMicroNetworkSources 
INTERFACE 
	main.cpp
	Library.h
	Library.cpp
	LFrameworkConfig.h
)

target_include_directories(libMicroNetworkSources INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/Interfaces")
target_include_directories(libMicroNetworkSources INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}")

add_subdirectory(Dependencies)

target_compile_definitions(LFramework INTERFACE LF_USB_HOST)


target_link_libraries(libMicroNetworkSources INTERFACE LFramework)
target_link_libraries(libMicroNetworkSources INTERFACE MicroNetworkCommon)

target_link_libraries(libMicroNetworkSources INTERFACE MicroNetworkHost)
target_link_libraries(libMicroNetworkSources INTERFACE MicroNetworkUser)

target_link_libraries(libMicroNetworkSources INTERFACE pthread)


add_executable(libMicroNetworkTest)
target_link_libraries(libMicroNetworkTest PRIVATE libMicroNetworkSources)

add_library(MicroNetwork SHARED)
target_link_libraries(MicroNetwork PRIVATE libMicroNetworkSources)


#target_link_libraries(libMicroNetwork PRIVATE User32 Advapi32 Ole32 Winusb SetupAPI)