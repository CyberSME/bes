AC_PREREQ([2.60])
AC_INIT([Snes9x], [1.53], [], [snes9x])
AC_REVISION([$Revision: 1.53 $])

AC_CONFIG_SRCDIR([sdlmain.cpp])

AC_CANONICAL_TARGET

AC_PROG_CC
AC_PROG_CXX
AC_LANG([C++])

S9XFLGS=""
S9XDEFS="-DBUILD_SNES"
S9XLIBS=""

AC_DEFUN([AC_S9X_COMPILER_FLAG],
[
	AC_MSG_CHECKING([whether g++ accepts $1])

	AC_CACHE_VAL([snes9x_cv_option_$2],
	[
		OLD_CXXFLAGS="[$]CXXFLAGS"
		CXXFLAGS="[$]OLD_CXXFLAGS $1"

		AC_TRY_RUN(
		[
			int	foo;

			int	main (int argc, char **argv)
			{
				/* The following code triggs gcc:s generation of aline opcodes,
				   which some versions of as does not support. */

				if (argc > 0)
					argc = 0;

				return (argc);
			}
		],
		[snes9x_cv_option_$2="yes"], [snes9x_cv_option_$2="no"])
	])

	CXXFLAGS="[$]OLD_CXXFLAGS"

	if test "x[$]snes9x_cv_option_$2" = "xyes"; then
		S9XFLGS="[$]S9XFLGS $1"
		AC_MSG_RESULT([yes])
	else
		AC_MSG_RESULT([no])
		$3
	fi
])

# *****************************
# *** Execution begins here ***
# *****************************

# Remove -g and -O2 flags manually.

if test "x$CFLAGS" != "x"; then
	CFLAGS="`echo \"$CFLAGS\" | sed -e 's/-g//'`"
	CFLAGS="`echo \"$CFLAGS\" | sed -e 's/-O2//'`"
fi

if test "x$CXXFLAGS" != "x"; then
	CXXFLAGS="`echo \"$CXXFLAGS\" | sed -e 's/-g//'`"
	CXXFLAGS="`echo \"$CXXFLAGS\" | sed -e 's/-O2//'`"
fi

# Test what compiler flags we should use.

AC_ARG_ENABLE([debug],
	[AS_HELP_STRING([--enable-debug],
		[leave debug information in the final binary (default: no)])],
	[], [enable_debug="no"])

if test "x$enable_debug" = "xyes"; then
	AC_S9X_COMPILER_FLAG([-g],  [g])
	AC_S9X_COMPILER_FLAG([-O0], [o0])
else
	AC_S9X_COMPILER_FLAG([-O3], [o3], [
		AC_S9X_COMPILER_FLAG([-O2], [o2], [
			AC_S9X_COMPILER_FLAG([-O1], [o1])])])
	AC_S9X_COMPILER_FLAG([-fomit-frame-pointer], [omit_frame_pointer])
fi

# <domaemon>: --enable-neon

AC_ARG_ENABLE([neon],
	[AS_HELP_STRING([--enable-neon],
		[use the optimisation flag to enable NEON instructions (default: no)])],
	[], [enable_neon="no"])

if test "x$enable_neon" != "xno"; then
	AC_S9X_COMPILER_FLAG([-mfpu=neon], [mfpu],
	[
		AC_MSG_ERROR(["NEON optimisation flag was not accepted."])
	])
# BeagleSNES - Changed "armv7a" to "arm7-a"
	AC_S9X_COMPILER_FLAG([-Wa,-march=arm7-a],		[march])
	AC_S9X_COMPILER_FLAG([-fprefetch-loop-arrays],	[prefetch])
# BeagleSNES - AWH: BeagleBoard/Bone-specific flags
	AC_S9X_COMPILER_FLAG([-mtune=cortex-a8], [mtune])
	S9XFLGS="$S9XFLGS -mfloat-abi=hard -fsigned-char -I./OGLES"
# BeagleSNES
fi

# </domaemon>

AC_S9X_COMPILER_FLAG([-fno-exceptions],	      [no_exceptions])
AC_S9X_COMPILER_FLAG([-fno-rtti],             [no_rtti])
AC_S9X_COMPILER_FLAG([-pedantic],             [pedantic])
AC_S9X_COMPILER_FLAG([-Wall],                 [Wall])
AC_S9X_COMPILER_FLAG([-W],                    [W])
AC_S9X_COMPILER_FLAG([-Wno-unused-parameter], [Wno_unused_parameter])

# Enable debugger.

S9XDEBUGGER="#S9XDEBUGGER=1"

AC_ARG_ENABLE([debugger],
	[AS_HELP_STRING([--enable-debugger],
		[enable debugger (default: no)])],
	[], [enable_debugger="no"])

if test "x$enable_debugger" = "xyes"; then
	S9XDEBUGGER="S9XDEBUGGER=1"
	S9XDEFS="$S9XDEFS -DDEBUGGER"
fi

# Enable netplay support if requested.

S9XNETPLAY="#S9XNETPLAY=1"

AC_ARG_ENABLE([netplay],
	[AS_HELP_STRING([--enable-netplay],
		[enable netplay support (default: no)])],
	[], [enable_netplay="no"])

if test "x$enable_netplay" = "xyes"; then
	S9XNETPLAY="S9XNETPLAY=1"
	S9XDEFS="$S9XDEFS -DNETPLAY_SUPPORT"
fi

# Enable GZIP support through zlib.

AC_CACHE_VAL([snes9x_cv_zlib],
[
	AC_CHECK_HEADER([zlib.h],
		[AC_CHECK_LIB([z], [gzread], [snes9x_cv_zlib="yes"], [snes9x_cv_zlib="no"])],
		[snes9x_cv_zlib="no"])
])

AC_ARG_ENABLE([gzip],
	[AS_HELP_STRING([--enable-gzip],
		[enable GZIP support through zlib (default: yes)])],
	[], [enable_gzip="yes"])

if test "x$enable_gzip" = "xyes"; then
	if test "x$snes9x_cv_zlib" = "xyes"; then
		S9XDEFS="$S9XDEFS -DZLIB"
		S9XLIBS="$S9XLIBS -lz"
	else
		AC_MSG_WARN([zlib not found. Build without GZIP support.])
		enable_gzip="no"
	fi
fi

# Enable ZIP support through zlib.

S9XZIP="#S9XZIP=1"

AC_ARG_ENABLE([zip],
	[AS_HELP_STRING([--enable-zip],
		[enable ZIP support through zlib (default: yes)])],
	[], [enable_zip="yes"])

if test "x$enable_zip" = "xyes"; then
	if test "x$snes9x_cv_zlib" = "xyes"; then
		S9XZIP="S9XZIP=1"
		S9XDEFS="$S9XDEFS -DUNZIP_SUPPORT"
		if test "x$enable_gzip" = "xno"; then 
			S9XLIBS="$S9XLIBS -lz"
		fi
	else
		AC_MSG_WARN([zlib not found. Build without ZIP support.])
		enable_zip="no"
	fi
fi

# Enable JMA support.

S9XJMA="#S9XJMA=1"

AC_ARG_ENABLE([jma],
	[AS_HELP_STRING([--enable-jma],
		[enable JMA support (default: yes)])],
	[], [enable_jma="yes"])

if test "x$enable_jma" = "xyes"; then
	S9XJMA="S9XJMA=1"
	S9XDEFS="$S9XDEFS -DJMA_SUPPORT"
fi

# Enable screenshot support through libpng.

AC_CACHE_VAL([snes9x_cv_libpng],
[
	AC_CHECK_HEADER([png.h],
		[AC_CHECK_LIB([png], [png_init_io], [snes9x_cv_libpng="yes"], [snes9x_cv_libpng="no"])],
		[snes9x_cv_libpng="no"])
])

AC_ARG_ENABLE([screenshot],
	[AS_HELP_STRING([--enable-screenshot],
		[enable screenshot support through libpng (default: yes)])],
	[], [enable_screenshot="yes"])

if test "x$enable_screenshot" = "xyes"; then
	if test "x$snes9x_cv_libpng" = "xyes"; then
		S9XDEFS="$S9XDEFS -DHAVE_LIBPNG"
		S9XLIBS="$S9XLIBS -lpng"
	else
		AC_MSG_WARN([libpng not found. Build without screenshot support.])
		enable_screenshot="no"
	fi
fi

# Check for functions

AC_CHECK_FUNC([mkstemp],
[
	S9XDEFS="$S9XDEFS -DHAVE_MKSTEMP"
])

# <domaemon>: Check SDL, messy.

AC_CACHE_VAL([snes9x_cv_sdl],
[
	AC_CHECK_HEADER([SDL/SDL.h],
		[AC_CHECK_LIB([SDL], [SDL_Init], [snes9x_cv_sdl="yes"], [snes9x_cv_sdl="no"])],
		[snes9x_cv_sdl="no"])
])

if test "x$snes9x_cv_sdl" = "xyes"; then
  	S9XFLGS="$S9XFLGS `sdl-config --cflags`"
	S9XLIBS="$S9XLIBS `sdl-config --libs`"
else
	AC_MSG_ERROR([SDL is required.])
fi

# AWH: Check for various needed library headers
AC_CHECK_HEADER([SDL/SDL_image.h],[],
	AC_MSG_ERROR([SDL_image headers not found]))

# AWH: Check for the libraries to link against
AC_CHECK_LIB(SDL_ttf, TTF_OpenFont,
	AC_SUBST(SDL_TTF_LIBS, -lSDL_ttf),
	AC_MSG_ERROR([SDL_ttf library not found]))
S9XLIBS="$S9XLIBS -lSDL_ttf"

AC_CHECK_LIB(SDL_image, IMG_Load,
	AC_SUBST(SDL_IMAGE_LIBS, -lSDL_image),
	AC_MSG_ERROR([SDL_image library not found]))
S9XLIBS="$S9XLIBS -lSDL_image"

AC_CHECK_LIB(EGL, eglGetDisplay,
	AC_SUBST(EGL_LIBS, -lEGL),
	AC_MSG_ERROR([EGL library not found]))
S9XLIBS="$S9XLIBS -lEGL"

AC_CHECK_LIB(GLESv1_CM, glEnable,
	AC_SUBST(EGL_LIBS, -lGLESv1_CM),
	AC_MSG_ERROR([GLES library not found]))
S9XLIBS="$S9XLIBS -lGLESv1_CM"

AC_CHECK_LIB(pthread, pthread_create,
	AC_SUBST(EGL_LIBS, -lpthread),
	AC_MSG_ERROR([PThread library not found]))
S9XLIBS="$S9XLIBS -lX11 -lpthread"

###


# </domaemon>

# Check for headers

snes9x_have_stdint_h="";

AC_CHECK_HEADER([strings.h],
[
	S9XDEFS="$S9XDEFS -DHAVE_STRINGS_H"
])

AC_CHECK_HEADER([sys/ioctl.h],
[
	S9XDEFS="$S9XDEFS -DHAVE_SYS_IOCTL_H"
])

AC_CHECK_HEADER([stdint.h],
[
	S9XDEFS="$S9XDEFS -DHAVE_STDINT_H"
	snes9x_have_stdint_h="-DHAVE_STDINT_H"
])

AC_CHECK_HEADERS([unistd.h sys/socket.h])

# Check whether the size of pointer is int.

if test "x$snes9x_have_stdint_h" = "x"; then
	AC_MSG_CHECKING([whether the size of pointer is int])
	
	AC_TRY_RUN(
	[
		int main (void)
		{
			return (!(sizeof(void *) == sizeof(int)));
		}
	],
	[snes9x_ptr_is_int="yes"], [snes9x_ptr_is_int="no"])
	
	if test "x$snes9x_ptr_is_int" = "xyes"; then
		AC_MSG_RESULT(yes)
	else
		AC_MSG_RESULT(no)
		S9XDEFS="$S9XDEFS -DPTR_NOT_INT"
	fi
fi

# Check whether right shift is arithmetic or not

AC_DEFUN([AC_S9X_CHECK_SAR],
[
	AC_MSG_CHECKING([whether right shift $1 is arithmetic])

	OLD_CXXFLAGS="[$]CXXFLAGS"
	CXXFLAGS="[$]OLD_CXXFLAGS $snes9x_have_stdint_h"

	AC_TRY_RUN(
	[
		#ifdef HAVE_STDINT_H
		#include <stdint.h>
		typedef int8_t			int8;
		typedef int16_t			int16;
		typedef int32_t			int32;
		typedef int64_t			int64;
		#else 
		typedef signed char		int8;
		typedef signed short	int16;
		typedef signed int		int32;
		#ifdef __GNUC__
		__extension__
		#endif
		typedef long long		int64;
		#endif

		int main (void)
		{
			$1	i;

			i = -1;
			i >>= 1;

			return (i < 0 ? 0 : 1);
		}
	],
	[snes9x_sar_$1="yes"], [snes9x_sar_$1="no"])

	CXXFLAGS="[$]OLD_CXXFLAGS"

	if test "x$snes9x_sar_$1" = "xno"; then
		AC_MSG_RESULT([no])
	else
		S9XDEFS="$S9XDEFS -DRIGHTSHIFT_$1_IS_SAR"
		AC_MSG_RESULT([yes])
	fi
])

AC_S9X_CHECK_SAR([int8])
AC_S9X_CHECK_SAR([int16])
AC_S9X_CHECK_SAR([int32])
AC_S9X_CHECK_SAR([int64])

if test "x$snes9x_sar_int8" = "xyes" -a "x$snes9x_sar_int16" = "xyes" -a "x$snes9x_sar_int32" = "xyes" -a "x$snes9x_sar_int64" = "xyes"; then
	S9XDEFS="`echo \"$S9XDEFS\" | sed -e 's/-DRIGHTSHIFT_int8_IS_SAR//'`"
	S9XDEFS="`echo \"$S9XDEFS\" | sed -e 's/-DRIGHTSHIFT_int16_IS_SAR//'`"
	S9XDEFS="`echo \"$S9XDEFS\" | sed -e 's/-DRIGHTSHIFT_int32_IS_SAR//'`"
	S9XDEFS="`echo \"$S9XDEFS\" | sed -e 's/-DRIGHTSHIFT_int64_IS_SAR//'`"
	S9XDEFS="$S9XDEFS -DRIGHTSHIFT_IS_SAR"
fi

# Output.

S9XFLGS="$CXXFLAGS $CPPFLAGS $LDFLAGS $S9XFLGS"
S9XLIBS="$LIBS $S9XLIBS"

S9XFLGS="`echo \"$S9XFLGS\" | sed -e 's/  */ /g'`"
S9XDEFS="`echo \"$S9XDEFS\" | sed -e 's/  */ /g'`"
S9XLIBS="`echo \"$S9XLIBS\" | sed -e 's/  */ /g'`"
S9XFLGS="`echo \"$S9XFLGS\" | sed -e 's/^  *//'`"
S9XDEFS="`echo \"$S9XDEFS\" | sed -e 's/^  *//'`"
S9XLIBS="`echo \"$S9XLIBS\" | sed -e 's/^  *//'`"

AC_SUBST(S9XFLGS)
AC_SUBST(S9XDEFS)
AC_SUBST(S9XLIBS)
AC_SUBST(S9XDEBUGGER)
AC_SUBST(S9XNETPLAY)
AC_SUBST(S9XZIP)
AC_SUBST(S9XJMA)

rm config.info 2>/dev/null

cat >config.info <<EOF

build information:
cc...............,,,. $CC
c++.................. $CXX
options.............. $S9XFLGS
defines.............. $S9XDEFS
libs................. $S9XLIBS

features:
screenshot support... $enable_screenshot
netplay support...... $enable_netplay
GZIP support......... $enable_gzip
ZIP support.......... $enable_zip
JMA support.......... $enable_jma
debugger............. $enable_debugger

EOF

cat config.info

AC_OUTPUT(Makefile)
