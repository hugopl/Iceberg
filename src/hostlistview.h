/*
    This file is part of Icecream.

    Copyright (c) 2004 Andre Wöbbeking <Woebbeking@web.de>

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

#ifndef ICEBERG_HOSTLISTVIEW_H
#define ICEBERG_HOSTLISTVIEW_H

#include "hostinfo.h"

#include <QPalette>
#include <Q3ListView>
#include <QTimer>

class HostListViewItem : public Q3ListViewItem
{
public:
    HostListViewItem(Q3ListView* parent, const HostInfo& info);

    const HostInfo& hostInfo() const;
    void setActiveNode(bool active);
    void updateText(const HostInfo& info);

    virtual int compare(Q3ListViewItem* i, int col, bool ascending) const;
    virtual void paintCell(QPainter* painter, const QColorGroup& cg, int column, int width, int align);
    virtual int width(const QFontMetrics& fm, const Q3ListView* lv, int column) const;

private:
    HostInfo m_hostInfo;
    bool m_active;
};


class HostListView : public Q3ListView
{
    Q_OBJECT

public:
    HostListView(HostInfoManager* manager, QWidget* parent);

    unsigned int activeNode() const;
    void setActiveNode(unsigned int hostid);

    void checkNode(unsigned int hostid);
    void removeNode(unsigned int hostid);
    virtual void clear();

signals:
    void nodeActivated(unsigned int hostid);

private slots:
    void slotNodeActivated(Q3ListViewItem* item);
    void updateSort();

private:
    void setActiveNode(unsigned int hostid, bool active);

    HostInfoManager* m_hostInfoManager;
    unsigned int m_activeNode;

    typedef QMap<unsigned int, HostListViewItem*> ItemMap;
    ItemMap m_items;

    QTimer m_updateSortTimer;
};


#endif
