
# ASSIMP_FOUND
# ASSIMP_INCLUDE_PATH
# ASSIMP_LIBRARY
# ASSIMP_BINARY

FIND_PATH(ASSIMP_INCLUDE_PATH assimp/Importer.hpp
    $ENV{ASSIMPDIR}/include
    $ENV{ASSIMP_HOME}/include
    $ENV{PROGRAMFILES}/ASSIMP/include
    /usr/include
    /usr/local/include
    /sw/include
    /opt/local/include
    DOC "The directory where assimp/Importer.hpp etc. resides")


FIND_LIBRARY(ASSIMP_LIBRARY
    NAMES assimp
    PATHS
    $ENV{ASSIMPDIR}/lib
    $ENV{ASSIMPDIR}/lib/.libs
    $ENV{ASSIMP_HOME}/lib
    $ENV{ASSIMP_HOME}/lib/.libs
    $ENV{ASSIMPDIR}
    $ENV{ASSIMP_HOME}
    /usr/lib64
    /usr/local/lib64
    /sw/lib64
    /opt/local/lib64
    /usr/lib
    /usr/local/lib
    /sw/lib
    /opt/local/lib
    DOC "The Assimp library")

if(WIN32)

    find_file(ASSIMP_BINARY
        NAMES assimp.dll
        PATHS
        $ENV{ASSIMPDIR}/bin
        $ENV{ASSIMP_HOME}/bin
        DOC "The Assimp binary")

endif()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(ASSIMP REQUIRED_VARS ASSIMP_INCLUDE_DIR ASSIMP_LIBRARY)
mark_as_advanced(ASSIMP_INCLUDE_DIR ASSIMP_LIBRARY)
