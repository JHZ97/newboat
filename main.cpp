#include "mainwindow.h"

#include <QApplication>
//预编译指令，防止中文乱码
#pragma execution_character_set("utf-8")

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();        
    return a.exec();
}
