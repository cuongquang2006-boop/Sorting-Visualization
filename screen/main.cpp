#include <QApplication>
#include "mainwindow_ui.h"


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setWindowIcon(QIcon(":/icons/sorticon.png"));
    qDebug() << "icon null:" << QPixmap(":/icons/menu.png").isNull();

    MainWindow w;
    w.show();
    return app.exec();
}