# Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
#
# Licensed under the Apache License, Version 2.0 (the "License").
# You may not use this file except in compliance with the License.
# A copy of the License is located at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# or in the "license" file accompanying this file. This file is distributed
# on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either
# express or implied. See the License for the specific language governing
# permissions and limitations under the License.


import contextlib
import logging
import os
import pathlib
import sys


CACHE_FILE = "cache.json"
CACHE_POINTER = ".litani_cache_dir"
CI_STAGES = ["build", "test", "report"]
JOBS_DIR = "jobs"
RUN_FILE = "run.json"
TIME_FORMAT_R = "%Y-%m-%dT%H:%M:%SZ"
TIME_FORMAT_W = "%Y-%m-%dT%H:%M:%SZ"


def _get_cache_dir(path=os.getcwd()):
    def cache_pointer_dirs():
        current = pathlib.Path(path).resolve(strict=True)
        yield current
        while current.parent != current:
            current = current.parent
            yield current
        current = pathlib.Path(os.getcwd()).resolve(strict=True)
        for root, _, dirs in os.walk(current):
            for dyr in dirs:
                yield pathlib.Path(os.path.join(root, dyr))

    for possible_dir in cache_pointer_dirs():
        logging.debug(
            "Searching for cache pointer in %s", possible_dir)
        possible_pointer = possible_dir / CACHE_POINTER
        try:
            if possible_pointer.exists():
                logging.debug(
                    "Found a cache pointer at %s", possible_pointer)
                with open(possible_pointer) as handle:
                    pointer = handle.read().strip()
                possible_cache = pathlib.Path(pointer)
                if possible_cache.exists():
                    logging.debug("cache is at %s", possible_cache)
                    return possible_cache
                logging.warning(
                    "Found a cache file at %s pointing to %s, but that "
                    "directory does not exist. Continuing search...",
                    possible_pointer, possible_cache)
        except PermissionError:
            pass

    logging.error(
        "Could not find a pointer to a litani cache. Did you forget "
        "to run `litani init`?")
    raise FileNotFoundError


def get_cache_dir(path=os.getcwd()):
    try:
        return _get_cache_dir(path)
    except FileNotFoundError:
        sys.exit(1)


def get_report_dir():
    return get_cache_dir() / "html"


def get_artifacts_dir():
    return get_cache_dir() / "artifacts"


def get_status_dir():
    return get_cache_dir() / "status"

@contextlib.contextmanager
def atomic_write(path, mode="w"):
    try:
        parent = pathlib.Path(path).parent
        parent.mkdir(exist_ok=True, parents=True)
        tmp = "%s~" % path
        handle = open(tmp, mode)
        yield handle
    except RuntimeError:
        try:
            os.unlink(tmp)
        except RuntimeError:
            pass
    else:
        handle.flush()
        handle.close()
        try:
            os.rename(tmp, path)
        except RuntimeError:
            os.unlink(tmp)
