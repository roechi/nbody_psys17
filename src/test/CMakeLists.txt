cmake_minimum_required(VERSION 3.3)

set(REPO ../../external)

project(Test)

include(CTest)
enable_testing()

#set(gtest_disable_pthreads on) #needed in MinGW
include(${REPO}/DownloadProject/DownloadProject.cmake)
download_project(
        PROJ                googletest
        GIT_REPOSITORY      https://github.com/google/googletest.git
        GIT_TAG             master
        UPDATE_DISCONNECTED 1
)

add_subdirectory(${googletest_SOURCE_DIR} ${googletest_BINARY_DIR} EXCLUDE_FROM_ALL)

set(test_SRCS
        gtest.cpp ConfigParser_test.cpp ConfigParser_test.h Body_test.cpp Body_test.h)
set(main_SRCS
        ../main/Body.cpp ../main/ConfigParser.cpp ../main/Body.cpp Body_test.cpp Body_test.h)
add_executable(runUnitTests gtest.cpp ${test_SRCS} ${main_SRCS})
target_link_libraries(runUnitTests gtest gmock)
#add_test(runUnitTests runUnitTests) #included in all tutorials but I don't know what it actually does.