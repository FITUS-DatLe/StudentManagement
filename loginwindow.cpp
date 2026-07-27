#include "loginwindow.h"
#include "ui_loginwindow.h"
#include "databasehelper.h"
#include <QGraphicsDropShadowEffect>
#include <QMessageBox>

LoginWindow::LoginWindow(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::LoginWindow)
{
    ui->setupUi(this);

    // Cấu hình cửa sổ không viền, nền trong suốt để lộ bóng đổ Card 3D
    this->setFixedSize(850, 500);
    this->setAttribute(Qt::WA_TranslucentBackground);
    this->setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);

    // Tạo hiệu ứng bóng đổ mượt mà cho khối thẻ chính (cardFrame)
    auto *shadow = new QGraphicsDropShadowEffect(this);
    shadow->setBlurRadius(20);
    shadow->setColor(QColor(0, 0, 0, 50));
    shadow->setOffset(0, 8);
    ui->cardFrame->setGraphicsEffect(shadow);

    // Cấu hình ô mật khẩu và định dạng con trỏ chuột
    ui->txtPassword->setEchoMode(QLineEdit::Password);
    ui->lblForgot->setOpenExternalLinks(false);

    // Gán sự kiện cho các nút bấm và phím Enter
    connect(ui->btnClose, &QPushButton::clicked, this, &QDialog::reject);
    connect(ui->btnLogin, &QPushButton::clicked, this, &LoginWindow::handleLogin);
    connect(ui->txtPassword, &QLineEdit::returnPressed, this, &LoginWindow::handleLogin);
    connect(ui->txtUsername, &QLineEdit::returnPressed, [this]() { ui->txtPassword->setFocus(); });

    // Áp dụng bộ màu dốc xanh dương HCMUS
    applyStyleSheets();
}

LoginWindow::~LoginWindow()
{
    delete ui;
}

void LoginWindow::applyStyleSheets()
{
    QString qss = R"(
        /* Khung thẻ chính bo tròn 16px */
        #cardFrame {
            background-color: #FFFFFF;
            border-radius: 16px;
        }

        /* Cột trái: Dải màu xanh dương HCMUS dốc Gradient */
        #leftPanel {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #002855, stop:1 #00509e);
            border-top-left-radius: 16px;
            border-bottom-left-radius: 16px;
        }

        /* Hộp icon biểu tượng mũ tốt nghiệp */
        #iconBox {
            background-color: rgba(255, 255, 255, 0.15);
            border: 2px solid rgba(255, 255, 255, 0.3);
            border-radius: 20px;
        }

        /* Tiêu đề Trang quản lý sinh viên */
        #lblTitle {
            color: #FFFFFF;
            font-family: 'Segoe UI', sans-serif;
            font-size: 20px;
            font-weight: bold;
            letter-spacing: 1px;
        }

        /* Đường phân cách xanh sáng */
        #divider {
            background-color: #4da6ff;
            border-radius: 2px;
        }

        /* Tiêu đề phụ Đại học KHTN */
        #lblSubtitle {
            color: #D0E1F9;
            font-family: 'Segoe UI', sans-serif;
            font-size: 11px;
            font-weight: 500;
        }

        /* Cột phải: Nền trắng sạch */
        #rightPanel {
            background-color: #FFFFFF;
            border-top-right-radius: 16px;
            border-bottom-right-radius: 16px;
        }

        /* Nút tắt cửa sổ X */
        #btnClose {
            background: transparent;
            color: #95a5a6;
            font-size: 16px;
            font-weight: bold;
            border: none;
        }
        #btnClose:hover {
            color: #e74c3c;
            background-color: #fcf3f2;
            border-radius: 15px;
        }

        /* Tiêu đề Xin chào */
        #lblWelcome {
            color: #1a252f;
            font-family: 'Segoe UI', sans-serif;
            font-size: 26px;
            font-weight: bold;
        }

        #lblInstruction {
            color: #7f8c8d;
            font-size: 13px;
        }

        /* Ô nhập liệu chuẩn SaaS */
        QLineEdit {
            background-color: #F8F9F9;
            border: 1.5px solid #E5E7E9;
            border-radius: 8px;
            padding: 0 15px;
            font-size: 14px;
            color: #2c3e50;
        }
        QLineEdit:focus {
            background-color: #FFFFFF;
            border: 1.5px solid #00509e;
        }
        QLineEdit::placeholder {
            color: #BDC3C7;
        }

        /* Hộp kiểm Ghi nhớ */
        QCheckBox {
            color: #5d6d7e;
            font-size: 13px;
        }
        QCheckBox::indicator {
            width: 18px;
            height: 18px;
            border-radius: 4px;
            border: 1.5px solid #BDC3C7;
        }
        QCheckBox::indicator:checked {
            background-color: #00509e;
            border: 1.5px solid #00509e;
        }

        /* Nút Đăng nhập xanh dương chủ đạo */
        #btnLogin {
            background-color: #00509e;
            color: #FFFFFF;
            font-family: 'Segoe UI', sans-serif;
            font-size: 15px;
            font-weight: bold;
            border: none;
            border-radius: 8px;
        }
        #btnLogin:hover {
            background-color: #003d7a;
        }
        #btnLogin:pressed {
            background-color: #002855;
        }

        /* Chân trang */
        #lblFooter {
            color: #BDC3C7;
            font-size: 11px;
        }
    )";

    this->setStyleSheet(qss);
}

QString LoginWindow::getUsername() const
{
    return ui->txtUsername->text().trimmed();
}

QString LoginWindow::getPassword() const
{
    return ui->txtPassword->text();
}

void LoginWindow::handleLogin()
{
    QString user = getUsername();
    QString pass = getPassword();

    if (user.isEmpty() || pass.isEmpty()) {
        QMessageBox::warning(this, "Thiếu thông tin", "Vui lòng nhập đầy đủ Tài khoản và Mật khẩu!");
        if (user.isEmpty()) ui->txtUsername->setFocus();
        else ui->txtPassword->setFocus();
        return;
    }

    // Xác thực qua SQLite
    if (DatabaseHelper::getInstance().authenticateUser(user, pass)) {
        this->accept();
    } else {
        QMessageBox::critical(this, "Đăng nhập thất bại", "Tài khoản hoặc Mật khẩu không chính xác!\n\n• Gợi ý tài khoản mặc định: 25127293\n• Mật khẩu: 123456");
        ui->txtPassword->clear();
        ui->txtPassword->setFocus();
    }
}