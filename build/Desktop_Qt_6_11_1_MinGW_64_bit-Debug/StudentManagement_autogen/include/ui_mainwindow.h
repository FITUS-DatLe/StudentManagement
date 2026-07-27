/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.11.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QFrame *frameHeader;
    QLabel *lblHeaderTitle;
    QLabel *lblUserInfo;
    QFrame *frameSidebar;
    QLabel *lblSidebarTitle;
    QListWidget *listMajors;
    QFrame *frameStats;
    QLabel *lblStatTitle;
    QLabel *lblStatTotal;
    QLabel *lblStatIT;
    QLabel *lblStatMath;
    QLabel *lblStatBio;
    QLabel *lblStatScholar;
    QFrame *frameContent;
    QLineEdit *txtSearch;
    QComboBox *cboSort;
    QPushButton *btnAdd;
    QPushButton *btnEdit;
    QPushButton *btnDelete;
    QTableWidget *tblStudents;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(1050, 650);
        MainWindow->setStyleSheet(QString::fromUtf8("\n"
"QMainWindow, QWidget#centralwidget { background-color: #F4F6F9; }\n"
"QFrame#frameHeader { background-color: #1E3A8A; border-bottom: 2px solid #1D4ED8; }\n"
"QLabel#lblHeaderTitle { color: #FFFFFF; font-size: 16px; font-weight: bold; }\n"
"QLabel#lblUserInfo { color: #E0F2FE; font-size: 13px; font-weight: bold; }\n"
"QFrame#frameSidebar, QFrame#frameContent { background-color: #FFFFFF; border: 1px solid #D1D5DB; border-radius: 8px; }\n"
"QLabel#lblSidebarTitle { color: #1E3A8A; font-size: 13px; font-weight: bold; border-bottom: 1px solid #E2E8F0; padding-bottom: 5px; }\n"
"QListWidget#listMajors { border: none; background-color: transparent; font-size: 13px; outline: 0; }\n"
"QListWidget#listMajors::item { padding: 12px 10px; border-radius: 6px; margin-bottom: 4px; color: #334155; font-weight: bold; }\n"
"QListWidget#listMajors::item:hover { background-color: #F1F5F9; color: #1E3A8A; }\n"
"QListWidget#listMajors::item:selected { background-color: #2563EB; color: #FFFFFF; }\n"
"QFrame#frameStats { backgrou"
                        "nd-color: #F8FAFC; border: 1px dashed #CBD5E1; border-radius: 6px; }\n"
"QLabel#lblStatTitle { color: #0F172A; font-size: 12px; font-weight: bold; }\n"
"QLabel#lblStatTotal, QLabel#lblStatIT, QLabel#lblStatMath, QLabel#lblStatBio, QLabel#lblStatScholar { color: #475569; font-size: 12px; font-weight: bold; }\n"
"QLineEdit#txtSearch { padding: 6px 12px; border: 1px solid #CBD5E1; border-radius: 6px; background-color: #F8FAFC; color: #0F172A; font-size: 13px; }\n"
"QLineEdit#txtSearch:focus { border: 2px solid #2563EB; background-color: #FFFFFF; }\n"
"QComboBox#cboSort { padding: 6px 10px; border: 1px solid #CBD5E1; border-radius: 6px; background-color: #FFFFFF; color: #334155; font-size: 13px; font-weight: bold; }\n"
"QPushButton#btnAdd { background-color: #2563EB; color: white; font-weight: bold; border-radius: 6px; font-size: 13px; }\n"
"QPushButton#btnAdd:hover { background-color: #1D4ED8; }\n"
"QPushButton#btnEdit { background-color: #F59E0B; color: white; font-weight: bold; border-radius: 6px; font-size: 13"
                        "px; }\n"
"QPushButton#btnEdit:hover { background-color: #D97706; }\n"
"QPushButton#btnDelete { background-color: #EF4444; color: white; font-weight: bold; border-radius: 6px; font-size: 13px; }\n"
"QPushButton#btnDelete:hover { background-color: #DC2626; }\n"
"QTableWidget#tblStudents { border: 1px solid #E2E8F0; border-radius: 6px; background-color: #FFFFFF; gridline-color: #F1F5F9; selection-background-color: #DBEAFE; selection-color: #1E3A8A; font-size: 13px; outline: 0; }\n"
"QHeaderView::section { background-color: #F8FAFC; color: #334155; font-weight: bold; border: none; border-bottom: 2px solid #CBD5E1; border-right: 1px solid #E2E8F0; padding: 8px; font-size: 13px; }\n"
"   "));
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        frameHeader = new QFrame(centralwidget);
        frameHeader->setObjectName("frameHeader");
        frameHeader->setGeometry(QRect(0, 0, 1050, 60));
        lblHeaderTitle = new QLabel(frameHeader);
        lblHeaderTitle->setObjectName("lblHeaderTitle");
        lblHeaderTitle->setGeometry(QRect(20, 10, 500, 40));
        lblUserInfo = new QLabel(frameHeader);
        lblUserInfo->setObjectName("lblUserInfo");
        lblUserInfo->setGeometry(QRect(780, 15, 250, 30));
        lblUserInfo->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        frameSidebar = new QFrame(centralwidget);
        frameSidebar->setObjectName("frameSidebar");
        frameSidebar->setGeometry(QRect(20, 80, 240, 550));
        lblSidebarTitle = new QLabel(frameSidebar);
        lblSidebarTitle->setObjectName("lblSidebarTitle");
        lblSidebarTitle->setGeometry(QRect(15, 15, 210, 25));
        listMajors = new QListWidget(frameSidebar);
        new QListWidgetItem(listMajors);
        new QListWidgetItem(listMajors);
        new QListWidgetItem(listMajors);
        new QListWidgetItem(listMajors);
        new QListWidgetItem(listMajors);
        listMajors->setObjectName("listMajors");
        listMajors->setGeometry(QRect(15, 50, 210, 220));
        frameStats = new QFrame(frameSidebar);
        frameStats->setObjectName("frameStats");
        frameStats->setGeometry(QRect(15, 280, 210, 250));
        lblStatTitle = new QLabel(frameStats);
        lblStatTitle->setObjectName("lblStatTitle");
        lblStatTitle->setGeometry(QRect(15, 15, 180, 25));
        lblStatTotal = new QLabel(frameStats);
        lblStatTotal->setObjectName("lblStatTotal");
        lblStatTotal->setGeometry(QRect(15, 55, 180, 25));
        lblStatIT = new QLabel(frameStats);
        lblStatIT->setObjectName("lblStatIT");
        lblStatIT->setGeometry(QRect(15, 90, 180, 25));
        lblStatMath = new QLabel(frameStats);
        lblStatMath->setObjectName("lblStatMath");
        lblStatMath->setGeometry(QRect(15, 125, 180, 25));
        lblStatBio = new QLabel(frameStats);
        lblStatBio->setObjectName("lblStatBio");
        lblStatBio->setGeometry(QRect(15, 160, 180, 25));
        lblStatScholar = new QLabel(frameStats);
        lblStatScholar->setObjectName("lblStatScholar");
        lblStatScholar->setGeometry(QRect(15, 205, 180, 25));
        lblStatScholar->setStyleSheet(QString::fromUtf8("color: #2563EB; font-weight: bold;"));
        frameContent = new QFrame(centralwidget);
        frameContent->setObjectName("frameContent");
        frameContent->setGeometry(QRect(280, 80, 750, 550));
        txtSearch = new QLineEdit(frameContent);
        txtSearch->setObjectName("txtSearch");
        txtSearch->setGeometry(QRect(20, 20, 240, 38));
        cboSort = new QComboBox(frameContent);
        cboSort->addItem(QString());
        cboSort->addItem(QString());
        cboSort->addItem(QString());
        cboSort->setObjectName("cboSort");
        cboSort->setGeometry(QRect(270, 20, 150, 38));
        btnAdd = new QPushButton(frameContent);
        btnAdd->setObjectName("btnAdd");
        btnAdd->setGeometry(QRect(435, 20, 95, 38));
        btnEdit = new QPushButton(frameContent);
        btnEdit->setObjectName("btnEdit");
        btnEdit->setGeometry(QRect(540, 20, 95, 38));
        btnDelete = new QPushButton(frameContent);
        btnDelete->setObjectName("btnDelete");
        btnDelete->setGeometry(QRect(645, 20, 85, 38));
        tblStudents = new QTableWidget(frameContent);
        if (tblStudents->columnCount() < 6)
            tblStudents->setColumnCount(6);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        tblStudents->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        tblStudents->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        tblStudents->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        tblStudents->setHorizontalHeaderItem(3, __qtablewidgetitem3);
        QTableWidgetItem *__qtablewidgetitem4 = new QTableWidgetItem();
        tblStudents->setHorizontalHeaderItem(4, __qtablewidgetitem4);
        QTableWidgetItem *__qtablewidgetitem5 = new QTableWidgetItem();
        tblStudents->setHorizontalHeaderItem(5, __qtablewidgetitem5);
        tblStudents->setObjectName("tblStudents");
        tblStudents->setGeometry(QRect(20, 75, 710, 455));
        tblStudents->setSelectionBehavior(QAbstractItemView::SelectRows);
        MainWindow->setCentralWidget(centralwidget);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "H\341\273\207 Th\341\273\221ng Qu\341\272\243n L\303\275 Sinh Vi\303\252n - HCMUS Dashboard", nullptr));
        lblHeaderTitle->setText(QCoreApplication::translate("MainWindow", "\360\237\216\223 H\341\273\206 TH\341\273\220NG QU\341\272\242N L\303\235 SINH VI\303\212N - HCMUS", nullptr));
        lblUserInfo->setText(QCoreApplication::translate("MainWindow", "\360\237\221\244 Admin: 25127293 | Tr\341\272\241ng th\303\241i: Online", nullptr));
        lblSidebarTitle->setText(QCoreApplication::translate("MainWindow", "\360\237\223\202 DANH M\341\273\244C NG\303\200NH", nullptr));

        const bool __sortingEnabled = listMajors->isSortingEnabled();
        listMajors->setSortingEnabled(false);
        QListWidgetItem *___qlistwidgetitem = listMajors->item(0);
        ___qlistwidgetitem->setText(QCoreApplication::translate("MainWindow", "\360\237\214\237 T\341\272\245t c\341\272\243 sinh vi\303\252n", nullptr));
        QListWidgetItem *___qlistwidgetitem1 = listMajors->item(1);
        ___qlistwidgetitem1->setText(QCoreApplication::translate("MainWindow", "\360\237\222\273 Ng\303\240nh CNTT", nullptr));
        QListWidgetItem *___qlistwidgetitem2 = listMajors->item(2);
        ___qlistwidgetitem2->setText(QCoreApplication::translate("MainWindow", "\360\237\223\220 Ng\303\240nh To\303\241n h\341\273\215c", nullptr));
        QListWidgetItem *___qlistwidgetitem3 = listMajors->item(3);
        ___qlistwidgetitem3->setText(QCoreApplication::translate("MainWindow", "\360\237\247\254 Ng\303\240nh Sinh h\341\273\215c", nullptr));
        QListWidgetItem *___qlistwidgetitem4 = listMajors->item(4);
        ___qlistwidgetitem4->setText(QCoreApplication::translate("MainWindow", "\360\237\217\206 Danh s\303\241ch H\341\273\215c b\341\273\225ng", nullptr));
        listMajors->setSortingEnabled(__sortingEnabled);

        lblStatTitle->setText(QCoreApplication::translate("MainWindow", "\360\237\223\212 TH\341\273\220NG K\303\212 NHANH", nullptr));
        lblStatTotal->setText(QCoreApplication::translate("MainWindow", "\342\200\242 T\341\273\225ng sinh vi\303\252n: 3", nullptr));
        lblStatIT->setText(QCoreApplication::translate("MainWindow", "\342\200\242 Ng\303\240nh CNTT: 1", nullptr));
        lblStatMath->setText(QCoreApplication::translate("MainWindow", "\342\200\242 Ng\303\240nh To\303\241n: 1", nullptr));
        lblStatBio->setText(QCoreApplication::translate("MainWindow", "\342\200\242 Ng\303\240nh Sinh: 1", nullptr));
        lblStatScholar->setText(QCoreApplication::translate("MainWindow", "\342\200\242 H\341\273\215c b\341\273\225ng: 2 SV", nullptr));
        txtSearch->setPlaceholderText(QCoreApplication::translate("MainWindow", "\360\237\224\215 T\303\254m ki\341\272\277m theo t\303\252n, m\303\243 s\341\273\221...", nullptr));
        cboSort->setItemText(0, QCoreApplication::translate("MainWindow", "S\341\272\257p x\341\272\277p: M\303\243 s\341\273\221 \342\254\206", nullptr));
        cboSort->setItemText(1, QCoreApplication::translate("MainWindow", "S\341\272\257p x\341\272\277p: \304\220i\341\273\203m TB \342\254\207", nullptr));
        cboSort->setItemText(2, QCoreApplication::translate("MainWindow", "S\341\272\257p x\341\272\277p: \304\220i\341\273\203m TB \342\254\206", nullptr));

        btnAdd->setText(QCoreApplication::translate("MainWindow", "+ Th\303\252m SV", nullptr));
        btnEdit->setText(QCoreApplication::translate("MainWindow", "\342\234\217\357\270\217 S\341\273\255a", nullptr));
        btnDelete->setText(QCoreApplication::translate("MainWindow", "\360\237\227\221\357\270\217 X\303\263a", nullptr));
        QTableWidgetItem *___qtablewidgetitem = tblStudents->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QCoreApplication::translate("MainWindow", "M\303\243 S\341\273\221", nullptr));
        QTableWidgetItem *___qtablewidgetitem1 = tblStudents->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QCoreApplication::translate("MainWindow", "H\341\273\215 v\303\240 T\303\252n", nullptr));
        QTableWidgetItem *___qtablewidgetitem2 = tblStudents->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QCoreApplication::translate("MainWindow", "Ng\303\240y Sinh", nullptr));
        QTableWidgetItem *___qtablewidgetitem3 = tblStudents->horizontalHeaderItem(3);
        ___qtablewidgetitem3->setText(QCoreApplication::translate("MainWindow", "Ng\303\240nh H\341\273\215c", nullptr));
        QTableWidgetItem *___qtablewidgetitem4 = tblStudents->horizontalHeaderItem(4);
        ___qtablewidgetitem4->setText(QCoreApplication::translate("MainWindow", "\304\220i\341\273\203m TB", nullptr));
        QTableWidgetItem *___qtablewidgetitem5 = tblStudents->horizontalHeaderItem(5);
        ___qtablewidgetitem5->setText(QCoreApplication::translate("MainWindow", "X\341\272\277p Lo\341\272\241i / H\341\273\215c B\341\273\225ng", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
