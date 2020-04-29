project(env-var)
cmake_minimum_required(VERSION 3.12)
set( EXECUTABLE_OUTPUT_PATH ../bin)
#set(CMAKE_CXX_FLAGS_RELEASE "-s -Wl,-gc-sections -mwindows")

add_executable(envvar src/main.c src/environment.c src/resource.rc)
	
#target_link_libraries(envvar -lntdll)