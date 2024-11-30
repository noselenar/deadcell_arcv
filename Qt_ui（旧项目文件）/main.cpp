#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    MainWindow window;


    window.setWindowTitle("死亡细胞存档自动备份工具");
    window.setWindowIcon(QIcon(":/images/app_icon.ico"));
    //window.resize(1000, 700); // 调整窗口大小
    window.show();

    return app.exec();
}
