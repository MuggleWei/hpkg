#!/bin/bash

# handle argv
if [ "$#" -lt 1 ]; then
	echo "[ERROR] build without build type"
	echo "Usage: build.sh <Debug|Release> [unbundle|bundle]"
	echo "e.g."
	echo "  build.sh Release"
	echo "  build.sh Release bundle"
	exit 1
else
	# to lowercase
	BUILD_TYPE=$(echo $1 | tr '[:upper:]' '[:lower:]')
fi

if [ "$#" -lt 2 ]; then
	BUILD_BUNDLE=0
else
	bundle_flag=$(echo $2 | tr '[:upper:]' '[:lower:]')
	if [ "$bundle_flag" = "bundle" ]; then
		BUILD_BUNDLE=1
	elif [ "$bundle_flag" = "unbundle" ]; then
		BUILD_BUNDLE=0
	else
		echo "[ERROR] build with unrecognized bundle flag: $bundle_flag"
		exit 1
	fi
fi

echo "Build Type: $BUILD_TYPE"
echo "Build Bundle Package: $BUILD_BUNDLE"

origin_dir="$(dirname "$(readlink -f "$0")")"
build_dir=$origin_dir/build
dep_pkg_dir=$build_dir/_hpkg/deps
output_dir=$build_dir/_hpkg/output
pkg_dir=$origin_dir/dist

cd $origin_dir

if [ -d $pkg_dir ]; then
	rm -rf $pkg_dir
fi

$origin_dir/bootstrap.sh $BUILD_TYPE
cmake --build $build_dir --config $BUILD_TYPE --target install

mkdir -p $pkg_dir
mkdir -p $pkg_dir/bin
mkdir -p $pkg_dir/lib
mkdir -p $pkg_dir/etc

echo "copy $output_dir/* -> $pkg_dir/"
cp -r $output_dir/* $pkg_dir/

if [ $BUILD_BUNDLE -eq 1 ]; then
	echo "copy $dep_pkg_dir/* -> $pkg_dir/"
	cp -r $dep_pkg_dir/* $pkg_dir/
fi
