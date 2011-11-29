/*
    This file is part of Icecream.

    Copyright (c) 2003 Frerich Raabe <raabe@kde.org>
    Copyright (c) 2003,2004 Stephan Kulow <coolo@kde.org>
    Copyright (c) 2003,2004 Cornelius Schumacher <schumacher@kde.org>
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
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSystemTrayIcon>

class HostInfoManager;
class Monitor;
class StatusView;
class QActionGroup;
class QMenu;
class QCloseEvent;

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget* parent);
    ~MainWindow();

    void setCurrentNet(const QByteArray& netName);

protected:
    void closeEvent(QCloseEvent* event);

private slots:
    void setupListView();
    void setupStarView();
    void setupDetailedHostView();

    void stopView();
    void startView();
    void checkNodes();
    void configureView();
    void showAboutDialog();

    void systemTrayIconActivated(QSystemTrayIcon::ActivationReason reason);

private:
    void readSettings();
    void writeSettings();

    void setupView(StatusView* view, bool rememberJobs);

    HostInfoManager* m_hostInfoManager;
    Monitor* m_monitor;
    StatusView* m_view;

    enum views {
        ListViewType,
        StarViewType,
        PoolViewType,
        GanttViewType,
        SummaryViewType,
        DetailedHostViewType
    };

    QActionGroup* m_viewMode;
    QAction* m_listView;
    QAction* m_starView;
    QAction* m_detailedView;
    QSystemTrayIcon* systemTrayIcon;
    QMenu* systemTrayMenu;
};

#endif // MAINWINDOW_H
