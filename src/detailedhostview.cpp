/*
    This file is part of Icecream.

    Copyright (c) 2004-2006 Andre Wöbbeking <Woebbeking@web.de>
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

#include <QLabel>
#include <QBoxLayout>
#include <QSplitter>

#include "detailedhostview.h"

#include "hostinfo.h"
#include "hostlistview.h"
#include "joblistview.h"

#include <sys/utsname.h>


static QString myHostName()
{
    struct utsname uname_buf;
    if (::uname(&uname_buf) == 0)
        return uname_buf.nodename;
    else
        return QString();
}

DetailedHostView::DetailedHostView(HostInfoManager* manager, QWidget* parent)
    : QWidget(parent),
      StatusView(manager)
{
    QBoxLayout* topLayout = new QVBoxLayout(this);
    topLayout->setMargin(10);

    QSplitter* viewSplitter = new QSplitter(Qt::Vertical, this);
    topLayout->addWidget(viewSplitter);

    QWidget *hosts = new QWidget(viewSplitter);
    QVBoxLayout *dummy = new QVBoxLayout(hosts);
    dummy->setSpacing(10);
    dummy->setMargin(0);

    dummy->addWidget(new QLabel(tr("Hosts"), hosts));
    m_hostListView = new HostListView(manager, hosts);
    dummy->addWidget(m_hostListView);

    QWidget *locals = new QWidget(viewSplitter);
    dummy = new QVBoxLayout(locals);
    dummy->setSpacing(10);
    dummy->setMargin(0);

    dummy->addWidget(new QLabel(tr("Outgoing jobs"), locals));
    m_localJobsView = new JobListView(manager, locals);
    m_localJobsView->setClientColumnVisible(false);
    m_localJobsView->setExpireDuration(5);
    dummy->addWidget(m_localJobsView);

    QWidget* remotes = new QWidget(viewSplitter);
    dummy = new QVBoxLayout(remotes);
    dummy->setSpacing(10);
    dummy->setMargin(0);

    dummy->addWidget(new QLabel(tr("Incoming jobs"), remotes));
    m_remoteJobsView = new JobListView(manager, remotes);
    m_remoteJobsView->setServerColumnVisible(false);
    m_remoteJobsView->setExpireDuration(5);
    dummy->addWidget(m_remoteJobsView);

    connect(m_hostListView, SIGNAL(nodeActivated(unsigned int)),
          this, SLOT(slotNodeActivated()));

    createKnownHosts();
}

void DetailedHostView::update(const Job &job)
{
    const unsigned int hostid = m_hostListView->activeNode();

    if (!hostid)
        return;

    if (job.client() != hostid && job.server() != hostid)
        return;

    if (job.client() == hostid)
        m_localJobsView->update(job);
    if (job.server() == hostid)
        m_remoteJobsView->update(job);
}

void DetailedHostView::checkNode(unsigned int hostid)
{
    if (!hostid)
        return;

    m_hostListView->checkNode(hostid);

    const unsigned int activeNode = m_hostListView->activeNode();

    if (!activeNode) {
        HostInfo* info = hostInfoManager()->find(hostid);
        if (info->name() == myHostName())
            m_hostListView->setActiveNode(hostid);
    }
}

void DetailedHostView::removeNode(unsigned int hostid)
{
    m_hostListView->removeNode(hostid);
}

void DetailedHostView::updateSchedulerState(bool online)
{
    if (!online) {
        m_hostListView->clear();
        m_localJobsView->clear();
        m_remoteJobsView->clear();
    }
}

void DetailedHostView::slotNodeActivated()
{
    m_localJobsView->clear();
    m_remoteJobsView->clear();
}

void DetailedHostView::createKnownHosts()
{
    const HostInfoManager::HostMap& hosts(hostInfoManager()->hostMap());
    HostInfoManager::HostMap::ConstIterator it(hosts.begin()), itEnd(hosts.end());
    for (; it != itEnd; ++it) {
        const unsigned int hostid((*it)->id());
        checkNode(hostid);
    }
}

QWidget* DetailedHostView::widget()
{
  return this;
}

#include "detailedhostview.moc"
