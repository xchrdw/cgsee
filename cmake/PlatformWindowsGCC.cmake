
# support for C++11 etc.

execute_process(COMMAND ${CMAKE_C_COMPILER} -dumpversion
	OUTPUT_VARIABLE GCC_VERSION)

if(GCC_VERSION VERSION_GREATER 4.7 OR GCC_VERSION VERSION_EQUAL 4.7)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11")
elseif(GCC_VERSION VERSION_EQUAL 4.6)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++0x")
else()
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
endif()


set(MINWG_COMPILE_DEFS
    WIN32                   	# Windows system
    UNICODE                 	# Use unicode
    _UNICODE                	# Use unicode
    _CRT_SECURE_NO_DEPRECATE	# Disable CRT deprecation warnings
	PIC 						# Position-independent code
	_REENTRANT					# Reentrant code
)
set(DEFAULT_COMPILE_DEFS_DEBUG
    ${MINWG_COMPILE_DEFS}
    _DEBUG						# Debug build
)
set(DEFAULT_COMPILE_DEFS_RELEASE
    ${MINWG_COMPILE_DEFS}
    NDEBUG						# Release build
)

set(MINGW_COMPILE_FLAGS
	"-pthread -fno-rtti -fno-exceptions -pipe -fPIC"
	# pthread       -> use pthread library
	# no-rtti       -> disable c++ rtti
	# no-exceptions -> disable exception handling
	# pipe          -> use pipes
	# fPIC          -> use position independent code
)

set(DEFAULT_COMPILE_FLAGS ${MINGW_COMPILE_FLAGS})


# Add platform specific libraries for linking
set(EXTRA_LIBS "")
