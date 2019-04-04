cmake_minimum_required(VERSION 3.0.0)
project(TI-Engine VERSION 0.1.0)
enable_testing()

aux_source_directory(${PROJECT_SOURCE_DIR} SRC) 
aux_source_directory(Components CMP)
aux_source_directory(Game GAM)
aux_source_directory(GUI GUI)
aux_source_directory(${PROJECT_SOURCE_DIR}/Libraries/ImGUI ImGUI)
aux_source_directory(${PROJECT_SOURCE_DIR}/Libraries JSON)

SET(FLAGS "-Wformat-security")

add_library(TIECore SHARED ${SRC} ${CMP} ${GAM} ${GUI} ${ImGUI} ${JSON})

find_package(OpenGL REQUIRED)
target_link_libraries (TIECore PUBLIC -lsfml-system -lsfml-window -lsfml-graphics -lsfml-audio -lBox2D -lpthread -lstdc++fs ${OPENGL_LIBRARIES})

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_INCLUDE_CURRENT_DIR_IN_INTERFACE ON)

set_property(TARGET TIECore PROPERTY VERSION ${PROJECT_VERSION})
set_property(TARGET TIECore PROPERTY SOVERSION 3)
set_property(TARGET TIECore PROPERTY INTERFACE_TIECore_MAJOR_VERSION 3)
set_property(TARGET TIECore APPEND PROPERTY COMPATIBLE_INTERFACE_STRING TIECore_MAJOR_VERSION)

install(
  TARGETS TIECore EXPORT TIECoreTargets
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    INCLUDES DESTINATION include
)

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
  "${CMAKE_CURRENT_BINARY_DIR}/TIECore/TIECoreConfigVersion.cmake"
  VERSION ${PROJECT_VERSION}
  COMPATIBILITY AnyNewerVersion
)

export(
  EXPORT TIECoreTargets
  FILE "${CMAKE_CURRENT_BINARY_DIR}/TIECore/TIECoreTargets.cmake"
  NAMESPACE eng::
)

# configure_file(
# 	write_basic_package_config
#   cmake/TIECoreConfig.cmake
#   "${CMAKE_CURRENT_BINARY_DIR}/TIECore/TIECoreConfig.cmake"
#   COPYONLY
# )

configure_package_config_file(
  cmake/TIECoreConfig.cmake
  "${CMAKE_CURRENT_BINARY_DIR}/TIECore/TIECoreConfig.cmake"	
  INSTALL_DESTINATION ${PROJECT_BINARY_DIR}
)

set(ConfigPackageLocation lib/cmake/TIECore)

install(
  EXPORT TIECoreTargets
  FILE
  TIECoreTargets.cmake
  NAMESPACE
    eng::
  DESTINATION
    ${ConfigPackageLocation}
)

install(
  FILES
    "${CMAKE_CURRENT_BINARY_DIR}/TIECore/TIECoreConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/TIECore/TIECoreConfigVersion.cmake"
  DESTINATION
    ${ConfigPackageLocation}
  COMPONENT
    Devel
)

install(DIRECTORY ./
        DESTINATION include/TI-EngineCore
        FILES_MATCHING PATTERN "*.h*" )

install(
  FILES ./SerializableGenerator.cpp
  DESTINATION include/TI-EngineCore/
)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
