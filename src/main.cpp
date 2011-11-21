#include <QApplication>
#include <QStringList>

#include "version.h"
#include "mainwindow.h"

int main(int argc, char** argv)
{
    QApplication app(argc, argv);
    app.setApplicationName("Iceberg");
    app.setApplicationVersion(ICEBERG_VERSION);
    MainWindow* mainWidget = new MainWindow( 0 );
    mainWidget->setWindowTitle("Iceberg - Icecc Monitor");
    QStringList args = app.arguments();
    int nIndex = args.indexOf("-n");
    if (nIndex && nIndex < args.count() +1)
        mainWidget->setCurrentNet(args[nIndex + 1].toLocal8Bit());
    mainWidget->show();

    return app.exec();
}
