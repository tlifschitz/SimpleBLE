import argparse
import os
import pathlib
import re
import subprocess
import sys
import xml.etree.ElementTree as ElementTree

import pybind11
import skbuild


def exclude_unnecessary_files(cmake_manifest):
    def is_necessary(name):
        is_necessary = (
            name.endswith(".so")
            or name.endswith(".dylib")
            or name.endswith("py")
            or name.endswith("pyd")
        )
        print(f"Parsing file: {name} - {is_necessary}")
        return is_necessary

    return list(filter(is_necessary, cmake_manifest))


def get_pypi_versions(package_name, test_repo=False):
    url = f"https://{'test.' if test_repo else ''}pypi.org/rss/project/{package_name}/releases.xml"
    result = subprocess.run(
        ['curl', '-s', '-w', '%{http_code}', '-o', '-', url],
        capture_output=True,
        text=True
    )
    response_code = result.stdout[-3:]
    content = result.stdout[:-3].strip()
    if response_code == '200':
        try:
            root = ElementTree.fromstring(content)
            versions = [item.find('title').text for item in root.findall('.//item')]
            return versions
        except ElementTree.ParseError:
            print(f"\033[91mERROR: Could not parse XML:\n\n {content}\033[0m")
            return None
    else:
        print(f"\033[91mWARNING: Could not fetch {url}: {response_code}\033[0m")
        return None

def get_last_pypi_version(package_name, test_repo=False):
    versions = get_pypi_versions(package_name, test_repo)
    return versions[0] if versions else None

argparser = argparse.ArgumentParser(add_help=False)
argparser.add_argument(
    "--plain", help="Use Plain SimpleBLE", required=False, action="store_true"
)
args, unknown = argparser.parse_known_args()
sys.argv = [sys.argv[0]] + unknown

root = pathlib.Path(__file__).parent.resolve()

# Generate the version string
version_str = (root / "VERSION").read_text(encoding="utf-8").strip()
tag = subprocess.run(["git", "describe", "--tags", "--exact-match"], capture_output=True, text=True).stdout.strip()
ref = subprocess.run(["git", "rev-parse", "--abbrev-ref", "HEAD"], capture_output=True, text=True).stdout.strip()
    
if tag:
    if tag != version_str:
        print(f"\033[91mWARNING: Repo VERSION ({version_str}) differs from tag ({tag}), ref: {ref}\033[0m")
else:
    # If the ref is not tagged (a release), PyPi pre-release version is increased automatically
    last_version = get_last_pypi_version("simplepyble")
    if last_version:
        last_dev = re.search(r'dev(\d+)', last_version)
        if version_str not in last_version:
            print(f"\033[91mWARNING: Repo VERSION ({version_str}) differs from last PyPi version ({last_version}), ref: {ref}\033[0m")
        else:
            print(f"\033[92mRepo VERSION ({version_str}) matches last PyPi version ({last_version}), ref: {ref}\033[0m")
        version_str += f".dev{0 if last_dev is None else int(last_dev.group(1)) + 1 }"

# Get the long description from the README file
long_description = (root / "simplepyble" / "README.rst").read_text(encoding="utf-8")

cmake_options = []
cmake_options.append(f"-Dpybind11_DIR={pybind11.get_cmake_dir()}")
if sys.platform == "win32":
    cmake_options.append("-DCMAKE_SYSTEM_VERSION=10.0.19041.0")
elif sys.platform.startswith("darwin"):
    cmake_options.append("-DCMAKE_OSX_DEPLOYMENT_TARGET=10.15")
cmake_options.append(f"-DPYTHON_EXECUTABLE={sys.executable}")
cmake_options.append(f"-DSIMPLEPYBLE_VERSION={version_str}")

if args.plain:
    cmake_options.append("-DSIMPLEBLE_PLAIN=ON")

if 'PIWHEELS_BUILD' in os.environ:
    cmake_options.append("-DLIBFMT_VENDORIZE=OFF")

# The information here can also be placed in setup.cfg - better separation of
# logic and declaration, and simpler if you include description/version in a file.
skbuild.setup(
    name="simplepyble",
    version=version_str,
    author="Kevin Dewald",
    author_email="kevin@dewald.me",
    url="https://github.com/OpenBluetoothToolbox/SimpleBLE",
    description="The ultimate fully-fledged cross-platform BLE library, designed for simplicity and ease of use.",
    long_description=long_description,
    long_description_content_type="text/x-rst",
    packages=["simplepyble"],
    package_dir={"": "simplepyble/src"},
    cmake_source_dir="simplepyble",
    cmake_args=cmake_options,
    cmake_process_manifest_hook=exclude_unnecessary_files,
    cmake_install_dir="simplepyble/src/simplepyble",
    setup_requires=[
        "setuptools>=42",
        "scikit-build",
        "ninja; platform_system!='Windows'",
        "cmake>=3.21",
        "pybind11",
    ],
    install_requires=[],
    extras_require={},
    platforms="Windows, macOS, Linux",
    python_requires=">=3.7",
    classifiers=[
        "License :: OSI Approved :: GNU General Public License v3 (GPLv3)",
        "License :: Other/Proprietary License",
        "Programming Language :: Python :: 3",
        "Programming Language :: Python :: 3.7",
        "Programming Language :: Python :: 3.8",
        "Programming Language :: Python :: 3.9",
        "Programming Language :: Python :: 3.10",
        "Programming Language :: Python :: 3.11",
        "Programming Language :: Python :: 3 :: Only",
    ],
)