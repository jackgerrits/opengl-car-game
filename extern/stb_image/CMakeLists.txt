add_library(stb_image OBJECT)
target_sources(stb_image PRIVATE stb_image.cpp stb_image.h)
target_include_directories(stb_image PRIVATE ${CMAKE_CURRENT_LIST_DIR} SYSTEM INTERFACE ${CMAKE_CURRENT_LIST_DIR})

if(MSVC)
  target_compile_options(stb_image PRIVATE /w)
else(MSVC)
  target_compile_options(stb_image PRIVATE -w)
endif(MSVC)