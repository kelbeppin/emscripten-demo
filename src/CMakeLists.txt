
add_executable(emscripten-demo
	"app.cpp"
	"bus.cpp"
	"renderer.cpp"
	"shader.cpp"
	"window.cpp")

target_include_directories(emscripten-demo PRIVATE "../include")
target_link_libraries(emscripten-demo PRIVATE glm::glm)

if(EMSCRIPTEN)
	set(CMAKE_EXECUTABLE_SUFFIX ".html")
	target_compile_options(emscripten-demo PRIVATE -sUSE_SDL=2 -pthread)
	target_link_options(emscripten-demo PRIVATE -sUSE_SDL=2 -pthread -sMIN_WEBGL_VERSION=2)
else()
	target_sources(emscripten-demo PRIVATE "glad.c")
	target_link_libraries(emscripten-demo PRIVATE OpenGL::GL SDL2::SDL2)

	if(WIN32)
		target_link_libraries(emscripten-demo PRIVATE SDL2::SDL2main)
	endif()
endif()
