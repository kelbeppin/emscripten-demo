
if(EMSCRIPTEN)
	set(TARGET_NAME "index")
else()
	set(TARGET_NAME "emscripten-demo")
endif()

add_executable(${TARGET_NAME}
	"app.cpp"
	"bus.cpp"
	"renderer.cpp"
	"shader.cpp"
	"window.cpp")

target_include_directories(${TARGET_NAME} PRIVATE "../include")
target_link_libraries(${TARGET_NAME} PRIVATE glm::glm)

if(EMSCRIPTEN)
	set(CMAKE_EXECUTABLE_SUFFIX ".html")
	target_compile_options(${TARGET_NAME} PRIVATE -sUSE_SDL=2 -pthread)
	target_link_options(${TARGET_NAME} PRIVATE -sUSE_SDL=2 -pthread -sMIN_WEBGL_VERSION=2)
else()
	target_sources(${TARGET_NAME} PRIVATE "glad.c")
	target_link_libraries(${TARGET_NAME} PRIVATE OpenGL::GL SDL2::SDL2)

	if(WIN32)
		target_link_libraries(${TARGET_NAME} PRIVATE SDL2::SDL2main)
	endif()
endif()
