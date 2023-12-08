#!/bin/bash

# handle argv
if [ "$#" -lt 1 ]; then
	echo "[ERROR] bootstrap without build type"
	echo "[ERROR] Usage: bootstrap.sh <Debug|Release>"
	exit 1
else
	# to lowercase
	BUILD_TYPE=$(echo $1 | tr '[:upper:]' '[:lower:]')
fi

build_shared_libs=ON
build_testing=ON
build_coverage=OFF

if [ "$BUILD_TYPE" = "coverage" ]; then
	BUILD_TYPE=release
	build_testing=ON
	build_coverage=ON
fi

echo "Build Type: $BUILD_TYPE"
echo "Build Shared libs: $build_shared_libs"
echo "Build Testing: $build_testing"
echo "Build Coverage: $build_coverage"

# directories
origin_dir="$(dirname "$(readlink -f "$0")")"
dep_dir=$origin_dir/_deps
build_dir=$origin_dir/build
dep_pkg_dir=$build_dir/_hpkg/deps
test_dep_pkg_dir=$build_dir/_hpkg/test_deps
output_dir=$build_dir/_hpkg/output

cd $origin_dir

# ensure deps exists
$origin_dir/prepare_deps.sh
status=$?
if [ $status -eq 0 ]; then
	echo "Success prepare dependencies"
else
	echo "Failed prepare dependencies"
	exit 1
fi

# create directory: build_dir, dep_pkg_dir, test_dep_pkg_dir
dir_array=("$build_dir" "$dep_pkg_dir" "$test_dep_pkg_dir")
for dirpath in ${dir_array[@]}; do
	if [ ! -d $dir_path ]; then
		mkdir -p $dir_path
	fi
done

# build dependencies - libyaml
libyaml_src_dir=$dep_dir/libyaml
libyaml_build_dir=$dep_dir/build/libyaml
if [ -d $libyaml_build_dir ]; then
	echo "Remove old libyaml build directory: $libyaml_build_dir"
	rm -rf $libyaml_build_dir
fi
mkdir -p $libyaml_build_dir

echo "Generate project: libyaml"
cmake \
	-S $libyaml_src_dir \
	-B $libyaml_build_dir \
	-DCMAKE_BUILD_TYPE=$BUILD_TYPE \
	-DBUILD_SHARED_LIBS=$build_shared_libs \
	-DCMAKE_INSTALL_PREFIX=$dep_pkg_dir \
	-DBUILD_TESTING=OFF

echo "Compile: libyaml"
cmake --build $libyaml_build_dir --config $BUILD_TYPE --target install

# build dependencies - mugglec
mugglec_src_dir=$dep_dir/mugglec
mugglec_build_dir=$dep_dir/build/mugglec
if [ -d $mugglec_build_dir ]; then
	echo "Remove old mugglec build directory: $mugglec_build_dir"
	rm -rf $mugglec_build_dir
fi
mkdir -p $mugglec_build_dir

echo "Generate project: mugglec"
cmake \
	-S $mugglec_src_dir \
	-B $mugglec_build_dir \
	-DCMAKE_BUILD_TYPE=$BUILD_TYPE \
	-DBUILD_SHARED_LIBS=$build_shared_libs \
	-DCMAKE_INSTALL_PREFIX=$dep_pkg_dir \
	-DBUILD_TESTING=OFF

echo "Compile: mugglec"
cmake --build $mugglec_build_dir --config $BUILD_TYPE --target install

# build test dependencies - unitytest
unitytest_src_dir=$dep_dir/unitytest
unitytest_build_dir=$dep_dir/build/unitytest
if [ -d $unitytest_build_dir ]; then
	echo "Remove old unitytest build directory: $unitytest_build_dir"
	rm -rf $unitytest_build_dir
fi
mkdir -p $unitytest_build_dir

echo "Generate project: unitytest"
cmake \
	-S $unitytest_src_dir \
	-B $unitytest_build_dir \
	-DCMAKE_BUILD_TYPE=$BUILD_TYPE \
	-DBUILD_SHARED_LIBS=$build_shared_libs \
	-DCMAKE_INSTALL_PREFIX=$test_dep_pkg_dir

echo "Compile: unitytest"
cmake --build $unitytest_build_dir --config $BUILD_TYPE --target install

# build project
cmake \
	-S $origin_dir \
	-B $build_dir \
	-DCMAKE_BUILD_TYPE=$BUILD_TYPE \
	-DBUILD_SHARED_LIBS=$build_shared_libs \
	-DCMAKE_PREFIX_PATH="$dep_pkg_dir;$test_dep_pkg_dir" \
	-DCMAKE_INSTALL_PREFIX=$output_dir \
	-DBUILD_TESTING=$build_testing \
	-DBUILD_COVERAGE=$build_coverage

if [ "$build_testing" = "ON" ]; then
	cd $build_dir
	cmake --build $build_dir --config $BUILD_TYPE
	#if ! valgrind ls &> /dev/null; then
	#    ctest
	#else
	#    ctest \
	#        -DCTEST_MEMORYCHECK_COMMAND=valgrind \
	#        -DMemoryCheckCommand=valgrind \
	#        -T memcheck
	#fi

	ctest
	if [ $? -eq 0 ]; then
		echo "test success"
	else
		echo "test failed"
		exit 1
	fi
fi
