include(FetchContent)

FetchContent_Declare(
        imgui
        GIT_REPOSITORY "https://github.com/ocornut/imgui.git"
        GIT_TAG "master"
)
FetchContent_Populate(imgui)

set(BUILD_SHARED_LIBS OFF)
set(SFML_STATIC_LIBRARIES ON)
FetchContent_Declare(
        sfml
        GIT_REPOSITORY "https://github.com/SFML/SFML.git"
        GIT_TAG "master"
)

set(IMGUI_SFML_FIND_SFML OFF)
set(IMGUI_DIR "${imgui_SOURCE_DIR}")
FetchContent_Declare(
        sfml-imgui
        GIT_REPOSITORY "https://github.com/eliasdaler/imgui-sfml.git"
        GIT_TAG "master"
)

FetchContent_MakeAvailable(sfml sfml-imgui imgui)
