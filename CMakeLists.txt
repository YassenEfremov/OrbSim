cmake_minimum_required(VERSION 3.27.0)
project(orbsim
	VERSION 0.5.0	# This line MUST be third in the file (bcs GitHub actions)
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

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets)
qt_standard_project_setup()

# add_library(liborbsim
# 	src/simulation/integrators/euler.cpp
# )

qt_add_executable(orbsim
	# src/simulation/integrators/euler.cpp
	# src/simulation/integrators/integrator.cpp
	# src/simulation/integrators/verlet.cpp
	# src/simulation/integrators/rk4.cpp
	# src/simulation/math_obj.cpp
	# src/main.cpp
	src/gui/main.cpp
	src/gui/main_window.cpp
	src/gui/main_window
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

target_link_libraries(orbsim
	PRIVATE
		Qt6::Core
		Qt6::Gui
		Qt6::Widgets
)

set_target_properties(orbsim PROPERTIES
    WIN32_EXECUTABLE ON
    MACOSX_BUNDLE ON
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
