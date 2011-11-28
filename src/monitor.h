/*
    This file is part of Icecream.

    Copyright (c) 2003 Frerich Raabe <raabe@kde.org>
    Copyright (c) 2003,2004 Stephan Kulow <coolo@kde.org>
    Copyright (c) 2003,2004 Cornelius Schumacher <schumacher@kde.org>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/
#ifndef ICEMON_MONITOR_H
#define ICEMON_MONITOR_H

#include "job.h"

#include <QtCore/QSocketNotifier>

class HostInfoManager;
class Msg;
class MsgChannel;
class StatusView;
class DiscoverSched;
class QSocketNotifier;

class Monitor : public QObject
{
    Q_OBJECT
  public:
    Monitor( HostInfoManager*, QObject* parent);
    ~Monitor();

    void setCurrentNet( const QByteArray& );
    QByteArray currentNet () const;

    void setCurrentView( StatusView*, bool rememberJobs );
    void checkScheduler(bool deleteit = false);

  protected:
    void setSchedulerState( bool );

  private slots:
    void slotCheckScheduler();
    void msgReceived();

  private:
    void registerNotify(int fd, QSocketNotifier::Type type, const char* slot);
    bool handleActivity();
    void handleGetcs( Msg* m );
    void handleJobBegin( Msg* m );
    void handleJobDone( Msg* m );
    void handleStats( Msg* m );
    void handleLocalBegin( Msg* m );
    void handleLocalDone( Msg* m );

    HostInfoManager* m_hostInfoManager;
    StatusView* m_view;
    JobList m_rememberedJobs;
    MsgChannel* m_scheduler;
    QByteArray m_currentNetName;
    bool m_schedulerOnline;

    DiscoverSched* m_discover;
    QSocketNotifier* m_fd_notify;
    QSocketNotifier::Type m_fd_type;

};

#endif // MON_KDE_H
// vim:ts=4:sw=4:noet
