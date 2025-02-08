cmake_minimum_required(VERSION 3.12...3.13)

set(PATCH_VERSION 2 CACHE INTERNAL "Path version") 
set(PROJECT_VERSION 0.0.${PATCH_VERSION})

project(ip_filter VERSION ${PROJECT_VERSION})


add_executable(allocator_cli main.cpp)

set_target_properties(allocator_cli hello_world_library PROPERTIES
    CXX_STADARD 11
    CXX_STANDARD_REQUIRED ON)
    

if (MSVC)
  target_compile_options(allocator_cli PRIVATE  /W4)
else ()
  target_compile_options(allocator_cli PRIVATE -Wall -Wextra -pedantic -Werror)
endif()

install(TARGETS allocator_cli RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)
set(CPACK_PACKAGE_NAME allocator)
set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")
set(CPACK_PACKAGE_CONTACT example@example.com)
include(CPack)
