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

#grab a hook to the parent directory
get_filename_component(EVIL_DIR "${CMAKE_SOURCE_DIR}/../evil" ABSOLUTE)
get_filename_component(EVIL_INC_DIR "${CMAKE_SOURCE_DIR}/../" ABSOLUTE)
set(EVIL_IMP_DIR ${EVIL_DIR}/imp)

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

include_directories(${EVIL_INC_DIR})

#grab the implementation source and headers
file(GLOB IMP_HEADERS ${EVIL_DIR}/*.h)
file(GLOB IMP_SOURCE ${EVIL_IMP_DIR}/*.cpp)
set(IMP_FILES ${IMP_HEADERS} ${IMP_SOURCE})

add_custom_target(${EVIL_NAME}-NOBUILD COMMENT "Project not built. Files are included by other targets" SOURCES ${IMP_FILES})

#build the samples and tests
list(APPEND SAMPLES_SOURCE src/dev-harness.cpp)
list(APPEND SAMPLES_SOURCE src/sample-simple.cpp)
list(APPEND SAMPLES_SOURCE src/sample-thread.cpp)
list(APPEND SAMPLES_SOURCE src/test-signals.cpp)
list(APPEND SAMPLES_SOURCE src/test-threads.cpp)
list(APPEND SAMPLES_SOURCE src/test-performance.cpp)

set(_EXR ${EVIL_STATIC_RELEASE_POSTFIX})
set(_EXD ${EVIL_STATIC_DEBUG_POSTFIX})
	
set(EVIL_WRAPPER src/evil-wrapper.cpp)

foreach(SAMPLE ${SAMPLES_SOURCE})
	get_filename_component(_NAME ${SAMPLE} NAME_WE)

	set(_SOURCES ${SAMPLE})
	set(_SOURCES ${_SOURCES} ${EVIL_WRAPPER})
	
	evil_add_executable(${_NAME} _SOURCES NONE)

	if (${EVIL_IS_WINDOWS})			
		set_target_properties(${_NAME} PROPERTIES LINK_FLAGS "/SUBSYSTEM:CONSOLE")
	endif()
					
endforeach()


#evil_dump_all()

