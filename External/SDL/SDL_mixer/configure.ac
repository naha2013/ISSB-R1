dnl Process this file with autoconf to produce a configure script.
AC_INIT
AC_CONFIG_SRCDIR([src/mixer.c])
AC_CONFIG_AUX_DIR(build-scripts)

dnl Set various version strings - taken gratefully from the GTk sources

# Making releases:
#   MICRO_VERSION += 1;
#   INTERFACE_AGE += 1;
#   BINARY_AGE += 1;
# if any functions have been added, set INTERFACE_AGE to 0.
# if backwards compatibility has been broken,
# set BINARY_AGE and INTERFACE_AGE to 0.
#
MAJOR_VERSION=2
MINOR_VERSION=0
MICRO_VERSION=4
INTERFACE_AGE=2
BINARY_AGE=4
VERSION=$MAJOR_VERSION.$MINOR_VERSION.$MICRO_VERSION

AC_SUBST(MAJOR_VERSION)
AC_SUBST(MINOR_VERSION)
AC_SUBST(MICRO_VERSION)
AC_SUBST(INTERFACE_AGE)
AC_SUBST(BINARY_AGE)
AC_SUBST(VERSION)

dnl libtool versioning
LT_INIT([win32-dll])

LT_RELEASE=$MAJOR_VERSION.$MINOR_VERSION
LT_CURRENT=`expr $MICRO_VERSION - $INTERFACE_AGE`
LT_REVISION=$INTERFACE_AGE
LT_AGE=`expr $BINARY_AGE - $INTERFACE_AGE`
LT_EXTRA="" dnl for OS2 dll name
m4_pattern_allow([^LT_])

AC_SUBST(LT_RELEASE)
AC_SUBST(LT_CURRENT)
AC_SUBST(LT_REVISION)
AC_SUBST(LT_AGE)
AC_SUBST(LT_EXTRA)

dnl Detect the canonical build and host environments
dnl AC_CANONICAL_HOST

dnl Check for tools
AC_PROG_CC
AC_PROG_INSTALL
AC_PROG_MAKE_SET
LT_PROG_RC

dnl Set up the compiler and linker flags
case "$host" in
    *-*-cygwin*)
        # We build SDL on cygwin without the UNIX emulation layer
        BASE_CFLAGS="-I/usr/include/mingw"
        BASE_LDFLAGS=""
        have_no_cygwin=no
        AC_MSG_CHECKING(for GCC -mno-cygwin option)
        save_CFLAGS="$CFLAGS"
        CFLAGS="$save_CFLAGS -mno-cygwin"
        AC_COMPILE_IFELSE([AC_LANG_PROGRAM([],[])], [have_no_cygwin=yes],[])
        AC_MSG_RESULT($have_no_cygwin)
        CFLAGS="$save_CFLAGS"
        if test x$have_no_cygwin = xyes; then
            BASE_CFLAGS="$BASE_CFLAGS -mno-cygwin"
            BASE_LDFLAGS="-mno-cygwin"
        fi
        ;;
    *-*-os2*)
        # disable static builds on os/2
        enable_static=no
        # -DBUILD_SDL is needed for DECLSPEC
        BASE_CFLAGS="-DBUILD_SDL"
        BASE_LDFLAGS=""
        # OS/2 does not support a DLL name longer than 8 characters.
        LT_EXTRA="-os2dllname SDL2mix"
        ;;
    *)
        BASE_CFLAGS="-D_GNU_SOURCE=1"
        BASE_LDFLAGS=""
        ;;
esac
BUILD_CFLAGS="$CFLAGS $CPPFLAGS -I$srcdir/include -I$srcdir/src -I$srcdir/src/codecs"
EXTRA_CFLAGS="$INCLUDE $BASE_CFLAGS"
BUILD_LDFLAGS="$LDFLAGS"
EXTRA_LDFLAGS="$BASE_LDFLAGS"
## These are common directories to find software packages
#for path in /usr/freeware /usr/pkg /usr/local; do
#    if test -d $path/include; then
#        EXTRA_CFLAGS="$EXTRA_CFLAGS -I$path/include"
#    fi
#    if test -d $path/lib; then
#        EXTRA_LDFLAGS="$EXTRA_LDFLAGS -L$path/lib"
#    fi
#done
CPPFLAGS="$CPPFLAGS $EXTRA_CFLAGS"
CFLAGS="$CFLAGS $EXTRA_CFLAGS"
LDFLAGS="$LDFLAGS $EXTRA_LDFLAGS"

dnl Check for compiler characteristics
AC_C_CONST

dnl See whether we can use gcc style dependency tracking
AC_ARG_ENABLE(dependency-tracking,
[AS_HELP_STRING([--enable-dependency-tracking],
               [Use gcc -MMD -MT dependency tracking [default=yes]])],
              , enable_dependency_tracking=yes)
if test x$enable_dependency_tracking = xyes; then
    have_gcc_mmd_mt=no
    AC_MSG_CHECKING(for GCC -MMD -MT option)
    AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[
    #if !defined(__GNUC__) || __GNUC__ < 3
    #error Dependency tracking requires GCC 3.0 or newer
    #endif
    ]],[])], [have_gcc_mmd_mt=yes],[])
    AC_MSG_RESULT($have_gcc_mmd_mt)

    if test x$have_gcc_mmd_mt = xyes; then
        DEPENDENCY_TRACKING_OPTIONS="-MMD -MT \$@"
    fi
fi

case "$host" in
    *-*-cygwin* | *-*-mingw*)
        VERSION_SOURCES="$srcdir/version.rc"
        EXE=".exe"
        if test "$build" != "$host"; then # cross-compiling
            # Default cross-compile location
            ac_default_prefix=/usr/local/cross-tools/$host
        else
            # Look for the location of the tools and install there
            if test "$BUILD_PREFIX" != ""; then
                ac_default_prefix=$BUILD_PREFIX
            fi
        fi
        ;;
    *-*-os2*)
        EXE=".exe"
        ;;
    *)
        EXE=""
        ;;
esac

# Standard C sources
SOURCES=`ls $srcdir/src/*.c $srcdir/src/codecs/*.c | fgrep -v playwave.c | fgrep -v playmus.c`

dnl set this to use on systems that use lib64 instead of lib
base_libdir=`echo \${libdir} | sed 's/.*\/\(.*\)/\1/; q'`

dnl See if GCC's -fvisibility=hidden is supported (gcc4 and later, usually).
CheckVisibilityHidden()
{
    AC_MSG_CHECKING(for GCC -fvisibility=hidden option)
    have_gcc_fvisibility=no
    case "$host" in
    *-*-cygwin* | *-*-mingw* | *-*-os2*)
        AC_MSG_RESULT([ignored for $host_os])
        return
        ;;
    esac

    visibility_CFLAGS="-fvisibility=hidden"
    save_CFLAGS="$CFLAGS"
    CFLAGS="$save_CFLAGS $visibility_CFLAGS -Werror"
    AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[
    #if !defined(__GNUC__) || __GNUC__ < 4
    #error SDL only uses visibility attributes in GCC 4 or newer
    #endif
    ]],[])], [have_gcc_fvisibility=yes],[])
    AC_MSG_RESULT($have_gcc_fvisibility)
    CFLAGS="$save_CFLAGS"

    if test x$have_gcc_fvisibility = xyes; then
        EXTRA_CFLAGS="$EXTRA_CFLAGS $visibility_CFLAGS"
    fi
}

dnl Function to find a library in the compiler search path
find_lib()
{
    gcc_bin_path=[`$CC -print-search-dirs 2>/dev/null | fgrep programs: | sed 's/[^=]*=\(.*\)/\1/' | sed 's/:/ /g'`]
    gcc_lib_path=[`$CC -print-search-dirs 2>/dev/null | fgrep libraries: | sed 's/[^=]*=\(.*\)/\1/' | sed 's/:/ /g'`]
    env_lib_path=[`echo $LIBS $LDFLAGS $* | sed 's/-L[ ]*//g'`]
    if test "$cross_compiling" = yes; then
        host_lib_path=""
    else
        host_lib_path="/usr/$base_libdir /usr/local/$base_libdir"
    fi
    for path in $env_lib_path $gcc_bin_path $gcc_lib_path $host_lib_path; do
        lib=[`ls -- $path/$1 2>/dev/null | sed -e '/\.so\..*\./d' -e 's,.*/,,' | sort | tail -1`]
        if test x$lib != x; then
            echo $lib
            return
        fi
    done
}

dnl Check for SDL
SDL_VERSION=2.0.7
AM_PATH_SDL2($SDL_VERSION,
            :,
            AC_MSG_ERROR([*** SDL version $SDL_VERSION not found!])
)
EXTRA_CFLAGS="$EXTRA_CFLAGS $SDL_CFLAGS"
EXTRA_LDFLAGS="$EXTRA_LDFLAGS $SDL_LIBS"

dnl check for GCC visibility attributes
CheckVisibilityHidden

dnl Check for math library
AC_CHECK_LIB(m, pow, [LIBM="-lm"])

AC_CHECK_HEADERS([signal.h], [EXTRA_CFLAGS="$EXTRA_CFLAGS -DHAVE_SIGNAL_H"])
AC_CHECK_FUNCS(setbuf, [EXTRA_CFLAGS="$EXTRA_CFLAGS -DHAVE_SETBUF"])

dnl Check command-line options

AC_ARG_ENABLE([music-cmd],
[AS_HELP_STRING([--enable-music-cmd], [support an external music player [default=yes]])],
              [], [enable_music_cmd=detect])
if test "x$enable_music_cmd" != xno; then
    AC_CHECK_FUNCS([fork vfork])
    if test "x$ac_cv_func_fork" = "xyes"; then
        EXTRA_CFLAGS="$EXTRA_CFLAGS -DHAVE_FORK"
    elif test "x$ac_cv_func_vfork" = "xyes"; then
        EXTRA_CFLAGS="$EXTRA_CFLAGS -DHAVE_VFORK"
    elif test "x$enable_music_cmd" = "xyes"; then
        AC_MSG_ERROR([external music player not available on your platform])
    else
        enable_music_cmd=no
    fi
    if test "x$enable_music_cmd" != xno; then
        EXTRA_CFLAGS="$EXTRA_CFLAGS -DMUSIC_CMD"
    fi
fi

AC_ARG_ENABLE([music-wave],
[AS_HELP_STRING([--enable-music-wave], [enable streaming WAVE music [default=yes]])],
              [], [enable_music_wave=yes])
if test x$enable_music_wave = xyes; then
    EXTRA_CFLAGS="$EXTRA_CFLAGS -DMUSIC_WAV"
fi

AC_ARG_ENABLE([music-mod],
[AS_HELP_STRING([--enable-music-mod], [enable MOD music [default=yes]])],
              [], [enable_music_mod=yes])

AC_ARG_ENABLE([music-mod-modplug],
[AS_HELP_STRING([--enable-music-mod-modplug], [enable MOD music via modplug [default=yes]])],
              [], [enable_music_mod_modplug=yes])
AC_ARG_ENABLE([music-mod-modplug-shared],
[AS_HELP_STRING([--enable-music-mod-modplug-shared], [dynamically load modplug library [default=yes]])],
              [], [enable_music_mod_modplug_shared=yes])
if test x$enable_music_mod = xyes -a x$enable_music_mod_modplug = xyes; then
    PKG_CHECK_MODULES([MODPLUG], [libmodplug >= 0.8.8], [dnl
            have_libmodplug_hdr=yes
            have_libmodplug_lib=yes
        ], [dnl
            AC_CHECK_HEADER([libmodplug/modplug.h], [have_libmodplug_hdr=yes])
            AC_CHECK_LIB([modplug], [ModPlug_Load], [have_libmodplug_lib=yes;MODPLUG_LIBS="-lmodplug"])
        ])

    if test x$have_libmodplug_hdr = xyes -a x$have_libmodplug_lib = xyes; then
        have_libmodplug=yes
        case "$host" in
            *-*-darwin*)
                modplug_lib=[`find_lib libmodplug.dylib`]
                ;;
            *-*-cygwin* | *-*-mingw*)
                modplug_lib=[`find_lib "libmodplug*.dll"`]
                ;;
            *)
                modplug_lib=[`find_lib "libmodplug[0-9]*.so.*"`]
                if test x$modplug_lib = x; then
                    modplug_lib=[`find_lib "libmodplug.so.*"`]
                fi
                ;;
        esac
        EXTRA_CFLAGS="$EXTRA_CFLAGS -DMUSIC_MOD_MODPLUG $MODPLUG_CFLAGS"
        if test x$enable_music_mod_modplug_shared = xyes && test x$modplug_lib != x; then
            echo "-- dynamic libmodplug -> $modplug_lib"
            EXTRA_CFLAGS="$EXTRA_CFLAGS -DMODPLUG_DYNAMIC=\\\"$modplug_lib\\\""
        else
            EXTRA_LDFLAGS="$EXTRA_LDFLAGS $MODPLUG_LIBS"
        fi
    else
        AC_MSG_WARN([*** Unable to find ModPlug library (http://modplug-xmms.sourceforge.net/)])
    fi
fi

AC_ARG_ENABLE([music-mod-xmp],
[AS_HELP_STRING([--enable-music-mod-xmp], [enable MOD music via libxmp [default=no]])],
              [], [enable_music_mod_xmp=no])
AC_ARG_ENABLE([music-mod-xmp-lite],
[AS_HELP_STRING([--enable-music-mod-xmp-lite], [use libxmp-lite instead of libxmp [default=no]])],
              [], [enable_music_mod_xmp_lite=no])
AC_ARG_ENABLE([music-mod-xmp-shared],
[AS_HELP_STRING([--enable-music-mod-xmp-shared], [dynamically load xmp library [default=yes]])],
              [], [enable_music_mod_xmp_shared=yes])
if test x$enable_music_mod = xyes -a x$enable_music_mod_xmp = xyes; then
    xmplib=xmp
    if test x$enable_music_mod_xmp_lite = xyes; then
      xmplib=xmp-lite
    fi
    PKG_CHECK_MODULES([XMP], [lib$xmplib >= 4.2], [dnl
            have_libxmp_hdr=yes
            have_libxmp_lib=yes
        ], [dnl
            AC_CHECK_HEADER([xmp.h], [have_libxmp_hdr=yes])
            AC_CHECK_LIB([xmp], [xmp_load_module_from_memory], [have_libxmp_lib=yes;XMP_LIBS="-l$xmplib"])
        ])

    if test x$have_libxmp_hdr = xyes -a x$have_libxmp_lib = xyes; then
        have_libxmp=yes
        case "$host" in
            *-*-darwin*)
                xmp_lib=[`find_lib lib$xmplib.dylib`]
                ;;
            *-*-cygwin* | *-*-mingw*)
                xmp_lib=[`find_lib "lib$xmplib*.dll"`]
                ;;
            *)
                xmp_lib=[`find_lib "lib$xmplib[0-9]*.so.*"`]
                if test x$xmp_lib = x; then
                    xmp_lib=[`find_lib "lib$xmplib.so.*"`]
                fi
                ;;
        esac
        EXTRA_CFLAGS="$EXTRA_CFLAGS -DMUSIC_MOD_XMP $XMP_CFLAGS"
        if test x$enable_music_mod_xmp_shared = xyes && test x$xmp_lib != x; then
            echo "-- dynamic libxmp -> $xmp_lib"
            EXTRA_CFLAGS="$EXTRA_CFLAGS -DXMP_DYNAMIC=\\\"$xmp_lib\\\""
        else
            EXTRA_LDFLAGS="$EXTRA_LDFLAGS $XMP_LIBS"
        fi
    else
        AC_MSG_WARN([*** Unable to find xmp library (http://xmp.sourceforge.net/)])
    fi
fi

AC_ARG_ENABLE([music-mod-mikmod],
[AS_HELP_STRING([--enable-music-mod-mikmod], [enable MOD music via mikmod [default=no]])],
              [], [enable_music_mod_mikmod=no])
AC_ARG_ENABLE([music-mod-mikmod-shared],
[AS_HELP_STRING([--enable-music-mod-mikmod-shared], [dynamically load mikmod library [default=yes]])],
              [], [enable_music_mod_mikmod_shared=yes])
if test x$enable_music_mod = xyes -a x$enable_music_mod_mikmod = xyes; then
    have_libmikmod=no
    libmikmod_maj=3
    libmikmod_min=1
    libmikmod_rev=10
    libmikmod_ver="$libmikmod_maj.$libmikmod_min.$libmikmod_rev"
    CFLAGS_SAVED="$CFLAGS"
    LIBS_SAVED="$LIBS"
    AC_PATH_PROG(LIBMIKMOD_CONFIG, libmikmod-config, no, [$PATH])
    if test "$LIBMIKMOD_CONFIG" != "no" ; then
        CFLAGS="$CFLAGS `$LIBMIKMOD_CONFIG --cflags`"
        LIBS="$LIBS `$LIBMIKMOD_CONFIG --libs`"
        have_libmikmod=yes
        AC_MSG_CHECKING([for libmikmod - version >= $libmikmod_ver])
        AC_RUN_IFELSE([AC_LANG_SOURCE([[
#include "mikmod.h"
#include "stdio.h"

int main(int argc, char **argv)
{
  long maj=$libmikmod_maj,min=$libmikmod_min,rev=$libmikmod_rev,ver=MikMod_GetVersion();
  if (ver>=((maj<<16)|(min<<8)|(rev))) {
      printf("yes\n");
      return 0;
  }
  printf("no\n*** libmikmod is older than %d.%d.%d, not using.\n",maj,min,rev);
  return 1;
}
]])],[],[have_libmikmod=no],[echo $ac_n "cross compiling; assumed OK... $ac_c"])
    fi

    if test x$have_libmikmod = xyes; then
        case "$host" in
            *-*-darwin*)
                mikmod_lib=[`find_lib libmikmod.dylib`]
                ;;
            *-*-cygwin* | *-*-mingw*)
                mikmod_lib=[`find_lib "libmikmod*.dll"`]
                ;;
            *)
                mikmod_lib=[`find_lib "libmikmod[0-9]*.so.*"`]
                if test x$mikmod_lib = x; then
                    mikmod_lib=[`find_lib "libmikmod.so.*"`]
                fi
                ;;
        esac
        EXTRA_CFLAGS="$EXTRA_CFLAGS -DMUSIC_MOD_MIKMOD `$LIBMIKMOD_CONFIG --cflags`"
        if test x$enable_music_mod_mikmod_shared = xyes && test x$mikmod_lib != x; then
            echo "-- dynamic libmikmod -> $mikmod_lib"
            EXTRA_CFLAGS="$EXTRA_CFLAGS -DMIKMOD_DYNAMIC=\\\"$mikmod_lib\\\""
        else
            EXTRA_LDFLAGS="$EXTRA_LDFLAGS `$LIBMIKMOD_CONFIG --libs`"
        fi
    else
        AC_MSG_WARN([*** Unable to find MikMod library (http://mikmod.sourceforge.net/)])
    fi
    LIBS="$LIBS_SAVED"
    CFLAGS="$CFLAGS_SAVED"
fi

if test x$have_libmodplug != xyes -a x$have_libmikmod != xyes -a x$have_libxmp != xyes; then
    AC_MSG_WARN([MOD support disabled])
fi

AC_ARG_ENABLE([music-midi],
[AS_HELP_STRING([--enable-music-midi], [enable MIDI music [default=yes]])],
              [], [enable_music_midi=yes])
if test x$enable_music_midi = xyes; then
    EXTRA_CFLAGS="$EXTRA_CFLAGS -DMUSIC_MID"
    AC_ARG_ENABLE([music-midi-timidity],
[AS_HELP_STRING([--enable-music-midi-timidity], [enable timidity MIDI output [default=yes]])],
                  [], [enable_music_midi_timidity=yes])
    if test x$enable_music_midi_timidity = xyes; then
        EXTRA_CFLAGS="$EXTRA_CFLAGS -DMUSIC_MID_TIMIDITY -I\$(srcdir)/src/codecs/timidity"
        SOURCES="$SOURCES $srcdir/src/codecs/timidity/*.c"
        timidity_cfg=no
        AC_ARG_WITH([timidity-cfg],
[AS_HELP_STRING([--with-timidity-cfg=FILE],[Specify full path to timidity.cfg])], timidity_cfg="$withval", [])
        if test x$timidity_cfg != xyes -a x$timidity_cfg != xno; then
                EXTRA_CFLAGS="$EXTRA_CFLAGS -DTIMIDITY_CFG=\\\"$timidity_cfg\\\""
        fi
    fi
    AC_ARG_ENABLE([music-midi-native],
[AS_HELP_STRING([--enable-music-midi-native], [enable native MIDI music output [default=yes]])],
                  [], [enable_music_midi_native=yes])
    if test x$enable_music_midi_native = xyes; then
        use_music_midi_native=no
        case "$host" in
            *-*-cygwin* | *-*-mingw*)
                use_music_midi_native=yes
                EXTRA_LDFLAGS="$EXTRA_LDFLAGS -lwinmm"
                ;;
            *-*-darwin*)
                use_music_midi_native=yes
                EXTRA_LDFLAGS="$EXTRA_LDFLAGS -Wl,-framework,AudioToolbox -Wl,-framework,AudioUnit -Wl,-framework,CoreServices"
                ;;
            *-*-haiku*)
                use_music_midi_native=yes_cpp
                EXTRA_LDFLAGS="$EXTRA_LDFLAGS -lmidi"
                ;;
        esac
        if test x$use_music_midi_native = xyes; then
            EXTRA_CFLAGS="$EXTRA_CFLAGS -DMUSIC_MID_NATIVE -I\$(srcdir)/src/codecs/native_midi"
            SOURCES="$SOURCES $srcdir/src/codecs/native_midi/*.c"
        elif test x$use_music_midi_native = xyes_cpp; then
            EXTRA_CFLAGS="$EXTRA_CFLAGS -DMUSIC_MID_NATIVE -I\$(srcdir)/src/codecs/native_midi"
            SOURCES="$SOURCES $srcdir/src/codecs/native_midi/*.c"
            SOURCES_CXX="$SOURCES_CXX $srcdir/src/codecs/native_midi/*.cpp"
        fi
    fi
    AC_ARG_ENABLE([music-midi-fluidsynth],
[AS_HELP_STRING([--enable-music-midi-fluidsynth], [enable FluidSynth MIDI output [default=yes]])],
                  [], [enable_music_midi_fluidsynth=yes])
    AC_ARG_ENABLE([music-midi-fluidsynth-shared],
[AS_HELP_STRING([--enable-music-midi-fluidsynth-shared], [dynamically load FluidSynth library [default=yes]])],
                  [], [enable_music_midi_fluidsynth_shared=yes])
    if test x$enable_music_midi_fluidsynth = xyes; then
        AC_CHECK_HEADER([fluidsynth.h], [have_fluidsynth_hdr=yes])
        AC_CHECK_LIB([fluidsynth], [fluid_player_add_mem], [have_fluidsynth_lib=yes])
        if test x$have_fluidsynth_hdr = xyes -a x$have_fluidsynth_lib = xyes; then
            have_fluidsynth=yes
            case "$host" in
                *-*-darwin*)
                    fluidsynth_lib=[`find_lib libfluidsynth.dylib`]
                    ;;
                *-*-cygwin* | *-*-mingw*)
                    fluidsynth_lib=[`find_lib "fluidsynth*.dll"`]
                    if test x$fluidsynth_lib = x; then
                        fluidsynth_lib=[`find_lib "libfluidsynth*.dll"`]
                    fi
                    ;;
                *)
                    fluidsynth_lib=[`find_lib "libfluidsynth[0-9]*.so.*"`]
                    if test x$fluidsynth_lib = x; then
                        fluidsynth_lib=[`find_lib "libfluidsynth.so.*"`]
                    fi
                    ;;
            esac
            EXTRA_CFLAGS="$EXTRA_CFLAGS -DMUSIC_MID_FLUIDSYNTH"
            if test x$enable_music_midi_fluidsynth_shared = xyes && test x$fluidsynth_lib != x; then
                echo "-- dynamic libfluidsynth -> $fluidsynth_lib"
                EXTRA_CFLAGS="$EXTRA_CFLAGS -DFLUIDSYNTH_DYNAMIC=\\\"$fluidsynth_lib\\\""
            else
                EXTRA_LDFLAGS="$EXTRA_LDFLAGS -lfluidsynth"
            fi
        else
            AC_MSG_WARN([*** Unable to find FluidSynth library (http://www.fluidsynth.org/)])
            AC_MSG_WARN([FluidSynth support disabled])
        fi
    fi
fi

if test x$enable_music_midi_timidity != xyes -a \
        x$use_music_midi_native != xyes -a x$use_music_midi_native != xyes_cpp -a \
        x$have_fluidsynth != xyes; then
    AC_MSG_WARN([MIDI support disabled])
fi

AC_ARG_ENABLE([music-ogg],
[AS_HELP_STRING([--enable-music-ogg], [enable Ogg Vorbis music [default=yes]])],
              [], [enable_music_ogg=yes])
AC_ARG_ENABLE(music-ogg-tremor,
[AS_HELP_STRING([--enable-music-ogg-tremor], [enable OGG Vorbis music via libtremor [default=no]])],
              [], enable_music_ogg_tremor=no)
AC_ARG_ENABLE([music-ogg-shared],
[AS_HELP_STRING([--enable-music-ogg-shared], [dynamically load Ogg Vorbis library [default=yes]])],
              [], [enable_music_ogg_shared=yes])
if test x$enable_music_ogg = xyes; then
    LIBS_SAVED="$LIBS"
    if test x$enable_music_ogg_tremor = xyes; then
        AC_CHECK_HEADER([tremor/ivorbisfile.h], [have_tremor_hdr=yes])
        AC_CHECK_LIB([vorbisidec], [ov_open_callbacks], [have_tremor_lib=yes], [], [-logg])
        if test x$have_tremor_hdr = xyes -a x$have_tremor_lib = xyes; then
            case "$host" in
                *-*-darwin*)
                    ogg_lib=[`find_lib libvorbisidec.dylib`]
                    ;;
                *-*-cygwin* | *-*-mingw*)
                    ogg_lib=[`find_lib "vorbisidec*.dll"`]
                    if test x$ogg_lib = x; then
                        ogg_lib=[`find_lib "libvorbisidec*.dll"`]
                    fi
                    ;;
                *)
                    ogg_lib=[`find_lib "libvorbisidec[0-9]*.so.*"`]
                    if test x$ogg_lib = x; then
                        ogg_lib=[`find_lib "libvorbisidec.so.*"`]
                    fi
                    ;;
            esac
            EXTRA_CFLAGS="$EXTRA_CFLAGS -DMUSIC_OGG -DOGG_USE_TREMOR"
            if test x$enable_music_ogg_shared = xyes && test x$ogg_lib != x; then
                echo "-- dynamic libvorbisidec -> $ogg_lib"
                EXTRA_CFLAGS="$EXTRA_CFLAGS -DOGG_DYNAMIC=\\\"$ogg_lib\\\""
            else
                EXTRA_LDFLAGS="$EXTRA_LDFLAGS -lvorbisidec"
            fi
        else
            AC_MSG_WARN([*** Unable to find Ogg Vorbis Tremor library (http://www.xiph.org/)])
            AC_MSG_WARN([Ogg Vorbis support disabled])
        fi
    else
        AC_CHECK_HEADER([vorbis/vorbisfile.h], [have_ogg_hdr=yes])
        AC_CHECK_LIB([vorbisfile], [ov_open_callbacks], [have_ogg_lib=yes], [], [-lvorbis -logg -lm])
        if test x$have_ogg_hdr = xyes -a x$have_ogg_lib = xyes; then
            case "$host" in
                *-*-darwin*)
                    ogg_lib=[`find_lib libvorbisfile.dylib`]
                    ;;
                *-*-cygwin* | *-*-mingw*)
                    ogg_lib=[`find_lib "libvorbisfile*.dll"`]
                    ;;
                *)
                    ogg_lib=[`find_lib "libvorbisfile[0-9]*.so.*"`]
                    if test x$ogg_lib = x; then
                        ogg_lib=[`find_lib "libvorbisfile.so.*"`]
                    fi
                    ;;
            esac
            EXTRA_CFLAGS="$EXTRA_CFLAGS -DMUSIC_OGG"
            if test x$enable_music_ogg_shared = xyes && test x$ogg_lib != x; then
                echo "-- dynamic libvorbisfile -> $ogg_lib"
                EXTRA_CFLAGS="$EXTRA_CFLAGS -DOGG_DYNAMIC=\\\"$ogg_lib\\\""
            else
                EXTRA_LDFLAGS="$EXTRA_LDFLAGS -lvorbisfile -lvorbis"
            fi
        else
            AC_MSG_WARN([*** Unable to find Ogg Vorbis library (http://www.xiph.org/)])
            AC_MSG_WARN([Ogg Vorbis support disabled])
        fi
    fi
    LIBS="$LIBS_SAVED"
fi

libflac_ver=8
AC_ARG_ENABLE([music-flac],
[AS_HELP_STRING([--enable-music-flac], [enable FLAC music [default=yes]])],
              [], [enable_music_flac=yes])
AC_ARG_ENABLE([music-flac-shared],
[AS_HELP_STRING([--enable-music-flac-shared],
              [dynamically load FLAC library [default=yes]])],
              [], [enable_music_flac_shared=yes])
if test x$enable_music_flac = xyes; then
    AC_CHECK_HEADER([FLAC/export.h], [have_flac_export=yes])
    if test x$have_flac_export = xyes; then
        LIBS_SAVED="$LIBS"
        LIBS="-lFLAC"
        AC_MSG_CHECKING([for libflac so-name version >= $libflac_ver])
        AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[
#include "FLAC/export.h"
#include "stdio.h"
]], [[
#if defined(FLAC_API_VERSION_CURRENT) && (FLAC_API_VERSION_CURRENT >= $libflac_ver)
#else
#error "old-flac"
#endif
]])], [have_flac_ver=yes],[have_flac_ver=no])
        LIBS="$LIBS_SAVED"
        AC_MSG_RESULT($have_flac_ver)
    fi

    if test x$have_flac_ver = xyes; then
        AC_CHECK_HEADER([FLAC/stream_decoder.h], [have_flac_hdr=yes])
        AC_CHECK_LIB([FLAC], [FLAC__stream_decoder_new], [have_flac_lib=yes])
        if test x$have_flac_hdr = xyes -a x$have_flac_lib = xyes; then
            case "$host" in
                *-*-darwin*)
                    flac_lib=[`find_lib libFLAC.dylib`]
                    ;;
                *-*-cygwin* | *-*-mingw*)
                    flac_lib=[`find_lib "libFLAC-*.dll"`]
                    ;;
                *)
                    flac_lib=[`find_lib "libFLAC[0-9]*.so.*"`]
                    if test x$flac_lib = x; then
                        flac_lib=[`find_lib "libFLAC.so.*"`]
                    fi
                    ;;
            esac
            EXTRA_CFLAGS="$EXTRA_CFLAGS -DMUSIC_FLAC"
            if test x$enable_music_flac_shared = xyes && test x$flac_lib != x; then
                echo "-- dynamic libFLAC -> $flac_lib"
                EXTRA_CFLAGS="$EXTRA_CFLAGS -DFLAC_DYNAMIC=\\\"$flac_lib\\\""
            else
                EXTRA_LDFLAGS="$EXTRA_LDFLAGS -lFLAC"
            fi
        fi
    else
        AC_MSG_WARN([*** Unable to find FLAC library (http://flac.sourceforge.net/)])
        AC_MSG_WARN([FLAC support disabled])
    fi
fi

AC_ARG_ENABLE(music-mp3,
[AS_HELP_STRING([--enable-music-mp3], [enable MP3 music [default=yes]])],
              [], enable_music_mp3=yes)

AC_ARG_ENABLE(music-mp3-mad-gpl,
[AS_HELP_STRING([--enable-music-mp3-mad-gpl], [enable MP3 music via libmad GPL code [default=no]])],
                  [], [enable_music_mp3_mad_gpl=no])
AC_ARG_ENABLE(music-mp3-mad-gpl-dithering,
[AS_HELP_STRING([--enable-music-mp3-mad-gpl-dithering], [enable MP3 music via libmad GPL code [default=yes]])],
                  [], [enable_music_mp3_mad_gpl_dithering=yes])
if test x$enable_music_mp3 = xyes -a x$enable_music_mp3_mad_gpl = xyes; then
    AC_MSG_CHECKING(for libmad headers)
    have_libmad=no
    AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[
     #include "mad.h"
    ]],[])], [have_libmad=yes],[])
    AC_MSG_RESULT($have_libmad)
    if test x$have_libmad = xyes; then
        EXTRA_CFLAGS="$EXTRA_CFLAGS -DMUSIC_MP3_MAD"
        if test x$enable_music_mp3_mad_gpl_dithering = xyes; then
            AC_MSG_WARN([*** Using GPL libmad and MP3 dithering routines, this build of SDL_mixer is now under the GPL])
            EXTRA_CFLAGS="$EXTRA_CFLAGS -DMUSIC_MP3_MAD_GPL_DITHERING"
        fi
        EXTRA_LDFLAGS="$EXTRA_LDFLAGS -lmad"
    else
        AC_MSG_WARN([*** Unable to find MAD library (http://www.underbit.com/products/mad/)])
    fi
fi

AC_ARG_ENABLE(music-mp3-mpg123,
[AS_HELP_STRING([--enable-music-mp3-mpg123], [enable MP3 music via libmpg123 [default=yes]])],
                  [], [enable_music_mp3_mpg123=yes])
AC_ARG_ENABLE([music-mp3-mpg123-shared],
[AS_HELP_STRING([--enable-music-mp3-mpg123-shared], [dynamically load libmpg123 library [default=yes]])],
              [], [enable_music_mp3_mpg123_shared=yes])
if test x$enable_music_mp3_mpg123 = xyes; then
    AC_CHECK_HEADER([mpg123.h], [have_mpg123_hdr=yes])
    AC_CHECK_LIB([mpg123], [mpg123_replace_reader_handle], [have_mpg123_lib=yes])
    if test x$have_mpg123_hdr = xyes -a x$have_mpg123_lib = xyes; then
        have_libmpg123=yes
        case "$host" in
            *-*-darwin*)
                mpg123_lib=[`find_lib libmpg123.dylib`]
                ;;
            *-*-cygwin* | *-*-mingw*)
                mpg123_lib=[`find_lib "libmpg123*.dll"`]
                ;;
            *)
                mpg123_lib=[`find_lib "libmpg123.so.*"`]
                ;;
        esac
        EXTRA_CFLAGS="$EXTRA_CFLAGS -DMUSIC_MP3_MPG123"
        if test x$enable_music_mp3_mpg123_shared = xyes && test x$mpg123_lib != x; then
            echo "-- dynamic libmpg123 -> $mpg123_lib"
            EXTRA_CFLAGS="$EXTRA_CFLAGS -DMPG123_DYNAMIC=\\\"$mpg123_lib\\\""
        else
            EXTRA_LDFLAGS="$EXTRA_LDFLAGS -lmpg123"
        fi
    else
        AC_MSG_WARN([*** Unable to find mpg123 library (https://www.mpg123.de)])
        AC_MSG_WARN([mpg123 support disabled])
    fi
fi

if test x$have_libmad = xyes -o x$have_libmpg123 = xyes; then
    :
else
    AC_MSG_WARN([MP3 support disabled])
fi

AC_ARG_ENABLE([music-opus],
[AS_HELP_STRING([--enable-music-opus], [enable Opus music [default=yes]])],
              [], [enable_music_opus=yes])

AC_ARG_ENABLE([music-opus-shared],
[AS_HELP_STRING([--enable-music-opus-shared], [dynamically load opusfile library [default=yes]])],
              [], [enable_music_opus_shared=yes])
if test x$enable_music_opus = xyes; then
    LIBS_SAVED="$LIBS"
    PKG_CHECK_MODULES([OPUSFILE], [opusfile >= 0.2], [dnl
            have_opusfile_hdr=yes
            have_opusfile_lib=yes
        ], [dnl
            AC_CHECK_HEADER([opus/opusfile.h], [have_opusfile_hdr=yes])
            AC_CHECK_LIB([opusfile], [op_open_callbacks], [have_opusfile_lib=yes;OPUSFILE_LIBS="-lopusfile -lopus"], [], [-lopus -logg -lm])
        ])
    LIBS="$LIBS_SAVED"

    if test x$have_opusfile_hdr = xyes -a x$have_opusfile_lib = xyes; then
        have_opusfile=yes
        case "$host" in
            *-*-darwin*)
                opusfile_lib=[`find_lib libopusfile.dylib`]
                ;;
            *-*-cygwin* | *-*-mingw*)
                opusfile_lib=[`find_lib "libopusfile*.dll"`]
                ;;
            *)
                opusfile_lib=[`find_lib "libopusfile[0-9]*.so.*"`]
                if test x$opusfile_lib = x; then
                    opusfile_lib=[`find_lib "libopusfile.so.*"`]
                fi
                ;;
        esac
        EXTRA_CFLAGS="$EXTRA_CFLAGS -DMUSIC_OPUS $OPUSFILE_CFLAGS"
        if test x$enable_music_opus_shared = xyes && test x$opusfile_lib != x; then
            echo "-- dynamic opusfile -> $opusfile_lib"
            EXTRA_CFLAGS="$EXTRA_CFLAGS -DOPUS_DYNAMIC=\\\"$opusfile_lib\\\""
        else
            EXTRA_LDFLAGS="$EXTRA_LDFLAGS $OPUSFILE_LIBS"
        fi
    else
        AC_MSG_WARN([*** Unable to find opusfile library (http://opus-codec.org/)])
    fi
fi

EXTRA_LDFLAGS="$EXTRA_LDFLAGS $LIBM"

OBJECTS=`echo $SOURCES`
DEPENDS=`echo $SOURCES`
OBJECTS=`echo "$OBJECTS" | sed 's,[[^ ]]*/\([[^ ]]*\)\.c,$(objects)/\1.lo,g'`
DEPENDS=`echo "$DEPENDS" | sed 's,\([[^ ]]*\)/\([[^ ]]*\)\.c,\\
$(objects)/\2.lo: \1/\2.c \$(objects)/.created\\
	\$(LIBTOOL) --mode=compile \$(CC) \$(CFLAGS) \$(EXTRA_CFLAGS) '"$DEPENDENCY_TRACKING_OPTIONS"' -c \$< -o \$@,g'`

OBJECTS_CXX=`echo $SOURCES_CXX`
DEPENDS_CXX=`echo $SOURCES_CXX`
OBJECTS_CXX=`echo "$OBJECTS_CXX" | sed 's,[[^ ]]*/\([[^ ]]*\)\.cpp,$(objects)/\1.lo,g'`
DEPENDS_CXX=`echo "$DEPENDS_CXX" | sed 's,\([[^ ]]*\)/\([[^ ]]*\)\.cpp,\\
$(objects)/\2.lo: \1/\2.cpp \$(objects)/.created\\
	\$(LIBTOOL) --mode=compile \$(CXX) \$(CFLAGS) \$(EXTRA_CFLAGS) '"$DEPENDENCY_TRACKING_OPTIONS"' -c \$< -o \$@,g'`
OBJECTS="$OBJECTS $OBJECTS_CXX"
DEPENDS="$DEPENDS $DEPENDS_CXX"
DEPENDS=`echo "$DEPENDS" | sed 's,\\$,\\\\$,g'`

VERSION_OBJECTS=`echo $VERSION_SOURCES`
VERSION_DEPENDS=`echo $VERSION_SOURCES`
VERSION_OBJECTS=`echo "$VERSION_OBJECTS" | sed 's,[[^ ]]*/\([[^ ]]*\)\.rc,$(objects)/\1.lo,g'`
VERSION_DEPENDS=`echo "$VERSION_DEPENDS" | sed 's,\([[^ ]]*\)/\([[^ ]]*\)\.rc,\\
$(objects)/\2.lo: \1/\2.rc \$(objects)/.created\\
	\$(LIBTOOL) --tag=RC --mode=compile \$(RC) \$< -o \$@,g'`
VERSION_DEPENDS=`echo "$VERSION_DEPENDS" | sed 's,\\$,\\\\$,g'`

PLAYWAVE_SOURCES="$srcdir/playwave.c"
PLAYWAVE_OBJECTS=`echo $PLAYWAVE_SOURCES`
PLAYWAVE_DEPENDS=`echo $PLAYWAVE_SOURCES`
PLAYWAVE_OBJECTS=`echo "$PLAYWAVE_OBJECTS" | sed 's,[[^ ]]*/\([[^ ]]*\)\.c,$(objects)/\1.lo,g'`
PLAYWAVE_DEPENDS=`echo "$PLAYWAVE_DEPENDS" | sed 's,\([[^ ]]*\)/\([[^ ]]*\)\.c,\\
$(objects)/\2.lo: \1/\2.c \$(objects)/.created\\
	\$(LIBTOOL) --mode=compile \$(CC) \$(CFLAGS) \$(EXTRA_CFLAGS) '"$DEPENDENCY_TRACKING_OPTIONS"' -c \$< -o \$@,g'`
PLAYWAVE_DEPENDS=`echo "$PLAYWAVE_DEPENDS" | sed 's,\\$,\\\\$,g'`

PLAYMUS_SOURCES="$srcdir/playmus.c"
PLAYMUS_OBJECTS=`echo $PLAYMUS_SOURCES`
PLAYMUS_DEPENDS=`echo $PLAYMUS_SOURCES`
PLAYMUS_OBJECTS=`echo "$PLAYMUS_OBJECTS" | sed 's,[[^ ]]*/\([[^ ]]*\)\.c,$(objects)/\1.lo,g'`
PLAYMUS_DEPENDS=`echo "$PLAYMUS_DEPENDS" | sed 's,\([[^ ]]*\)/\([[^ ]]*\)\.c,\\
$(objects)/\2.lo: \1/\2.c \$(objects)/.created\\
	\$(LIBTOOL) --mode=compile \$(CC) \$(CFLAGS) \$(EXTRA_CFLAGS) '"$DEPENDENCY_TRACKING_OPTIONS"' -c \$< -o \$@,g'`
PLAYMUS_DEPENDS=`echo "$PLAYMUS_DEPENDS" | sed 's,\\$,\\\\$,g'`

dnl Expand the sources and objects needed to build the library
AC_SUBST(ac_aux_dir)
AC_SUBST(OBJECTS)
AC_SUBST(VERSION_OBJECTS)
AC_SUBST(PLAYWAVE_OBJECTS)
AC_SUBST(PLAYMUS_OBJECTS)
AC_SUBST(BUILD_CFLAGS)
AC_SUBST(EXTRA_CFLAGS)
AC_SUBST(BUILD_LDFLAGS)
AC_SUBST(EXTRA_LDFLAGS)
AC_SUBST(EXE)
AC_SUBST(SDL_VERSION)
AC_SUBST(SDL_CFLAGS)
AC_SUBST(SDL_LIBS)

AC_CONFIG_FILES([
    Makefile SDL2_mixer.spec SDL2_mixer.pc
])
AC_CONFIG_COMMANDS([default],
                   [cat >>Makefile <<__EOF__

# Build rules for objects
-include \$(OBJECTS:.lo=.d)
$DEPENDS
$VERSION_DEPENDS

-include \$(PLAYWAVE_OBJECTS:.lo=.d)
$PLAYWAVE_DEPENDS

-include \$(PLAYMUS_OBJECTS:.lo=.d)
$PLAYMUS_DEPENDS
__EOF__
], [
DEPENDS="$DEPENDS"
VERSION_DEPENDS="$VERSION_DEPENDS"
PLAYWAVE_DEPENDS="$PLAYWAVE_DEPENDS"
PLAYMUS_DEPENDS="$PLAYMUS_DEPENDS"
])
AC_OUTPUT
