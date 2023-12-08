#!/bin/bash

origin_dir="$(dirname "$(readlink -f "$0")")"
dep_dir=$origin_dir/_deps
build_dir=$origin_dir/build
dist_dir=$origin_dir/dist

remove_dirs=(
	"$dep_dir"
	"$build_dir"
	"$dist_dir"
)
for d in ${remove_dirs[@]}; do
	echo "clean $d"
	if [ -d $d ]; then
		rm -rf $d
	fi
done
