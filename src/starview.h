/*
    This file is part of Icecream.

    Copyright (c) 2003 Frerich Raabe <raabe@kde.org>
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
#ifndef ICEBERG_STARVIEW_H
#define ICEBERG_STARVIEW_H

#include "job.h"
#include "statusview.h"

#include <QDialog>
#include <QResizeEvent>
#include <QLabel>
#include <QGraphicsEllipseItem>

class HostInfo;

class QSlider;
class QLabel;
class QLineEdit;
class QCheckBox;
class QGraphicsView;

class StarViewConfigDialog : public QDialog
{
    Q_OBJECT

public:
    StarViewConfigDialog(QWidget *parent);

    int nodesPerRing();
    bool suppressDomainName() const;

    void setMaxNodes(int);

    QString archFilter();

protected slots:
    void slotNodesPerRingChanged(int nodes);
    void slotSuppressDomainName(bool);

signals:
    void configChanged();

private:
    QSlider *m_nodesPerRingSlider;
    QLabel *m_nodesPerRingLabel;
    QLineEdit *m_archFilterEdit;
    QCheckBox *m_suppressDomainName;
};


class HostItem : public QGraphicsItemGroup
{
public:
    enum { RttiHostItem = 1000 };

    HostItem(const QString &text, QGraphicsScene *canvas, HostInfoManager *);
    HostItem(HostInfo *hostInfo, QGraphicsScene *canvas, HostInfoManager *);
    ~HostItem();

    void init();

    HostInfo *hostInfo() const { return m_hostInfo; }

    void setHostColor(const QColor &color);

    void setIsActiveClient(bool active) { m_isActiveClient = active; }
    bool isActiveClient() const { return m_isActiveClient; }

    void setIsCompiling(bool compiling) { m_isCompiling = compiling; }
    bool isCompiling() const { return m_isCompiling; }

    void setStateItem(QGraphicsItem *item) { m_stateItem = item; }
    QGraphicsItem *stateItem() { return m_stateItem; }

    void setClient(unsigned int client) { m_client = client; }
    unsigned int client() const { return m_client; }

    QString hostName() const;
    void updateName();

    double centerPosX() const { return pos().x()+m_textItem->boundingRect().width()/2; }
    double centerPosY() const { return pos().y()+m_textItem->boundingRect().height()/2; }

    double relativeCenterPosX() const { return m_textItem->boundingRect().width()/2; }
    double relativeCenterPosY() const { return m_textItem->boundingRect().height()/2; }

    void setCenterPos(double x, double y);

    void update(const Job &job);

protected:
    void createJobHalo(const Job &);
    void deleteJobHalo(const Job &job);
    void updateHalos();

private:
    HostInfo *m_hostInfo;
    HostInfoManager *m_hostInfoManager;

    bool m_isActiveClient;
    bool m_isCompiling;

    QGraphicsItem *m_stateItem;
    QGraphicsSimpleTextItem* m_textItem;
    unsigned int m_client;

    qreal m_baseWidth;
    qreal m_baseHeight;

    QGraphicsEllipseItem *m_boxItem;

    QMap<Job, QGraphicsEllipseItem *> m_jobHalos;

    JobList m_jobs;
};


class StarView : public QWidget, public StatusView
{
    Q_OBJECT

public:
    StarView(HostInfoManager *, QWidget *parent);

    void update(const Job &job);
    QWidget *widget();

    QString id() const { return "star"; }

    HostItem *findHostItem(unsigned int hostid);

    void checkNode(unsigned int hostid);

    void removeNode(unsigned int hostid);

    void updateSchedulerState(bool online);

    bool isConfigurable() { return true; }

    void configureView();

protected:
    virtual void resizeEvent(QResizeEvent *e);
    virtual bool event(QEvent *event);

    /**
       Return true if node should be shown and false if not.
    */
    bool filterArch(unsigned int hostid);
    /**
       Return true if node should be shown and false if not.
    */
    bool filterArch(HostInfo *);

    void removeItem(HostItem *);
    void forceRemoveNode(unsigned int hostid);

    protected slots:
    void slotConfigChanged();

private:
    void createKnownHosts();
    void centerSchedulerItem();
    HostItem *createHostItem(unsigned int hostid);
    void arrangeHostItems();
    void drawNodeStatus();
    void drawState(HostItem *node);

    StarViewConfigDialog *m_configDialog;

    QGraphicsScene *m_canvas;
    QGraphicsView *m_canvasView;
    HostItem *m_schedulerItem;
    QMap<unsigned int,HostItem *> m_hostItems;
    QMap<unsigned int,HostItem *> m_jobMap;
    QString m_iconPath;
};

#endif
