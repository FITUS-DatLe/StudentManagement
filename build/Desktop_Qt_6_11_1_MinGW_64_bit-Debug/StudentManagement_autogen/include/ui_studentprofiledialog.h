/********************************************************************************
** Form generated from reading UI file 'studentprofiledialog.ui'
**
** Created by: Qt User Interface Compiler version 6.11.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_STUDENTPROFILEDIALOG_H
#define UI_STUDENTPROFILEDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_StudentProfileDialog
{
public:
    QVBoxLayout *mainLayout;
    QFrame *headerFrame;
    QHBoxLayout *headerLayout;
    QVBoxLayout *avatarLayout;
    QLabel *lblAvatar;
    QPushButton *btnChangeAvatar;
    QVBoxLayout *infoLayout;
    QLabel *lblFullName;
    QLabel *lblStudentId;
    QLabel *lblMajorBadge;
    QSpacerItem *horizontalSpacer;
    QVBoxLayout *gpaLayout;
    QLabel *lblGpaTitle;
    QLabel *lblGpaValue;
    QLabel *lblClassificationBadge;
    QTabWidget *tabWidget;
    QWidget *tabAdmin;
    QFormLayout *formLayout;
    QLabel *label_1;
    QLineEdit *txtDob;
    QLabel *label_2;
    QLineEdit *txtPhone;
    QLabel *label_3;
    QLineEdit *txtEmail;
    QLabel *label_4;
    QLineEdit *txtAdvisor;
    QLabel *label_5;
    QLineEdit *txtAddress;
    QWidget *tabAcademic;
    QVBoxLayout *verticalLayout_2;
    QLabel *lblAccumulatedCredits;
    QTableWidget *tblCourses;
    QWidget *tabTraining;
    QVBoxLayout *verticalLayout_3;
    QTableWidget *tblTrainingPoints;
    QWidget *tabAchievements;
    QVBoxLayout *verticalLayout_4;
    QHBoxLayout *achievementBtnLayout;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *btnAddAchievement;
    QTableWidget *tblAchievements;
    QHBoxLayout *footerLayout;
    QSpacerItem *horizontalSpacer_3;
    QPushButton *btnEditProfile;
    QPushButton *btnCloseProfile;

    void setupUi(QDialog *StudentProfileDialog)
    {
        if (StudentProfileDialog->objectName().isEmpty())
            StudentProfileDialog->setObjectName("StudentProfileDialog");
        StudentProfileDialog->resize(900, 650);
        mainLayout = new QVBoxLayout(StudentProfileDialog);
        mainLayout->setSpacing(15);
        mainLayout->setObjectName("mainLayout");
        mainLayout->setContentsMargins(20, 20, 20, 20);
        headerFrame = new QFrame(StudentProfileDialog);
        headerFrame->setObjectName("headerFrame");
        headerFrame->setMinimumSize(QSize(0, 140));
        headerLayout = new QHBoxLayout(headerFrame);
        headerLayout->setSpacing(20);
        headerLayout->setObjectName("headerLayout");
        avatarLayout = new QVBoxLayout();
        avatarLayout->setObjectName("avatarLayout");
        avatarLayout->setAlignment(Qt::AlignCenter);
        lblAvatar = new QLabel(headerFrame);
        lblAvatar->setObjectName("lblAvatar");
        lblAvatar->setMinimumSize(QSize(100, 100));
        lblAvatar->setMaximumSize(QSize(100, 100));
        lblAvatar->setAlignment(Qt::AlignCenter);

        avatarLayout->addWidget(lblAvatar);

        btnChangeAvatar = new QPushButton(headerFrame);
        btnChangeAvatar->setObjectName("btnChangeAvatar");
        btnChangeAvatar->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));

        avatarLayout->addWidget(btnChangeAvatar);


        headerLayout->addLayout(avatarLayout);

        infoLayout = new QVBoxLayout();
        infoLayout->setSpacing(5);
        infoLayout->setObjectName("infoLayout");
        lblFullName = new QLabel(headerFrame);
        lblFullName->setObjectName("lblFullName");
        QFont font;
        font.setPointSize(18);
        font.setBold(true);
        lblFullName->setFont(font);

        infoLayout->addWidget(lblFullName);

        lblStudentId = new QLabel(headerFrame);
        lblStudentId->setObjectName("lblStudentId");
        QFont font1;
        font1.setPointSize(11);
        lblStudentId->setFont(font1);

        infoLayout->addWidget(lblStudentId);

        lblMajorBadge = new QLabel(headerFrame);
        lblMajorBadge->setObjectName("lblMajorBadge");

        infoLayout->addWidget(lblMajorBadge);


        headerLayout->addLayout(infoLayout);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        headerLayout->addItem(horizontalSpacer);

        gpaLayout = new QVBoxLayout();
        gpaLayout->setObjectName("gpaLayout");
        gpaLayout->setAlignment(Qt::AlignCenter);
        lblGpaTitle = new QLabel(headerFrame);
        lblGpaTitle->setObjectName("lblGpaTitle");
        lblGpaTitle->setAlignment(Qt::AlignCenter);

        gpaLayout->addWidget(lblGpaTitle);

        lblGpaValue = new QLabel(headerFrame);
        lblGpaValue->setObjectName("lblGpaValue");
        QFont font2;
        font2.setPointSize(22);
        font2.setBold(true);
        lblGpaValue->setFont(font2);
        lblGpaValue->setAlignment(Qt::AlignCenter);

        gpaLayout->addWidget(lblGpaValue);

        lblClassificationBadge = new QLabel(headerFrame);
        lblClassificationBadge->setObjectName("lblClassificationBadge");
        lblClassificationBadge->setAlignment(Qt::AlignCenter);

        gpaLayout->addWidget(lblClassificationBadge);


        headerLayout->addLayout(gpaLayout);


        mainLayout->addWidget(headerFrame);

        tabWidget = new QTabWidget(StudentProfileDialog);
        tabWidget->setObjectName("tabWidget");
        tabAdmin = new QWidget();
        tabAdmin->setObjectName("tabAdmin");
        formLayout = new QFormLayout(tabAdmin);
        formLayout->setObjectName("formLayout");
        formLayout->setHorizontalSpacing(20);
        formLayout->setVerticalSpacing(15);
        formLayout->setContentsMargins(30, 30, 30, 30);
        label_1 = new QLabel(tabAdmin);
        label_1->setObjectName("label_1");

        formLayout->setWidget(0, QFormLayout::ItemRole::LabelRole, label_1);

        txtDob = new QLineEdit(tabAdmin);
        txtDob->setObjectName("txtDob");
        txtDob->setReadOnly(true);

        formLayout->setWidget(0, QFormLayout::ItemRole::FieldRole, txtDob);

        label_2 = new QLabel(tabAdmin);
        label_2->setObjectName("label_2");

        formLayout->setWidget(1, QFormLayout::ItemRole::LabelRole, label_2);

        txtPhone = new QLineEdit(tabAdmin);
        txtPhone->setObjectName("txtPhone");
        txtPhone->setReadOnly(true);

        formLayout->setWidget(1, QFormLayout::ItemRole::FieldRole, txtPhone);

        label_3 = new QLabel(tabAdmin);
        label_3->setObjectName("label_3");

        formLayout->setWidget(2, QFormLayout::ItemRole::LabelRole, label_3);

        txtEmail = new QLineEdit(tabAdmin);
        txtEmail->setObjectName("txtEmail");
        txtEmail->setReadOnly(true);

        formLayout->setWidget(2, QFormLayout::ItemRole::FieldRole, txtEmail);

        label_4 = new QLabel(tabAdmin);
        label_4->setObjectName("label_4");

        formLayout->setWidget(3, QFormLayout::ItemRole::LabelRole, label_4);

        txtAdvisor = new QLineEdit(tabAdmin);
        txtAdvisor->setObjectName("txtAdvisor");
        txtAdvisor->setReadOnly(true);

        formLayout->setWidget(3, QFormLayout::ItemRole::FieldRole, txtAdvisor);

        label_5 = new QLabel(tabAdmin);
        label_5->setObjectName("label_5");

        formLayout->setWidget(4, QFormLayout::ItemRole::LabelRole, label_5);

        txtAddress = new QLineEdit(tabAdmin);
        txtAddress->setObjectName("txtAddress");
        txtAddress->setReadOnly(true);

        formLayout->setWidget(4, QFormLayout::ItemRole::FieldRole, txtAddress);

        tabWidget->addTab(tabAdmin, QString());
        tabAcademic = new QWidget();
        tabAcademic->setObjectName("tabAcademic");
        verticalLayout_2 = new QVBoxLayout(tabAcademic);
        verticalLayout_2->setSpacing(10);
        verticalLayout_2->setObjectName("verticalLayout_2");
        verticalLayout_2->setContentsMargins(15, 15, 15, 15);
        lblAccumulatedCredits = new QLabel(tabAcademic);
        lblAccumulatedCredits->setObjectName("lblAccumulatedCredits");
        QFont font3;
        font3.setBold(true);
        lblAccumulatedCredits->setFont(font3);

        verticalLayout_2->addWidget(lblAccumulatedCredits);

        tblCourses = new QTableWidget(tabAcademic);
        tblCourses->setObjectName("tblCourses");
        tblCourses->setEditTriggers(QAbstractItemView::NoEditTriggers);
        tblCourses->setSelectionBehavior(QAbstractItemView::SelectRows);

        verticalLayout_2->addWidget(tblCourses);

        tabWidget->addTab(tabAcademic, QString());
        tabTraining = new QWidget();
        tabTraining->setObjectName("tabTraining");
        verticalLayout_3 = new QVBoxLayout(tabTraining);
        verticalLayout_3->setSpacing(10);
        verticalLayout_3->setObjectName("verticalLayout_3");
        verticalLayout_3->setContentsMargins(15, 15, 15, 15);
        tblTrainingPoints = new QTableWidget(tabTraining);
        tblTrainingPoints->setObjectName("tblTrainingPoints");
        tblTrainingPoints->setEditTriggers(QAbstractItemView::NoEditTriggers);
        tblTrainingPoints->setSelectionBehavior(QAbstractItemView::SelectRows);

        verticalLayout_3->addWidget(tblTrainingPoints);

        tabWidget->addTab(tabTraining, QString());
        tabAchievements = new QWidget();
        tabAchievements->setObjectName("tabAchievements");
        verticalLayout_4 = new QVBoxLayout(tabAchievements);
        verticalLayout_4->setSpacing(10);
        verticalLayout_4->setObjectName("verticalLayout_4");
        verticalLayout_4->setContentsMargins(15, 15, 15, 15);
        achievementBtnLayout = new QHBoxLayout();
        achievementBtnLayout->setObjectName("achievementBtnLayout");
        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        achievementBtnLayout->addItem(horizontalSpacer_2);

        btnAddAchievement = new QPushButton(tabAchievements);
        btnAddAchievement->setObjectName("btnAddAchievement");
        btnAddAchievement->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));

        achievementBtnLayout->addWidget(btnAddAchievement);


        verticalLayout_4->addLayout(achievementBtnLayout);

        tblAchievements = new QTableWidget(tabAchievements);
        tblAchievements->setObjectName("tblAchievements");
        tblAchievements->setEditTriggers(QAbstractItemView::NoEditTriggers);
        tblAchievements->setSelectionBehavior(QAbstractItemView::SelectRows);

        verticalLayout_4->addWidget(tblAchievements);

        tabWidget->addTab(tabAchievements, QString());

        mainLayout->addWidget(tabWidget);

        footerLayout = new QHBoxLayout();
        footerLayout->setObjectName("footerLayout");
        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        footerLayout->addItem(horizontalSpacer_3);

        btnEditProfile = new QPushButton(StudentProfileDialog);
        btnEditProfile->setObjectName("btnEditProfile");
        btnEditProfile->setMinimumSize(QSize(130, 40));
        btnEditProfile->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));

        footerLayout->addWidget(btnEditProfile);

        btnCloseProfile = new QPushButton(StudentProfileDialog);
        btnCloseProfile->setObjectName("btnCloseProfile");
        btnCloseProfile->setMinimumSize(QSize(120, 40));
        btnCloseProfile->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));

        footerLayout->addWidget(btnCloseProfile);


        mainLayout->addLayout(footerLayout);


        retranslateUi(StudentProfileDialog);

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(StudentProfileDialog);
    } // setupUi

    void retranslateUi(QDialog *StudentProfileDialog)
    {
        StudentProfileDialog->setWindowTitle(QCoreApplication::translate("StudentProfileDialog", "H\341\273\223 S\306\241 Sinh Vi\303\252n Chi Ti\341\272\277t 360\302\260", nullptr));
        lblAvatar->setText(QCoreApplication::translate("StudentProfileDialog", "\360\237\221\244", nullptr));
        btnChangeAvatar->setText(QCoreApplication::translate("StudentProfileDialog", "\304\220\341\273\225i \341\272\243nh", nullptr));
        lblFullName->setText(QCoreApplication::translate("StudentProfileDialog", "H\341\273\214 V\303\200 T\303\212N SINH VI\303\212N", nullptr));
        lblStudentId->setText(QCoreApplication::translate("StudentProfileDialog", "MSSV: 25127000 | L\341\273\233p: 25DTH1", nullptr));
        lblMajorBadge->setText(QCoreApplication::translate("StudentProfileDialog", "Chuy\303\252n ng\303\240nh: C\303\264ng ngh\341\273\207 Th\303\264ng tin", nullptr));
        lblGpaTitle->setText(QCoreApplication::translate("StudentProfileDialog", "\304\220I\341\273\202M TRUNG B\303\214NH", nullptr));
        lblGpaValue->setText(QCoreApplication::translate("StudentProfileDialog", "0.00", nullptr));
        lblClassificationBadge->setText(QCoreApplication::translate("StudentProfileDialog", "X\341\272\277p lo\341\272\241i", nullptr));
        label_1->setText(QCoreApplication::translate("StudentProfileDialog", "Ng\303\240y sinh:", nullptr));
        label_2->setText(QCoreApplication::translate("StudentProfileDialog", "S\341\273\221 \304\221i\341\273\207n tho\341\272\241i:", nullptr));
        label_3->setText(QCoreApplication::translate("StudentProfileDialog", "Email tr\306\260\341\273\235ng:", nullptr));
        label_4->setText(QCoreApplication::translate("StudentProfileDialog", "C\341\273\221 v\341\272\245n h\341\273\215c t\341\272\255p:", nullptr));
        txtAdvisor->setText(QCoreApplication::translate("StudentProfileDialog", "TS. Nguy\341\273\205n V\304\203n A (Khoa CNTT)", nullptr));
        label_5->setText(QCoreApplication::translate("StudentProfileDialog", "\304\220\341\273\213a ch\341\273\211 li\303\252n l\341\272\241c:", nullptr));
        txtAddress->setText(QCoreApplication::translate("StudentProfileDialog", "K\303\275 t\303\272c x\303\241 \304\220HQG-HCM, Linh Trung, Th\341\273\247 \304\220\341\273\251c, TP.HCM", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tabAdmin), QCoreApplication::translate("StudentProfileDialog", "\360\237\223\213 Th\303\264ng Tin H\303\240nh Ch\303\255nh", nullptr));
        lblAccumulatedCredits->setText(QCoreApplication::translate("StudentProfileDialog", "\342\200\242 T\341\273\225ng s\341\273\221 t\303\255n ch\341\273\211 t\303\255ch l\305\251y: 0 / 135 TC", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tabAcademic), QCoreApplication::translate("StudentProfileDialog", "\360\237\223\232 B\341\272\243ng \304\220i\341\273\203m & T\303\255n Ch\341\273\211", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tabTraining), QCoreApplication::translate("StudentProfileDialog", "\360\237\216\257 \304\220i\341\273\203m R\303\250n Luy\341\273\207n", nullptr));
        btnAddAchievement->setText(QCoreApplication::translate("StudentProfileDialog", "+ Th\303\252m gi\341\272\243i th\306\260\341\273\237ng / NCKH", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tabAchievements), QCoreApplication::translate("StudentProfileDialog", "\360\237\217\206 Gi\341\272\243i Th\306\260\341\273\237ng & NCKH", nullptr));
        btnEditProfile->setText(QCoreApplication::translate("StudentProfileDialog", "\342\234\217\357\270\217 Ch\341\273\211nh s\341\273\255a", nullptr));
        btnCloseProfile->setText(QCoreApplication::translate("StudentProfileDialog", "\304\220\303\263ng h\341\273\223 s\306\241", nullptr));
    } // retranslateUi

};

namespace Ui {
    class StudentProfileDialog: public Ui_StudentProfileDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_STUDENTPROFILEDIALOG_H
