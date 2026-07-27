/********************************************************************************
** Form generated from reading UI file 'studentdialog.ui'
**
** Created by: Qt User Interface Compiler version 6.11.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_STUDENTDIALOG_H
#define UI_STUDENTDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_StudentDialog
{
public:
    QFrame *frameCard;
    QLabel *lblTitle;
    QLabel *lblId;
    QLineEdit *txtId;
    QLabel *lblName;
    QLineEdit *txtName;
    QLabel *lblDob;
    QLineEdit *txtDob;
    QLabel *lblClass;
    QLineEdit *txtClass;
    QLabel *lblMajor;
    QComboBox *cboMajor;
    QGroupBox *grpScores;
    QStackedWidget *stackedScores;
    QWidget *pageIT;
    QLabel *lblIT1;
    QLineEdit *txtScoreIT1;
    QLabel *lblIT2;
    QLineEdit *txtScoreIT2;
    QLabel *lblIT3;
    QLineEdit *txtScoreIT3;
    QWidget *pageMath;
    QLabel *lblMath1;
    QLineEdit *txtScoreMath1;
    QLabel *lblMath2;
    QLineEdit *txtScoreMath2;
    QWidget *pageBio;
    QLabel *lblBio1;
    QLineEdit *txtScoreBio1;
    QLabel *lblBio2;
    QLineEdit *txtScoreBio2;
    QPushButton *btnSave;
    QPushButton *btnCancel;

    void setupUi(QDialog *StudentDialog)
    {
        if (StudentDialog->objectName().isEmpty())
            StudentDialog->setObjectName("StudentDialog");
        StudentDialog->resize(550, 600);
        StudentDialog->setStyleSheet(QString::fromUtf8("\n"
"QDialog { background-color: #F4F6F9; }\n"
"QFrame#frameCard { background-color: #FFFFFF; border: 1px solid #D1D5DB; border-radius: 8px; }\n"
"QLabel#lblTitle { background-color: #1E3A8A; color: #FFFFFF; font-weight: bold; font-size: 15px; padding: 12px; border-top-left-radius: 7px; border-top-right-radius: 7px; }\n"
"QLabel { font-weight: bold; color: #334155; font-size: 13px; }\n"
"QLineEdit, QComboBox { padding: 8px; border: 1px solid #CBD5E1; border-radius: 5px; background-color: #F8FAFC; color: #0F172A; font-size: 13px; }\n"
"QLineEdit:focus, QComboBox:focus { border: 2px solid #2563EB; background-color: #FFFFFF; }\n"
"QGroupBox { font-weight: bold; color: #1E3A8A; border: 1px solid #94A3B8; border-radius: 6px; margin-top: 10px; padding-top: 15px; background-color: #F8FAFC; }\n"
"QGroupBox::title { subcontrol-origin: margin; left: 10px; padding: 0 5px; background-color: #F8FAFC; }\n"
"QPushButton#btnSave { background-color: #2563EB; color: white; font-weight: bold; border-radius: 5px; padding: 8px 20p"
                        "x; font-size: 13px; }\n"
"QPushButton#btnSave:hover { background-color: #1D4ED8; }\n"
"QPushButton#btnCancel { background-color: #E5E7EB; color: #374151; font-weight: bold; border-radius: 5px; padding: 8px 20px; font-size: 13px; }\n"
"QPushButton#btnCancel:hover { background-color: #D1D5DB; }\n"
"   "));
        frameCard = new QFrame(StudentDialog);
        frameCard->setObjectName("frameCard");
        frameCard->setGeometry(QRect(20, 20, 510, 560));
        frameCard->setFrameShape(QFrame::StyledPanel);
        lblTitle = new QLabel(frameCard);
        lblTitle->setObjectName("lblTitle");
        lblTitle->setGeometry(QRect(0, 0, 510, 45));
        lblTitle->setAlignment(Qt::AlignCenter);
        lblId = new QLabel(frameCard);
        lblId->setObjectName("lblId");
        lblId->setGeometry(QRect(30, 65, 110, 30));
        txtId = new QLineEdit(frameCard);
        txtId->setObjectName("txtId");
        txtId->setGeometry(QRect(150, 65, 330, 35));
        lblName = new QLabel(frameCard);
        lblName->setObjectName("lblName");
        lblName->setGeometry(QRect(30, 115, 110, 30));
        txtName = new QLineEdit(frameCard);
        txtName->setObjectName("txtName");
        txtName->setGeometry(QRect(150, 115, 330, 35));
        lblDob = new QLabel(frameCard);
        lblDob->setObjectName("lblDob");
        lblDob->setGeometry(QRect(30, 165, 110, 30));
        txtDob = new QLineEdit(frameCard);
        txtDob->setObjectName("txtDob");
        txtDob->setGeometry(QRect(150, 165, 330, 35));
        lblClass = new QLabel(frameCard);
        lblClass->setObjectName("lblClass");
        lblClass->setGeometry(QRect(30, 215, 110, 30));
        txtClass = new QLineEdit(frameCard);
        txtClass->setObjectName("txtClass");
        txtClass->setGeometry(QRect(150, 215, 330, 35));
        lblMajor = new QLabel(frameCard);
        lblMajor->setObjectName("lblMajor");
        lblMajor->setGeometry(QRect(30, 265, 110, 30));
        cboMajor = new QComboBox(frameCard);
        cboMajor->addItem(QString());
        cboMajor->addItem(QString());
        cboMajor->addItem(QString());
        cboMajor->setObjectName("cboMajor");
        cboMajor->setGeometry(QRect(150, 265, 330, 35));
        grpScores = new QGroupBox(frameCard);
        grpScores->setObjectName("grpScores");
        grpScores->setGeometry(QRect(30, 315, 450, 180));
        stackedScores = new QStackedWidget(grpScores);
        stackedScores->setObjectName("stackedScores");
        stackedScores->setGeometry(QRect(10, 25, 430, 145));
        pageIT = new QWidget();
        pageIT->setObjectName("pageIT");
        lblIT1 = new QLabel(pageIT);
        lblIT1->setObjectName("lblIT1");
        lblIT1->setGeometry(QRect(10, 10, 140, 30));
        txtScoreIT1 = new QLineEdit(pageIT);
        txtScoreIT1->setObjectName("txtScoreIT1");
        txtScoreIT1->setGeometry(QRect(160, 10, 250, 32));
        lblIT2 = new QLabel(pageIT);
        lblIT2->setObjectName("lblIT2");
        lblIT2->setGeometry(QRect(10, 50, 140, 30));
        txtScoreIT2 = new QLineEdit(pageIT);
        txtScoreIT2->setObjectName("txtScoreIT2");
        txtScoreIT2->setGeometry(QRect(160, 50, 250, 32));
        lblIT3 = new QLabel(pageIT);
        lblIT3->setObjectName("lblIT3");
        lblIT3->setGeometry(QRect(10, 90, 140, 30));
        txtScoreIT3 = new QLineEdit(pageIT);
        txtScoreIT3->setObjectName("txtScoreIT3");
        txtScoreIT3->setGeometry(QRect(160, 90, 250, 32));
        stackedScores->addWidget(pageIT);
        pageMath = new QWidget();
        pageMath->setObjectName("pageMath");
        lblMath1 = new QLabel(pageMath);
        lblMath1->setObjectName("lblMath1");
        lblMath1->setGeometry(QRect(10, 25, 140, 30));
        txtScoreMath1 = new QLineEdit(pageMath);
        txtScoreMath1->setObjectName("txtScoreMath1");
        txtScoreMath1->setGeometry(QRect(160, 25, 250, 35));
        lblMath2 = new QLabel(pageMath);
        lblMath2->setObjectName("lblMath2");
        lblMath2->setGeometry(QRect(10, 75, 140, 30));
        txtScoreMath2 = new QLineEdit(pageMath);
        txtScoreMath2->setObjectName("txtScoreMath2");
        txtScoreMath2->setGeometry(QRect(160, 75, 250, 35));
        stackedScores->addWidget(pageMath);
        pageBio = new QWidget();
        pageBio->setObjectName("pageBio");
        lblBio1 = new QLabel(pageBio);
        lblBio1->setObjectName("lblBio1");
        lblBio1->setGeometry(QRect(10, 25, 140, 30));
        txtScoreBio1 = new QLineEdit(pageBio);
        txtScoreBio1->setObjectName("txtScoreBio1");
        txtScoreBio1->setGeometry(QRect(160, 25, 250, 35));
        lblBio2 = new QLabel(pageBio);
        lblBio2->setObjectName("lblBio2");
        lblBio2->setGeometry(QRect(10, 75, 140, 30));
        txtScoreBio2 = new QLineEdit(pageBio);
        txtScoreBio2->setObjectName("txtScoreBio2");
        txtScoreBio2->setGeometry(QRect(160, 75, 250, 35));
        stackedScores->addWidget(pageBio);
        btnSave = new QPushButton(frameCard);
        btnSave->setObjectName("btnSave");
        btnSave->setGeometry(QRect(250, 510, 110, 38));
        btnCancel = new QPushButton(frameCard);
        btnCancel->setObjectName("btnCancel");
        btnCancel->setGeometry(QRect(370, 510, 110, 38));

        retranslateUi(StudentDialog);

        stackedScores->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(StudentDialog);
    } // setupUi

    void retranslateUi(QDialog *StudentDialog)
    {
        StudentDialog->setWindowTitle(QCoreApplication::translate("StudentDialog", "Th\303\264ng Tin Sinh Vi\303\252n", nullptr));
        lblTitle->setText(QCoreApplication::translate("StudentDialog", "\360\237\223\235 C\341\272\254P NH\341\272\254T TH\303\224NG TIN SINH VI\303\212N", nullptr));
        lblId->setText(QCoreApplication::translate("StudentDialog", "M\303\243 S\341\273\221 SV (*):", nullptr));
        txtId->setPlaceholderText(QCoreApplication::translate("StudentDialog", "V\303\255 d\341\273\245: 25127001", nullptr));
        lblName->setText(QCoreApplication::translate("StudentDialog", "H\341\273\215 v\303\240 T\303\252n (*):", nullptr));
        txtName->setPlaceholderText(QCoreApplication::translate("StudentDialog", "Nh\341\272\255p h\341\273\215 t\303\252n sinh vi\303\252n...", nullptr));
        lblDob->setText(QCoreApplication::translate("StudentDialog", "Ng\303\240y Sinh:", nullptr));
        txtDob->setPlaceholderText(QCoreApplication::translate("StudentDialog", "DD/MM/YYYY (V\303\255 d\341\273\245: 15/05/2007)", nullptr));
        lblClass->setText(QCoreApplication::translate("StudentDialog", "L\341\273\233p SH:", nullptr));
        txtClass->setPlaceholderText(QCoreApplication::translate("StudentDialog", "V\303\255 d\341\273\245: 25DTH1", nullptr));
        lblMajor->setText(QCoreApplication::translate("StudentDialog", "Ng\303\240nh H\341\273\215c:", nullptr));
        cboMajor->setItemText(0, QCoreApplication::translate("StudentDialog", "\360\237\222\273 Ng\303\240nh C\303\264ng Ngh\341\273\207 Th\303\264ng Tin", nullptr));
        cboMajor->setItemText(1, QCoreApplication::translate("StudentDialog", "\360\237\223\220 Ng\303\240nh To\303\241n H\341\273\215c", nullptr));
        cboMajor->setItemText(2, QCoreApplication::translate("StudentDialog", "\360\237\247\254 Ng\303\240nh Sinh H\341\273\215c", nullptr));

        grpScores->setTitle(QCoreApplication::translate("StudentDialog", "\360\237\223\212 \304\220i\341\273\203m M\303\264n Chuy\303\252n Ng\303\240nh (Thang 10)", nullptr));
        lblIT1->setText(QCoreApplication::translate("StudentDialog", "\342\200\242 \304\220i\341\273\203m C++:", nullptr));
        txtScoreIT1->setPlaceholderText(QCoreApplication::translate("StudentDialog", "0.0 -> 10.0", nullptr));
        lblIT2->setText(QCoreApplication::translate("StudentDialog", "\342\200\242 To\303\241n r\341\273\235i r\341\272\241c:", nullptr));
        txtScoreIT2->setPlaceholderText(QCoreApplication::translate("StudentDialog", "0.0 -> 10.0", nullptr));
        lblIT3->setText(QCoreApplication::translate("StudentDialog", "\342\200\242 C\341\272\245u tr\303\272c d\341\273\257 li\341\273\207u:", nullptr));
        txtScoreIT3->setPlaceholderText(QCoreApplication::translate("StudentDialog", "0.0 -> 10.0", nullptr));
        lblMath1->setText(QCoreApplication::translate("StudentDialog", "\342\200\242 Gi\341\272\243i t\303\255ch:", nullptr));
        txtScoreMath1->setPlaceholderText(QCoreApplication::translate("StudentDialog", "0.0 -> 10.0", nullptr));
        lblMath2->setText(QCoreApplication::translate("StudentDialog", "\342\200\242 \304\220\341\272\241i s\341\273\221 tuy\341\272\277n t\303\255nh:", nullptr));
        txtScoreMath2->setPlaceholderText(QCoreApplication::translate("StudentDialog", "0.0 -> 10.0", nullptr));
        lblBio1->setText(QCoreApplication::translate("StudentDialog", "\342\200\242 Sinh h\341\273\215c ph\303\242n t\341\273\255:", nullptr));
        txtScoreBio1->setPlaceholderText(QCoreApplication::translate("StudentDialog", "0.0 -> 10.0", nullptr));
        lblBio2->setText(QCoreApplication::translate("StudentDialog", "\342\200\242 Di truy\341\273\201n h\341\273\215c:", nullptr));
        txtScoreBio2->setPlaceholderText(QCoreApplication::translate("StudentDialog", "0.0 -> 10.0", nullptr));
        btnSave->setText(QCoreApplication::translate("StudentDialog", "\360\237\222\276 L\306\260u L\341\272\241i", nullptr));
        btnCancel->setText(QCoreApplication::translate("StudentDialog", "\342\235\214 H\341\273\247y B\341\273\217", nullptr));
    } // retranslateUi

};

namespace Ui {
    class StudentDialog: public Ui_StudentDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_STUDENTDIALOG_H
