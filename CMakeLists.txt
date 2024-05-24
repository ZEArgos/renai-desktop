#! CMake version required to compile the application.
cmake_minimum_required(VERSION 3.29)
find_package(Git REQUIRED)

#! Get application version. Major (X.x.x) is human-updated, minor (x.X.x) is decided by tag number,
#! and revision (x.x.X) is decided by number of commits since last tag.
set(GET_REVIS_VERSION "git rev-list  `git rev-list --tags --no-walk --max-count=1`..HEAD --count")
set(GET_MINOR_VERSION "git tag | wc -l")
execute_process(COMMAND bash "-c" ${GET_REVIS_VERSION} OUTPUT_VARIABLE PROJECT_REVIS_VERS OUTPUT_STRIP_TRAILING_WHITESPACE)
execute_process(COMMAND bash "-c" ${GET_MINOR_VERSION} OUTPUT_VARIABLE PROJECT_MINOR_VERS OUTPUT_STRIP_TRAILING_WHITESPACE)

set(PROJECT_DEBUG_MODE 1)
if("PROJECT_REVIS_VERS" STREQUAL "" OR "PROJECT_MINOR_VERS" STREQUAL "")
    file(READ ${CMAKE_SOURCE_DIR}/Source/Assets/version.txt PROJECT_VERSION_STRING)
    set(PROJECT_DEBUG_MODE 0)
else()
    set(PROJECT_VERSION_STRING 0.${PROJECT_MINOR_VERS}.${PROJECT_REVIS_VERS})
    file(WRITE ${CMAKE_SOURCE_DIR}/Source/Assets/version.txt ${PROJECT_VERSION_STRING})
endif()

#! Set some variables that have to do with the project, like description, 
#! languages, and name.
set(PROJECT_NAME_STRING "Renai")
set(PROJECT_LANGUAGES_STRING C)
set(PROJECT_DESCRIPTION_STRING "Renai is a high fantasy adult game set in the world of Silre.")
project(${PROJECT_NAME_STRING} VERSION ${PROJECT_VERSION_STRING} LANGUAGES ${PROJECT_LANGUAGES_STRING} 
    DESCRIPTION ${PROJECT_DESCRIPTION_STRING})

#! Set variables having to do with building the project.
set(CMAKE_EXPORT_COMPILE_COMMANDS YES)
set(CMAKE_C_STANDARD 99)
add_compile_options(-Os -g -Wall -Werror -Wpedantic)

#! Directories that the project needs to be able to access.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/Build/${PROJECT_NAME})
set(PROJECT_SOURCE_DIRECTORY ${CMAKE_SOURCE_DIR}/Source)
set(PROJECT_ENGINE_DIRECTORY ${CMAKE_SOURCE_DIR}/Source/Engine)
include_directories(${CMAKE_SOURCE_DIR}/Dependencies ${PROJECT_ENGINE_DIRECTORY})
link_directories(${CMAKE_SOURCE_DIR}/Dependencies)

#! Source files.
file(GLOB SOURCE_FILES ${PROJECT_SOURCE_DIRECTORY}/*.c ${PROJECT_ENGINE_DIRECTORY}/*.c)
file(GLOB HEADER_FILES ${INCLUDE_DIRECTORIES}/glfw3.h ${PROJECT_ENGINE_DIRECTORY}/*.h)

#! Creating the executable file and linking/including all directories needed.
add_executable(${PROJECT_NAME} ${SOURCE_FILES} ${HEADER_FILES})
target_link_libraries(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/Dependencies/glfw/libglfw3.a)

#! Copying runtime binaries and Assets to their place in the compiled game 
#! files.
file(COPY ${PROJECT_SOURCE_DIRECTORY}/Assets DESTINATION ${CMAKE_SOURCE_DIR}/build/${PROJECT_NAME})
file(WRITE ${CMAKE_SOURCE_DIR}/Build/${PROJECT_NAME}/Logs/_debug_log_location_ "renai_debug_log_directory")

#! Defining variables to use in the program.
target_compile_definitions(${PROJECT_NAME} PRIVATE VERSION="v${PROJECT_VERSION_STRING}")
target_compile_definitions(${PROJECT_NAME} PRIVATE TITLE="${PROJECT_NAME}")
target_compile_definitions(${PROJECT_NAME} PRIVATE DEBUG_MODE=${PROJECT_DEBUG_MODE})