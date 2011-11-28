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
    MainWindow* mainWidget = new MainWindow( 0 );
    mainWidget->setWindowTitle("Iceberg - Icecc Monitor");

    // Set the netname if there is "-n <name>" arguments.
    // Otherwise, use the netname stored in the config file
    QStringList args = app.arguments();
    int nIndex = args.indexOf("-n");
    if ((nIndex != -1) && (nIndex < args.count() + 1))
        mainWidget->setCurrentNet(args[nIndex + 1].toLocal8Bit());
    mainWidget->show();

    return app.exec();
}
