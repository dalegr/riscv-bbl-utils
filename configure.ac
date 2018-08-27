dnl
dnl Process this file in order to make a configure script
dnl

dnl Setup autoconf
AC_INIT([riscv-bbl], [0.1], [arei@basealt.ru])
dnl AM_INIT_AUTOMAKE

dnl debug option
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
AC_CHECK_TOOL(GDISK, gdisk)
AC_CHECK_TOOL(SGDISK, sgdisk)

dnl disable all CFLAGS if it was not declarated
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

if [ test "x$GDISK" == "x"; ] then
  AC_MSG_ERROR([gdisk from gdisk package required])
fi

if [ test "x$SGDISK" == "x"; ] then
  AC_MSG_ERROR([sgdisk from gdisk package required])
fi

dnl Configure Makefile from Makefile.in
AC_OUTPUT(Makefile)

AC_OUTPUT
