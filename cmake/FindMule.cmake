# Copyright (C) 2018 by Godlike
# This code is licensed under the MIT license (MIT)
# (http://opensource.org/licenses/MIT)
#
# FindMule
# --------
#
# Find Mule config directory.
#
# This module sets the following result variables:
#   Mule_CONFIG_DIR    = directory that contains MuleUtilitiesConfig.cmake file
#   Mule_FOUND         = TRUE if Mule config was found
#

include(FindPackageHandleStandardArgs)

find_path( Mule_CONFIG_DIR
    NAMES MuleUtilitiesConfig.cmake
    PATHS
        ${MULE_ROOT}/cmake
    DOC "The directory that contains MuleUtilitiesConfig.cmake file")

find_package_handle_standard_args(Mule DEFAULT_MSG Mule_CONFIG_DIR)

mark_as_advanced(Mule_CONFIG_DIR)
