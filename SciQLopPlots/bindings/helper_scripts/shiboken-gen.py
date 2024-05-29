#!/bin/env python3

import os
import sys
import importlib
import argparse
from glob import glob
import subprocess
import json
import platform

parser = argparse.ArgumentParser(description='PySide/shiboken generator wrapper')
parser.add_argument('--input-header')
parser.add_argument('--input-xml')
parser.add_argument('--shiboken')
parser.add_argument('--typesystem-paths')
parser.add_argument('--output-directory')
parser.add_argument('--build-directory')
parser.add_argument('--ref-build-target')
args = parser.parse_args()


def cpp_flags(build_dir, ref_build_target):
    with open(f"{build_dir}/meson-info/intro-targets.json", 'r') as targets_f:
        targets = json.load(targets_f)
        # this is extra fragile!
        ref_target = list(filter(lambda k: k['name'].startswith(ref_build_target), targets))[0]
        params = ref_target['target_sources'][0]['parameters']
        return list(filter(lambda p: p.upper().startswith('-I') or p.startswith('-F'), params))


shiboken_constant_args=['--generator-set=shiboken',
    '--enable-parent-ctor-heuristic',
    '--enable-return-value-heuristic',
    '--use-isnull-as-nb_nonzero',
    '--avoid-protected-hack',
    '--enable-pyside-extensions',
    '--debug-level=full',
    '-std=c++17',
    '--generator-set=shiboken']

if 'linux' in platform.system().lower() and os.environ.get('AUDITWHEEL_PLAT', '') == 'manylinux_2_28_x86_64':
    gcc_found = False
    for v in reversed(range(11,14)):
        prefix = f'/opt/rh/gcc-toolset-{v}/root/usr'
        if os.path.exists(prefix):
            shiboken_constant_args += [
                f"-I/opt/rh/gcc-toolset-{v}/root/usr/lib/gcc/x86_64-redhat-linux/{v}/../../../../include/c++/{v}",
                f"-I/opt/rh/gcc-toolset-{v}/root/usr/lib/gcc/x86_64-redhat-linux/{v}/../../../../include/c++/{v}/x86_64-redhat-linux",
                f"-I/opt/rh/gcc-toolset-{v}/root/usr/lib/gcc/x86_64-redhat-linux/{v}/../../../../include/c++/{v}/backward",
                f"-I/opt/rh/gcc-toolset-{v}/root/usr/lib/gcc/x86_64-redhat-linux/{v}/include",
                f"-I/opt/rh/gcc-toolset-13/root/usr/include",
                f"-I/usr/local/include",
                f"-I/usr/include",
            ]
            gcc_found = True
            shiboken_constant_args += ['--compiler=g++']
            break


cmd = [args.shiboken, args.input_header, args.input_xml ] + shiboken_constant_args + cpp_flags(args.build_directory, args.ref_build_target) + [ f'--typesystem-paths={args.typesystem_paths}', f'--output-directory={args.output_directory}']

subprocess.run(
    cmd,
    check=True
)

