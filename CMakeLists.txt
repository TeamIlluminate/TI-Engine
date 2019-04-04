cmake_minimum_required(VERSION 3.0.0)
project(TIEngine VERSION 1.0.0)

include(CTest)
enable_testing()

add_subdirectory(EngineCore)
add_subdirectory(Editor)

aux_source_directory(EngineCode ENG)

add_executable(TIEngine ${ENG})
target_link_libraries (TIEngine TIECore TIEditor -ldl)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
