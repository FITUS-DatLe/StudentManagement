#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "studentdialog.h"
#include <QMessageBox>
#include <QTableWidgetItem>
#include <QHeaderView>
#include <QColor>
#include <QLabel>
#include <QListWidget>
#include <QTreeWidget>
#include <QComboBox>
#include <QAbstractButton>
#include <QDebug>
#include <utility> // Sử dụng std::as_const
#include "studentprofiledialog.h"
#include <QGridLayout>
#include <QFrame>
#include <QMessageBox>
#include <QPen>
#include <QColor>
#include <QDebug>
#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QValueAxis>
#include <QtCharts/QLineSeries>
// ============================================================================
// KHỞI TẠO & HỦY DIỆT GIAO DIỆN
// ============================================================================

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , currentFilterMajor("Tất cả")
{
    if (!StudentManager::getInstance().initManager()) {
        QMessageBox::critical(this, "Lỗi Nghiêm Trọng", "Không thể khởi tạo CSDL!");
    }

    setupAnalyticsTab();
    ui->setupUi(this);
    this->setWindowTitle("Hệ Thống Quản Lý Sinh Viên - HCMUS Dashboard (Pro Enterprise)");

    // 1. Cấu hình bảng dữ liệu
    setupTableHeaders();

    // 2. Khởi tạo Controller & CSDL SQLite vĩnh viễn
    if (!StudentManager::getInstance().initManager()) {
        QMessageBox::critical(this, "Lỗi Nghiêm Trọng", "Không thể khởi tạo cơ sở dữ liệu SQLite! Vui lòng kiểm tra quyền truy cập ổ đĩa.");
    }

    // 3. Kết nối tín hiệu (Signals) từ StudentManager tới giao diện (Slots)
    connect(&StudentManager::getInstance(), &StudentManager::dataChanged, this, &MainWindow::refreshTable);
    connect(&StudentManager::getInstance(), &StudentManager::operationError, this, &MainWindow::handleManagerError);
    connect(ui->tblStudents, &QTableWidget::cellDoubleClicked, this, [this](int row, int col) {
        Q_UNUSED(col);
        QTableWidgetItem *item = ui->tblStudents->item(row, 0); // Lấy MSSV ở cột 0
        if (item) {
            QString studentId = item->text().trimmed();
            if (!studentId.isEmpty()) {
                StudentProfileDialog profileDialog(studentId, this);
                profileDialog.exec(); // Mở cửa sổ Hồ sơ sinh viên chi tiết 360°
            }
        }
    });
    // 4. Kết nối tự động toàn bộ thanh điều hướng bên trái
    bindUniversalSidebarEvents();

    // 5. Nạp dữ liệu lên bảng lần đầu tiên

    setupAnalyticsTab();
    refreshDashboardCharts();
}

MainWindow::~MainWindow()
{
    delete ui;
}

// ============================================================================
// BỘ LIÊN KẾT ĐIỀU HƯỚNG TỰ ĐỘNG (SIDEBAR BINDING ENGINE)
// ============================================================================

void MainWindow::bindUniversalSidebarEvents()
{
    // Liên kết QListWidget
    QList<QListWidget*> listWidgets = this->findChildren<QListWidget*>();
    for (QListWidget *lw : listWidgets) {
        connect(lw, &QListWidget::itemClicked, this, [this](QListWidgetItem *item) {
            if (item) applyCategoryFilter(item->text());
        });
        connect(lw, &QListWidget::currentTextChanged, this, [this](const QString &text) {
            applyCategoryFilter(text);
        });
    }

    // Liên kết QTreeWidget
    QList<QTreeWidget*> treeWidgets = this->findChildren<QTreeWidget*>();
    for (QTreeWidget *tw : treeWidgets) {
        connect(tw, &QTreeWidget::itemClicked, this, [this](QTreeWidgetItem *item, int) {
            if (item) applyCategoryFilter(item->text(0));
        });
    }

    // Liên kết toàn bộ các nút bấm tiêu chuẩn (QPushButton, QToolButton...)
    QList<QAbstractButton*> allButtons = this->findChildren<QAbstractButton*>();
    for (QAbstractButton *btn : allButtons) {
        QString btnName = btn->objectName().toLower();
        if (btnName.contains("add") || btnName.contains("edit") || btnName.contains("delete") || btnName.contains("save")) {
            continue;
        }
        connect(btn, &QAbstractButton::clicked, this, [this, btn]() {
            applyCategoryFilter(btn->text());
        });
    }

    // Liên kết QComboBox lọc ngành
    QList<QComboBox*> comboBoxes = this->findChildren<QComboBox*>();
    for (QComboBox *cbo : comboBoxes) {
        if (cbo->objectName().contains("Filter", Qt::CaseInsensitive) || cbo->objectName().contains("Major", Qt::CaseInsensitive)) {
            connect(cbo, &QComboBox::currentTextChanged, this, [this](const QString &text) {
                applyCategoryFilter(text);
            });
        }
    }
}

void MainWindow::applyCategoryFilter(const QString &selectedCategoryText)
{
    QString cleanText = selectedCategoryText.trimmed();
    if (cleanText.isEmpty()) return;

    qInfo() << "[NAV FILTER] Chọn danh mục từ giao diện:" << cleanText;

    if (cleanText.contains("Tất cả", Qt::CaseInsensitive) || cleanText.contains("All", Qt::CaseInsensitive)) {
        currentFilterMajor = "Tất cả";
    } else if (cleanText.contains("CNTT", Qt::CaseInsensitive) || cleanText.contains("Công nghệ", Qt::CaseInsensitive) || cleanText.contains("IT", Qt::CaseInsensitive)) {
        currentFilterMajor = "CNTT";
    } else if (cleanText.contains("Toán", Qt::CaseInsensitive) || cleanText.contains("Math", Qt::CaseInsensitive)) {
        currentFilterMajor = "Toán";
    } else if (cleanText.contains("Sinh", Qt::CaseInsensitive) || cleanText.contains("Bio", Qt::CaseInsensitive)) {
        currentFilterMajor = "Sinh";
    } else if (cleanText.contains("Học bổng", Qt::CaseInsensitive) || cleanText.contains("Scholarship", Qt::CaseInsensitive)) {
        currentFilterMajor = "Học bổng";
    } else {
        return;
    }

    refreshTable();
}

// ============================================================================
// HIỂN THỊ DỮ LIỆU & BỘ LỌC ĐỒNG NGHĨA
// ============================================================================

void MainWindow::setupTableHeaders()
{
    ui->tblStudents->setColumnCount(6);
    QStringList headers = {"Mã Số", "Họ và Tên", "Ngày Sinh", "Ngành Học", "Điểm TB", "Xếp Loại / Học Bổng"};
    ui->tblStudents->setHorizontalHeaderLabels(headers);

    ui->tblStudents->horizontalHeader()->setStretchLastSection(true);
    ui->tblStudents->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    ui->tblStudents->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    ui->tblStudents->horizontalHeader()->setSectionResizeMode(3, QHeaderView::ResizeToContents);

    ui->tblStudents->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tblStudents->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tblStudents->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tblStudents->setAlternatingRowColors(true);
}

void MainWindow::formatRowColor(int row, const QString &classification)
{
    if (classification.contains("🔥") || classification.contains("🏆") || classification.contains("🧬") || classification.contains("🌟")) {
        for (int col = 0; col < ui->tblStudents->columnCount(); ++col) {
            QTableWidgetItem *item = ui->tblStudents->item(row, col);
            if (item) {
                item->setBackground(QColor(255, 248, 220)); // Màu nền vàng kem làm nổi bật nhân tố tài năng
                item->setFont(QFont("Segoe UI", 9, QFont::Bold));
            }
        }
    }
}

void MainWindow::refreshTable()
{
    ui->tblStudents->setRowCount(0);

    const QVector<Student*> allStudents = StudentManager::getInstance().getAllStudents();
    QVector<Student*> filteredList;
    QString searchKey = ui->txtSearch->text().trimmed();

    // 1. Ưu tiên bộ lọc tìm kiếm trực tiếp (Live-Search)
    if (!searchKey.isEmpty()) {
        filteredList = StudentManager::getInstance().searchStudents(searchKey);
    }
    // 2. Bộ lọc danh sách đạt Học bổng
    else if (currentFilterMajor == "Học bổng") {
        filteredList = StudentManager::getInstance().filterScholarshipCandidates();
    }
    // 3. Hiển thị tất cả sinh viên
    else if (currentFilterMajor == "Tất cả") {
        filteredList = allStudents;
    }
    // 4. Lọc theo chuyên ngành
    else {
        for (Student *s : std::as_const(allStudents)) {
            QString major = s->getMajor().toLower();
            if (currentFilterMajor == "CNTT" && (major.contains("cntt") || major.contains("công nghệ") || major.contains("it") || major.contains("inform"))) {
                filteredList.append(s);
            } else if (currentFilterMajor == "Toán" && (major.contains("toán") || major.contains("math"))) {
                filteredList.append(s);
            } else if (currentFilterMajor == "Sinh" && (major.contains("sinh") || major.contains("bio"))) {
                filteredList.append(s);
            } else if (major.contains(currentFilterMajor.toLower())) {
                filteredList.append(s);
            }
        }
    }

    // Đổ dữ liệu vào bảng hiển thị
    for (int i = 0; i < filteredList.size(); ++i) {
        const Student *s = filteredList.at(i);
        ui->tblStudents->insertRow(i);

        auto *item0 = new QTableWidgetItem(s->getId());
        auto *item1 = new QTableWidgetItem(s->getFullName());
        auto *item2 = new QTableWidgetItem(s->getFormattedDob());
        auto *item3 = new QTableWidgetItem(s->getMajor());
        auto *item4 = new QTableWidgetItem(QString::number(s->calculateGPA(), 'f', 2));
        auto *item5 = new QTableWidgetItem(s->getClassification());

        item0->setTextAlignment(Qt::AlignCenter);
        item2->setTextAlignment(Qt::AlignCenter);
        item4->setTextAlignment(Qt::AlignCenter);

        ui->tblStudents->setItem(i, 0, item0);
        ui->tblStudents->setItem(i, 1, item1);
        ui->tblStudents->setItem(i, 2, item2);
        ui->tblStudents->setItem(i, 3, item3);
        ui->tblStudents->setItem(i, 4, item4);
        ui->tblStudents->setItem(i, 5, item5);

        formatRowColor(i, s->getClassification());
    }

    updateDashboardStats();
}

void MainWindow::updateDashboardStats()
{
    int total = StudentManager::getInstance().getTotalCount();
    QMap<QString, int> majors = StudentManager::getInstance().getMajorDistribution();

    int itCount = 0, mathCount = 0, bioCount = 0;
    for (auto it = majors.begin(); it != majors.end(); ++it) {
        QString key = it.key().toLower();
        if (key.contains("cntt") || key.contains("công nghệ") || key.contains("it")) itCount += it.value();
        else if (key.contains("toán") || key.contains("math")) mathCount += it.value();
        else if (key.contains("sinh") || key.contains("bio")) bioCount += it.value();
    }

    int scholarshipCount = StudentManager::getInstance().getScholarshipCount();
    int currentShownCount = ui->tblStudents->rowCount(); // Lấy đúng số lượng sinh viên đang hiển thị

    // Xây dựng nội dung cho tiêu đề đỉnh theo đúng mục đang chọn
    QString headerTitleText;
    QString searchKey = ui->txtSearch->text().trimmed();
    if (!searchKey.isEmpty()) {
        headerTitleText = QString("• Kết quả tìm kiếm: %1 SV").arg(currentShownCount);
    } else if (currentFilterMajor == "CNTT") {
        headerTitleText = QString("• Ngành CNTT: %1").arg(currentShownCount);
    } else if (currentFilterMajor == "Toán") {
        headerTitleText = QString("• Ngành Toán học: %1").arg(currentShownCount);
    } else if (currentFilterMajor == "Sinh") {
        headerTitleText = QString("• Ngành Sinh học: %1").arg(currentShownCount);
    } else if (currentFilterMajor == "Học bổng") {
        headerTitleText = QString("• Danh sách Học bổng: %1 SV").arg(currentShownCount);
    } else {
        headerTitleText = QString("• Tất cả sinh viên: %1").arg(total);
    }

    // Quét toàn bộ các QLabel trên màn hình và phân loại theo tọa độ không gian
    QList<QLabel*> allLabels = this->findChildren<QLabel*>();
    for (QLabel *lbl : allLabels) {
        // ✅ BẢO VỆ 1: Nếu nhãn không nằm trực tiếp trên cửa sổ MainWindow -> Bỏ qua ngay
        if (lbl->window() != this) continue;

        // ✅ BẢO VỆ 2: Chỉ xử lý các nhãn thống kê bắt đầu bằng dấu chấm tròn "•"
        if (!lbl->text().startsWith("•")) continue;
        QPoint posInWindow = lbl->mapTo(this, QPoint(0, 0));
        int relativeY = posInWindow.y();
        int relativeX = posInWindow.x();

        // 1. NHẬN DIỆN NHÃN TIÊU ĐỀ GÓC TRÊN BÊN TRÁI (Y < 80 pixel và X < 400 pixel)
        // Đảm bảo không ghi đè nhầm vào nhãn Admin bên phải hoặc bảng thống kê bên dưới
        if (relativeY < 80 && relativeX < 400) {
            lbl->setText(headerTitleText);
            continue; // Đã xử lý xong tiêu đề đỉnh, bỏ qua các bước kiểm tra bên dưới
        }

        // 2. NHẬN DIỆN CÁC NHÃN TRONG KHỐI "THỐNG KÊ NHANH" (Nằm ở vùng dưới Y > 100)
        QString txt = lbl->text();
        if (txt.contains("Tổng sinh viên", Qt::CaseInsensitive)) {
            lbl->setText(QString("• Tổng sinh viên: %1").arg(total));
        } else if (txt.contains("CNTT", Qt::CaseInsensitive)) {
            lbl->setText(QString("• Ngành CNTT: %1").arg(itCount));
        } else if (txt.contains("Toán", Qt::CaseInsensitive)) {
            lbl->setText(QString("• Ngành Toán: %1").arg(mathCount));
        } else if (txt.contains("Sinh", Qt::CaseInsensitive)) {
            lbl->setText(QString("• Ngành Sinh: %1").arg(bioCount));
        } else if (txt.contains("Học bổng", Qt::CaseInsensitive)) {
            lbl->setText(QString("• Học bổng: %1 SV").arg(scholarshipCount));
        }
    }
    refreshDashboardCharts();

}

void MainWindow::handleManagerError(const QString &errorMessage)
{
    QMessageBox::warning(this, "Thông báo từ Hệ thống", errorMessage);
}

// ============================================================================
// CÁC SLOTS TÌM KIẾM & SẮP XẾP
// ============================================================================

void MainWindow::on_txtSearch_textChanged(const QString &keyword)
{
    Q_UNUSED(keyword);
    refreshTable();
}

void MainWindow::on_cboSort_currentIndexChanged(int index)
{
    if (index == 0) StudentManager::getInstance().sortListByIdAsc();
    else if (index == 1) StudentManager::getInstance().sortListByGpaDesc();
    else if (index == 2) StudentManager::getInstance().sortListByNameAsc();
}

// ============================================================================
// CÁC THAO TÁC NGHIỆP VỤ (CRUD TRỰC TIẾP VỚI SQLITE)
// ============================================================================

void MainWindow::on_btnAdd_clicked()
{
    StudentDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        Student *newStudent = dialog.getStudent();
        if (newStudent) {
            if (StudentManager::getInstance().addStudent(newStudent)) {
                QMessageBox::information(this, "Thành công", QString("Đã thêm sinh viên [%1 - %2] vào CSDL SQLite vĩnh viễn!").arg(newStudent->getId(), newStudent->getFullName()));
            } else {
                delete newStudent;
            }
        }
    }
}

void MainWindow::on_btnEdit_clicked()
{
    int currentRow = ui->tblStudents->currentRow();
    if (currentRow < 0) {
        QMessageBox::warning(this, "Chưa chọn bản ghi", "Vui lòng click chọn một sinh viên trên bảng dữ liệu để tiến hành chỉnh sửa!");
        return;
    }

    QString studentId = ui->tblStudents->item(currentRow, 0)->text();
    Student *targetStudent = StudentManager::getInstance().getStudentById(studentId);

    if (targetStudent) {
        StudentDialog dialog(this);
        dialog.setStudentData(targetStudent);

        if (dialog.exec() == QDialog::Accepted) {
            Student *updatedStudent = dialog.getStudent();
            if (updatedStudent) {
                if (StudentManager::getInstance().updateStudent(updatedStudent)) {
                    QMessageBox::information(this, "Cập nhật thành công", "Thông tin sinh viên đã được cập nhật đồng bộ vào RAM và SQLite!");
                } else {
                    delete updatedStudent;
                }
            }
        }
    } else {
        QMessageBox::critical(this, "Lỗi dữ liệu", "Không tìm thấy con trỏ đối tượng sinh viên tương ứng trên bộ nhớ RAM!");
    }
}

void MainWindow::on_btnDelete_clicked()
{
    int currentRow = ui->tblStudents->currentRow();
    if (currentRow < 0) {
        QMessageBox::warning(this, "Chưa chọn bản ghi", "Vui lòng click chọn một sinh viên trên bảng để xóa!");
        return;
    }

    QString studentId = ui->tblStudents->item(currentRow, 0)->text();
    QString studentName = ui->tblStudents->item(currentRow, 1)->text();

    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Xác nhận xóa bản ghi",
                                  QString("Bạn có chắc chắn muốn xóa vĩnh viễn sinh viên:\n\n• Mã số: %1\n• Họ tên: %2\n\nKhỏi bộ nhớ và cơ sở dữ liệu SQLite không?").arg(studentId, studentName),
                                  QMessageBox::Yes | QMessageBox::No, QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        if (StudentManager::getInstance().deleteStudent(studentId)) {
            QMessageBox::information(this, "Đã xóa", QString("Đã xóa thành công sinh viên [%1] khỏi hệ thống.").arg(studentId));
        }
    }
}

// ============================================================================
// BƯỚC 2 (CHƯẨN HÓA TẬN GỐC): KHỞI TẠO KHUNG HÌNH PHÂN TÍCH VÀ TAB ĐỘNG
// ============================================================================
void MainWindow::setupAnalyticsTab()
{
    // 1. Khởi tạo 3 khung nhìn biểu đồ với chế độ Khử răng cưa (Antialiasing)
    pieChartView = new QChartView(this);
    pieChartView->setRenderHint(QPainter::Antialiasing);
    pieChartView->setMinimumHeight(320);
    pieChartView->setStyleSheet("background: white; border-radius: 8px; border: 1px solid #E0E0E0;");

    barChartView = new QChartView(this);
    barChartView->setRenderHint(QPainter::Antialiasing);
    barChartView->setMinimumHeight(320);
    barChartView->setStyleSheet("background: white; border-radius: 8px; border: 1px solid #E0E0E0;");

    lineChartView = new QChartView(this);
    lineChartView->setRenderHint(QPainter::Antialiasing);
    lineChartView->setMinimumHeight(260);
    lineChartView->setStyleSheet("background: white; border-radius: 8px; border: 1px solid #E0E0E0;");

    // 2. Lấy Widget trung tâm hiện tại của MainWindow (đang chứa bảng sinh viên, nút tìm kiếm cũ)
    QWidget *oldCentralWidget = this->centralWidget();

    // 3. Tạo một QTabWidget mới làm trung tâm chỉ huy 360° cho toàn phần mềm
    QTabWidget *mainTabWidget = new QTabWidget(this);
    mainTabWidget->setStyleSheet(R"(
        QTabWidget::pane { border: 1px solid #D5D8DC; background: #FFFFFF; border-radius: 6px; }
        QTabBar::tab { background: #EAECEE; color: #5D6D7E; padding: 10px 22px; font-weight: bold; font-size: 13px; border-top-left-radius: 6px; border-top-right-radius: 6px; margin-right: 3px; }
        QTabBar::tab:selected { background: #00509e; color: #FFFFFF; }
        QTabBar::tab:hover:!selected { background: #D5D8DC; color: #2C3E50; }
    )");

    // 4. Đưa toàn bộ giao diện quản lý cũ vào Tab 1 (Mọi nút bấm, sự kiện cũ giữ nguyên 100%)
    if (oldCentralWidget) {
        mainTabWidget->addTab(oldCentralWidget, "📋 Danh Sách & Quản Lý Sinh Viên");
    }

    // 5. Tạo QFrame chứa lưới 3 biểu đồ (KHÔNG truyền 'this' vào QFrame để tránh bị đè lơ lửng)
    QFrame *analyticsFrame = new QFrame();
    analyticsFrame->setStyleSheet("background-color: #F8F9FA;");

    QGridLayout *gridLayout = new QGridLayout(analyticsFrame);
    gridLayout->setContentsMargins(15, 15, 15, 15);
    gridLayout->setSpacing(15);

    // Phân bổ lưới tỷ lệ vàng:
    gridLayout->addWidget(pieChartView, 0, 0);        // Góc trên trái (50%)
    gridLayout->addWidget(barChartView, 0, 1);        // Góc trên phải (50%)
    gridLayout->addWidget(lineChartView, 1, 0, 1, 2); // Hàng dưới trải dài 2 cột (100%)

    // 6. Đưa khung biểu đồ vào Tab 2
    mainTabWidget->addTab(analyticsFrame, "📊 Trung Tâm Phân Tích & Thống Kê 360°");

    // 7. Thiết lập QTabWidget làm Central Widget mới cho MainWindow
    this->setCentralWidget(mainTabWidget);
}
void MainWindow::refreshDashboardCharts()
{
    int totalStudents = StudentManager::getInstance().getTotalCount();
    if (totalStudents == 0) return;

    // ------------------------------------------------------------------------
    // 1. BIỂU ĐỒ TRÒN (PIE CHART)
    // ------------------------------------------------------------------------
    QMap<QString, int> majors = StudentManager::getInstance().getMajorDistribution();
    QPieSeries *pieSeries = new QPieSeries();

    for (auto it = majors.constBegin(); it != majors.constEnd(); ++it) {
        if (it.value() > 0) {
            QPieSlice *slice = pieSeries->append(it.key(), it.value());
            double percentage = (it.value() * 100.0) / totalStudents;
            slice->setLabel(QString("%1: %2 SV (%3%)").arg(it.key()).arg(it.value()).arg(percentage, 0, 'f', 1));
            slice->setLabelVisible(true);
        }
    }

    // FIX CẢNH BÁO: clazy-detaching-temporary & range-loop-detach
    const auto slices = pieSeries->slices();
    if (!slices.isEmpty()) {
        QPieSlice *maxSlice = slices.first(); // Gọi .first() trên biến lvalue 'slices'
        for (QPieSlice *s : slices) {
            if (s->value() > maxSlice->value()) maxSlice = s;
        }
        maxSlice->setExploded(true);
        maxSlice->setLabelFont(QFont("Segoe UI", 9, QFont::Bold));
        maxSlice->setPen(QPen(QColor(0, 40, 85), 2));       // FIX CẢNH BÁO: RGB int giá trị #002855
        maxSlice->setBrush(QColor(0, 80, 158));             // FIX CẢNH BÁO: RGB int giá trị #00509e
    }

    QChart *pieChart = new QChart();
    pieChart->addSeries(pieSeries);
    pieChart->setTitle("TỶ LỆ PHÂN BỔ SINH VIÊN THEO CHUYÊN NGÀNH");
    pieChart->setTitleFont(QFont("Segoe UI", 11, QFont::Bold));
    pieChart->setAnimationOptions(QChart::SeriesAnimations);
    pieChart->legend()->setAlignment(Qt::AlignBottom);

    pieChartView->setChart(pieChart);

    // ------------------------------------------------------------------------
    // 2. BIỂU ĐỒ CỘT (BAR CHART) - Tối ưu nhãn góc nghiêng -45 độ
    // ------------------------------------------------------------------------
    int countWeak = 0, countAvg = 0, countGood = 0, countExcellent = 0;
    const auto allStudents = StudentManager::getInstance().getAllStudents();

    for (const Student *s : allStudents) {
        double gpa = s->calculateGPA();
        if (gpa < 5.0) countWeak++;
        else if (gpa < 7.0) countAvg++;
        else if (gpa < 8.5) countGood++;
        else countExcellent++;
    }

    QBarSet *barSet = new QBarSet("Số lượng sinh viên");
    *barSet << countWeak << countAvg << countGood << countExcellent;
    barSet->setColor(QColor(39, 174, 96));

    QBarSeries *barSeries = new QBarSeries();
    barSeries->append(barSet);
    barSeries->setLabelsVisible(true);

    QChart *barChart = new QChart();
    barChart->addSeries(barSeries);
    barChart->setTitle("PHÂN BỔ PHỔ ĐIỂM GPA TOÀN TRƯỜNG");
    barChart->setTitleFont(QFont("Segoe UI", 11, QFont::Bold));
    barChart->setAnimationOptions(QChart::SeriesAnimations);
    barChart->legend()->setVisible(false);

    QStringList categories = {"Yếu (<5.0)", "Trung bình (5.0-6.9)", "Khá (7.0-8.4)", "Xuất sắc (8.5-10)"};
    QBarCategoryAxis *axisX = new QBarCategoryAxis();
    axisX->append(categories);
    // ✅ FIX LỖI MẤT CHỮ: Xoay nhãn nghiêng -45 độ để hiển thị trọn vẹn
    axisX->setLabelsAngle(-45);
    barChart->addAxis(axisX, Qt::AlignBottom);
    barSeries->attachAxis(axisX);

    QValueAxis *axisY = new QValueAxis();
    axisY->setRange(0, qMax(5, totalStudents));
    axisY->setLabelFormat("%d SV");
    barChart->addAxis(axisY, Qt::AlignLeft);
    barSeries->attachAxis(axisY);

    barChartView->setChart(barChart);

    // ------------------------------------------------------------------------
    // 3. BIỂU ĐỒ ĐƯỜNG (LINE CHART) - Tối ưu font nhãn trục X
    // ------------------------------------------------------------------------
    QLineSeries *lineSeries = new QLineSeries();
    lineSeries->setName("Đường xu hướng GPA");
    lineSeries->setPen(QPen(QColor(230, 126, 34), 3));
    lineSeries->setPointsVisible(true);

    int index = 1;
    for (const Student *s : allStudents) {
        lineSeries->append(index++, s->calculateGPA());
    }

    QChart *lineChart = new QChart();
    lineChart->addSeries(lineSeries);
    lineChart->setTitle("BIỂU ĐỒ BIẾN ĐỘNG ĐIỂM TRUNG BÌNH THEO DANH SÁCH SINH VIÊN");
    lineChart->setTitleFont(QFont("Segoe UI", 11, QFont::Bold));
    lineChart->setAnimationOptions(QChart::SeriesAnimations);

    QValueAxis *lineAxisX = new QValueAxis();
    lineAxisX->setRange(1, qMax(2, totalStudents));
    // ✅ FIX LỖI ENCODING: Dùng định dạng mã số sinh viên "SV %d" để tránh lỗi dấu ?
    lineAxisX->setLabelFormat("SV %d");
    lineAxisX->setTickCount(qMin(10, totalStudents)); // Chia vạch khoảng cách đều đẹp
    lineChart->addAxis(lineAxisX, Qt::AlignBottom);
    lineSeries->attachAxis(lineAxisX);

    QValueAxis *lineAxisY = new QValueAxis();
    lineAxisY->setRange(0.0, 10.0);
    lineAxisY->setLabelFormat("%.1f");
    lineChart->addAxis(lineAxisY, Qt::AlignLeft);
    lineSeries->attachAxis(lineAxisY);

    lineChartView->setChart(lineChart);
}
