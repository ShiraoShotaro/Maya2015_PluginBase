# (c) 2015 Shirao Shotaro

# CMakeFile.txt
# for wlibCameraTools

####################
# Project Settings
####################
cmake_minimum_required(VERSION 3.0.0)

# TEMPLATE for Maya 2015 Plug-in
# REPLACEMENT :
# __PROJECT_NAME__ : Project name


### SETTINGS
# MLL Path
set(CMAKE_RANTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin CACHE PATH "Directory to export mll file")

# Library name
set(LIBRARY_NAME "__LIBRARY_NAME__" CACHE STRING "Your Plug-in name")

# Maya Path
set(MAYA_ROOT_PATH "$ENV{MAYA2015_PATH}" CACHE PATH "Autodesk Maya 2015 install directory")

# Source / Include Directory
set(SOURCE_DIRECTORY "src" CACHE PATH "Source Directory")



### CMake
project(__PROJECT_NAME__)

# Source Files
file(GLOB_RECURSE proj_cpp_files ${SOURCE_DIRECTORY}/*.cpp)
file(GLOB_RECURSE proj_hpp_files ${SOURCE_DIRECTORY}/*.hpp)

# Flag configurations
set(VS_COMPILE_FLAGS "-GR")
set(VS_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -export:initializePlugin -export:uninitializePlugin")
add_definitions(${VS_COMPILE_FLAGS} -DWIN32 -D_WIN64 -D_WINDOWS -D_USRDLL -DNT_PLUGIN -DREQUIRE_IOSTREAM)
set(CMAKE_CXX_FLAGS  ${CMAKE_CXX_FLAGS} ${VS_COMPILE_FLAGS} )
set(CMAKE_SHARED_LINKER_FLAGS ${VS_LINKER_FLAGS})

# Include Directories
set(INCLUDE_DIR "src" ${MAYA_ROOT_PATH}/include)
include_directories("${INCLUDE_DIR}")

# Link Directories
set(LIBRARY_DIR ${MAYA_ROOT_PATH}/lib)
link_directories("${LIBRARY_DIR}")
add_library(${LIBRARY_NAME} SHARED ${proj_cpp_files} ${proj_hpp_files})

target_link_libraries(${LIBRARY_NAME} Foundation.lib OpenMaya.lib OpenMayaUI.lib OpenMayaRender.lib OpenMayaAnim.lib)

# Target Properties
set_target_properties(${LIBRARY_NAME} PROPERTIES SUFFIX ".mll")

# Source Group is same as the directory structure.
function(assign_source_group)
    foreach(_source IN ITEMS ${ARGN})
        if (IS_ABSOLUTE "${_source}")
            file(RELATIVE_PATH _source_rel "${CMAKE_CURRENT_SOURCE_DIR}" "${_source}")
        else()
            set(_source_rel "${_source}")
        endif()
        get_filename_component(_source_path "${_source_rel}" PATH)
        string(REPLACE "/" "\\" _source_path_msvc "${_source_path}")
        source_group("${_source_path_msvc}" FILES "${_source}")
    endforeach()
endfunction(assign_source_group)

set(_source_list ${proj_cpp_files} ${proj_hpp_files})
assign_source_group(${_source_list})
