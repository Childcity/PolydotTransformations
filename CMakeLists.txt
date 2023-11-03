cmake_minimum_required(VERSION 3.14)

project(PolydotTransformations VERSION 0.1 LANGUAGES CXX)

set(APP_NAME PolydotTransformation)
set(SOURCE_DIR src)

# set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 6.5 REQUIRED COMPONENTS
    Core
    Quick
    Quick3D
    Qml
)

include_directories(${SOURCE_DIR}/controllers)
include_directories(${SOURCE_DIR}/models)
include_directories(${SOURCE_DIR}/utils)

# Find all Sources
file(GLOB_RECURSE found_sources CONFIGURE_DEPENDS
    ${SOURCE_DIR}/*.cpp ${SOURCE_DIR}/*.h ${SOURCE_DIR}/*.hpp)
file(GLOB_RECURSE found_qml_files CONFIGURE_DEPENDS ${SOURCE_DIR}/*.qml)

# Make paths relative to CURRENT_SOURCE_DIR
get_filename_component(current_dir ${CMAKE_CURRENT_SOURCE_DIR} ABSOLUTE)
foreach(file_path ${found_sources})
    string(REPLACE ${current_dir}/ "" modified_file_path ${file_path})
    list(APPEND sources ${modified_file_path})
endforeach()
foreach(file_path ${found_qml_files})
    string(REPLACE ${current_dir}/ "" modified_file_path ${file_path})
    list(APPEND qml_files ${modified_file_path})
endforeach()

qt_add_executable(${APP_NAME}
)

qt_add_qml_module(${APP_NAME}
    URI PolydotTransformationUi
    VERSION 1.0
    QML_FILES ${qml_files}
    SOURCES ${sources}
    RESOURCES
        ${SOURCE_DIR}/res/logo.png
    RESOURCE_PREFIX "/"
    IMPORTS
        QtCore
        QtQuick3D
)

set_target_properties(${APP_NAME} PROPERTIES
    WIN32_EXECUTABLE TRUE
)

target_compile_definitions(${APP_NAME}
  PRIVATE
  $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>
)
target_link_libraries(${APP_NAME} PRIVATE
    Qt6::Core
    Qt6::Qml
    Qt6::Quick
    Qt6::Quick3D
)
