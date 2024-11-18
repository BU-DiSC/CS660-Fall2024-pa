include(FetchContent)

FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG release-1.12.1
)
FetchContent_MakeAvailable(googletest)

#add_subdirectory(pa0)
#add_subdirectory(pa1)
#add_subdirectory(pa2)
#add_subdirectory(pa3)
add_subdirectory(pa4)
