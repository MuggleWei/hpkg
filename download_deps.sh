#!/bin/bash

# dependencies version
libyaml_repo="https://github.com/yaml/libyaml.git"
libyaml_ver="0.2.5"

mugglec_repo="https://github.com/MuggleWei/mugglec.git"
mugglec_ver="v1.2.4"

unitytest_repo="https://github.com/ThrowTheSwitch/Unity.git"
unitytest_ver="v2.5.2"

dep_array=(
	"libyaml" "mugglec" "unitytest"
)

# directories
origin_dir="$(dirname "$(readlink -f "$0")")"
build_dir=$origin_dir/build
tmp_dep_dir=$build_dir/_download_deps
dep_dir=$origin_dir/_deps

if [ -d $dep_dir ]; then
	echo "Dependencies source directory already exists: $dep_dir"
	exit 0
fi

# create tmp dependencies source directory
if [ -d $tmp_dep_dir ]; then
	rm -rf $tmp_dep_dir
fi
mkdir -p $tmp_dep_dir
cd $tmp_dep_dir

for dep in ${dep_array[@]}; do
	repo_name="${dep}_repo"
	ver_name="${dep}_ver"
	repo="${!repo_name}"
	ver="${!ver_name}"
	
	echo "git clone --branch $ver --depth 1 $repo $dep"
	git clone --branch $ver --depth 1 $repo $dep
	status=$?
	if [ $status -eq 0 ]; then
		echo "Success download $dep"
	else
		echo "Failed download $dep"
		exit 1
	fi
done

# move tmp deps to deps
cd $origin
mv $tmp_dep_dir $dep_dir
echo "move $tmp_dep_dir -> $dep_dir"
