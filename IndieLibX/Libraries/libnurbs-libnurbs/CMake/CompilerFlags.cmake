INCLUDE(CheckCCompilerFlag)
INCLUDE(CheckCXXCompilerFlag)

MACRO(CHECK_C_FLAG flag)
	STRING(TOUPPER ${flag} UPPER_FLAG)
	STRING(REGEX REPLACE " " "_" UPPER_FLAG ${UPPER_FLAG})
	STRING(REGEX REPLACE "=" "_" UPPER_FLAG ${UPPER_FLAG})
	IF(${ARGC} LESS 2)
		CHECK_C_COMPILER_FLAG(-${flag} ${UPPER_FLAG}_COMPILER_FLAG)
		IF(${UPPER_FLAG}_COMPILER_FLAG)
			SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -${flag}")
		ENDIF(${UPPER_FLAG}_COMPILER_FLAG)
	ELSE(${ARGC} LESS 2)
		IF(NOT ${ARGV1})
			CHECK_C_COMPILER_FLAG(-${flag} ${UPPER_FLAG}_COMPILER_FLAG)
			IF(${UPPER_FLAG}_COMPILER_FLAG)
				MESSAGE("Found ${ARGV1} - setting to -${flag}")
				SET(${ARGV1} "-${flag}" CACHE STRING "${ARGV1}" FORCE)
			ENDIF(${UPPER_FLAG}_COMPILER_FLAG)
		ENDIF(NOT ${ARGV1})
	ENDIF(${ARGC} LESS 2)
	IF(${UPPER_FLAG}_COMPILER_FLAG)
		SET(${UPPER_FLAG}_COMPILER_FLAG "-${flag}")
	ENDIF(${UPPER_FLAG}_COMPILER_FLAG)
ENDMACRO()

MACRO(CHECK_CXX_FLAG flag)
	STRING(TOUPPER ${flag} UPPER_FLAG)
	STRING(REGEX REPLACE " " "_" UPPER_FLAG ${UPPER_FLAG})
	STRING(REGEX REPLACE "=" "_" UPPER_FLAG ${UPPER_FLAG})
	IF(${ARGC} LESS 2)
		CHECK_CXX_COMPILER_FLAG(-${flag} ${UPPER_FLAG}_COMPILER_FLAG)
		IF(${UPPER_FLAG}_COMPILER_FLAG)
			SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -${flag}")
		ENDIF(${UPPER_FLAG}_COMPILER_FLAG)
	ELSE(${ARGC} LESS 2)
		IF(NOT ${ARGV1})
			CHECK_CXX_COMPILER_FLAG(-${flag} ${UPPER_FLAG}_COMPILER_FLAG)
			IF(${UPPER_FLAG}_COMPILER_FLAG)
				MESSAGE("Found ${ARGV1} - setting to -${flag}")
				SET(${ARGV1} "-${flag}" CACHE STRING "${ARGV1}" FORCE)
			ENDIF(${UPPER_FLAG}_COMPILER_FLAG)
		ENDIF(NOT ${ARGV1})
	ENDIF(${ARGC} LESS 2)
	IF(${UPPER_FLAG}_COMPILER_FLAG)
		SET(${UPPER_FLAG}_COMPILER_FLAG "-${flag}")
	ENDIF(${UPPER_FLAG}_COMPILER_FLAG)
ENDMACRO()


MACRO(CHECK_C_FLAG_GATHER flag FLAGS)
	STRING(TOUPPER ${flag} UPPER_FLAG)
	STRING(REGEX REPLACE " " "_" UPPER_FLAG ${UPPER_FLAG})
	CHECK_C_COMPILER_FLAG(-${flag} ${UPPER_FLAG}_COMPILER_FLAG)
	IF(${UPPER_FLAG}_COMPILER_FLAG)
		SET(${FLAGS} "${${FLAGS}} -${flag}")
	ENDIF(${UPPER_FLAG}_COMPILER_FLAG)
ENDMACRO()

SET(CMAKE_C_FLAGS "")
SET(CMAKE_CXX_FLAGS "")
# try to use -pipe to speed up the compiles
CHECK_C_FLAG(pipe)
CHECK_CXX_FLAG(pipe)

# check for -fno-strict-aliasing
CHECK_C_FLAG(fno-strict-aliasing)
CHECK_CXX_FLAG(fno-strict-aliasing)

# check for -fexceptions
CHECK_C_FLAG(fexceptions)
CHECK_CXX_FLAG(fexceptions)

# check for -ftemplate-depth-NN this is needed in code using 
# boost where the template instantiation depth needs to be 
# increased from the default ANSI minimum of 17.
CHECK_CXX_FLAG(ftemplate-depth-50)

# dynamic SSE optimizations for NURBS processing
CHECK_C_FLAG(msse)
CHECK_C_FLAG(msse2)

# Check for gnu c99 support
CHECK_C_FLAG("std=gnu99" C99_FLAG)
MARK_AS_ADVANCED(C99_FLAG)

# Debugging flags
IF(APPLE)
	EXEC_PROGRAM(sw_vers ARGS -productVersion OUTPUT_VARIABLE MACOSX_VERSION)
	IF(${MACOSX_VERSION} VERSION_LESS "10.5")
		CHECK_C_FLAG(ggdb3 DEBUG_FLAG)
	ELSE(${MACOSX_VERSION} VERSION_LESS "10.5")
		#CHECK_C_COMPILER_FLAG silently eats gstabs+
		SET(DEBUG_FLAG "-gstabs+")
	ENDIF(${MACOSX_VERSION} VERSION_LESS "10.5")
ELSE(APPLE)
	CHECK_C_FLAG(ggdb3 DEBUG_FLAG)
ENDIF(APPLE)
CHECK_C_FLAG(g DEBUG_FLAG)
CHECK_C_FLAG(debug DEBUG_FLAG)
# add -D_FORTIFY_SOURCE=2 to flags. provides compile-time
# best-practice error checking on certain libc functions
# (e.g., memcpy), and provides run-time checks on buffer
# lengths and memory regions.
CHECK_C_FLAG_GATHER("D_FORTIFY_SOURCE=2" DEBUG_FLAG)
SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} ${DEBUG_FLAG}")
SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} ${DEBUG_FLAG}")
MARK_AS_ADVANCED(DEBUG_FLAG)

# -fast provokes a stack corruption in the shadow computations because
# of strict aliasing getting enabled. 
IF(ENABLE_OPTIMIZED_BUILD)
	CHECK_C_FLAG_GATHER(O3 OPTIMIZE_FLAGS)
	CHECK_C_FLAG_GATHER(ffast-math OPTIMIZE_FLAGS)
	CHECK_C_FLAG_GATHER(fstrength-reduce OPTIMIZE_FLAGS)
	CHECK_C_FLAG_GATHER(fexpensive-optimizations OPTIMIZE_FLAGS)
	CHECK_C_FLAG_GATHER(finline-functions OPTIMIZE_FLAGS)
	CHECK_C_FLAG_GATHER("finline-limit=10000" OPTIMIZE_FLAGS)
	IF(NOT ${CMAKE_BUILD_TYPE} MATCHES "^Debug$" AND NOT ENABLE_DEBUG)
		CHECK_C_FLAG_GATHER(fomit-frame-pointer OPTIMIZE_FLAGS)
	ELSE(NOT ${CMAKE_BUILD_TYPE} MATCHES "^Debug$" AND NOT ENABLE_DEBUG)
		CHECK_C_FLAG_GATHER(fno-omit-frame-pointer OPTIMIZE_FLAGS)
	ENDIF(NOT ${CMAKE_BUILD_TYPE} MATCHES "^Debug$" AND NOT ENABLE_DEBUG)
	SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OPTIMIZE_FLAGS}")
	SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OPTIMIZE_FLAGS}")
ENDIF(ENABLE_OPTIMIZED_BUILD)
MARK_AS_ADVANCED(OPTIMIZE_FLAGS)

# verbose warning flags
IF(ENABLE_COMPILER_WARNINGS OR ENABLE_STRICT_COMPILE)
	# also of interest:
	# -Wunreachable-code -Wmissing-declarations -Wmissing-prototypes -Wstrict-prototypes -ansi
	# -Wformat=2 (after bu_fopen_uniq() is obsolete)
	CHECK_C_FLAG_GATHER(pedantic WARNING_FLAGS)
	# The Wall warnings are too verbose with Visual C++
	IF(NOT MSVC)
		CHECK_C_FLAG_GATHER(Wall WARNING_FLAGS)
	ELSE(NOT MSVC)
		CHECK_C_FLAG_GATHER(W4 WARNING_FLAGS)
	ENDIF(NOT MSVC)
	CHECK_C_FLAG_GATHER(Wextra WARNING_FLAGS)
	CHECK_C_FLAG_GATHER(Wundef WARNING_FLAGS)
	CHECK_C_FLAG_GATHER(Wfloat-equal WARNING_FLAGS)
	CHECK_C_FLAG_GATHER(Wshadow WARNING_FLAGS)
	CHECK_C_FLAG_GATHER(Winline WARNING_FLAGS)
	# Need this for tcl.h
	CHECK_C_FLAG_GATHER(Wno-long-long WARNING_FLAGS) 
	SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${WARNING_FLAGS}")
	SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${WARNING_FLAGS}")
ENDIF(ENABLE_COMPILER_WARNINGS OR ENABLE_STRICT_COMPILE)
MARK_AS_ADVANCED(WARNING_FLAGS)

IF(ENABLE_STRICT_COMPILE)
	CHECK_C_FLAG_GATHER(Werror STRICT_FLAGS) 
	SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${STRICT_FLAGS}")
	SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${STRICT_FLAGS}")
ENDIF(ENABLE_STRICT_COMPILE)
MARK_AS_ADVANCED(STRICT_FLAGS)
