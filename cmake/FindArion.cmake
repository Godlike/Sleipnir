# Copyright (C) 2017 by Godlike
# This code is licensed under the MIT license (MIT)
# (http://opensource.org/licenses/MIT)
#
# FindArion
# --------
#
# Find Arion config directory.
#
# This module sets the following result variables:
#   Arion_CONFIG_DIR    = directory that contains ArionConfig.cmake file
#   Arion_FOUND         = TRUE if Arion config was found
#

include(FindPackageHandleStandardArgs)

find_path( Arion_CONFIG_DIR
    NAMES ArionConfig.cmake
    PATHS
        ${ARION_ROOT}/cmake
    DOC "The directory that contains ArionConfig.cmake file")

find_package_handle_standard_args(Arion DEFAULT_MSG Arion_CONFIG_DIR)

mark_as_advanced(Arion_CONFIG_DIR)
