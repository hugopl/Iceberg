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

#include "mon-kde.h"
#include <QApplication>
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

    readSettings();
}

MainWindow::~MainWindow()
{
  writeSettings();

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

    m_viewMode->blockSignals(false);
}

void MainWindow::writeSettings()
{
  QSettings cfg;
  cfg.setValue( "CurrentView", m_view->id() );
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

const char * rs_program_name = "icemon";
const char * const appName = QT_TR_NOOP( "Icecream Monitor" );
const char * const version = "2.0";
const char * const description = QT_TR_NOOP( "Icecream monitor for KDE" );
const char * const copyright = QT_TR_NOOP( "(c) 2003,2004, The icecream developers" );

int main(int argc, char** argv)
{
    QApplication app(argc, argv);
    app.setApplicationName(appName);
    app.setApplicationVersion(version);
    MainWindow* mainWidget = new MainWindow( 0 );
    QStringList args = app.arguments();
    int nIndex = args.indexOf("-n");
    if (nIndex && nIndex < args.count() +1)
        mainWidget->setCurrentNet(args[nIndex + 1].toLocal8Bit());
    mainWidget->show();

    return app.exec();
}

#include "mon-kde.moc"
