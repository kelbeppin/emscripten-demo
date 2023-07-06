
cmake_minimum_required(VERSION 3.13)

if(POLICY CMP0135)
	cmake_policy(SET CMP0135 NEW)
endif()

# Enable Hot Reload for MSVC compilers if supported.
if(POLICY CMP0141)
	cmake_policy(SET CMP0141 NEW)
	set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project("emscripten-demo")

find_package(OpenGL REQUIRED)

include(FetchContent)

FetchContent_Declare(glm
	GIT_REPOSITORY https://github.com/g-truc/glm.git
	GIT_TAG bf71a834948186f4097caa076cd2663c69a10e1e)

FetchContent_MakeAvailable(glm)

if(NOT EMSCRIPTEN)
	FetchContent_Declare(SDL2
		URL https://www.libsdl.org/release/SDL2-2.28.1.tar.gz)

	FetchContent_MakeAvailable(SDL2)
endif()

add_subdirectory("third-party")
add_subdirectory("src")
