cmake_minimum_required(VERSION 3.12...3.13)

set(PATCH_VERSION 2 CACHE INTERNAL "Path version") 
set(PROJECT_VERSION 0.0.${PATCH_VERSION})

project(HelloWorld VERSION ${PROJECT_VERSION})

configure_file(FileVithVersion.h.in FileVithVersion.h)


add_executable(hello_world_cli main.cpp)
add_library(hello_world_library lib.cpp lib.h)

set_target_properties(hello_world_cli hello_world_library PROPERTIES
    CXX_STADARD 11
    CXX_STANDARD_REQUIRED ON)
    

target_include_directories(hello_world_library PRIVATE "${PROJECT_SOURCE_DIR}")

target_link_libraries(hello_world_cli PRIVATE hello_world_library)

if (MSVC)
  target_compile_options(hello_world_library PRIVATE /W4)
  target_compile_options(hello_world_cli PRIVATE  /W4)
else ()
  target_compile_options(hello_world_library PRIVATE -Wall -Wextra -pedantic -Werror)
  target_compile_options(hello_world_cli PRIVATE -Wall -Wextra -pedantic -Werror)
endif()

install(TARGETS hello_world_cli RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)
set(CPACK_PACKAGE_NAME helloworld)
set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")
set(CPACK_PACKAGE_CONTACT example@example.com)
include(CPack)



