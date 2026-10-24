cmake_minimum_required(VERSION 3.0)

set(core_src
	src/TinyXML/tinyxml.cpp
	src/TinyXML/tinyxmlerror.cpp
	src/TinyXML/tinyxmlparser.cpp
	src/allocator.cpp
	src/allocatorchunk.cpp
	src/bufferedfilestream.cpp
	src/checksum.cpp
	src/clock.cpp
	src/configmanager.cpp
	src/configparser.cpp
	src/configvar.cpp
	src/customassert.cpp
	src/customnew.cpp
	src/datapipe.cpp
	src/display.cpp
	src/dynamicmemorystream.cpp
	src/exceptiontrace.cpp
	src/filestream.cpp
	src/fileutil.cpp
	src/hashedstring.cpp
	src/inputcontext.cpp
	src/inputsystem.cpp
	src/keyboard.cpp
	src/memorystream.cpp
	src/mouse.cpp
	src/packstream.cpp
	src/printmanager.cpp
	src/profiler.cpp
	src/profilerhook.cpp
	src/profilertable.cpp
	src/reversehash.cpp
	src/simplestring.cpp
	src/stopwatch.cpp
	src/stringmanager.cpp
	src/test.cpp
	src/timedate.cpp
	src/timer.cpp
	src/windowwrapper.cpp
	src/xinputcontroller.cpp
)

add_library(Core ${core_src})

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

# PortMaster: link zlib statically so the binary does not depend on the device's libz
set(CORE_ZLIB z)
if(PORTMASTER)
	find_library(ZLIB_STATIC_LIBRARY NAMES libz.a)
	if(ZLIB_STATIC_LIBRARY)
		set(CORE_ZLIB ${ZLIB_STATIC_LIBRARY})
	endif()
endif()
target_link_libraries(Core ${CORE_ZLIB} ${SDL2_LIBRARIES})
