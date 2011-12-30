/*
    This file is part of Icecream.

    Copyright (c) 2004 Cornelius Schumacher <schumacher@kde.org>
    Copyright (c) 2011 Hugo Parente Lima <hugo.pl@gmail.com>

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
#ifndef ICEBERG_HOSTINFO_H
#define ICEBERG_HOSTINFO_H

#include <QCoreApplication>
#include <QString>
#include <QColor>
#include <QMap>
#include <QtCore/QVector>

class HostInfo
{
    Q_DECLARE_TR_FUNCTIONS(HostInfo)
public:
    HostInfo(unsigned int id);

    unsigned int id() const { return m_id; }
    QString name() const { return m_name; }
    QColor color() const { return m_color; }
    QString ip() const { return m_ip; }
    QString platform() const { return m_platform; }
    unsigned int maxJobs() const { return m_maxJobs; }
    bool isOffline() const { return m_offline; }
    float serverSpeed() const { return m_serverSpeed; }
    unsigned int serverLoad() const { return m_serverLoad; }

    typedef QMap<QString,QString> StatsMap;
    void updateFromStatsMap(const StatsMap& stats);
private:
    unsigned int m_id;
    QString m_name;
    QColor m_color;
    QString m_platform;
    QString m_ip;
    unsigned int m_maxJobs;
    bool m_offline;
    float m_serverSpeed;
    unsigned int m_serverLoad;
};

class HostInfoManager
{
    Q_DECLARE_TR_FUNCTIONS(HostInfoManager)
public:
    HostInfoManager();
    ~HostInfoManager();

    HostInfo* find(unsigned int hostid) const;

    typedef QMap<unsigned int,HostInfo*> HostMap;
    HostMap hostMap() const;

    HostInfo* checkNode(unsigned int hostid, const HostInfo::StatsMap& statmsg);

    QString nameForHost(unsigned int id) const;
    QColor hostColor(unsigned int id) const;
    unsigned int maxJobs(unsigned int id) const;

    QString schedulerName() const { return m_schedulerName; }
    void setSchedulerName(const QString& name) { m_schedulerName = name; }

    QString networkName() const { return m_networkName; }
    void setNetworkName(const QString& name) { m_networkName = name; }

private:
    HostMap m_hostMap;
    QString m_schedulerName;
    QString m_networkName;
};

#endif
