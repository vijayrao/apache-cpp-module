SUBDIRS =	src example test
DISTFILES =     AUTHORS README COPYING aclocal.m4 config.guess configure \
		install-sh missing mkinstalldirs config.sub Makefile.am Makefile.in  \
		configure.in INSTALL ChangeLog NEWS libtool include \
		mod_cplusplus.dsp depcomp

install:
	@echo ...installing module.......
	cp src/.libs/libmod_cplusplus@SHLIB_EXT@ $(DESTDIR)@MODULES_DIR@
	@echo ...checking config file......
	if ! grep cplusplus $(DESTDIR)@CONF_DIR@/httpd.conf; then echo ...updating config file....;echo 'LoadModule cplusplus_module @MODULES_DIR@/libmod_cplusplus@SHLIB_EXT@' >> $(DESTDIR)@CONF_DIR@/httpd.conf ; else echo ***config file already contains modcplusplus***; fi
	@echo ...install complete...

