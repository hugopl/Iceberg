/*
    This file is part of Icecream.

    Copyright (c) 2003 Frerich Raabe <raabe@kde.org>
    Copyright (c) 2003,2004 Stephan Kulow <coolo@kde.org>
    Copyright (c) 2003,2004 Cornelius Schumacher <schumacher@kde.org>
    Copyright (c) 2011 Hugo Parente Lima <hugo.pl@gmail.com>
    Copyright (c) 2011 Anselmo L. S. Melo <anselmolsm@gmail.com>

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

#include <QApplication>
#include <QStringList>
#include "version.h"
#include "mainwindow.h"

int main(int argc, char** argv)
{
    QApplication app(argc, argv);
    app.setOrganizationName("Iceberg");
    app.setApplicationName("Iceberg");
    app.setApplicationVersion(ICEBERG_VERSION);
    MainWindow mainWidget;
    mainWidget.setWindowTitle("Iceberg - Icecc Monitor");

    // Set the netname if there is "-n <name>" arguments.
    // Otherwise, use the netname stored in the config file
    QStringList args = app.arguments();
    int nIndex = args.indexOf("-n");
    if ((nIndex != -1) && (nIndex < args.count() + 1))
        mainWidget.setCurrentNet(args[nIndex + 1].toLocal8Bit());
    mainWidget.show();

    return app.exec();
}
