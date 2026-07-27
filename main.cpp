#include "mainwindow.h"
#include "loginwindow.h"
#include "studentmanager.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

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