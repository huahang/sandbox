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

blade () {
  BLADE_ROOT=`_find_blade_root`
  if [ -z $BLADE_ROOT ]; then
    echo "Unable to find BLADE_ROOT"
    return 1
  fi
  $BLADE_ROOT/tools/docker-builder/build.sh
  docker run -i -t -u $UID:$GROUPS -w $PWD \
  -e BLADE_ROOT=$BLADE_ROOT \
  -v $BLADE_ROOT:$BLADE_ROOT huahang/sandbox-docker-builder \
  $BLADE_ROOT/tools/docker-builder/run_blade_in_docker.sh $@
}

enter_blade_builder () {
  BLADE_ROOT=`_find_blade_root`
  if [ -z $BLADE_ROOT ]; then
    echo "Unable to find BLADE_ROOT"
    return 1
  fi
  $BLADE_ROOT/tools/docker-builder/build.sh
  docker run -i -t -w $PWD \
  -e BLADE_ROOT=$BLADE_ROOT \
  -e XM_USER=$USER \
  -e XM_UID=$UID \
  -v $BLADE_ROOT:$BLADE_ROOT huahang/sandbox-docker-builder \
  $BLADE_ROOT/tools/docker-builder/run_bash_in_docker.sh
}
