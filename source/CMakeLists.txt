
# Generate meta-header
configure_file(cgsee-version.h.in ${CMAKE_CURRENT_BINARY_DIR}/cgsee-version.h)

# We do NOT want to deploy this header (at least not for binary packages)
#install(FILES ${CMAKE_CURRENT_BINARY_DIR}/cgsee/cgsee-version.h DESTINATION ${INSTALL_INCLUDE}/cgsee)

# Include directories
include_directories(
    ${CMAKE_CURRENT_BINARY_DIR}
)

# Libraries
set(IDE_FOLDER "")
add_subdirectory(core)
add_subdirectory(gui)

# Applications
set(IDE_FOLDER "Applications")
add_subdirectory(cgsee)

# Tests
set(IDE_FOLDER "Tests")
add_subdirectory(tests)
