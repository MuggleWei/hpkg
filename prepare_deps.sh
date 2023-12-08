#!/bin/bash

# dependencies version
libyaml_repo="https://github.com/yaml/libyaml.git"
libyaml_ver="0.2.5"

mugglec_repo="https://github.com/MuggleWei/mugglec.git"
mugglec_ver="v1.4.4"

unitytest_repo="https://github.com/ThrowTheSwitch/Unity.git"
unitytest_ver="v2.5.2"

dep_array=(
	"libyaml" "mugglec" "unitytest"
)

# directories
origin_dir="$(dirname "$(readlink -f "$0")")"
build_dir=$origin_dir/build
deps_dir=$origin_dir/_deps

if [ ! -d $deps_dir ]; then
	mkdir -p $deps_dir
fi
cd $deps_dir

for dep in ${dep_array[@]}; do
	repo_name="${dep}_repo"
	ver_name="${dep}_ver"
	repo="${!repo_name}"
	ver="${!ver_name}"

	echo "--------------------------------"
	echo "prepare deps: $dep tags/$ver"

	if [ -d $dep ]; then
		cd $dep
		git checkout $ver
		if [ $? -eq 0 ]; then
			echo "$dep tags/$ver already exists"
			continue
		else
			cd ..
			echo "remove $dep that version != $ver"
			rm -rf $dep
		fi
	fi
	
	echo "git clone --branch $ver --depth 1 $repo $dep"
	git clone --branch $ver --depth 1 $repo $dep
	status=$?
	if [ $status -eq 0 ]; then
		echo "Success download $dep tags/$ver"
	else
		echo "Failed download $dep tags/$ver"
		exit 1
	fi
done
