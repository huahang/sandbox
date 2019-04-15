#!/usr/bin/env bash

export CCACHE_DIR=$BLADE_ROOT/.builder_ccache

$BLADE_ROOT/tools/blade-build/blade $@
