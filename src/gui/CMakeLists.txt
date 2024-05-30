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
file(READ shaders/glsl/central_body.vert CENTRAL_BODY_VERT_SHADER)
file(READ shaders/glsl/central_body.frag CENTRAL_BODY_FRAG_SHADER)
file(READ shaders/glsl/orbit.vert ORBIT_VERT_SHADER)
file(READ shaders/glsl/orbit.frag ORBIT_FRAG_SHADER)
file(READ shaders/glsl/xyz_gizmo.vert XYZ_GIZMO_VERT_SHADER)
file(READ shaders/glsl/xyz_gizmo.frag XYZ_GIZMO_FRAG_SHADER)
configure_file(shaders/central_body_shaders.hpp.in shaders/central_body_shaders.hpp)
configure_file(shaders/orbit_shaders.hpp.in shaders/orbit_shaders.hpp)
configure_file(shaders/xyz_gizmo_shaders.hpp.in shaders/xyz_gizmo_shaders.hpp)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets OpenGLWidgets)
qt_standard_project_setup()

qt_add_executable(orbsim
	central_body.cpp
	xyz_gizmo.cpp
	orbit.cpp
	main_window.cpp
	main_window.ui
	output_window.cpp
	main.cpp
)

# if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME AND ORBSIM_BUILD_TESTS)
# 	include(CTest)
# 	enable_testing()
#     add_subdirectory(test)
# endif()

target_include_directories(orbsim
	PRIVATE
		${PROJECT_SOURCE_DIR}/src/gui
		${PROJECT_SOURCE_DIR}/src
		${PROJECT_BINARY_DIR}
		${PROJECT_BINARY_DIR}/src/gui
)

target_link_libraries(orbsim
	PRIVATE
		liborbsim
		Qt6::Core
		Qt6::Gui
		Qt6::Widgets
		Qt6::OpenGLWidgets
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
