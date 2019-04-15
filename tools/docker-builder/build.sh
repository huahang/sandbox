#!/usr/bin/env bash

_find_blade_root () {
  local dir
  dir=$PWD;
  while [ "$dir" != "/" ]; do
    if [ -f "$dir/BLADE_ROOT" ]; then
      echo "$dir"
      return 0
    fi;
    dir=`dirname "$dir"`
  done
  return 1
}

BLADE_ROOT=`_find_blade_root`
if [ -z $BLADE_ROOT ]; then
  echo "Unable to find BLADE_ROOT"
  return 1
fi

docker build -t huahang/sandbox-docker-builder $BLADE_ROOT/tools/docker-builder
