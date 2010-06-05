AC_INIT([FMMS_STATUS], [0.1])
AM_INIT_AUTOMAKE([foreign])
AC_PROG_CC
AC_PROG_INSTALL
AC_PROG_LIBTOOL
PKG_CHECK_MODULES([FMMS_STATUS], [
	gtk+-2.0 >= 2.4.0
	glib-2.0 >= 2.2.0
	hildon-1 >= 1.0.9
	libhildondesktop-1 >= 2.1.0
])  
			    
AC_SUBST(FMMS_STATUS)
AC_SUBST(FMMS_STATUS_CFLAGS)

hildondesktoplibdir="${libdir}/hildon-desktop"
AC_SUBST(hildondesktoplibdir)

hildondesktoplibdir="${libdir}/hildon-desktop"
AC_SUBST(hildondesktoplibdir)

AC_CONFIG_FILES([
	Makefile
	data/Makefile
	src/Makefile
]) 
AC_OUTPUT

