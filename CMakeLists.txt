cmake_minimum_required (VERSION 3.0.2)
project(hpkg)

################################
# general config
################################

message("-- use c compiler ${CMAKE_C_COMPILER}")
message("-- use c++ compiler ${CMAKE_CXX_COMPILER}")

# set compile parameter
if (${CMAKE_C_COMPILER_ID} STREQUAL GNU)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -march=native -Wall -Wextra")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=native -Wall -Wextra")
elseif (${CMAKE_C_COMPILER_ID} MATCHES Clang)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -march=native -Wall -Wextra -Wno-missing-field-initializers")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=native -Wall -Wextra -Wno-missing-field-initializers")
elseif (${CMAKE_C_COMPILER_ID} STREQUAL MSVC)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS=1 -D_UNICODE -DUNICODE)
endif()

# set standard and print features
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

message("-- c compiler support features: ")
foreach(feature ${CMAKE_C_COMPILE_FEATURES})
	message("support feature: ${feature}")
endforeach()

# for vim plugin - YCM
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
# deps
################################

find_package(mugglec)
if (NOT mugglec_FOUND)
	message(FATAL_ERR "Failed found mugglec")
endif()
message("Found mugglec: ${mugglec_DIR}")

find_package(yaml)
if (NOT yaml_FOUND)
	message(FATAL_ERR "Failed found yaml")
endif()
message("Found yaml: ${yaml_DIR}")

################################
# hpkg
################################

# version
file(STRINGS "version.txt" hpkg_version)
string(REPLACE "-" ";" hpkg_semver_ext ${hpkg_version})
list(GET hpkg_semver_ext 0 hpkg_semver)
string(REPLACE "." ";" hpkg_semver_list ${hpkg_semver})

list(GET hpkg_semver_list 0 HPKG_VERSION_MAJOR)
list(GET hpkg_semver_list 1 HPKG_VERSION_MINOR)
list(GET hpkg_semver_list 2 HPKG_VERSION_PATCH)
set(HPKG_VERSION "${HPKG_VERSION_MAJOR}.${HPKG_VERSION_MINOR}.${HPKG_VERSION_PATCH}")

# configure
set(hpkg_src_dir ${CMAKE_CURRENT_LIST_DIR}/src)
set(hpkg_gen_dir ${CMAKE_CURRENT_BINARY_DIR}/generated)
if (BUILD_SHARED_LIBS)
	set(HPKG_USE_DLL ON)
	set(HPKG_LIB_TYPE SHARED)
else()
	set(HPKG_USE_DLL OFF)
	set(HPKG_LIB_TYPE STATIC)
endif()

configure_file(
	${hpkg_src_dir}/hpkg/config.h.in
	${hpkg_gen_dir}/hpkg/config.h)

# add library
FILE(GLOB_RECURSE hpkg_src_c "${hpkg_src_dir}/hpkg/*.c")
FILE(GLOB_RECURSE hpkg_src_h "${hpkg_src_dir}/hpkg/*.h")
FILE(GLOB_RECURSE hpkg_gen_h "${hpkg_gen_dir}/hpkg/*.h")
add_library(hpkg ${HPKG_LIB_TYPE}
	${hpkg_src_c}
	${hpkg_src_h}
	${hpkg_gen_h})
target_include_directories(hpkg PUBLIC 
	$<BUILD_INTERFACE:${hpkg_src_dir}>
	$<BUILD_INTERFACE:${hpkg_gen_dir}>
	$<INSTALL_INTERFACE:include>)
target_link_libraries(hpkg mugglec yaml)

target_compile_definitions(hpkg PRIVATE HPKG_EXPORTS)
set_target_properties(hpkg PROPERTIES
	LINKER_LANGUAGE C
	DEBUG_POSTFIX d
	VERSION ${HPKG_VERSION}
	SOVERSION ${HPKG_VERSION_MAJOR})

# add cli
add_executable(hpkg_cli ${CMAKE_CURRENT_LIST_DIR}/src/hpkg_cli/main.c)
target_link_libraries(hpkg_cli hpkg)
target_include_directories(hpkg_cli PUBLIC 
	$<BUILD_INTERFACE:${hpkg_src_dir}>
	$<BUILD_INTERFACE:${hpkg_gen_dir}>
	$<INSTALL_INTERFACE:include>)

set_target_properties(hpkg_cli PROPERTIES
	OUTPUT_NAME hpkg
	LINKER_LANGUAGE C
	DEBUG_POSTFIX d
	VERSION ${HPKG_VERSION}
	SOVERSION ${HPKG_VERSION_MAJOR})

if (MSVC OR MINGW)
	set_target_properties(hpkg_cli
		PROPERTIES
		VS_DEBUGGER_WORKING_DIRECTORY "$(OutDir)")
else()
	if (APPLE)
		set_target_properties(hpkg_cli
			PROPERTIES
			INSTALL_RPATH "@executable_path/../lib")
	elseif (UNIX)
		set_target_properties(hpkg_cli
			PROPERTIES
			INSTALL_RPATH "\$ORIGIN/../lib")
	endif()
endif()

# install
include(GNUInstallDirs)
install(TARGETS hpkg hpkg_cli
	EXPORT hpkgTargets
	RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
	ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
	LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}")
install(DIRECTORY "${hpkg_src_dir}/hpkg" "${hpkg_gen_dir}/hpkg"
	DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
	FILES_MATCHING
	PATTERN "*.h"
	PATTERN "*.hpp")

# cmake package config
set(hpkg_export_dir "${CMAKE_INSTALL_LIBDIR}/cmake/hpkg")
install(EXPORT hpkgTargets
	FILE hpkgTargets.cmake
	DESTINATION "${hpkg_export_dir}")

include(CMakePackageConfigHelpers)
configure_package_config_file(
	"${CMAKE_CURRENT_LIST_DIR}/cmake/hpkgConfig.cmake.in"
	"${hpkg_gen_dir}/hpkgConfig.cmake"
	INSTALL_DESTINATION "${hpkg_export_dir}"
	NO_SET_AND_CHECK_MACRO
	NO_CHECK_REQUIRED_COMPONENTS_MACRO)
write_basic_package_version_file(
	"${hpkg_gen_dir}/hpkgConfigVersion.cmake"
	VERSION ${HPKG_VERSION}
	COMPATIBILITY SameMajorVersion)
install(FILES
	"${hpkg_gen_dir}/hpkgConfig.cmake"
	"${hpkg_gen_dir}/hpkgConfigVersion.cmake"
	DESTINATION "${hpkg_export_dir}")

# pkgconfig
configure_file(
	"${CMAKE_CURRENT_LIST_DIR}/cmake/hpkg.pc.in"
	"${hpkg_gen_dir}/hpkg.pc" @ONLY)
install(FILES
	"${hpkg_gen_dir}/hpkg.pc"
	DESTINATION "${CMAKE_INSTALL_LIBDIR}/pkgconfig")

# cpack
include(InstallRequiredSystemLibraries)
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_LIST_DIR}/LICENSE")
set(CPACK_PACKAGE_VERSION_MAJOR ${HPKG_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${HPKG_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${HPKG_VERSION_PATCH})
set(CPACK_SOURCE_GENERATOR "TGZ")
include(CPack)

################################
# functions
################################

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

function(add_example name folder)
	message("add example ${name} ${folder}")
	
	set(name example_${name})

	file(GLOB_RECURSE tmp_h ${folder}/*.h)
	file(GLOB_RECURSE tmp_c ${folder}/*.c)
	file(GLOB_RECURSE tmp_cpp ${folder}/*.cpp)
	file(GLOB_RECURSE tmp_cc ${folder}/*.cc)

	if (MSVC OR MINGW)
		add_executable(${name} ${tmp_h} ${tmp_c} ${tmp_cpp} ${tmp_cc})
		set_target_properties(${name}
			PROPERTIES
			FOLDER "example"
			VS_DEBUGGER_WORKING_DIRECTORY "$(OutDir)"
		)
	else()
		add_executable(${name} ${tmp_c} ${tmp_cpp} ${tmp_cc})
		if (APPLE)
			set_target_properties(${name}
				PROPERTIES
				INSTALL_RPATH "@executable_path/../lib"
			)
		elseif (UNIX)
			set_target_properties(${name}
				PROPERTIES
				INSTALL_RPATH "\$ORIGIN/../lib"
			)
		endif()
	endif()
	target_compile_definitions(${name} PUBLIC MUGGLE_HOLD_LOG_MACRO)
	target_include_directories(${name} PUBLIC ${folder} ${LIBXML2_INCLUDE_DIR})
	target_link_libraries(${name} mugglec yaml)
endfunction()

################################
# sync resources
################################

sync_dir(COPY_RES ${CMAKE_CURRENT_LIST_DIR}/etc ${CMAKE_BINARY_DIR}/etc)

################################
# examples
################################

set(example_root_dir ${CMAKE_CURRENT_LIST_DIR}/example)
FILE(GLOB subdirs RELATIVE ${example_root_dir} ${example_root_dir}/*)
FOREACH(subdir ${subdirs})
	IF(IS_DIRECTORY ${example_root_dir}/${subdir})
		add_example(${subdir} ${example_root_dir}/${subdir})
	ENDIF()
ENDFOREACH()
