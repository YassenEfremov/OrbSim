cmake_minimum_required(VERSION 3.27.0)
project(orbsim_test VERSION 1.0.0)

# list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)
# include(CodeCoverage)
# append_coverage_compiler_flags()

add_subdirectory(${orbsim_SOURCE_DIR}/lib/Catch2 ${orbsim_BINARY_DIR}/lib/Catch2)

include_directories(
	${orbsim_SOURCE_DIR}/src
	${orbsim_SOURCE_DIR}/lib/Catch2/src
	# ${orbsim_SOURCE_DIR}/lib/json/include
	# ${orbsim_SOURCE_DIR}/lib/spdlog/include
	${orbsim_BINARY_DIR}
)

link_libraries(
	Catch2::Catch2WithMain
	# nlohmann_json::nlohmann_json
	# spdlog::spdlog
)


# For reference
# add_executable(test_lockfile
# 	test_lockfile.cpp
# 	${orbsim_SOURCE_DIR}/src/logger/logger.cpp
# 	${orbsim_SOURCE_DIR}/src/dep-man/lockfile.cpp
# 	${orbsim_SOURCE_DIR}/src/package.cpp
# 	${orbsim_SOURCE_DIR}/src/paths.cpp
# 	${orbsim_SOURCE_DIR}/src/semver.cpp
# 	${orbsim_SOURCE_DIR}/src/util.cpp
# )

# setup_target_for_coverage_gcovr_html(
# 	NAME coverage
# 	EXECUTABLE test_util
# 	# BASE_DIRECTORY "${PROJECT_SOURCE_DIR}/src"
# 	# EXCLUDE "dir2/*"
# )

# For reference
# add_test(
# 	NAME test_lockfile
# 	COMMAND test_lockfile
# )
