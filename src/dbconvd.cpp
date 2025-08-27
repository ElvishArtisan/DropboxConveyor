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

#include <stdio.h>
#include <stdlib.h>
#include <syslog.h>
#include <sys/mount.h>
#include <unistd.h>

#include <QDir>

#include "cmdswitch.h"
#include "compat.h"
#include "dbconvd.h"

MainObject::MainObject()
{
  CmdSwitch *cmd=new CmdSwitch("dbconvd",VERSION,DBCONVD_USAGE);
  bool debug=false;
  bool dry_run=false;
  bool dump_config=false;

  for(int i=0;i<cmd->keys();i++) {
    if(cmd->key(i)=="--debug") {
      debug=true;
      cmd->setProcessed(i,true);
    }
    if(cmd->key(i)=="--dry-run") {
      dry_run=true;
      debug=true;
      cmd->setProcessed(i,true);
    }
    if(cmd->key(i)=="--dump-config") {
      dump_config=true;
      cmd->setProcessed(i,true);
    }
    if(!cmd->processed(i)) {
      fprintf(stderr,"dbconvd: unknown option \"%s\"\n",
	      cmd->key(i).toUtf8().constData());
      exit(1);
    }
  }

  //
  // Open Syslog
  //
  if(debug) {
    openlog("dbconvd",LOG_PERROR,LOG_USER);
  }
  else {
    openlog("dbconvd",0,LOG_USER);
  }

  //
  // Open Configuration
  //
  d_config=new Config();
  d_config->load();
  syslog(LOG_INFO,"loaded %d conveyors",d_config->conveyorQuantity());
  if(dump_config) {
    printf("%s",d_config->dump().toUtf8().constData());
    exit(0);
  }

  //
  // Dry Run
  //
  if(dry_run) {
    int errors=0;
    for(int i=0;i<d_config->conveyorQuantity();i++) {
      if(!RunConveyor(i,dry_run)) {
	errors++;
      }
    }
    if(errors>0) {
      printf("%d errors seen\n",errors);
      exit(1);
    }
    printf("no errors seen\n");
    exit(0);
  }
  
  //
  // Main Loop
  //
  while(1==1) {
    for(int i=0;i<d_config->conveyorQuantity();i++) {
      RunConveyor(i);
    }
    sleep(10);
  }
  
  exit(0);
}


bool MainObject::RunConveyor(int num,bool dry_run)
{
  QStringList args;

  //
  // Ensure That The Source Is Mounted
  //
  if(!IsMounted(d_config->sourceMountPoint(num))) {
    QDir srcdir(d_config->sourceMountPoint(num));
    if(!srcdir.exists()) {
      if(!srcdir.mkdir("/")) {
	syslog(LOG_ERR,"failed to create source mountpoint at \"%s\" [%s]",
	       d_config->sourceMountPoint(num).toUtf8().constData(),
	       strerror(errno));
	return false;
      }
    }
    args.clear();
    args.push_back(d_config->sourceMountPoint(num));
    if(!Run("/usr/bin/mount",args)) {
      return false;
    }
  }

  //
  // Run rsync(1)
  //
  if(!dry_run) {
    args.clear();
    args.push_back("-a");
    args.push_back("--delete");
    args.push_back(d_config->sourceMountPoint(num).trimmed());
    args.push_back(d_config->pickupPath(num).trimmed());
    if(!Run("/usr/bin/rsync",args)) {
      return false;
    }
  }
  else {
    sleep(5);
  }

  return true;
}


bool MainObject::Run(const QString &pgm,const QStringList &args)
{
  QProcess *proc=new QProcess();
  proc->start(pgm,args);
  proc->waitForFinished(-1);
  if(proc->exitStatus()!=QProcess::NormalExit) {
    syslog(LOG_WARNING,"process crashed: \"%s %s",
	   proc->program().toUtf8().constData(),
	   proc->arguments().join(" ").toUtf8().constData());
    delete proc;
    return false;
  }
  else {
    if(proc->exitCode()!=0) {
      syslog(LOG_WARNING,
	     "process returned non-zero exit code %d: \"%s %s\":\"%s\"",
	     proc->exitCode(),
	     proc->program().toUtf8().constData(),
	     proc->arguments().join(" ").toUtf8().constData(),
	     proc->readAllStandardError().constData());
      delete proc;
      return false;
    }
  }
  delete proc;
  return true;
}


bool MainObject::IsMounted(const QString &mntpt)
{
  FILE *f=NULL;
  char line[1024];
  
  if((f=fopen("/etc/mtab","r"))!=NULL) {
    while(fgets(line,1023,f)!=NULL) {
      QStringList f0=
	QString::fromUtf8(line).trimmed().split("\n",COMPAT_QT_SKIP_EMPTY_PARTS);
      for(int i=0;i<f0.size();i++) {
	QStringList f1=f0.at(i).trimmed().split(" ",COMPAT_QT_SKIP_EMPTY_PARTS);
	if(f1.size()>=2) {
	  if(f1.at(1)==mntpt) {
	    fclose(f);
	    //printf("YES\n");
	    return true;
	  }
	}
      }
    }
    fclose(f);
  }
  //printf("NO\n");
  return false;
}


int main(int argc,char *argv[]) {
  QCoreApplication a(argc,argv);

  new MainObject();

  return a.exec();
}
