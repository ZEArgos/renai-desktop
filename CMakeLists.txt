#! CMake version required to compile the application.
cmake_minimum_required(VERSION 3.29)
message(STATUS "Found CMake version 3.29 or higher.")

#! Get the version of the application. This either relies off of Git, or the
#! local file Source/Assets/version.txt.
macro(get_version_string)
    if(EXISTS "${CMAKE_SOURCE_DIR}/.git")
        message(STATUS "Git repository files found. Getting current version from Git.")
        find_package(Git REQUIRED)
        message(STATUS "Found Git successfully.")

        set(GET_REVIS_VERSION "git rev-list  `git rev-list --tags --no-walk --max-count=1`..HEAD --count")
        set(GET_MINOR_VERSION "git tag | wc -l")

        execute_process(COMMAND bash "-c" ${GET_REVIS_VERSION} OUTPUT_VARIABLE PROJECT_REVIS_VERS OUTPUT_STRIP_TRAILING_WHITESPACE)
        execute_process(COMMAND bash "-c" ${GET_MINOR_VERSION} OUTPUT_VARIABLE PROJECT_MINOR_VERS OUTPUT_STRIP_TRAILING_WHITESPACE)

        if("${PROJECT_REVIS_VERS}" STREQUAL "" OR "${PROJECT_MINOR_VERS}" STREQUAL "")
            message(FATAL_ERROR "Failure to find Git version has caused a fatal exception.")
        endif()

        set(PROJECT_VERSION_STRING 0.${PROJECT_MINOR_VERS}.${PROJECT_REVIS_VERS})
        file(WRITE ${CMAKE_SOURCE_DIR}/Source/Assets/version.txt ${PROJECT_VERSION_STRING})

        message(STATUS "Found version ${PROJECT_VERSION_STRING}. Debug mode on.")
        set(CMAKE_EXPORT_COMPILE_COMMANDS YES)
        set(PROJECT_DEBUG_MODE 1)
    else()
        message(STATUS "Failed to find Git repository in file directory. Relying on local version for compilation.")
        file(READ ${CMAKE_SOURCE_DIR}/Source/Assets/version.txt PROJECT_VERSION_STRING)
    endif()
endmacro()
get_version_string()

#! Set some variables that have to do with the project, like description, 
#! languages, name, and compilation options.
macro(setup_project)
    project("Renai" VERSION ${PROJECT_VERSION_STRING} LANGUAGES C DESCRIPTION 
        "Renai is a high fantasy adult RPG game set in the world of Silre.")
    message(STATUS "Verified project information.")

    set(CMAKE_C_STANDARD 99)
    add_compile_options(-Os -g -Wall -Werror -Wpedantic)
    message(STATUS "Set compile options for the project. Using the ISO C${CMAKE_C_STANDARD} standard.")

    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME})
    include_directories(${CMAKE_SOURCE_DIR}/Dependencies ${CMAKE_SOURCE_DIR}/Source/Modules)
    link_directories(${CMAKE_SOURCE_DIR}/Dependencies/glfw ${CMAKE_SOURCE_DIR}/Dependencies/glad 
        ${CMAKE_SOURCE_DIR}/Dependencies/stbi ${CMAKE_SOURCE_DIR}/Dependencies/cglm)
endmacro()
setup_project()

#! Setup the final bits of the application, readying it for compilation.
macro(create_application)
    file(GLOB SOURCE_FILES ${CMAKE_SOURCE_DIR}/Source/*.c ${CMAKE_SOURCE_DIR}/Source/Modules/*.c)
    add_executable(${PROJECT_NAME} ${SOURCE_FILES})

    if("${CMAKE_SYSTEM_NAME}" STREQUAL "Linux")
        target_link_libraries(${PROJECT_NAME} PRIVATE libglfw3-linux.a PRIVATE libglad-linux.a PRIVATE libstbi-linux.a 
            PRIVATE libglm-linux.a PRIVATE m)
    elseif("{CMAKE_SYSTEM_NAME}" STREQUAL "Windows")
        target_link_libraries(${PROJECT_NAME} PRIVATE libglfw3-win32.a PRIVATE libglad-win32.a PRIVATE libstbi-win32.a 
            PRIVATE libglm-win32.a)
    else()
        message(FATAL_ERROR "Renai doesn't support this operating system.")
    endif()
    message(STATUS "Got operating system '${CMAKE_SYSTEM_NAME}'.")

    file(COPY ${CMAKE_SOURCE_DIR}/Source/Assets DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
    target_compile_definitions(${PROJECT_NAME} PRIVATE TITLE="${PROJECT_NAME} | v${PROJECT_VERSION_STRING}")
    if(PROJECT_DEBUG_MODE)
        file(WRITE ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Logs/debug_log_location "renai_debug_log_directory")
        target_compile_definitions(${PROJECT_NAME} PRIVATE DEBUG_MODE=1)
    endif()
endmacro()
create_application()


#! Separate our messages from the CMake generated ones.
message(STATUS "")