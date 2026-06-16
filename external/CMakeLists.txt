# set up the external libraries that aren't popular enough to be found on most
# package managers are included with the project (in the 'external' folder)

add_subdirectory("nanosvg")
if (NOT USE_SYSTEM_PUGIXML)
  add_subdirectory("pugixml")
endif()
add_subdirectory("id3v2lib")
add_subdirectory("libcheevos")

# SDL2 is built from source for Android (the NDK does not provide it).
# For Linux/Windows it is found via find_package(SDL2) in the root CMakeLists.txt.
if(ANDROID)
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/SDL2/CMakeLists.txt")
        # SDL2 Android CMake support: sets SDL2_INCLUDE_DIRS and builds libSDL2.so
        set(SDL2_DISABLE_INSTALL ON CACHE BOOL "" FORCE)
        add_subdirectory("SDL2")
    else()
        message(FATAL_ERROR
            "SDL2 source not found at external/SDL2.\n"
            "Run: git submodule update --init external/SDL2")
    endif()
endif()
