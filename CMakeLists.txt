cmake_minimum_required(VERSION 3.0.0)
project(TI-Engine VERSION 0.1.0)
enable_testing()

# SET(TIPC "${CMAKE_CURRENT_SOURCE_DIR}/TI-PreCompiler.exec")

# execute_process(WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} COMMAND ${TIPC})

aux_source_directory(EngineCore SRC) 
aux_source_directory(EngineCore/Components CMP)
aux_source_directory(EngineCore/Game GAM)
aux_source_directory(EngineCore/GUI GUI)

aux_source_directory(TI-Engine/Editor EDT)
aux_source_directory(TI-Engine/EditorExt EDTL)

include_directories(EngineCore)

include_directories(EngineCore/Libraries/ImGUI)
aux_source_directory(EngineCore/Libraries/ImGUI ImGUI)

include_directories(EngineCore/Libraries)
aux_source_directory(EngineCore/Libraries JSON)


SET(FLAGS "-Wformat-security")

add_library(TIECore SHARED ${SRC} ${CMP} ${GAM} ${GUI} ${ImGUI} ${JSON})

find_package(OpenGL REQUIRED)
target_include_directories(TIECore INTERFACE ${SRC} ${CMP} ${GAM} ${GUI} ${ImGUI} ${JSON})
target_link_libraries (TIECore -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio -lBox2D -lpthread -lstdc++fs ${OPENGL_LIBRARIES} )

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_INCLUDE_CURRENT_DIR_IN_INTERFACE ON)

include(GenerateExportHeader)

generate_export_header(TIECore)
set_property(TARGET TIECore PROPERTY VERSION ${PROJECT_VERSION})
set_property(TARGET TIECore PROPERTY SOVERSION 3)
set_property(TARGET TIECore PROPERTY
  INTERFACE_TIECore_MAJOR_VERSION 3)
set_property(TARGET TIECore APPEND PROPERTY
  COMPATIBLE_INTERFACE_STRING TIECore_MAJOR_VERSION
)

install(TARGETS TIECore EXPORT TIECoreTargets
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
  RUNTIME DESTINATION bin
  INCLUDES DESTINATION include
)
install(
  FILES
    API.h
    "${CMAKE_CURRENT_BINARY_DIR}/API_export.h"
  DESTINATION
    include
  COMPONENT
    Devel
)

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
  "${CMAKE_CURRENT_BINARY_DIR}/TIECore/TIECoreConfigVersion.cmake"
  VERSION ${PROJECT_VERSION}
  COMPATIBILITY AnyNewerVersion
)

export(EXPORT TIECoreTargets
  FILE "${CMAKE_CURRENT_BINARY_DIR}/TIECore/TIECoreTargets.cmake"
  NAMESPACE eng::
)
configure_file(cmake/TIECoreConfig.cmake
  "${CMAKE_CURRENT_BINARY_DIR}/TIECore/TIECoreConfig.cmake"
  COPYONLY
)

set(ConfigPackageLocation lib/cmake/TIECore)
install(EXPORT TIECoreTargets
  FILE
  TIECoreTargets.cmake
  NAMESPACE
    eng::
  DESTINATION
    ${ConfigPackageLocation}
)
install(
  FILES
    cmake/TIECoreConfig.cmake
    "${CMAKE_CURRENT_BINARY_DIR}/TIECore/TIECoreConfigVersion.cmake"
  DESTINATION
    ${ConfigPackageLocation}
  COMPONENT
    Devel
)

# set_target_properties(TIECore PROPERTIES VERSION ${PROJECT_VERSION})

add_executable(TIEngine TI-Engine/Editor/main.cpp ${SRC} ${CMP} ${GAM} ${GUI} ${ImGUI} ${JSON})

add_definitions(${FLAGS})

target_link_libraries (TIEngine PRIVATE TIECore)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
