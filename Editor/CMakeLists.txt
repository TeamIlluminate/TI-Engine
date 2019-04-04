cmake_minimum_required(VERSION 3.0.0)
project(TIEditor VERSION 0.1.0)

include(CTest)
enable_testing()

find_package(TIECore REQUIRED)

aux_source_directory(${PROJECT_SOURCE_DIR}/EditorCode EDT)

add_library(TIEditor SHARED ${EDT})

target_link_libraries (TIEditor PRIVATE eng::TIECore ${TIECore_LIBS})

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)