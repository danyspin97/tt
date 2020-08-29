#!/bin/sh

GCOV_DIR="${MESON_SOURCE_ROOT}/gcov"
mkdir "${GCOV_DIR}"

cd "${MESON_BUILD_ROOT}"
gcov-9 -p src/*/*.gcno > /dev/null
gcov-9 -p test/*/*.gcno > /dev/null
gcov-9 -p test/svc/*.gcno > /dev/null
mv *.gcov "${GCOV_DIR}"
