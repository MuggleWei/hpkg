cmake_minimum_required (VERSION 3.18.6)
project(hpkg)

################################
# prepares
################################
include(${CMAKE_CURRENT_LIST_DIR}/cmake/hpkg_utils.cmake)

get_version("version.txt" HPKG_VERSION HPKG_VERSION_MAJOR HPKG_VERSION_MINOR HPKG_VERSION_PATCH)

################################
# output options
################################
message("--------------------------------")
message("# version: ${HPKG_VERSION}")
message("# so version: ${HPKG_VERSION_MAJOR}")
message("# c compiler ${CMAKE_C_COMPILER}")
message("# c compiler flags: ${CMAKE_C_FLAGS}")
message("# cxx compiler ${CMAKE_CXX_COMPILER}")
message("# cxx compiler flags: ${CMAKE_CXX_FLAGS}")
message("# install prefix: ${CMAKE_INSTALL_PREFIX}")
foreach(path ${CMAKE_PREFIX_PATH})
	message("# deps search prefix: ${path}")
endforeach()
message("# CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE}")
message("# BUILD_SHARED_LIBS: ${BUILD_SHARED_LIBS}")
message("# BUILD_TESTING: ${BUILD_TESTING}")
message("# BUILD_SANITIZER: ${BUILD_SANITIZER}")
message("# BUILD_COVERAGE: ${BUILD_COVERAGE}")
message("--------------------------------")

################################
# configure
################################

set(hpkg_src_dir ${CMAKE_CURRENT_LIST_DIR}/src)
set(hpkg_gen_dir ${CMAKE_CURRENT_BINARY_DIR}/generated)
if (BUILD_SHARED_LIBS)
	set(HPKG_USE_DLL ON)
	set(HPKG_LIB_TYPE SHARED)
else()
	set(HPKG_USE_DLL OFF)
	set(HPKG_LIB_TYPE STATIC)
endif()

include(CheckIncludeFile)
check_include_file(threads.h HPKG_HAVE_THREADS)

configure_file(
	${hpkg_src_dir}/hpkg/config.h.in
	${hpkg_gen_dir}/hpkg/config.h)
FILE(GLOB_RECURSE hpkg_gen_h "${hpkg_gen_dir}/hpkg/*.h")

################################
# hpkg
################################

# object
FILE(GLOB_RECURSE hpkg_object_src_c "${hpkg_src_dir}/hpkg/object/*.c")
FILE(GLOB_RECURSE hpkg_object_src_h "${hpkg_src_dir}/hpkg/object/*.h")
add_library(hpkgobj ${HPKG_LIB_TYPE}
	${hpkg_object_src_c}
	${hpkg_object_src_h}
	${hpkg_gen_h})
target_include_directories(hpkgobj PUBLIC
	$<BUILD_INTERFACE:${hpkg_src_dir}>
	$<BUILD_INTERFACE:${hpkg_gen_dir}>
	$<INSTALL_INTERFACE:include>)

target_compile_definitions(hpkgobj PRIVATE HPKG_OBJ_EXPORTS)
set_target_properties(hpkgobj PROPERTIES
	LINKER_LANGUAGE C
	DEBUG_POSTFIX d
	VERSION ${HPKG_VERSION}
	SOVERSION ${HPKG_VERSION_MAJOR})
target_link_libraries(hpkgobj mugglec)

if (UNIX)
	set_target_properties(hpkgobj
		PROPERTIES
		INSTALL_RPATH "\$ORIGIN")
endif()

# cli
FILE(GLOB_RECURSE hpkg_cli_src_c "${hpkg_src_dir}/hpkg/cli/*.c")
FILE(GLOB_RECURSE hpkg_cli_src_h "${hpkg_src_dir}/hpkg/cli/*.h")
add_library(hpkgcli ${HPKG_LIB_TYPE}
	${hpkg_cli_src_c}
	${hpkg_cli_src_h}
	${hpkg_gen_h})
target_include_directories(hpkgcli PUBLIC
	$<BUILD_INTERFACE:${hpkg_src_dir}>
	$<BUILD_INTERFACE:${hpkg_gen_dir}>
	$<INSTALL_INTERFACE:include>)
target_link_libraries(hpkgcli hpkgobj mugglec yaml)

target_compile_definitions(hpkgcli PRIVATE HPKG_CLI_EXPORTS)
set_target_properties(hpkgcli PROPERTIES
	LINKER_LANGUAGE C
	DEBUG_POSTFIX d
	VERSION ${HPKG_VERSION}
	SOVERSION ${HPKG_VERSION_MAJOR})

if (UNIX)
	set_target_properties(hpkgcli
		PROPERTIES
		INSTALL_RPATH "\$ORIGIN")
endif()

# cli exe
FILE(GLOB_RECURSE hpkg_cliexe_src_c "${hpkg_src_dir}/hpkg/cliexe/*.c")
add_executable(hpkg ${hpkg_cliexe_src_c})
target_include_directories(hpkg PUBLIC
	$<BUILD_INTERFACE:${hpkg_src_dir}>
	$<BUILD_INTERFACE:${hpkg_gen_dir}>
	$<INSTALL_INTERFACE:include>)
target_link_libraries(hpkg hpkgcli)

set_target_properties(hpkg PROPERTIES
	LINKER_LANGUAGE C
	DEBUG_POSTFIX d
	VERSION ${HPKG_VERSION}
	SOVERSION ${HPKG_VERSION_MAJOR})

if (MSVC OR MINGW)
	set_target_properties(hpkg
		PROPERTIES
		VS_DEBUGGER_WORKING_DIRECTORY "$(OutDir)")
else()
	if (APPLE)
		set_target_properties(hpkg
			PROPERTIES
			INSTALL_RPATH "@executable_path/../lib")
	elseif (UNIX)
		set_target_properties(hpkg
			PROPERTIES
			INSTALL_RPATH "\$ORIGIN/../lib")
	endif()
endif()

# win debug
if (WIN32)
	foreach(subdir ${CMAKE_PREFIX_PATH})
		file(GLOB dll_files "${subdir}/bin/*.dll")
		foreach(src_file ${dll_files})
			file(RELATIVE_PATH rel_path ${src_file} ${subdir}/bin)
			get_filename_component(dst_name ${src_file} NAME)
			set(dst_file "$(OutDir)${dst_name}")
			add_custom_command(
				TARGET hpkgcli
				POST_BUILD
				COMMAND ${CMAKE_COMMAND} -E copy_if_different "${src_file}" ${dst_file}
				DEPENDS "${src_file}"
				COMMENT "sync ${src_file} to ${dst_file}"
			)
		endforeach(src_file ${dll_files})
	endforeach(subdir ${CMAKE_PREFIX_PATH})
endif()

# install
include(GNUInstallDirs)

# force set install libdir to "lib", avoid of being set to "lib64" in some situation
set(CMAKE_INSTALL_LIBDIR "lib")

install(TARGETS hpkg hpkgcli hpkgobj
	EXPORT hpkgTargets
	RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
	ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
	LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}")
install(DIRECTORY "${hpkg_src_dir}/hpkg" "${hpkg_gen_dir}/hpkg"
	DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
	FILES_MATCHING
	PATTERN "*.h"
	PATTERN "*.hpp")
install(DIRECTORY "${CMAKE_CURRENT_LIST_DIR}/etc/"
	DESTINATION "${CMAKE_INSTALL_SYSCONFDIR}/hpkg")

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

function(add_unittest name folder)
	message("add test ${name} ${folder}")

	file(GLOB tmp_h ${folder}/*.h)
	file(GLOB tmp_c ${folder}/*.c)
	file(GLOB tmp_cpp ${folder}/*.cpp)
	file(GLOB tmp_cc ${folder}/*.cc)

	if (MSVC OR MINGW)
		add_executable(${name} ${tmp_h} ${tmp_c} ${tmp_cpp} ${tmp_cc})
	else()
		add_executable(${name} ${tmp_c} ${tmp_cpp} ${tmp_cc})
	endif(MSVC OR MINGW)
	add_dependencies(${name} hpkgcli)
	target_include_directories(${name} PUBLIC
		${hpkg_src_dir}
		${hpkg_gen_dir})
	target_link_libraries(${name} hpkgcli unity::framework)

	add_test(NAME ${name} COMMAND ${name})
endfunction()

################################
# sync resources
################################

sync_dir(COPY_RES ${CMAKE_CURRENT_LIST_DIR}/etc ${CMAKE_BINARY_DIR}/etc)

################################
# tests
################################
if (BUILD_TESTING)
	if (NOT unity_FOUND)
		message(FATAL_ERROR "Failed found Unity Test")
	endif()

	#enable_testing()
	include(CTest)

	set(test_root_dir ${CMAKE_CURRENT_LIST_DIR}/tests)
	FILE(GLOB subdirs RELATIVE ${test_root_dir} ${test_root_dir}/*)
	FOREACH(subdir ${subdirs})
		IF(IS_DIRECTORY ${test_root_dir}/${subdir})
			add_unittest(${subdir} ${test_root_dir}/${subdir})
		ENDIF()
	ENDFOREACH()
endif()

################################
# coverage
################################

if (BUILD_COVERAGE)
	if (${CMAKE_C_COMPILER_ID} MATCHES Clang)
		set(COVERAGE_COMPILER_FLAGS "-fprofile-instr-generate -fcoverage-mapping")

		find_program(LLVM_PROFDATA llvm-profdata)
		find_program(LLVM_COV_PATH llvm-cov)
		if (LLVM_PROFDATA AND LLVM_COV_PATH)
			set(COV_OUT_NAME hpkgcli_coverage)
			add_custom_target(
				coverage
				COMMAND echo "run converage"
				COMMAND rm -rf ${COV_OUT_NAME}
				COMMAND rm -rf hpkgcli-*profraw
				COMMAND rm -rf hpkgcli.profdata
				COMMAND LLVM_PROFILE_FILE=hpkgcli-%m.profraw make test
				COMMAND ${LLVM_PROFDATA} merge hpkgcli-*.profraw -o hpkgcli.profdata
				COMMAND ${LLVM_COV_PATH} report ./lib/libhpkgcli.* -instr-profile=hpkgcli.profdata
				COMMAND ${LLVM_COV_PATH} show ./lib/libhpkgcli.* -instr-profile=hpkgcli.profdata -output-dir=${COV_OUT_NAME} -format=html
				)
		else()
			message(SEND_ERROR "failed generate coverage report, llvm-profdata or llvm-cov not found")
		endif()
	elseif(${CMAKE_C_COMPILER_ID} STREQUAL GNU)
		set(COVERAGE_COMPILER_FLAGS "--coverage")

		find_program(LCOV_PATH lcov)
		find_program(GENHTML_PATH genhtml)
		if (LCOV_PATH AND GENHTML_PATH)
			set(COV_OUT_NAME hpkgcli_coverage)
			set(COV_DIR ./CMakeFiles/hpkgcli.dir)
			add_custom_target(
				coverage
				COMMAND echo "run converage"
				COMMAND rm -rf ${COV_OUT_NAME}.info ${COV_OUT_NAME}
				COMMAND ${LCOV_PATH} --zerocounters -d ${COV_DIR}
				COMMAND rm -rf ${COV_OUT_NAME} ${COV_OUT_NAME}.info
				COMMAND make test
				COMMAND ${LCOV_PATH} -d ${COV_DIR} -c -o ${COV_OUT_NAME}.info
				COMMAND ${GENHTML_PATH} ${COV_OUT_NAME}.info -o ${COV_OUT_NAME}
				)
		else()
			message(SEND_ERROR "failed generate coverage report, lcov or genhtml not found")
		endif()
	else()
		message("Failed add coverage flags")
		set(COVERAGE_COMPILER_FLAGS "")
	endif()

	message("add coverage flags: ${COVERAGE_COMPILER_FLAGS}")
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${COVERAGE_COMPILER_FLAGS}")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${COVERAGE_COMPILER_FLAGS}")
endif()
