###########################################################
# (c) 2018 Shirao Shotaro
# CMakeFile.txt
# 
# For creating a plug-in used by Autodesk Maya 2015 API
#
# This software and all scripts is released under the MIT License.
# Please read LICENSE to get more informations.
###########################################################
cmake_minimum_required(VERSION 3.0.0)

###########################################################
# SETTINGS

# MLL Path
set(CMAKE_RANTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin CACHE PATH "Directory to export mll file")

# Project name
set(PROJECT_NAME "ProjectName" CACHE STRING "This project name")

# Library name
set(PROJECT_LIBRARY_NAME "OutputMLLName" CACHE STRING "Your Plug-in name")

# Maya Path
set(PROJECT_MAYA_INSTALLED_DIRECTORY "$ENV{MAYA2015_PATH}" CACHE PATH "Autodesk Maya 2015 install directory")

# Source / Include Directory
set(PROJECT_SOURCE_DIRECTORY "src" CACHE PATH "Source Directory")


###########################################################
# CMake
project(${PROJECT_NAME})

# Source Files
file(GLOB_RECURSE proj_cpp_files ${PROJECT_SOURCE_DIRECTORY}/*.cpp)
file(GLOB_RECURSE proj_hpp_files ${PROJECT_SOURCE_DIRECTORY}/*.hpp)

# Flag configurations
set(VS_COMPILE_FLAGS "-GR")
set(VS_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -export:initializePlugin -export:uninitializePlugin")
add_definitions(${VS_COMPILE_FLAGS} -DWIN32 -D_WIN64 -D_WINDOWS -D_USRDLL -DNT_PLUGIN -DREQUIRE_IOSTREAM)
set(CMAKE_CXX_FLAGS  ${CMAKE_CXX_FLAGS} ${VS_COMPILE_FLAGS} )
set(CMAKE_SHARED_LINKER_FLAGS ${VS_LINKER_FLAGS})

# Include Directories
set(INCLUDE_DIR ${PROJECT_SOURCE_DIRECTORY} ${PROJECT_MAYA_INSTALLED_DIRECTORY}/include)
include_directories("${INCLUDE_DIR}")

# Link Directories
set(LIBRARY_DIR ${PROJECT_MAYA_INSTALLED_DIRECTORY}/lib)
link_directories("${LIBRARY_DIR}")
add_library(${PROJECT_LIBRARY_NAME} SHARED ${proj_cpp_files} ${proj_hpp_files})

target_compile_definitions(${PROJECT_LIBRARY_NAME} PRIVATE __PROJECT_NAME="${PROJECT_NAME}")
target_link_libraries(${PROJECT_LIBRARY_NAME} Foundation.lib OpenMaya.lib OpenMayaUI.lib OpenMayaRender.lib OpenMayaAnim.lib)

# Target Properties
set_target_properties(${PROJECT_LIBRARY_NAME} PROPERTIES SUFFIX ".mll")

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
