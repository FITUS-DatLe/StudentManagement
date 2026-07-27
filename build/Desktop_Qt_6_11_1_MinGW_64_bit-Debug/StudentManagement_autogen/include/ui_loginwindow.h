/********************************************************************************
** Form generated from reading UI file 'loginwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.11.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LOGINWINDOW_H
#define UI_LOGINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_LoginWindow
{
public:
    QHBoxLayout *mainLayout;
    QFrame *cardFrame;
    QHBoxLayout *cardLayout;
    QFrame *leftPanel;
    QVBoxLayout *leftLayout;
    QSpacerItem *verticalSpacer_1;
    QLabel *iconBox;
    QLabel *lblTitle;
    QFrame *divider;
    QLabel *lblSubtitle;
    QSpacerItem *verticalSpacer_2;
    QFrame *rightPanel;
    QVBoxLayout *rightLayout;
    QHBoxLayout *topBarLayout;
    QSpacerItem *horizontalSpacer_1;
    QPushButton *btnClose;
    QSpacerItem *verticalSpacer_3;
    QLabel *lblWelcome;
    QLabel *lblInstruction;
    QLineEdit *txtUsername;
    QLineEdit *txtPassword;
    QHBoxLayout *optionsLayout;
    QCheckBox *chkRemember;
    QSpacerItem *horizontalSpacer_2;
    QLabel *lblForgot;
    QPushButton *btnLogin;
    QSpacerItem *verticalSpacer_4;
    QLabel *lblFooter;

    void setupUi(QDialog *LoginWindow)
    {
        if (LoginWindow->objectName().isEmpty())
            LoginWindow->setObjectName("LoginWindow");
        LoginWindow->resize(850, 500);
        mainLayout = new QHBoxLayout(LoginWindow);
        mainLayout->setObjectName("mainLayout");
        mainLayout->setContentsMargins(15, 15, 15, 15);
        cardFrame = new QFrame(LoginWindow);
        cardFrame->setObjectName("cardFrame");
        cardLayout = new QHBoxLayout(cardFrame);
        cardLayout->setSpacing(0);
        cardLayout->setObjectName("cardLayout");
        cardLayout->setContentsMargins(0, 0, 0, 0);
        leftPanel = new QFrame(cardFrame);
        leftPanel->setObjectName("leftPanel");
        leftPanel->setMinimumSize(QSize(380, 0));
        leftPanel->setMaximumSize(QSize(380, 16777215));
        leftLayout = new QVBoxLayout(leftPanel);
        leftLayout->setObjectName("leftLayout");
        leftLayout->setAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        leftLayout->setContentsMargins(30, 40, 30, 40);
        verticalSpacer_1 = new QSpacerItem(0, 0, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        leftLayout->addItem(verticalSpacer_1);

        iconBox = new QLabel(leftPanel);
        iconBox->setObjectName("iconBox");
        iconBox->setMinimumSize(QSize(95, 95));
        iconBox->setMaximumSize(QSize(95, 95));
        QFont font;
        font.setPointSize(42);
        iconBox->setFont(font);
        iconBox->setAlignment(Qt::AlignCenter);

        leftLayout->addWidget(iconBox, 0, Qt::AlignHCenter);

        lblTitle = new QLabel(leftPanel);
        lblTitle->setObjectName("lblTitle");
        lblTitle->setAlignment(Qt::AlignCenter);

        leftLayout->addWidget(lblTitle, 0, Qt::AlignHCenter);

        divider = new QFrame(leftPanel);
        divider->setObjectName("divider");
        divider->setMinimumSize(QSize(130, 3));
        divider->setMaximumSize(QSize(130, 3));

        leftLayout->addWidget(divider, 0, Qt::AlignHCenter);

        lblSubtitle = new QLabel(leftPanel);
        lblSubtitle->setObjectName("lblSubtitle");
        lblSubtitle->setAlignment(Qt::AlignCenter);

        leftLayout->addWidget(lblSubtitle, 0, Qt::AlignHCenter);

        verticalSpacer_2 = new QSpacerItem(0, 0, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        leftLayout->addItem(verticalSpacer_2);


        cardLayout->addWidget(leftPanel);

        rightPanel = new QFrame(cardFrame);
        rightPanel->setObjectName("rightPanel");
        rightLayout = new QVBoxLayout(rightPanel);
        rightLayout->setSpacing(15);
        rightLayout->setObjectName("rightLayout");
        rightLayout->setContentsMargins(45, 40, 45, 40);
        topBarLayout = new QHBoxLayout();
        topBarLayout->setObjectName("topBarLayout");
        horizontalSpacer_1 = new QSpacerItem(0, 0, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        topBarLayout->addItem(horizontalSpacer_1);

        btnClose = new QPushButton(rightPanel);
        btnClose->setObjectName("btnClose");
        btnClose->setMinimumSize(QSize(30, 30));
        btnClose->setMaximumSize(QSize(30, 30));
        btnClose->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));

        topBarLayout->addWidget(btnClose);


        rightLayout->addLayout(topBarLayout);

        verticalSpacer_3 = new QSpacerItem(0, 0, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        rightLayout->addItem(verticalSpacer_3);

        lblWelcome = new QLabel(rightPanel);
        lblWelcome->setObjectName("lblWelcome");

        rightLayout->addWidget(lblWelcome);

        lblInstruction = new QLabel(rightPanel);
        lblInstruction->setObjectName("lblInstruction");

        rightLayout->addWidget(lblInstruction);

        txtUsername = new QLineEdit(rightPanel);
        txtUsername->setObjectName("txtUsername");
        txtUsername->setMinimumSize(QSize(0, 45));

        rightLayout->addWidget(txtUsername);

        txtPassword = new QLineEdit(rightPanel);
        txtPassword->setObjectName("txtPassword");
        txtPassword->setMinimumSize(QSize(0, 45));

        rightLayout->addWidget(txtPassword);

        optionsLayout = new QHBoxLayout();
        optionsLayout->setObjectName("optionsLayout");
        chkRemember = new QCheckBox(rightPanel);
        chkRemember->setObjectName("chkRemember");
        chkRemember->setChecked(true);

        optionsLayout->addWidget(chkRemember);

        horizontalSpacer_2 = new QSpacerItem(0, 0, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        optionsLayout->addItem(horizontalSpacer_2);

        lblForgot = new QLabel(rightPanel);
        lblForgot->setObjectName("lblForgot");
        lblForgot->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));

        optionsLayout->addWidget(lblForgot);


        rightLayout->addLayout(optionsLayout);

        btnLogin = new QPushButton(rightPanel);
        btnLogin->setObjectName("btnLogin");
        btnLogin->setMinimumSize(QSize(0, 48));
        btnLogin->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));

        rightLayout->addWidget(btnLogin);

        verticalSpacer_4 = new QSpacerItem(0, 0, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        rightLayout->addItem(verticalSpacer_4);

        lblFooter = new QLabel(rightPanel);
        lblFooter->setObjectName("lblFooter");
        lblFooter->setAlignment(Qt::AlignCenter);

        rightLayout->addWidget(lblFooter);


        cardLayout->addWidget(rightPanel);


        mainLayout->addWidget(cardFrame);


        retranslateUi(LoginWindow);

        QMetaObject::connectSlotsByName(LoginWindow);
    } // setupUi

    void retranslateUi(QDialog *LoginWindow)
    {
        LoginWindow->setWindowTitle(QCoreApplication::translate("LoginWindow", "\304\220\304\203ng nh\341\272\255p - H\341\273\207 Th\341\273\221ng Qu\341\272\243n L\303\275 Sinh Vi\303\252n HCMUS", nullptr));
        iconBox->setText(QCoreApplication::translate("LoginWindow", "\360\237\216\223", nullptr));
        lblTitle->setText(QCoreApplication::translate("LoginWindow", "TRANG QU\341\272\242N L\303\235\n"
"SINH VI\303\212N", nullptr));
        lblSubtitle->setText(QCoreApplication::translate("LoginWindow", "TR\306\257\341\273\234NG \304\220\341\272\240I H\341\273\214C KHOA H\341\273\214C T\341\273\260 NHI\303\212N,\n"
"\304\220\341\272\240I H\341\273\214C QU\341\273\220C GIA TP. H\341\273\222 CH\303\215 MINH", nullptr));
        btnClose->setText(QCoreApplication::translate("LoginWindow", "\342\234\225", nullptr));
        lblWelcome->setText(QCoreApplication::translate("LoginWindow", "Xin ch\303\240o \360\237\221\213", nullptr));
        lblInstruction->setText(QCoreApplication::translate("LoginWindow", "Vui l\303\262ng \304\221\304\203ng nh\341\272\255p t\303\240i kho\341\272\243n h\341\273\207 th\341\273\221ng \304\221\341\273\203 ti\341\272\277p t\341\273\245c", nullptr));
        txtUsername->setPlaceholderText(QCoreApplication::translate("LoginWindow", "T\303\240i kho\341\272\243n (VD: 25127293)", nullptr));
        txtPassword->setPlaceholderText(QCoreApplication::translate("LoginWindow", "M\341\272\255t kh\341\272\251u truy c\341\272\255p", nullptr));
        chkRemember->setText(QCoreApplication::translate("LoginWindow", "Ghi nh\341\273\233 \304\221\304\203ng nh\341\272\255p", nullptr));
        lblForgot->setText(QCoreApplication::translate("LoginWindow", "<a style='color: #00509e; text-decoration: none;' href='#'>Qu\303\252n m\341\272\255t kh\341\272\251u?</a>", nullptr));
        btnLogin->setText(QCoreApplication::translate("LoginWindow", "\304\220\304\203ng nh\341\272\255p \342\236\224", nullptr));
        lblFooter->setText(QCoreApplication::translate("LoginWindow", "Protected by HCMUS Security Pro & SQLite Encryption", nullptr));
    } // retranslateUi

};

namespace Ui {
    class LoginWindow: public Ui_LoginWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LOGINWINDOW_H
