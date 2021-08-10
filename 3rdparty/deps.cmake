include(FetchContent)

FetchContent_Declare(
        sfml
        GIT_REPOSITORY "https://github.com/SFML/SFML.git"
        GIT_TAG "master"
)

#set(IMGUI_SFML_FIND_SFML OFF)
#FetchContent_Declare(
#        sfml-imgui
#        GIT_REPOSITORY "https://github.com/eliasdaler/imgui-sfml.git"
#        GIT_TAG "master"
#)


FetchContent_MakeAvailable(sfml) # sfml-imgui
