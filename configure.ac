dnl
dnl Process this file in order to make a configure script
dnl

dnl Setup autoconf
AC_INIT([riscv-bbl], [0.3], [arei@altlinux.org])
dnl AM_INIT_AUTOMAKE

dnl Debug option
AC_ARG_ENABLE(debug,
              AS_HELP_STRING([--enable-debug],[enable debugging, defult: no]),
              [case "${enableval}" in
               yes) debug = true  ;;
               no)  debug = false ;;
               *)   AC_MSG_ERROR([bad value ${enablevalue} for --enable-debug]) ;;
             esac], [debug=false])

dnl Check dependences
echo "Cheking dependences"
AC_LANG(C)
AC_CHECK_TOOL(OBJCOPY, objcopy)

dnl Disable all CFLAGS if it was not declarated
if [test -z $CFLAGS; ] then
  if test x"${debug}" == x"true"; then
    CFLAGS="-g -O0"
  else
    CFLAGS="-O2"
  fi
fi
AC_PROG_CC

if [ test "x$OBJCOPY" == "x"; ] then
  AC_MSG_ERROR([objcopy from GNU binutils required])
fi

dnl Get source code path
dnl AC_DEFINE_UNQUOTED([SRC_PATH], ["$srcdir"], [source path])

dnl Configure Makefile from Makefile.in
AC_CONFIG_FILES([$srcdir/Makefile])

AC_OUTPUT
mv $srcdir/Makefile .
