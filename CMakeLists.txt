cmake_minimum_required(VERSION 3.12...3.13)

set(PATCH_VERSION 2 CACHE INTERNAL "Path version") 
set(PROJECT_VERSION 0.0.${PATCH_VERSION})

project(LibAsync VERSION ${PROJECT_VERSION})

add_executable(libasync_cli main.cpp)
add_library(libasync_library async.cpp async.h)

set_target_properties(libasync_cli libasync_library PROPERTIES
    CXX_STADARD 20
    CXX_STANDARD_REQUIRED ON)
    

target_include_directories(libasync_library PRIVATE "${PROJECT_SOURCE_DIR}")

target_link_libraries(libasync_cli PRIVATE libasync_library)

if (MSVC)
  target_compile_options(libasync_library PRIVATE /W4)
  target_compile_options(libasync_cli PRIVATE  /W4)
else ()
  target_compile_options(libasync_library PRIVATE -Wall -Wextra -pedantic -Werror)
  target_compile_options(libasync_cli PRIVATE -Wall -Wextra -pedantic -Werror)
endif()

install(TARGETS libasync_cli RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)
set(CPACK_PACKAGE_NAME libasync)
set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")
set(CPACK_PACKAGE_CONTACT example@example.com)
include(CPack)



