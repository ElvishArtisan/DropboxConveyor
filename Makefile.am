## Makefile.am
##
## (C) Copyright 2025 Fred Gleason <fredg@paravelsystems.com>
##
##
##   This program is free software; you can redistribute it and/or modify
##   it under the terms of the GNU General Public License as
##   published by the Free Software Foundation; either version 2 of
##   the License, or (at your option) any later version.
##
##   This program is distributed in the hope that it will be useful,
##   but WITHOUT ANY WARRANTY; without even the implied warranty of
##   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
##   GNU General Public License for more details.
##
##   You should have received a copy of the GNU General Public
##   License along with this program; if not, write to the Free Software
##   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
##
## Use automake to process this into a Makefile.in

SUBDIRS = conf\
          debian\
          helpers\
          src\
          systemd

deb:	dist
	./build_debs.sh

rpm:    dist
	mkdir -p $(HOME)/rpmbuild/SOURCES
	cp dropbox-conveyor-$(VERSION).tar.gz $(HOME)/rpmbuild/SOURCES/
	$(RPMBUILD) -ba --target $(ARCH)-$(VENDOR)-linux dropbox-conveyor.spec
	mv $(HOME)/rpmbuild/RPMS/$(ARCH)/dropbox-conveyor-*.rpm $(top_srcdir)/
	mv $(HOME)/rpmbuild/SRPMS/dropbox-conveyor-*.src.rpm $(top_srcdir)/
	rm $(HOME)/rpmbuild/SOURCES/dropbox-conveyor-$(VERSION).tar.gz


EXTRA_DIST = build_debs.sh.in\
             dropbox-conveyor.spec.in\
             get_distro.pl\
             get_target.sh\
             PACKAGE_VERSION

CLEANFILES = *~\
             *.buildinfo\
             *.changes\
             *.ddeb\
             *.deb\
             *.dll\
             *.dsc\
             *.exe\
             *.mo\
             *.rpm\
             dropbox-conveyor*.tar.gz\
             *.tar.xz\
             *.tgz\
             *.zip

DISTCLEANFILES = build_debs.sh\
                 config.guess\
                 config.sub\
                 glassplayer.spec\
                 ltmain.sh

MAINTAINERCLEANFILES = *~\
                       aclocal.m4\
                       compile\
                       configure\
                       depcomp\
                       install-sh\
                       Makefile.in\
                       missing
