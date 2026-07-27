#include "mainwindow.h"
#include "loginwindow.h"
#include "studentmanager.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
<<<<<<< HEAD

    // Khởi tạo CSDL SQLite trước khi mở màn hình Đăng nhập
    StudentManager::getInstance().initManager();

    // Hiển thị màn hình Đăng nhập
    LoginWindow loginWindow;
    if (loginWindow.exec() == QDialog::Accepted) {
        MainWindow w;
        w.show();
        return a.exec();
    }

    return 0;
}
=======
    MainWindow w;
    w.show();
    return QApplication::exec();
}
// taolagautruc
//taolagaumeo
//taolabaotran
//hello
//ê 
>>>>>>> 1ddc4bafdd217c0564d7f284cce747867b8aac49
