# list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)
# include(CodeCoverage)
# append_coverage_compiler_flags()

add_subdirectory(${orbsim_SOURCE_DIR}/lib/googletest ${orbsim_BINARY_DIR}/lib/googletest)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)


add_executable(orbsimlib_test
	integrators/integrator_factory_test.cpp
	vec3_test.cpp
	# satellite_test.cpp
)

target_include_directories(orbsimlib_test
	PRIVATE
		${orbsim_SOURCE_DIR}/src
		# ${orbsim_SOURCE_DIR}/lib/json/include
		# ${orbsim_SOURCE_DIR}/lib/spdlog/include
		${orbsim_BINARY_DIR}
)

target_link_libraries(orbsimlib_test
	PRIVATE
		liborbsim
		GTest::gtest_main
		# nlohmann_json::nlohmann_json
		# spdlog::spdlog
)

include(GoogleTest)
gtest_discover_tests(orbsimlib_test)


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
