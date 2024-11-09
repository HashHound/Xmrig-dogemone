cmake_minimum_required(VERSION 3.0)
project(xmrig C)

option(WITH_LIBCPUID "Use Libcpuid" ON)
option(WITH_AEON "CryptoNight-Lite support" OFF)

set(HEADERS ...)
set(HEADERS_CRYPTO ...)
set(HEADERS_COMPAT ...)
set(HEADERS_UTILS ...)
set(SOURCES ...)
set(SOURCES_CRYPTO ...)
set(SOURCES_UTILS ...)

# OS-specific settings
if (WIN32)
    set(SOURCES_OS win/cpu_win.c win/memory_win.c win/xmrig_win.c win/app.rc compat/winansi.c)
    set(EXTRA_LIBS ws2_32)
    add_definitions(/D_WIN32_WINNT=0x600)
else()
    set(SOURCES_OS unix/cpu_unix.c unix/memory_unix.c unix/xmrig_unix.c)
    set(EXTRA_LIBS pthread)
endif()

include_directories(.)
add_definitions(/DUSE_NATIVE_THREADS /D_GNU_SOURCE /DUNICODE)

# Set compile flags
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -maes -march=native -Wno-pointer-to-int-cast")
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -Ofast -march=native -ffast-math -funroll-loops -fvariable-expansion-in-unroller -ftree-loop-if-convert-stores -fmerge-all-constants -fbranch-target-load-optimize2")

# Static linking on Windows
if (WIN32)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static")
endif()

# Include directories
include_directories(compat/jansson)
add_subdirectory(compat/jansson)

# Dependencies
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")
find_package(CURL REQUIRED)
find_package(ZLIB REQUIRED)
find_package(Brotli REQUIRED)
find_package(Zstd REQUIRED)
find_package(NGHTTP2 REQUIRED)

# Include directories for dependencies
include_directories(${CURL_INCLUDE_DIRS} ${ZLIB_INCLUDE_DIRS} ${Brotli_INCLUDE_DIRS} ${Zstd_INCLUDE_DIRS} ${NGHTTP2_INCLUDE_DIRS})

# Definitions
add_definitions(/DCURL_STATICLIB)

# Link directories
link_directories(${CURL_LIBRARIES} ${ZLIB_LIBRARIES} ${Brotli_LIBRARIES} ${Zstd_LIBRARIES} ${NGHTTP2_LIBRARIES})

# Enable Libcpuid support if specified
if (WITH_LIBCPUID)
    add_subdirectory(compat/libcpuid)
    include_directories(compat/libcpuid)
    set(CPUID_LIB cpuid)
    set(SOURCES_CPUID cpu.c)
else()
    add_definitions(/DXMRIG_NO_LIBCPUID)
    set(SOURCES_CPUID cpu_stub.c)
endif()

# Enable AEON support if specified
if (WITH_AEON)
    set(SOURCES_AEON ...)
else()
    add_definitions(/DXMRIG_NO_AEON)
endif()

# Define executable
if (CMAKE_SIZEOF_VOID_P EQUAL 8)
    add_executable(xmrig ${HEADERS} ${HEADERS_CRYPTO} ${SOURCES} ${SOURCES_CRYPTO} ${HEADERS_UTILS} ${SOURCES_UTILS} ${HEADERS_COMPAT} ${SOURCES_COMPAT} ${SOURCES_OS} ${SOURCES_CPUID} ${SOURCES_AEON})
    target_link_libraries(xmrig jansson curl z brotlidec zstd nghttp2 ${CPUID_LIB} ${EXTRA_LIBS})
else()
    add_executable(xmrig32 ${HEADERS} ${HEADERS_CRYPTO} ${SOURCES} ${SOURCES_CRYPTO} ${HEADERS_UTILS} ${SOURCES_UTILS} ${HEADERS_COMPAT} ${SOURCES_COMPAT} ${SOURCES_OS} ${SOURCES_CPUID} ${SOURCES_AEON})
    target_link_libraries(xmrig32 jansson curl z brotlidec zstd nghttp2 ${CPUID_LIB} ${EXTRA_LIBS})
endif()

source_group("HEADERS" FILES ${HEADERS})
