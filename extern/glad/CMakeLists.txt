add_library(glad include/glad/glad.h src/glad.c)
target_include_directories(glad PUBLIC include/)
add_library(glad::glad ALIAS glad)
