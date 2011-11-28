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
#include <QMenu>
#include <QMenuBar>
#include <QSettings>
#include "detailedhostview.h"
#include "ganttstatusview.h"
#include "hostinfo.h"
#include "listview.h"
#include "monitor.h"
#include "starview.h"
#include "poolview.h"
#include "summaryview.h"
#include "version.h"
#include <QVBoxLayout>
#include <QDialogButtonBox>
#include <QDesktopWidget>
#include <QCloseEvent>

MainWindow::MainWindow( QWidget *parent )
  : QMainWindow( parent ), m_view( 0 )
{
    m_hostInfoManager = new HostInfoManager;

    m_monitor = new Monitor( m_hostInfoManager, this );

    m_viewMode = new QActionGroup(this);

    QMenu* fileMenu = menuBar()->addMenu(tr("&File"));
    QAction* quitAction = fileMenu->addAction(tr("&Quit"));
    connect(quitAction, SIGNAL( triggered() ), this, SLOT( close() ));

    QMenu* viewMenu = menuBar()->addMenu(tr("&View"));
    QMenu* modeMenu = viewMenu->addMenu(tr("&Mode"));

    m_listView = modeMenu->addAction(tr( "&List View" ));
    m_listView->setCheckable(true);
    m_viewMode->addAction(m_listView);
    connect( m_listView, SIGNAL( triggered() ), this, SLOT( setupListView() ) );

    m_starView = modeMenu->addAction(tr( "&Star View" ));
    m_starView->setCheckable(true);
    m_viewMode->addAction(m_starView);
    connect( m_starView, SIGNAL( triggered() ), this, SLOT( setupStarView() ) );

    m_poolView = modeMenu->addAction(tr( "&Pool View" ));
    m_poolView->setCheckable(true);
    m_viewMode->addAction(m_poolView);
    connect( m_poolView, SIGNAL( triggered() ), this, SLOT( setupPoolView() ) );

    m_ganttView = modeMenu->addAction(tr( "&Gantt View" ));
    m_ganttView->setCheckable(true);
    m_viewMode->addAction(m_ganttView);
    connect( m_ganttView, SIGNAL( triggered() ), this, SLOT( setupGanttView() ) );

    m_summaryView = modeMenu->addAction(tr( "Summary &View" ));
    m_summaryView->setCheckable(true);
    m_viewMode->addAction(m_summaryView);
    connect( m_summaryView, SIGNAL( triggered() ), this, SLOT( setupSummaryView() ) );

    m_detailedView = modeMenu->addAction(tr( "&Detailed Host View" ));
    m_detailedView->setCheckable(true);
    m_viewMode->addAction(m_detailedView);
    connect( m_detailedView, SIGNAL( triggered() ), this, SLOT( setupDetailedHostView() ) );


    QAction* actionStart = viewMenu->addAction( tr("&Start") );
    connect( actionStart, SIGNAL( triggered() ), this, SLOT( startView() ) );
    QAction* actionStop = viewMenu->addAction( tr("Stop") );
    connect( actionStop, SIGNAL( triggered() ), this, SLOT( stopView() ) );
    viewMenu->addSeparator();
    QAction* actionCheckNodes = viewMenu->addAction( tr("Check Nodes") );
    connect( actionCheckNodes, SIGNAL( triggered() ), this, SLOT( checkNodes() ) );
    viewMenu->addSeparator();
    QAction* actionConfigView = viewMenu->addAction( tr("Configure View...") );
    connect( actionConfigView, SIGNAL( triggered() ), this, SLOT( configureView() ) );

    QMenu* helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(tr("About..."), this, SLOT(showAboutDialog()));

    // Avoid useless creation and connection if the system does not have a systray
    if ( QSystemTrayIcon::isSystemTrayAvailable() ) {
        systemTrayIcon = new QSystemTrayIcon( this );
        systemTrayIcon->setIcon( QIcon( ":bigIcon.png" ) );
        systemTrayIcon->show();

        systemTrayMenu = new QMenu( this );
        systemTrayMenu->addAction( quitAction );

        systemTrayIcon->setContextMenu( systemTrayMenu );

        connect( systemTrayIcon, SIGNAL( activated( QSystemTrayIcon::ActivationReason ) ),
                this, SLOT( systemTrayIconActivated( QSystemTrayIcon::ActivationReason ) ) );
    }

    setWindowIcon( QIcon(":bigIcon.png") );

    readSettings();

    QDesktopWidget w;
    QRect screenGeometry = w.availableGeometry( this );
    const QRect defaultRect = QRect( screenGeometry.center()/2, screenGeometry.size()/2 );

    QSettings cfg;
    QVariant geom = cfg.value( "geometry", defaultRect );
    setGeometry( geom.toRect() );
}

MainWindow::~MainWindow()
{
  delete m_hostInfoManager;
}

void MainWindow::readSettings()
{
    QSettings cfg;
    QString viewId = cfg.value( "CurrentView", "star" ).toString();

    m_viewMode->blockSignals(true);

    if ( viewId == "gantt" ) {
        setupGanttView();
        m_ganttView->setChecked(true);
    } else if ( viewId == "list" ) {
        setupListView();
        m_listView->setChecked(true);
    } else if ( viewId == "star" ) {
        setupStarView();
        m_starView->setChecked(true);
    } else if ( viewId == "pool" ) {
        setupPoolView();
        m_poolView->setChecked(true);
    } else if ( viewId == "detailedhost" ) {
        setupDetailedHostView();
        m_detailedView->setChecked(true);
    } else {
        setupSummaryView();
        m_summaryView->setChecked(true);
    }

    // "icecream" is the default netname used by iceccd
    QByteArray netname = cfg.value("netname", "icecream").toByteArray();
    setCurrentNet(netname);

    m_viewMode->blockSignals(false);
}

void MainWindow::writeSettings()
{
  QSettings cfg;
  cfg.setValue( "netname", m_monitor->currentNet() );
  cfg.setValue( "CurrentView", m_view->id() );
  cfg.setValue( "geometry", this->geometry() );
  cfg.sync();
}

void MainWindow::setupView( StatusView *view, bool rememberJobs )
{
  delete m_view;
  m_view = view;
  m_monitor->setCurrentView( m_view, rememberJobs );
  setCentralWidget( m_view->widget() );
  m_view->widget()->show();
}

void MainWindow::setupListView()
{
    setupView( new ListStatusView( m_hostInfoManager, this ), true );
}

void MainWindow::setupSummaryView()
{
    setupView( new SummaryView( m_hostInfoManager, this ), false );
    m_summaryView->setChecked(true);
}

void MainWindow::setupGanttView()
{
    setupView( new GanttStatusView( m_hostInfoManager, this ), false );
    m_ganttView->setChecked(true);
}

void MainWindow::setupPoolView()
{
    setupView( new PoolView( m_hostInfoManager, this ), false );
    m_poolView->setChecked(true);
}

void MainWindow::setupStarView()
{
    setupView( new StarView( m_hostInfoManager, this ), false );
    m_starView->setChecked(true);
}

void MainWindow::setupDetailedHostView()
{
    setupView( new DetailedHostView( m_hostInfoManager, this ), false );
    m_detailedView->setChecked(true);
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

void MainWindow::setCurrentNet( const QByteArray &netName )
{
  m_monitor->setCurrentNet( netName );
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
    layout->addWidget(text);
    QDialogButtonBox* buttonBox = new QDialogButtonBox(QDialogButtonBox::Close, Qt::Horizontal);
    buttonBox->setCenterButtons(true);
    layout->addWidget(buttonBox);
    connect(buttonBox, SIGNAL(clicked(QAbstractButton*)), &dlg, SLOT(close()));
    dlg.setLayout(layout);
    dlg.exec();
}

void MainWindow::systemTrayIconActivated( QSystemTrayIcon::ActivationReason reason )
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

void MainWindow::closeEvent( QCloseEvent * event )
{
    writeSettings();
}

#include "mainwindow.moc"
