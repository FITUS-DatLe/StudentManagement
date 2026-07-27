#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "studentprofiledialog.h"
#include <QMainWindow>
#include <QString>
#include "studentmanager.h"

// --- BỔ SUNG MODULE QTCHARTS (CHUẨN QT6 / QT5) ---
#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QValueAxis>
#include <QtCharts/QLineSeries>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

/**
 * @brief MainWindow - Enterprise Dashboard for Student Management System
 * Features Universal Event Interception for dynamic UI binding and Synonym-Aware filtering.
 * Integrated with QtCharts Interactive Analytics Engine.
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private slots:
    // --- AUTOMATIC UI REFRESH SLOTS ---
    void refreshTable();
    void updateDashboardStats();
    void handleManagerError(const QString &errorMessage);

    // --- SEARCH & SORTING SLOTS ---
    void on_txtSearch_textChanged(const QString &keyword);
    void on_cboSort_currentIndexChanged(int index);

    // --- CRUD OPERATION SLOTS ---
    void on_btnAdd_clicked();
    void on_btnEdit_clicked();
    void on_btnDelete_clicked();

private:
    Ui::MainWindow *ui;
    QString currentFilterMajor; // Stores the active filter category

    // --- QUẢN LÝ BIỂU ĐỒ QTCHARTS ---
    QChartView *pieChartView;   // Biểu đồ tròn: Tỷ lệ ngành
    QChartView *barChartView;   // Biểu đồ cột: Phổ điểm GPA
    QChartView *lineChartView;  // Biểu đồ đường: Xu hướng GPA

    // Dynamic Chart Engine
    void setupAnalyticsTab();
    void refreshDashboardCharts();

    // Core initialization and styling helpers
    void setupTableHeaders();
    void formatRowColor(int row, const QString &classification);
    void bindUniversalSidebarEvents(); // Universal binding engine for any UI container
    void applyCategoryFilter(const QString &selectedCategoryText);
};

#endif // MAINWINDOW_H