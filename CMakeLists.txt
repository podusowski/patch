cmake_minimum_required(VERSION 3.5)

find_package(PkgConfig REQUIRED)
pkg_check_modules(ELFIN REQUIRED libelf++)

add_executable(demo demo.cpp)
target_link_libraries(demo ${ELFIN_LIBRARIES})
target_include_directories(demo PUBLIC ${ELFIN_INCLUDE_DIRS})
target_compile_options(demo PUBLIC ${ELFIN_CFLAGS_OTHER})
