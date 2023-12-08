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
	BUILD_BUNDLE=1
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
dist_dir=$origin_dir/dist

cd $origin_dir

if [ -d $dist_dir ]; then
	rm -rf $dist_dir
fi

$origin_dir/bootstrap.sh $BUILD_TYPE
if [ $? -eq 0 ]; then
	echo "success bootstrap"
else
	echo "failed bootstrap"
	exit 1
fi

cmake --build $build_dir --config $BUILD_TYPE --target install

# package
ver=$(head -1 version.txt)
uname_s="$(uname -s)"
case "${uname_s}" in
	Linux*) sys_name=linux;;
	Darwin*) sys_name=mac;;
	*) sys_name=unknown
esac
glibc_ver=$(ldd --version | awk '/ldd/ {print $NF}')
arch=$(uname -m)

pkg_name=hpkg-v$ver-exe-$BUILD_TYPE-$sys_name-glibc$glibc_ver-$arch

#cd $output_dir
#tar -czvf $pkg_name.tar.gz bin/* lib/*.so etc

#echo "copy $output_dir/$pkg_name.tar.gz -> $dist_dir/"
#mv $pkg_name.tar.gz $dist_dir

mkdir -p $dist_dir
mkdir -p $dist_dir/bin
mkdir -p $dist_dir/lib
mkdir -p $dist_dir/etc

echo "copy $output_dir/* -> $dist_dir/"
cp -r $output_dir/bin/* $dist_dir/bin/
cp -r $output_dir/lib/*.so* $dist_dir/lib/
cp -r $output_dir/etc/* $dist_dir/etc/
if [ $BUILD_BUNDLE -eq 1 ]; then
	cp -r $dep_pkg_dir/lib/*.so* $dist_dir/lib/
fi
