################################
# general config
################################

# set compile parameter
if (${CMAKE_C_COMPILER_ID} STREQUAL GNU)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
elseif (${CMAKE_C_COMPILER_ID} MATCHES Clang)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
elseif (${CMAKE_C_COMPILER_ID} STREQUAL MSVC)
    add_definitions(-D_UNICODE -DUNICODE)
	add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
	add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")
endif()

# set standard and print features
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

message("-- c compiler support features: ")
foreach(feature ${CMAKE_C_COMPILE_FEATURES})
	message("support feature: ${feature}")
endforeach()

# for LSP
if (NOT ${CMAKE_CXX_COMPILER_ID} STREQUAL MSVC)
	set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
endif()

# set output directory
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# set use folder in vs
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

################################
# options
################################
if (NOT CMAKE_CONFIGURATION_TYPES AND 
		NOT CMAKE_NO_BUILD_TYPE AND
		NOT CMAKE_BUILD_TYPE AND
		CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
	set(CMAKE_BUILD_TYPE Release)
endif()

if (NOT CMAKE_CONFIGURATION_TYPES AND 
    NOT CMAKE_NO_BUILD_TYPE)
    set(CMAKE_TRY_COMPILE_CONFIGURATION "${CMAKE_BUILD_TYPE}")
else()
	set(CMAKE_TRY_COMPILE_CONFIGURATION Release)
endif()

if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
	set(CMAKE_INSTALL_PREFIX ${CMAKE_BINARY_DIR}/dist CACHE PATH "Install prefix" FORCE)
endif()

option(BUILD_SHARED_LIBS "Use deps shared lib" ON)
option(BUILD_TESTING "Build testing" OFF)
option(BUILD_SANITIZER "Build sanitizer" OFF)
option(BUILD_COVERAGE "Build coverage" OFF)

################################
# deps
################################

# deps: 
include(${CMAKE_CURRENT_LIST_DIR}/hpkg_deps.cmake)

################################
# macro & functions
################################

# macro: get version
macro(get_version filepath ver ver_major ver_minor ver_patch)
	file(STRINGS "${filepath}" app_version)
	string(REPLACE "-" ";" app_semver_ext ${app_version})
	list(GET app_semver_ext 0 app_semver)
	string(REPLACE "." ";" app_semver_list ${app_semver})

	list(GET app_semver_list 0 app_version_major)
	list(GET app_semver_list 1 app_version_minor)
	list(GET app_semver_list 2 app_version_patch)

	set(${ver} "${app_version}")
	set(${ver_major} "${app_version_major}")
	set(${ver_minor} "${app_version_minor}")
	set(${ver_patch} "${app_version_patch}")
endmacro()

# function: sync directory
function(sync_dir name src_dir dst_dir)
	file(GLOB_RECURSE src_files "${src_dir}/*")
	foreach(src_file ${src_files})
		file(RELATIVE_PATH rel_path ${src_dir} ${src_file})
		set(dst_file "${dst_dir}/${rel_path}")
		add_custom_command(
			OUTPUT "${dst_file}"
			COMMAND ${CMAKE_COMMAND} -E copy_if_different "${src_file}" ${dst_file}
			DEPENDS "${src_file}"
			COMMENT "copy ${src_file} to ${dst_file}"
		)
		list(APPEND dst_files "${dst_file}")
	endforeach()
	add_custom_target(${name} ALL DEPENDS ${dst_files})
endfunction()
