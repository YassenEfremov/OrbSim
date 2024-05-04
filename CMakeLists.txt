cmake_minimum_required(VERSION 3.27.0)
project(orbsim
	VERSION 0.4.0
	DESCRIPTION "A simple orbital simulator"
	HOMEPAGE_URL "https://github.com/YassenEfremov/OrbSim"
	LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
# set(BUILD_SHARED_LIBS OFF)

option(ORBSIM_BUILD_TESTS "Build tests" OFF)


# configure_file(cmake/version.hpp.in version.hpp)
# configure_file(cmake/install_prefix.hpp.in install_prefix.hpp)

# add_library(liborbsim
# 	src/simulation/integrators/euler.cpp
# )

add_executable(orbsim
	# src/simulation/integrators/euler.cpp
	# src/simulation/integrators/integrator.cpp
	# src/simulation/integrators/verlet.cpp
	# src/simulation/integrators/rk4.cpp
	# src/simulation/math_obj.cpp
	# src/main.cpp
	src/gui/main.cpp
)

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME AND ORBSIM_BUILD_TESTS)
	include(CTest)
	enable_testing()
    add_subdirectory(test)
endif()

target_include_directories(orbsim
	PRIVATE
		${PROJECT_SOURCE_DIR}/src
		${PROJECT_BINARY_DIR}
)

find_package(wxWidgets REQUIRED COMPONENTS net core base)
include(${wxWidgets_USE_FILE})

target_link_libraries(orbsim
	PRIVATE
		${wxWidgets_LIBRARIES}
)


install(
	TARGETS orbsim
	DESTINATION bin
)

# install(
# 	FILES ${PROJECT_SOURCE_DIR}/data/
# 	DESTINATION share/${PROJECT_NAME}
# )


include(InstallRequiredSystemLibraries)
set(CPACK_RESOURCE_FILE_LICENSE "${PROJECT_SOURCE_DIR}/LICENSE.txt")
set(CPACK_PACKAGE_VERSION_MAJOR "${orbsim_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${orbsim_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${orbsim_VERSION_PATCH}")
include(CPack)
