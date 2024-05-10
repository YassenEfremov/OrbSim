# cmake_minimum_required(VERSION 3.27.0)
# project(orbsim
# 	VERSION 0.6.1	# This line MUST be third in the file (bcs GitHub actions)
# 	DESCRIPTION "A simple orbital simulator"
# 	HOMEPAGE_URL "https://github.com/YassenEfremov/OrbSim"
# 	LANGUAGES CXX
# )

# set(CMAKE_CXX_STANDARD 17)
# set(CMAKE_CXX_STANDARD_REQUIRED ON)
# set(BUILD_SHARED_LIBS OFF)

# option(ORBSIM_BUILD_TESTS "Build tests" OFF)


# configure_file(cmake/version.hpp.in version.hpp)
# configure_file(cmake/install_prefix.hpp.in install_prefix.hpp)

add_library(liborbsim
	integrators/euler.cpp
	integrators/integrator.cpp
	integrators/verlet.cpp
	integrators/rk4.cpp
	math_obj.cpp
)

# if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME AND ORBSIM_BUILD_TESTS)
# 	include(CTest)
# 	enable_testing()
#     add_subdirectory(test)
# endif()

target_include_directories(liborbsim
	PRIVATE
		${PROJECT_SOURCE_DIR}/src
		${PROJECT_BINARY_DIR}
)


install(
	TARGETS liborbsim
	DESTINATION bin
)

# install(
# 	FILES ${PROJECT_SOURCE_DIR}/data/
# 	DESTINATION share/${PROJECT_NAME}
# )
