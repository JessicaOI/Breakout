find_path(SDL2_TTF_INCLUDE_DIR SDL_ttf.h
        PATH_SUFFIXES SDL2
        HINTS
        ${SDL2_TTF_PATH}/include
        ${SDL2_TTF_PATH}/x86_64-w64-mingw32/include
)

find_library(SDL2_TTF_LIBRARY SDL2_ttf
        HINTS
        ${SDL2_TTF_PATH}/lib
        ${SDL2_TTF_PATH}/x86_64-w64-mingw32/lib
)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(SDL2_ttf DEFAULT_MSG SDL2_TTF_INCLUDE_DIR SDL2_TTF_LIBRARY)

mark_as_advanced(SDL2_TTF_INCLUDE_DIR SDL2_TTF_LIBRARY)
