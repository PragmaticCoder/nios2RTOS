#!/usr/bin/env bash
# Auto Project Generation Script. Copyright (c) 2020 Alvi Akbar

set -o errexit
set -o pipefail
set -o nounset
# set -o xtrace

# Magic variables for current file & dir
__dir="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
__file="${__dir}/$(basename "${BASH_SOURCE[0]}")"
__base="$(basename ${__file} .sh)"
__root="$(cd "$(dirname "${__dir}")" && pwd)" # <-- change this as it depends on your app

read -p "Enter Project Name: " project_name

project_dir="${__dir}/${project_name}"
micrium_dir="${project_name}/Micrium"
de2_115_dir="${micrium_dir}/DE2-115_Computer"
verilog_dir="${de2_115_dir}/Verilog"

mkdir -p "${__dir}/${project_name}"
mkdir -p "${micrium_dir}"
mkdir -p "${de2_115_dir}"
mkdir -p "${verilog_dir}"

cp -r /opt/intelFPGA_lite/18.1/Univeristy_Program/Computer_Systems/DE2-115/DE2-115_Computer/verilog/* "${verilog_dir}/"
