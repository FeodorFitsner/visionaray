# This file is distributed under the MIT license.
# See the LICENSE file for details.

add_subdirectory(common)

if(VSNRAY_ENABLE_EXAMPLES)
add_subdirectory(examples)
endif()

if(VSNRAY_ENABLE_VIEWER)
add_subdirectory(viewer)
endif()

add_subdirectory(visionaray)
