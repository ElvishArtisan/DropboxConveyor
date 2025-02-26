// config.h
//
// Configuration for dbconvd(8)
//
//   (C) Copyright 2025 Fred Gleason <fredg@paravelsystems.com>
//
//    This program is free software; you can redistribute it and/or modify
//    it under the terms of version 2.1 of the GNU Lesser General Public
//    License as published by the Free Software Foundation;
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU Lesser General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with this program; if not, write to the Free Software
//    Foundation, Inc., 59 Temple Place, Suite 330, 
//    Boston, MA  02111-1307  USA
//

#ifndef CONFIG_H
#define CONFIG_H

#include <QString>
#include <QStringList>

#define CONFIG_DEFAULT_LOOP_INTERVAL 10
#define DBCONVD_CONFIG_FILE "/etc/dbconvd.conf"

class Config
{
 public:
  Config();
  int loopInterval() const;
  int conveyorQuantity();
  QString sourceMountPoint(int n) const;
  QString pickupPath(int n) const;
  QString dump();
  void load();
  void clear();
  
 private:
  int d_loop_interval;
  QStringList d_source_mount_points;
  QStringList d_pickup_paths;
};


#endif  // CONFIG_H
