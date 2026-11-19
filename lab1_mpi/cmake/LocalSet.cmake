set(CMAKE_TEST_PROJECTNAME "test_${PROJECT_NAME}")
set(MPI_METHOD 3 CACHE STRING "MPI method: 1 (send/recv), 2 (scatter/gather), 3 (reduce)")
set(CMAKE_CONFIGURATION_TYPES "Debug;Release;RelWithDebInfo;MinSizeRel" CACHE STRING "" FORCE)
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION_RELEASE TRUE) # if cant build with "ld.lld: error: undefined symbol: main" set it to FALSE