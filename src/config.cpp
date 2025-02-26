// config.cpp
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

#include "config.h"
#include "profile.h"

Config::Config()
{
  clear();
}


int Config::loopInterval() const
{
  return d_loop_interval;
}


int Config::conveyorQuantity()
{
  return d_source_mount_points.size();
}


QString Config::sourceMountPoint(int n) const
{
  return d_source_mount_points.at(n);
}


QString Config::pickupPath(int n) const
{
  return d_pickup_paths.at(n);
}


QString Config::dump()
{
  QString ret;

  ret+="[Global]\n";
  ret+=QString::asprintf("LoopInterval=%d\n",d_loop_interval);
  ret+="\n";

  for(int i=0;i<d_source_mount_points.size();i++) {
    ret+=QString::asprintf("[Conveyor%d]\n",1+i);
    ret+="PickupPath="+d_pickup_paths.at(i)+"\n";
    ret+="SourceMountPoint="+d_source_mount_points.at(i)+"\n";
    ret+="\n";
  }
  
  return ret;
}


void Config::load()
{
  bool ok=false;

  clear();

  Profile *p=new Profile();
  if(p->loadFile(DBCONVD_CONFIG_FILE)) {
    d_loop_interval=
      p->intValue("Global","LoopInterval",CONFIG_DEFAULT_LOOP_INTERVAL);
  
    int count=0;
    QString section=QString::asprintf("Conveyor%d",1+count);
    QString src_pt=p->stringValue(section,"SourceMountPoint","",&ok);
    while(!src_pt.isEmpty()) {
      d_source_mount_points.push_back(src_pt);
      d_pickup_paths.push_back(p->stringValue(section,"PickupPath"));
      count++;
      section=QString::asprintf("Conveyor%d",1+count);
      src_pt=p->stringValue(section,"SourceMountPoint","",&ok);
    }
  }
  delete p;
}


void Config::clear()
{
  d_loop_interval=CONFIG_DEFAULT_LOOP_INTERVAL;
  d_source_mount_points.clear();
  d_pickup_paths.clear();
}
