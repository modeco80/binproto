set(CMAKE_CXX_STANDARD 20)

option(BINPROTO_BUILD_EXAMPLES "Build BinProto examples" ON)

add_library(binproto
		EndianUtils.cpp
		BufferReader.cpp
		BufferWriter.cpp
)

target_include_directories(binproto PUBLIC ${PROJECT_SOURCE_DIR}/include)

if(BINPROTO_BUILD_EXAMPLES)
	message(STATUS "Building examples.")
	add_executable(test
			test/main.cpp
	)
	target_link_libraries(test binproto)
endif()