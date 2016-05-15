cmake_minimum_required(VERSION 2.8)

#remove the build types we dont support -before we call project which locks it in
set(CMAKE_CONFIGURATION_TYPES "Debug;Release")
set(CMAKE_INSTALL_PREFIX  "${CMAKE_SOURCE_DIR}" CACHE INTERNAL "NOT USED")

project(evil-signals-examples)
set(EVIL_NAME evil-signals)

if(NOT CMAKE_MODULE_PATH)
	set (CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake")
endif()
include(EvilMacros)

evil_setup_environment()

#grab a hook to the repo root directory
get_filename_component(EVIL_REPO_DIR "${CMAKE_SOURCE_DIR}/.." ABSOLUTE)

#the evil dir where our actual source is 
set(EVIL_DIR ${EVIL_REPO_DIR}/evil)
#and its imp dir
set(EVIL_IMP_DIR ${EVIL_DIR}/imp) 

#the example dir - which is where we actually are - for completeness
set(EVIL_EXAMPLE_DIR ${EVIL_REPO_DIR}/examples) 

#the example src
set(EVIL_EXAMPLE_SRC_DIR ${EVIL_EXAMPLE_DIR}/src) 

#the location of our test harness files
set(EVIL_TEST_DIR ${EVIL_EXAMPLE_SRC_DIR}/test) 

#and its imp dir
set(EVIL_TEST_IMP_DIR ${EVIL_TEST_DIR}/imp)  #and its imp dir

#setup compile options
if(${EVIL_IS_ROOT})
	#need to setup our global preprocessor, compile and link definitions 
	evil_add_preprocessor_def("UNICODE;_UNICODE")	
	if (${EVIL_IS_WINDOWS})
			evil_set_msvc_static_runtime()
			
			evil_add_preprocessor_def("_CRT_SECURE_NO_WARNINGS;NOMINMAX;STRICT")	
			evil_add_compile_options(" $<$<CONFIG:Release>:/Ox>")
			#evil_add_link_options()
	endif()

	if (${EVIL_IS_APPLE})
			#havent done osx yet
	endif()
		
	if (${EVIL_IS_UNIX})
			#haven't done unix yet
	endif()
endif()

#include paths
include_directories(${EVIL_REPO_DIR})
include_directories(${EVIL_TEST_DIR})
include_directories(${EVIL_IMP_DIR})

#grab the evil source and headers
file(GLOB EVIL_HEADERS ${EVIL_DIR}/*.h)
file(GLOB EVIL_IMP_HEADERS ${EVIL_IMP_DIR}/*.h)
file(GLOB EVIL_SOURCE ${EVIL_IMP_DIR}/*.cpp)
#file(GLOB EVIL_WRAPPER ${EVIL_DIR}/evil-wrapper.cpp) unused now it is templated
set(EVIL_FILES ${EVIL_HEADERS} ${EVIL_SOURCE} ${EVIL_IMP_HEADERS})

#the example files
file(GLOB EXAMPLE_HEADERS ${EVIL_EXAMPLE_DIR}/*.h)
file(GLOB EXAMPLE_SOURCE ${EVIL_EXAMPLE_IMP_DIR}/*.cpp)
set(EXAMPLE_FILES ${EXAMPLE_HEADERS} ${EXAMPLE_SOURCE})

#the test harness files
file(GLOB TEST_HEADERS ${EVIL_TEST_DIR}/*.h)
file(GLOB TEST_SOURCE ${EVIL_TEST_IMP_DIR}/*.cpp)
set(TEST_FILES ${TEST_HEADERS} ${TEST_SOURCE})

#create a no build target
add_custom_target(${EVIL_NAME}-NOBUILD COMMENT "Project not built. Files are included by other targets" SOURCES ${EVIL_FILES})

add_custom_target(test-harness-NOBUILD COMMENT "Project not built. Files are included by other targets" SOURCES ${TEST_FILES})

#build the samples and tests
list(APPEND EXAMPLE_SOURCE src/dev-harness.cpp)
list(APPEND EXAMPLE_SOURCE src/sample-simple.cpp)
list(APPEND EXAMPLE_SOURCE src/sample-thread.cpp)
list(APPEND EXAMPLE_SOURCE src/test-signals.cpp)
list(APPEND EXAMPLE_SOURCE src/test-threads.cpp)
list(APPEND EXAMPLE_SOURCE src/test-performance.cpp)

set(_EXR ${EVIL_STATIC_RELEASE_POSTFIX})
set(_EXD ${EVIL_STATIC_DEBUG_POSTFIX})
	
#set(EVIL_WRAPPER src/evil-wrapper.cpp)

foreach(EXAMPLE ${EXAMPLE_SOURCE})
	get_filename_component(_NAME ${EXAMPLE} NAME_WE)

	set(_SOURCES ${EXAMPLE})
	
	evil_add_executable(${_NAME} _SOURCES NONE)

	if (${EVIL_IS_WINDOWS})			
		set_target_properties(${_NAME} PROPERTIES LINK_FLAGS "/SUBSYSTEM:CONSOLE")
	endif()
					
endforeach()


#evil_dump_all()

