#                                               -*- Autoconf -*- 
# Process this file with autoconf to produce a configure script. 
 
AC_PREREQ([2.69]) 
AC_INIT([SCP_Libraries],[0.01],[sobhaniiest@gmail.com],[https://github.com/sobhaniiest/SCP_Libraries]) 
AM_INIT_AUTOMAKE([-Wall -Werror foreign])  

: ${CFLAGS=""}

# Check for a C compiler 
AC_PROG_CC 

#Check for archiver
AM_PROG_AR

LT_INIT

PKG_CHECK_MODULES([GIO],[gio-2.0]) 
PKG_CHECK_MODULES([GIOUNIX],[gio-unix-2.0]) 
PKG_CHECK_MODULES([GLIB],[glib-2.0]) 
PKG_CHECK_MODULES([GTK],[gtk+-3.0])

# Checks for header files. 
AC_CHECK_HEADERS([stdio.h stdlib.h string.h unistd.h fcntl.h])
AC_CHECK_HEADERS([cups/cups.h stdbool.h cups/ppd.h ctype.h])
AC_CHECK_HEADERS([pthread.h sys/types.h libgen.h locale.h])
 
# Checks for typedefs, structures, and compiler characteristics. 
AC_TYPE_SIZE_T 
 
# Checks for library functions.
AC_FUNC_MALLOC
AC_CHECK_FUNCS([getline strstr dup])

AC_CONFIG_FILES([Makefile
                 lib/Makefile
                 SCPService.pc])

AC_OUTPUT