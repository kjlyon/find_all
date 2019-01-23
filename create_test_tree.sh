#!/bin/bash

if (( $# < 3 )); then
	echo "USE: $0 tree-root tree-depth files-per-level"
	exit 1
fi
tree_root=$1
tree_depth=$2
files_per_level=$3

if [[ -d $tree_root ]]; then
	read -p "Directory root $tree_root already exists. Do you want to delete it? (y/N) " deldir
	if [[ $deldir = "y" ]]; then
		rm -fr $tree_root
	fi
fi

create_subdir () {
	if (( $2 > 0 )); then
		mkdir $1
		cd $1
		for i in `seq 1 $3`; do
			new_name=$(cat /dev/urandom | tr -dc 'a-z' | fold -w 32 | head -n 1)
			touch $new_name
		done
		for i in `seq 1 3`; do
			new_name=$(cat /dev/urandom | tr -dc 'a-z' | fold -w 5 | head -n 1)
			create_subdir $new_name $(($2-1)) $3
		done
		cd ..
	fi
}

create_subdir $tree_root $tree_depth $files_per_level
