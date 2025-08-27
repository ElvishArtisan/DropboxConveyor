dnl configure.ac
dnl
dnl Autoconf configuration
dnl
dnl Use autoconf to process this into a configure script
dnl
dnl   (C) Copyright 2025 Fred Gleason <fredg@paravelsystems.com>
dnl
dnl   All Rights Reserved.
dnl

AC_INIT(dropbox-conveyor,m4_esyscmd(cat PACKAGE_VERSION))
AM_INIT_AUTOMAKE
AC_SUBST(RPM_RELEASE,1)
m4_ifdef([AM_SILENT_RULES],[AM_SILENT_RULES([yes])])

#
# Option Switches
#
AC_ARG_WITH(qt5,[  --with-qt5              force build using Qt v5],[REQUIRE_QT5=yes],[])
AC_ARG_WITH(qt6,[  --with-qt6              force build using Qt v6],[REQUIRE_QT6=yes],[])
if test $REQUIRE_QT5 ; then
  if test $REQUIRE_QT6 ; then
    AC_MSG_ERROR(['--with-qt5' and '--with-qt6' arguments are mutually exclusive])
  fi
fi

#
# Basic Compiler Checks
#
AC_PROG_CXX
AC_LANG(C++)

#
# Determine the target architecture
#
AR_GCC_TARGET()
AC_SUBST(VENDOR,$ar_gcc_distro)
AC_SUBST(ARCH,$ar_gcc_arch)

#
# Determine Distro
#
AR_GET_DISTRO()
AC_SUBST(DISTRO,$ar_gcc_distro)

#
# Check for Qt (Mandatory)
#
if test -z $REQUIRE_QT5 ; then
  AQ_FIND_QT6(QT,Core Gui Widgets Network,[USING_QT6=yes],[])
  if test $USING_QT6 ; then
    AC_SUBST(QT_STD_ARGS,-std=c++17)
    AC_SUBST(COMPAT_QT_KEEP_EMPTY_PARTS,Qt::KeepEmptyParts)
    AC_SUBST(COMPAT_QT_SKIP_EMPTY_PARTS,Qt::SkipEmptyParts)
  else
    if test $REQUIRE_QT6 ; then
      AC_MSG_ERROR([*** Qt6 not found ***])
    fi
  fi
fi
if test -z $USING_QT6 ; then
  PKG_CHECK_MODULES(QT,Qt5Core Qt5Gui Qt5Widgets Qt5Network,[],[AC_MSG_ERROR([*** Qt5 not found ***])])
  AC_SUBST(QT_STD_ARGS,-std=c++11)
    AC_SUBST(COMPAT_QT_KEEP_EMPTY_PARTS,QString::KeepEmptyParts)
    AC_SUBST(COMPAT_QT_SKIP_EMPTY_PARTS,QString::SkipEmptyParts)
  AC_CHECK_PROG(MOC_NAME,moc-qt5,[moc-qt5],[moc])
  AC_SUBST(QT_MOC,$MOC_NAME)
  USING_QT5=yes
fi

#
# Configure RPM Build
#
AC_CHECK_PROG(RPMBUILD_FOUND,rpmbuild,[yes],[])
if test -z $RPMBUILD_FOUND ; then
  AC_SUBST(RPMBUILD,rpm)
else
  AC_SUBST(RPMBUILD,rpmbuild)
fi
if test -d /usr/src/redhat ; then
  AC_SUBST(RPM_ROOT,/usr/src/redhat)
  AC_SUBST(VENDOR,redhat)
else
  AC_SUBST(RPM_ROOT,/usr/src/packages)
  AC_SUBST(VENDOR,suse)
fi 

#
# Generated File Warnings
#
AC_SUBST_FILE(GENERATED_C_FILE_WARNING)
GENERATED_C_FILE_WARNING=$srcdir/helpers/c_file_warning

AC_SUBST_FILE(GENERATED_SCRIPT_FILE_WARNING)
GENERATED_SCRIPT_FILE_WARNING=$srcdir/helpers/script_file_warning

AC_SUBST_FILE(GENERATED_XML_FILE_WARNING)
GENERATED_XML_FILE_WARNING=$srcdir/helpers/xml_file_warning

AC_CONFIG_FILES([ \
    conf/Makefile \
    debian/Makefile \
    debian/source/Makefile \
    helpers/Makefile \
    src/compat.h \
    src/Makefile \
    systemd/dbconvd.service \
    systemd/Makefile \
    build_debs.sh \
    Makefile \
])
AC_OUTPUT()
chmod 755 build_debs.sh

AC_MSG_NOTICE()
AC_MSG_NOTICE("|-----------------------------------------------------|")
AC_MSG_NOTICE("|    *** DropboxConveyor CONFIGURATION SUMMARY ***    |")
AC_MSG_NOTICE("|-----------------------------------------------------|")
if test $USING_QT5 ; then
AC_MSG_NOTICE("|                   Using Qt Major Version ... 5.x    |")
fi
if test $USING_QT6 ; then
AC_MSG_NOTICE("|                   Using Qt Major Version ... 6.x    |")
fi
AC_MSG_NOTICE("|-----------------------------------------------------|")
AC_MSG_NOTICE()
AC_MSG_NOTICE(Now enter 'make' to build the software.)
AC_MSG_NOTICE()
