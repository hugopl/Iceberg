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

#include "mainwindow.h"
#include <QCloseEvent>
#include <QDesktopWidget>
#include <QDialogButtonBox>
#include <QMenu>
#include <QMenuBar>
#include <QSettings>
#include <QVBoxLayout>
#include "detailedhostview.h"
#include "hostinfo.h"
#include "listview.h"
#include "monitor.h"
#include "starview.h"
#include "version.h"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent), m_view(0)
{
    m_hostInfoManager = new HostInfoManager;

    m_monitor = new Monitor(m_hostInfoManager, this);

    m_viewMode = new QActionGroup(this);

    QMenu* fileMenu = menuBar()->addMenu(tr("&File"));
    QAction* quitAction = fileMenu->addAction(tr("&Quit"));
    connect(quitAction, SIGNAL(triggered()), this, SLOT(close()));

    QMenu* viewMenu = menuBar()->addMenu(tr("&View"));
    QMenu* modeMenu = viewMenu->addMenu(tr("&Mode"));

    m_listView = modeMenu->addAction(tr("&List View"));
    m_listView->setCheckable(true);
    m_viewMode->addAction(m_listView);
    connect( m_listView, SIGNAL(triggered()), this, SLOT(setupListView()));

    m_starView = modeMenu->addAction(tr("&Star View"));
    m_starView->setCheckable(true);
    m_viewMode->addAction(m_starView);
    connect( m_starView, SIGNAL(triggered()), this, SLOT(setupStarView()));

    m_detailedView = modeMenu->addAction(tr("&Detailed Host View"));
    m_detailedView->setCheckable(true);
    m_viewMode->addAction(m_detailedView);
    connect( m_detailedView, SIGNAL(triggered()), this, SLOT(setupDetailedHostView()));


    QAction* actionStart = viewMenu->addAction(tr("&Start"));
    connect( actionStart, SIGNAL(triggered()), this, SLOT(startView()));
    QAction* actionStop = viewMenu->addAction(tr("Stop"));
    connect( actionStop, SIGNAL(triggered()), this, SLOT(stopView()));
    viewMenu->addSeparator();
    QAction* actionCheckNodes = viewMenu->addAction(tr("Check Nodes"));
    connect( actionCheckNodes, SIGNAL(triggered()), this, SLOT(checkNodes()));
    viewMenu->addSeparator();
    m_configView = viewMenu->addAction(tr("Configure View..."));
    connect(m_configView, SIGNAL(triggered()), this, SLOT( configureView()));

    QMenu* helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(tr("About..."), this, SLOT(showAboutDialog()));

    // Avoid useless creation and connection if the system does not have a systray
    if (QSystemTrayIcon::isSystemTrayAvailable()) {
        systemTrayIcon = new QSystemTrayIcon(this);
        systemTrayIcon->setIcon(QIcon(":bigIcon.png"));
        systemTrayIcon->show();

        systemTrayMenu = new QMenu(this);
        systemTrayMenu->addAction(quitAction);

        systemTrayIcon->setContextMenu(systemTrayMenu);

        connect(systemTrayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
                this, SLOT(systemTrayIconActivated(QSystemTrayIcon::ActivationReason)));
    }

    setWindowIcon(QIcon(":bigIcon.png"));

    readSettings();
    m_monitor->checkScheduler();
}

MainWindow::~MainWindow()
{
    delete m_hostInfoManager;
}

void MainWindow::readSettings()
{
    QSettings cfg;
    QString viewId = cfg.value("CurrentView").toString();

    m_viewMode->blockSignals(true);

    if (viewId == "list") {
        setupListView();
        m_listView->setChecked(true);
    } else if (viewId == "detailedhost") {
        setupDetailedHostView();
        m_detailedView->setChecked(true);
    } else {
        setupStarView();
        m_starView->setChecked(true);
    }

    // "icecream" is the default netname used by iceccd
    if (m_monitor->currentNet().isEmpty()) {
        QByteArray netname = cfg.value("netname", "icecream").toByteArray();
        setCurrentNet(netname);
    }


    // Load window geometry
    QRect geom = cfg.value( "geometry" ).toRect();
    if (geom.isNull()) {
        QDesktopWidget w;
        QRect screenGeometry = w.availableGeometry( this );
        geom = QRect( screenGeometry.center()/2, screenGeometry.size()/2 );
    }
    setGeometry( geom );

    m_viewMode->blockSignals(false);
}

void MainWindow::writeSettings()
{
    QSettings cfg;
    cfg.setValue("netname", m_monitor->currentNet());
    cfg.setValue("CurrentView", m_view->id());
    cfg.setValue("geometry", this->geometry());
    cfg.sync();
}

void MainWindow::setupView(StatusView* view, bool rememberJobs)
{
    delete m_view;
    m_view = view;
    m_monitor->setCurrentView(m_view, rememberJobs);
    setCentralWidget(m_view->widget());
    m_view->widget()->show();
    m_configView->setEnabled(m_view->isConfigurable());
}

void MainWindow::setupListView()
{
    setupView(new ListStatusView(m_hostInfoManager, this), true);
}

void MainWindow::setupStarView()
{
    setupView(new StarView( m_hostInfoManager, this), false);
}

void MainWindow::setupDetailedHostView()
{
    setupView(new DetailedHostView(m_hostInfoManager, this), false);
}

void MainWindow::stopView()
{
    m_view->stop();
}

void MainWindow::startView()
{
    m_view->start();
}

void MainWindow::checkNodes()
{
    m_view->checkNodes();
}

void MainWindow::configureView()
{
    m_view->configureView();
}

void MainWindow::setCurrentNet(const QByteArray& netName)
{
    m_monitor->setCurrentNet(netName);
}

void MainWindow::showAboutDialog()
{
    QDialog dlg(0, Qt::Dialog);
    QVBoxLayout* layout = new QVBoxLayout();
    QLabel* text = new QLabel(
        "<table align=\"center\"><tr>"
        "<td><img source=\":/bigIcon.png\" align=\"center\"/></td>"
        "<td><h1 align=\"center\">Iceberg</h1><p align=\"center\">Version " ICEBERG_VERSION "</p></td>"
        "</tr></table>"
        "<p>Distributed under GPLv2, source code available at <a href=\"http://www.github.com/hugopl/Iceberg\">GitHub</a>!</p>"
        "<p>Copyright (c) 2003 Frerich Raabe &lt;raabe@kde.org><br>"
        "Copyright (c) 2003,2004 Stephan Kulow &lt;coolo@kde.org><br>"
        "Copyright (c) 2003,2004 Cornelius Schumacher &lt;schumacher@kde.org><br>"
        "Copyright (c) 2011 Hugo Parente Lima &lt;hugo.pl@gmail.com><br>"
        "Copyright (c) 2011 Anselmo L. S. Melo &lt;anselmolsm@gmail.com></p>");
    text->setOpenExternalLinks(true);
    layout->addWidget(text);
    QDialogButtonBox* buttonBox = new QDialogButtonBox(QDialogButtonBox::Close, Qt::Horizontal);
    buttonBox->setCenterButtons(true);
    layout->addWidget(buttonBox);
    connect(buttonBox, SIGNAL(clicked(QAbstractButton*)), &dlg, SLOT(close()));
    dlg.setLayout(layout);
    dlg.exec();
}

void MainWindow::systemTrayIconActivated(QSystemTrayIcon::ActivationReason reason)
{
    switch ( reason ) {
        case QSystemTrayIcon::Trigger:
            isVisible() ? hide() : showNormal();
            break;
        case QSystemTrayIcon::Context:
            systemTrayMenu->show();
            break;
        default:
            ;
     }
}

void MainWindow::closeEvent(QCloseEvent* event)
{
    writeSettings();
}

#include "mainwindow.moc"
