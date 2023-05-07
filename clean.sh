#!/bin/bash

origin_dir="$(dirname "$(readlink -f "$0")")"
dep_dir=$origin_dir/_deps
build_dir=$origin_dir/build
pkg_dir=$origin_dir/_package

remove_dirs=(
	"$dep_dir"
	"$build_dir"
	"$pkg_dir"
)
for d in ${remove_dirs[@]}; do
	echo $d
	if [ -d $d ]; then
		rm -rf $d
	fi
done
