
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
	target_compile_options(${TARGET_NAME} PRIVATE -sUSE_SDL=2)
	target_link_options(${TARGET_NAME} PRIVATE
		-sUSE_SDL=2
		--shell-file "${CMAKE_SOURCE_DIR}/src/shell.html")
	
	add_custom_command(TARGET ${TARGET_NAME}
		POST_BUILD
			COMMAND ${CMAKE_COMMAND} -E copy_if_different
				"${CMAKE_SOURCE_DIR}/src/coi-serviceworker.min.js"
				$<TARGET_FILE_DIR:${TARGET_NAME}>)
else()
	target_link_libraries(${TARGET_NAME} PRIVATE glad OpenGL::GL SDL2::SDL2)

	if(WIN32)
		target_link_libraries(${TARGET_NAME} PRIVATE SDL2::SDL2main)
	endif()
endif()
