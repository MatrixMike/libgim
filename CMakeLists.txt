cmake_minimum_required(VERSION 3.7.0)
project(util-cruft)

include(CheckFunctionExists)
include(CheckCXXCompilerFlag)

include (search_libs)

find_package (PythonInterp 3 REQUIRED)
find_package (RAGEL 6.9 REQUIRED)


###############################################################################
include (TestBigEndian)
TEST_BIG_ENDIAN(ENDIANNESS)
if (ENDIANNESS)
    add_definitions(-DWORDS_BIGENDIAN)
endif()


###############################################################################
RAGEL_TARGET(json-flat json/flat.cpp.rl ${CMAKE_CURRENT_BINARY_DIR}/json/flat.cpp COMPILE_FLAGS -G2)
RAGEL_TARGET(uri uri.cpp.rl ${CMAKE_CURRENT_BINARY_DIR}/uri.cpp COMPILE_FLAGS -G2)
RAGEL_TARGET(version version.cpp.rl ${CMAKE_CURRENT_BINARY_DIR}/version.cpp)
RAGEL_TARGET(format.cpp format.cpp.rl ${CMAKE_CURRENT_BINARY_DIR}/format.cpp)
RAGEL_TARGET(parse8601 time/parse8601.cpp.rl ${CMAKE_CURRENT_BINARY_DIR}/time/parse8601.cpp)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})


###############################################################################
set (UTIL_FILES)


###############################################################################
search_libs (BACKTRACE_LIB backtrace execinfo)
search_libs (DBGHELP_LIB SymFromAddr dbghelp)
check_function_exists (RtlCaptureStackBackTrace HAVE_CAPTURESTACKBACKTRACE)


##-----------------------------------------------------------------------------
if (BACKTRACE_LIB_FOUND)
    list (APPEND UTIL_FILES backtrace_execinfo.cpp)
elseif (HAVE_CAPTURESTACKBACKTRACE)
    list (APPEND UTIL_FILES backtrace_stackwalk.cpp)
else ()
    list (APPEND UTIL_FILES backtrace_null.cpp)
endif ()


##-----------------------------------------------------------------------------
if (NOT WIN32)
    find_program(ADDR2LINE NAMES addr2line)
    if (ADDR2LINE)
        add_definitions(-DADDR2LINE="${ADDR2LINE}")
    endif()
endif ()


###############################################################################
# Platform wrappers
if (LINUX)
    list (APPEND UTIL_FILES exe_linux.cpp)
elseif (FREEBSD)
    list (APPEND UTIL_FILES exe_freebsd.cpp)
elseif (WIN32)
    list (APPEND UTIL_FILES exe_win32.cpp)
else ()
    message (FATAL_ERROR "unhandled platform")
endif ()


###############################################################################
# platform wrappers
list (
    APPEND UTIL_FILES
    posix/dir.cpp
    posix/dir.hpp
    posix/dir.ipp
    posix/except.cpp
    posix/except.hpp
    posix/fd.cpp
    posix/fd.hpp
    posix/socket.cpp
    posix/socket.hpp
)


if (NOT WINDOWS)
    list (
        APPEND UTIL_FILES
        memory/buffer/circular.cpp
        memory/buffer/circular.hpp
        memory/buffer/paged.cpp
        memory/buffer/paged.hpp
        memory/system.cpp
        memory/system.hpp
        debug_posix.cpp
        io_posix.cpp
        io_posix.hpp
        io_posix.ipp
        library_posix.hpp
        library_posix.cpp
        posix/fwd.hpp
        posix/map.cpp
        posix/map.hpp
        time_posix.cpp
    )
endif ()


##-----------------------------------------------------------------------------
if (WINDOWS)
    list (
        APPEND UTIL_FILES
        debug_win32.cpp
        exe_win32.cpp
        io_win32.cpp
        io_win32.hpp
        io_win32.ipp
        library_win32.hpp
        library_win32.cpp
        time_win32.cpp
        win32/except.cpp
        win32/except.hpp
        win32/handle.cpp
        win32/handle.hpp
        win32/registry.hpp
        win32/registry.cpp
    )
endif ()


###############################################################################
# Common files
list (
    APPEND UTIL_FILES
    adapter.hpp
    adapter.cpp
    algo/sort.cpp
    algo/sort.hpp
    algo/sort.ipp
    alloc/fwd.hpp
    alloc/allocator.cpp
    alloc/allocator.hpp
    alloc/allocator.ipp
    alloc/arena.cpp
    alloc/arena.hpp
    alloc/arena.ipp
    alloc/raw/affix.cpp
    alloc/raw/affix.hpp
    alloc/raw/aligned.hpp
    alloc/raw/dynamic.hpp
    alloc/raw/fallback.cpp
    alloc/raw/fallback.hpp
    alloc/raw/linear.cpp
    alloc/raw/linear.hpp
    alloc/raw/malloc.cpp
    alloc/raw/malloc.hpp
    alloc/raw/null.cpp
    alloc/raw/null.hpp
    alloc/raw/stack.cpp
    alloc/raw/stack.hpp
    annotation.hpp
    ascii.hpp
    backtrace.hpp
    bezier.cpp
    bezier1.cpp
    bezier2.cpp
    bezier3.cpp
    bezier.hpp
    bitwise.cpp
    bitwise.hpp
    cast.hpp
    cmdopt.cpp
    cmdopt.hpp
    cmdopt.ipp
    colour.cpp
    colour.hpp
    colour.ipp
    coord/fwd.hpp
    coord/base.hpp
    coord.hpp
    coord/init.hpp
    coord/iostream.hpp
    coord/ops.hpp
    coord/store.hpp
    coord/traits.hpp
    debug.cpp
    debug.hpp
    debug.ipp
    encode/base.cpp
    encode/base.hpp
    endian.cpp
    endian.hpp
    exe.hpp
    extent.cpp
    extent.hpp
    extent.ipp
    fixed.cpp
    fixed.hpp
    float.cpp
    float.hpp
    ${CMAKE_CURRENT_BINARY_DIR}/format.cpp
    format.hpp
    fourcc.cpp
    fourcc.hpp
    geom/fwd.hpp
    geom/aabb.cpp
    geom/aabb.hpp
    geom/cylinder.cpp
    geom/cylinder.hpp
    geom/ellipse.cpp
    geom/ellipse.hpp
    geom/ellipse.ipp
    geom/iostream.cpp
    geom/iostream.hpp
    geom/ops.hpp
    geom/plane.cpp
    geom/plane.hpp
    geom/ray.cpp
    geom/ray.hpp
    geom/rect.cpp
    geom/rect.hpp
    geom/sample.hpp
    geom/sphere.cpp
    geom/sphere.hpp
    geom/tri.cpp
    geom/tri.hpp
    hash.hpp
    hash/fwd.hpp
    hash/adler.cpp
    hash/adler.hpp
    hash/bsdsum.cpp
    hash/bsdsum.hpp
    hash/crc.cpp
    hash/crc.hpp
    hash/fasthash.cpp
    hash/fasthash.hpp
    hash/fletcher.cpp
    hash/fletcher.hpp
    hash/fnv1a.cpp
    hash/fnv1a.hpp
    hash/murmur/common.cpp
    hash/murmur/common.hpp
    hash/murmur.hpp
    hash/murmur/murmur1.cpp
    hash/murmur/murmur1.hpp
    hash/murmur/murmur2.cpp
    hash/murmur/murmur2.hpp
    hash/murmur/murmur3.cpp
    hash/murmur/murmur3.hpp
    hash/wang.hpp
    hash/xxhash.cpp
    hash/xxhash.hpp
    introspection.cpp
    introspection.hpp
    io.cpp
    io.hpp
    io.ipp
    iterator.hpp
    job/queue.cpp
    job/queue.hpp
    json/fwd.hpp
    json/except.cpp
    json/except.hpp
    ${CMAKE_CURRENT_BINARY_DIR}/json/flat.cpp
    json/flat.hpp
    json/schema.cpp
    json/schema.hpp
    json/tree.cpp
    json/tree.hpp
    json2/fwd.hpp
    json2/event.hpp
    json2/event.cpp
    json2/except.hpp
    json2/personality/base.cpp
    json2/personality/base.hpp
    json2/personality/jsonish.cpp
    json2/personality/jsonish.hpp
    json2/personality/rfc7519.cpp
    json2/personality/rfc7519.hpp
    json2/tree.cpp
    json2/tree.hpp
    library.hpp
    log.cpp
    log.hpp
    log.ipp
    maths.cpp
    maths.hpp
    matrix.cpp
    matrix2.cpp
    matrix3.cpp
    matrix4.cpp
    matrix.hpp
    memory/deleter.cpp
    memory/deleter.hpp
    nocopy.hpp
    parse.cpp
    parse.hpp
    pascal.cpp
    pascal.hpp
    platform.hpp
    point.cpp
    point.hpp
    point.ipp
    pointer.hpp
    polynomial.cpp
    polynomial.hpp
    polynomial.ipp
    pool.cpp
    pool.hpp
    pool.ipp
    preprocessor.hpp
    quaternion.cpp
    quaternion.hpp
    quaternion.ipp
    raii.hpp
    rand/lcg.cpp
    rand/lcg.hpp
    rand/xorshift.cpp
    rand/xorshift.hpp
    rand/mwc64x.cpp
    rand/mwc64x.hpp
    random.cpp
    random.hpp
    range.cpp
    range.hpp
    range.ipp
    rational.cpp
    rational.hpp
    rational.ipp
    region.cpp
    region.hpp
    region.ipp
    roots/bisection.hpp
    sarray.cpp
    sarray.hpp
    si.cpp
    signal.cpp
    signal.hpp
    signal.ipp
    si.hpp
    stats.cpp
    stats.hpp
    stream.cpp
    stream.hpp
    string.cpp
    string.hpp
    stringid.cpp
    stringid.hpp
    strongdef.cpp
    strongdef.hpp
    tap.cpp
    tap.hpp
    term.cpp
    term.hpp
    time.cpp
    time.hpp
    time/parse.hpp
    time/parse8601.cpp
    tuple.cpp
    tuple.hpp
    typeidx.cpp
    typeidx.hpp
    types/bits.hpp
    types/comparator.hpp
    types.hpp
    types/string.cpp
    types/string.hpp
    types/traits.hpp
    uri.cpp
    uri.hpp
    utf8.cpp
    utf8.hpp
    variadic.cpp
    variadic.hpp
    vector.cpp
    vector.hpp
    vector.ipp
    version.cpp
    version.hpp
    view.cpp
    view.hpp
)


##-----------------------------------------------------------------------------
## We shouldn't be building into the source directory, but I can't stand trying
## to coax CMake into behaving here any longer. Feel free to fix it.
add_custom_command (
OUTPUT
    "${CMAKE_CURRENT_SOURCE_DIR}/preprocessor.hpp"
COMMENT
    "[preprocessor.py] preprocessor.hpp"
COMMAND
    "${PYTHON_EXECUTABLE}"
    "${CMAKE_CURRENT_SOURCE_DIR}/preprocessor.py"
    "${CMAKE_CURRENT_SOURCE_DIR}/preprocessor.hpp"
    320
DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/preprocessor.py"
)


###############################################################################
add_library(cruft-util ${UTIL_FILES})
target_link_libraries (cruft-util PUBLIC ${LIBS})
target_include_directories(cruft-util PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}")

###############################################################################
# platform libraries
search_libs (SHM_LIBS shm_open rt)
search_libs (DL_LIBS dlopen dl)
search_libs (CLOCK_LIBS clock_gettime rt c)
search_libs (MATH_LIBS cos m)

target_link_libraries(cruft-util PUBLIC ${SHM_LIBS})
target_link_libraries(cruft-util PUBLIC ${DL_LIBS})
target_link_libraries(cruft-util PUBLIC ${CLOCK_LIBS})
target_link_libraries(cruft-util PUBLIC ${MATH_LIBS})


###############################################################################
foreach (tool json-clean json-schema json-validate scratch)
    add_executable (util_${tool} tools/${tool}.cpp)
    set_target_properties (util_${tool} PROPERTIES OUTPUT_NAME ${tool})
    target_link_libraries (util_${tool} cruft-util)
    target_include_directories(util_${tool} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
endforeach ()


###############################################################################
option (TESTS "enable unit testing" ON)

if (TESTS)
    include(CTest)
    enable_testing()

    list (
        APPEND TEST_BIN
        ascii
        algo/sort
        alloc/aligned
        alloc/arena
        alloc/dynamic
        alloc/linear
        alloc/stack
        affine
        backtrace
        bezier
        bitwise
        cmdopt
        colour
        comparator
        coord
        encode/base
        endian
        exe
        extent
        fixed
        float
        format
        geom/aabb
        geom/ray
        hash/checksum
        hash/crc
        hash/fasthash
        hash/fnv1a
        hash/murmur
        hash/xxhash
        hton
        introspection
        iterator
        job/queue
        json_types
        json2/event
        maths
        matrix
        memory/deleter
        parse
        point
        polynomial
        pool
        preprocessor
        quaternion
        rand/buckets
        range
        rational
        region
        roots/bisection
        sarray
        signal
        stream
        string
        stringid
        strongdef
        time/8601
        traits
        tuple
        typeidx
        uri
        utf8
        vector
        version
        view
    )

    foreach(t ${TEST_BIN})
        string(REPLACE "/" "_" name "test/${t}")
        add_executable(util_${name} test/${t}.cpp)
        target_link_libraries(util_${name} PRIVATE cruft-util)
        target_include_directories(util_${name} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
        add_test(NAME util_${name} COMMAND util_${name})
    endforeach(t)
endif ()


###############################################################################
get_directory_property (HAS_PARENT PARENT_DIRECTORY)
if (HAS_PARENT)
    set (CRUFT_UTIL_FOUND 1 PARENT_SCOPE)
endif ()


##-----------------------------------------------------------------------------
configure_file(libcruft-util-system.pc.in libcruft-util.pc)
configure_file(Doxyfile.in Doxyfile)
