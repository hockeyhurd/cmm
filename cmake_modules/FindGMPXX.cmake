# Try to find the GMPXX library
# https://gmplib.org/
#
# This module supports requiring a minimum version, e.g. you can do
#   find_package(GMPXX 6.2.1)
# to require version 6.2.1 to newer of GMP.
#
# Once done this will define
#
#  GMPXX_FOUND - system has GMPXX lib with correct version
#  GMPXX_INCLUDE_DIRS - the GMPXX include directory
#  GMPXX_LIBRARIES - the GMPXX library
#

# Set GMPXX_FIND_VERSION to 6.0.0 if no minimum version is specified
if (NOT GMPXX_FIND_VERSION)
  if (NOT GMPXX_FIND_VERSION_MAJOR)
    set(GMPXX_FIND_VERSION_MAJOR 6)
  endif ()
  if (NOT GMPXX_FIND_VERSION_MINOR)
    set(GMPXX_FIND_VERSION_MINOR 0)
  endif ()
  if (NOT GMPXX_FIND_VERSION_PATCH)
    set(GMPXX_FIND_VERSION_PATCH 0)
  endif ()
  set(GMPXX_FIND_VERSION
    "${GMPXX_FIND_VERSION_MAJOR}.${GMPXX_FIND_VERSION_MINOR}.${GMPXX_FIND_VERSION_PATCH}")
endif ()

find_path(GMPXX_INCLUDE_DIRS
          NAMES gmp.h gmpxx.h
          PATHS $ENV{GMPXXDIR} ${INCLUDE_INSTALL_DIR})

find_library(GMPXX_LIBRARIES
             gmp gmpxx
             PATHS $ENV{GMPXXDIR} ${LIB_INSTALL_DIR})

if (GMPXX_INCLUDE_DIRS AND GMPXX_LIBRARIES)

  # This program will fail to compile if GMPXX is too old.
  # We prefer to perform this "test" at compile-time to
  # avoid problems with e.g. try_run() during cross-compilation.
  file(WRITE ${PROJECT_BINARY_DIR}/gmpxx_version_check.cpp ""
  "#include <gmp.h>\n"
  "#include <gmpxx.h>\n"
  "\n"
  "#define GMPXX_FIND_VERSION_MAJOR ${GMPXX_FIND_VERSION_MAJOR}\n"
  "#define GMPXX_FIND_VERSION_MINOR ${GMPXX_FIND_VERSION_MINOR}\n"
  "#define GMPXX_FIND_VERSION_PATCH ${GMPXX_FIND_VERSION_PATCH}\n"
  "\n"
  "#if __GNU_MP_VERSION < GMPXX_FIND_VERSION_MAJOR\n"
  "#error insufficient GMPXX major version\n"
  "#elif __GNU_MP_VERSION == GMPXX_FIND_VERSION_MAJOR\n"
  "#if __GNU_MP_VERSION_MINOR < GMPXX_FIND_VERSION_MINOR\n"
  "#error insufficient GMPXX minor version\n"
  "#elif __GNU_MP_VERSION_MINOR == GMPXX_FIND_VERSION_MINOR\n"
  "#if __GNU_MP_VERSION_PATCHLEVEL < GMPXX_FIND_VERSION_PATCH\n"
  "#error insufficient GMPXX patch version\n"
  "#endif\n"
  "#endif\n"
  "#endif\n"
  "\n"
  "int main(int argc, char* argv[]) { return 0; }\n")

  # Try to compile the test program above with the appropriate version
  # strings substituted in.
  try_compile(GMPXX_VERSION_OK
          "${PROJECT_BINARY_DIR}"
          "${PROJECT_BINARY_DIR}/gmpxx_version_check.cpp"
          CMAKE_FLAGS "-DINCLUDE_DIRECTORIES=${GMPXX_INCLUDE_DIRS}")
endif ()

if (NOT GMPXX_VERSION_OK)
  message(STATUS "No sufficient GMPXX version detected")
endif ()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(GMPXX DEFAULT_MSG
                                  GMPXX_INCLUDE_DIRS GMPXX_LIBRARIES GMPXX_VERSION_OK)
mark_as_advanced(GMPXX_INCLUDE_DIRS GMPXX_LIBRARIES)

