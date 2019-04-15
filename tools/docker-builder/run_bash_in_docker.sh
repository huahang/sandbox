#!/usr/bin/env bash
mkdir -p /builder_users
useradd $XM_USER -b /builder_users -m -u $XM_UID
echo "$XM_USER ALL=(ALL) NOPASSWD:ALL" > /etc/sudoers.d/$XM_USER
XM_USER=$XM_USER $BLADE_ROOT/tools/docker-builder/switch_user_in_docker.sh
