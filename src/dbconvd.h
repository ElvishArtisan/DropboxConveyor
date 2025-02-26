// dbconvd.h
//
// System for transporting files from a remote system.
//
//   (C) Copyright 2025 Fred Gleason <fredg@paravelsystems.com>
//
//   This program is free software; you can redistribute it and/or modify
//   it under the terms of the GNU General Public License as
//   published by the Free Software Foundation; either version 2 of
//   the License, or (at your option) any later version.
//
//   This program is distributed in the hope that it will be useful,
//   but WITHOUT ANY WARRANTY; without even the implied warranty of
//   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//   GNU General Public License for more details.
//
//   You should have received a copy of the GNU General Public
//   License along with this program; if not, write to the Free Software
//   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
//

#ifndef DBCONVD_H
#define DBCONVD_H

#include <QCoreApplication>
#include <QObject>
#include <QProcess>

#define DBCONVD_SCAN_INTERVAL 5000
#define DBCONVD_USAGE "[--debug] [--dump-config] [--dump-config]\n"

#include "config.h"

class MainObject : public QObject
{
  Q_OBJECT
 public:
  MainObject();

 private:
  bool RunConveyor(int num,bool dry_run=false);
  bool Run(const QString &pgm,const QStringList &args);
  bool IsMounted(const QString &mntpt);
  Config *d_config; 
};


#endif  // DBCONVD_H
