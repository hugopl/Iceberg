/*
    This file is part of Icecream.

    Copyright (c) 2004 Cornelius Schumacher <schumacher@kde.org>
    Copyright (c) 2011 Hugo Parente Lima <hugo.pl@gmail.com>
    Copyright (c) 2012 Anselmo L. S. Melo <anselmolsm@gmail.com>

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

#include "hostinfo.h"
#include "utils.h"

#include <assert.h>
#include <QDebug>

HostInfo::HostInfo(unsigned int id) : m_id(id)
{
}

void HostInfo::updateFromStatsMap(const StatsMap& stats)
{
    const QString& name = stats["Name"];

    if (name != m_name) {
        m_name = name;

        m_color = Utils::colorByName(m_name);

        m_ip = stats["IP"];
        m_platform = stats["Platform"];
    }

    m_maxJobs = stats["MaxJobs"].toUInt();
    m_offline = stats["State"] == "Offline";
    m_serverSpeed = stats["Speed"].toFloat();
    m_serverLoad = stats["Load"].toUInt();
}
HostInfoManager::HostInfoManager()
{
    Utils::initCrc24Table();
}

HostInfoManager::~HostInfoManager()
{
    qDeleteAll(m_hostMap);
}

HostInfo* HostInfoManager::find(unsigned int hostid) const
{
    return m_hostMap.value(hostid, 0);
}

HostInfo* HostInfoManager::checkNode(unsigned int hostid, const HostInfo::StatsMap& stats)
{
    HostMap::ConstIterator it = m_hostMap.constFind(hostid);
    HostInfo* hostInfo;
    if (it == m_hostMap.constEnd()) {
        hostInfo = new HostInfo(hostid);
        m_hostMap.insert(hostid, hostInfo);
    } else {
        hostInfo = *it;
    }

    hostInfo->updateFromStatsMap(stats);

    return hostInfo;
}

QString HostInfoManager::nameForHost(unsigned int id) const
{
    HostInfo* hostInfo = find(id);
    if (hostInfo)
        return hostInfo->name();

    return tr("<unknown>");
}

QColor HostInfoManager::hostColor(unsigned int id) const
{
    if (id) {
        HostInfo* hostInfo = find(id);
        if (hostInfo) {
            QColor tmp = hostInfo->color();
            assert(tmp.isValid() && (tmp.red() + tmp.green() + tmp.blue()));
            return tmp;
        }
    }

    qWarning() << "id " << id << " got no color\n";
    assert(false);

    return QColor(0, 0, 0);
}

unsigned int HostInfoManager::maxJobs(unsigned int id) const
{
    if (id) {
        HostInfo* hostInfo = find(id);
        if (hostInfo)
            return hostInfo->maxJobs();
    }

    return 0;
}

HostInfoManager::HostMap HostInfoManager::hostMap() const
{
    return m_hostMap;
}
