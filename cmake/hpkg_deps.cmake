################################
# deps
################################

find_package(mugglec)
if (NOT mugglec_FOUND)
	message(FATAL_ERROR "Failed found mugglec")
endif()
message("# Found mugglec: ${mugglec_DIR}")

find_package(yaml)
if (NOT yaml_FOUND)
	message(FATAL_ERR "Failed found yaml")
endif()
message("# Found yaml: ${yaml_DIR}")

find_package(unity)
if (unity_FOUND)
	message("# Found Unity Test: ${unity_DIR}")
endif()
