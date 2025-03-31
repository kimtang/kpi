cmake_minimum_required(VERSION 3.29)
project(kpi)
set(CMAKE_CXX_STANDARD 26)

# set(Boost_DEBUG ON)
set(BOOST_ROOT "C:\\boost")
add_library(kpi SHARED "kpi.cpp" )
target_include_directories(kpi PRIVATE "${PROJECT_SOURCE_DIR}/kdb")
find_package(Boost REQUIRED)

include(FetchContent)
FetchContent_Declare(chk
        GIT_REPOSITORY https://github.com/KxSystems/kdb.git
)

FetchContent_MakeAvailable(chk)

set(CMAKE_VS_INCLUDE_INSTALL_TO_DEFAULT_BUILD 1)


# I recommend using SYSTEM when dealing with 3rd party code.
# Avoids the hassle of warnings from a library you don't own.
# target_include_directories(kpi SYSTEM PRIVATE
#         "${CMAKE_BINARY_DIR}/_deps/chk-src/c/c"
# )
message(PROJECT_SOURCE_DIR="${PROJECT_SOURCE_DIR}")
include_directories("${PROJECT_SOURCE_DIR}")

include_directories("${Boost_INCLUDE_DIRS}")
# target_include_directories(kpi SYSTEM PUBLIC ${Boost_INCLUDE_DIR})

if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    set_source_files_properties(kpi PROPERTIES COMPILE_OPTIONS "/bigobj")
    target_link_options(kpi PRIVATE 
        /DEF:${CMAKE_CURRENT_SOURCE_DIR}/kpi.def
    )

    if(CMAKE_SIZEOF_VOID_P EQUAL 8)
        target_link_libraries(kpi PRIVATE "${CMAKE_BINARY_DIR}/_deps/chk-src/w64/q.lib")
        install(TARGETS kpi DESTINATION "${PROJECT_SOURCE_DIR}/bin/w64/${CMAKE_BUILD_TYPE}")
    else()
        target_link_libraries(kpi PRIVATE "${CMAKE_BINARY_DIR}/_deps/chk-src/w32/q.lib")
        install(TARGETS kpi DESTINATION "${PROJECT_SOURCE_DIR}/bin/w32/${CMAKE_BUILD_TYPE}")
    endif()
endif()


# target_link_libraries(kpi PUBLIC Boost::headers Boost::system Boost::filesystem)
