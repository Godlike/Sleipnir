# Copyright (C) 2017 by Godlike
# This code is licensed under the MIT license (MIT)
# (http://opensource.org/licenses/MIT)
#
# FindEpona
# --------
#
# Find Epona config directory.
#
# This module sets the following result variables:
#   Epona_CONFIG_DIR    = directory that contains EponaConfig.cmake file
#   Epona_FOUND         = TRUE if Epona config was found
#

include(FindPackageHandleStandardArgs)

find_path( Epona_CONFIG_DIR
    NAMES EponaConfig.cmake
    PATHS
        ${EPONA_ROOT}/cmake
    DOC "The directory that contains EponaConfig.cmake file")

find_package_handle_standard_args(Epona DEFAULT_MSG Epona_CONFIG_DIR)

mark_as_advanced(Epona_CONFIG_DIR)
