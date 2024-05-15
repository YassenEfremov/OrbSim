cmake_minimum_required(VERSION 3.27.0)
project(orbsim
	VERSION 0.8.0	# This line MUST be third in the file (bcs GitHub actions)
	DESCRIPTION "A simple orbital simulator"
	HOMEPAGE_URL "https://github.com/YassenEfremov/OrbSim"
	LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# set(BUILD_SHARED_LIBS OFF)

option(ORBSIM_BUILD_TESTS "Build tests" OFF)


# configure_file(cmake/version.hpp.in version.hpp)
# configure_file(cmake/install_prefix.hpp.in install_prefix.hpp)

add_subdirectory(src/simulation)
add_subdirectory(src/gui)

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME AND ORBSIM_BUILD_TESTS)
	include(CTest)
	enable_testing()
    add_subdirectory(test)
endif()


include(InstallRequiredSystemLibraries)
set(CPACK_RESOURCE_FILE_LICENSE "${PROJECT_SOURCE_DIR}/LICENSE.txt")
set(CPACK_PACKAGE_VERSION_MAJOR "${orbsim_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${orbsim_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${orbsim_VERSION_PATCH}")
include(CPack)
