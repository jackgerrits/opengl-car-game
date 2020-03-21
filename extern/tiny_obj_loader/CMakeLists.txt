add_library(tiny_obj_loader OBJECT)
target_sources(tiny_obj_loader PRIVATE tiny_obj_loader.cpp tiny_obj_loader.h)
target_include_directories(tiny_obj_loader PRIVATE ${CMAKE_CURRENT_LIST_DIR} SYSTEM INTERFACE ${CMAKE_CURRENT_LIST_DIR})

if(MSVC)
  target_compile_options(tiny_obj_loader PRIVATE /w)
else(MSVC)
  target_compile_options(tiny_obj_loader PRIVATE -w)
endif(MSVC)