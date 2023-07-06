
if(NOT EMSCRIPTEN)
    add_library(glad STATIC "glad/glad.c")
    target_include_directories(glad PUBLIC "glad/include")
endif()
