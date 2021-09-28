cmake_minimum_required (VERSION 3.15.0)

project(libMicroNetwork LANGUAGES CXX)

set(LFRAMEWORK_CONFIG_FILE_DIR ${CMAKE_CURRENT_SOURCE_DIR})

add_library(libMicroNetwork SHARED)
set_property(TARGET libMicroNetwork PROPERTY CXX_STANDARD 17)

target_sources(libMicroNetwork 
	PRIVATE 
		main.cpp
		Library.h
		Library.cpp
		LFrameworkConfig.h
)

target_include_directories(libMicroNetwork PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/Interfaces")

add_subdirectory(Dependencies)

target_link_libraries(libMicroNetwork PRIVATE LFramework)

#LIBS += -lUser32 -lAdvapi32 -lOle32 -lWinusb -lSetupAPI