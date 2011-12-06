/*
    This file is part of Icecream.

    Copyright (c) 2003 Frerich Raabe <raabe@kde.org>
    Copyright (c) 2003,2004 Stephan Kulow <coolo@kde.org>
    Copyright (c) 2004 Andre Wöbbeking <Woebbeking@web.de>
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

#include "listview.h"

#include "joblistview.h"

#include <QVBoxLayout>

ListStatusView::ListStatusView(HostInfoManager* manager, QWidget* parent)
    : QWidget(parent),
      StatusView(manager),
      m_jobsListView(new JobListView(manager, this, "Jobs"))
{
    QVBoxLayout* topLayout = new QVBoxLayout(this);
    topLayout->addWidget(m_jobsListView);
}

void ListStatusView::update(const Job &job)
{
    m_jobsListView->update(job);
}

#include "listview.moc"
