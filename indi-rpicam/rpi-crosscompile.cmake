#
# Crosscompilation file for cross-compiling indi-rpicam for Raspberry PI
#
set(CMAKE_SYSTEM_NAME Linux)

set(CMAKE_SYSTEM_PROCESSOR armhf)

set(CMAKE_STAGING_PREFIX /tmp/indi-cross)

set(CMAKE_FIND_ROOT_PATH /usr/arm-linux-gnueabihf/)

set(CMAKE_LIBRARY_ARCHITECTURE arm-linux-gnueabihf)

set(CMAKE_INCLUDE_PATH ${CMAKE_STAGING_PREFIX}/usr/include)

set(CMAKE_LIBRARY_PATH ${CMAKE_STAGING_PREFIX}/usr/lib/${CMAKE_LIBRARY_ARCHITECTURE})

set(CMAKE_C_COMPILER /usr/bin/arm-linux-gnueabihf-gcc)
set(CMAKE_CXX_COMPILER /usr/bin/arm-linux-gnueabihf-g++)
string(APPEND CMAKE_CXX_FLAGS " -Wl,-rpath-link=${CMAKE_STAGING_PREFIX}/lib/arm-linux-gnueabihf")
string(APPEND CMAKE_CXX_FLAGS " -Wl,-rpath-link=${CMAKE_STAGING_PREFIX}/usr/lib/arm-linux-gnueabihf")
string(APPEND CMAKE_CXX_FLAGS " -L${CMAKE_STAGING_PREFIX}/lib/arm-linux-gnueabihf")
string(APPEND CMAKE_CXX_FLAGS " -L${CMAKE_STAGING_PREFIX}/usr/lib/arm-linux-gnueabihf")
string(APPEND CMAKE_CXX_FLAGS " -L${CMAKE_STAGING_PREFIX}/opt/vc/lib")

set(m_LIBRARY "/usr/arm-linux-gnueabihf/lib/libm.so")

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY BOTH)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE BOTH)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)
