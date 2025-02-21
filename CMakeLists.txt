cmake_minimum_required(VERSION 3.12...3.13)

set(PATCH_VERSION 2 CACHE INTERNAL "Path version") 
set(PROJECT_VERSION 0.0.${PATCH_VERSION})

project(editor VERSION ${PROJECT_VERSION})


add_executable(editor_cli main.cpp)

if (MSVC)
  target_compile_options(editor_cli PRIVATE  /W4)
else ()
  target_compile_options(editor_cli PRIVATE -Wall -Wextra -pedantic -Werror)
endif()

install(TARGETS editor_cli RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)
set(CPACK_PACKAGE_NAME editor)
set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")
set(CPACK_PACKAGE_CONTACT example@example.com)
include(CPack)
