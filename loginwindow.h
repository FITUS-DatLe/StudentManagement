#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QDialog>
#include <QString>

namespace Ui {
class LoginWindow;
}

/**
 * @brief Lớp LoginWindow - Giao diện Đăng nhập chuẩn cấu trúc 3 file của dự án
 * Tích hợp kiểm tra tài khoản qua SQLite và tự động tạo hiệu ứng Card 3D.
 */
class LoginWindow : public QDialog
{
    Q_OBJECT

public:
    explicit LoginWindow(QWidget *parent = nullptr);
    ~LoginWindow() override;

    QString getUsername() const;
    QString getPassword() const;

private slots:
    void handleLogin();

private:
    Ui::LoginWindow *ui;
    void applyStyleSheets();
};

#endif // LOGINWINDOW_H