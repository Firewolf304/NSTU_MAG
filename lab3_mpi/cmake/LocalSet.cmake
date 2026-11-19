set(CMAKE_TEST_PROJECTNAME "test_${PROJECT_NAME}")
set(MPI_METHOD 1)
message(STATUS "Method: ${MPI_METHOD}")
if(NOT MPI_METHOD STREQUAL "1" AND NOT MPI_METHOD STREQUAL "2")
    message(FATAL_ERROR "MPI_METHOD must be 1 or 2")
endif()
set(CMAKE_CONFIGURATION_TYPES "Debug;Release;RelWithDebInfo;MinSizeRel" CACHE STRING "" FORCE)
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION_RELEASE TRUE) # if cant build with "ld.lld: error: undefined symbol: main" set it to FALSE