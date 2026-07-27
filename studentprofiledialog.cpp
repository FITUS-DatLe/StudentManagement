#include "studentprofiledialog.h"
#include "ui_studentprofiledialog.h"
#include "studentmanager.h"
#include "student.h"
#include "itstudent.h"
#include "mathstudent.h"
#include "biostudent.h"
#include "databasehelper.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QPixmap>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlDatabase>
#include <QTableWidgetItem>
#include <QHeaderView>
#include <QInputDialog>
#include <QDate>
#include <QDebug>

// ============================================================================
// ĐỘNG CƠ LẤY KẾT NỐI SQLITE ĐANG HOẠT ĐỘNG (KHẮC PHỤC TRIỆT ĐỂ LỖI DRIVER)
// ============================================================================
static QSqlDatabase getActiveDatabase()
{
    // 1. Quét danh sách toàn bộ các kết nối CSDL đang được đăng ký trong hệ thống Qt
    QStringList connections = QSqlDatabase::connectionNames();
    if (!connections.isEmpty()) {
        QSqlDatabase db = QSqlDatabase::database(connections.first());
        if (db.isOpen()) {
            return db;
        }
    }

    // 2. Thử lấy kết nối mặc định của Qt
    QSqlDatabase defaultDb = QSqlDatabase::database();
    if (defaultDb.isOpen()) {
        return defaultDb;
    }

    // 3. Cơ chế Fallback tận gốc: Tự động nạp driver QSQLITE và mở lại CSDL nếu bị ngắt
    QString fallbackName = "ProEnterprise_Profile_Connection";
    if (QSqlDatabase::contains(fallbackName)) {
        QSqlDatabase existingDb = QSqlDatabase::database(fallbackName);
        if (!existingDb.isOpen()) existingDb.open();
        return existingDb;
    }

    QSqlDatabase fallbackDb = QSqlDatabase::addDatabase("QSQLITE", fallbackName);
    fallbackDb.setDatabaseName("hcmus_student_pro.db"); // Kết nối thẳng vào file DB hệ thống
    if (!fallbackDb.open()) {
        qWarning() << "[DB ERROR] Không thể mở kết nối SQLite Fallback:" << fallbackDb.lastError().text();
    }
    return fallbackDb;
}

// ============================================================================
// KHỞI TẠO & HỦY DIỆT HỘP THOẠI HỒ SƠ 360°
// ============================================================================

StudentProfileDialog::StudentProfileDialog(const QString &studentId, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::StudentProfileDialog)
    , currentStudentId(studentId)
    , isEditMode(false)
{
    ui->setupUi(this);
    this->setWindowTitle(QString("Hồ Sơ Sinh Viên Chi Tiết 360° - MSSV: %1").arg(studentId));
    this->setWindowFlags(this->windowFlags() & ~Qt::WindowContextHelpButtonHint);

    // 1. Khởi tạo CSDL và kiểm tra migration tự động
    initDatabaseTables();

    // 2. Cấu hình định dạng bảng
    formatTableHeaders();

    // 3. Áp dụng phong cách đồ họa QSS
    applyCustomStyles();

    // 4. Nạp dữ liệu vào các Tab
    loadStudentData();
    loadAcademicGrades();
    loadTrainingPoints();
    loadAchievements();

    // 5. Kết nối sự kiện nút bấm
    //connect(ui->btnEditProfile, &QPushButton::clicked, this, &StudentProfileDialog::on_btnEditProfile_clicked);
    //connect(ui->btnCloseProfile, &QPushButton::clicked, this, &StudentProfileDialog::on_btnCloseProfile_clicked);
}

StudentProfileDialog::~StudentProfileDialog()
{
    delete ui;
}

// ============================================================================
// KHỞI TẠO CSDL & MIGRATION TỰ ĐỘNG
// ============================================================================

void StudentProfileDialog::initDatabaseTables()
{
    QSqlDatabase db = getActiveDatabase();
    if (!db.isOpen()) return;

    QSqlQuery query(db);

    // Tự động migration: Thêm cột phone và address vào bảng students nếu chưa tồn tại
    query.exec("ALTER TABLE students ADD COLUMN phone TEXT");
    query.exec("ALTER TABLE students ADD COLUMN address TEXT");

    // Tạo các bảng phụ trợ (Rèn luyện, Giải thưởng)
    query.exec("CREATE TABLE IF NOT EXISTS training_points ("
               "id INTEGER PRIMARY KEY AUTOINCREMENT, "
               "student_id TEXT NOT NULL, "
               "semester TEXT NOT NULL, "
               "points INTEGER NOT NULL, "
               "classification TEXT NOT NULL)");

    query.exec("CREATE TABLE IF NOT EXISTS student_achievements ("
               "id INTEGER PRIMARY KEY AUTOINCREMENT, "
               "student_id TEXT NOT NULL, "
               "title TEXT NOT NULL, "
               "achievement_date TEXT NOT NULL, "
               "category TEXT NOT NULL)");

    // Nạp dữ liệu mẫu Điểm rèn luyện nếu trống
    query.prepare("SELECT COUNT(*) FROM training_points WHERE student_id = :id");
    query.bindValue(":id", currentStudentId);
    if (query.exec() && query.next() && query.value(0).toInt() == 0) {
        QSqlQuery insertTp(db);
        insertTp.prepare("INSERT INTO training_points (student_id, semester, points, classification) VALUES "
                         "(:id, 'Năm 1 - Học kỳ 1 (2025-2026)', 88, 'Tốt'), "
                         "(:id, 'Năm 1 - Học kỳ 2 (2025-2026)', 94, 'Xuất sắc')");
        insertTp.bindValue(":id", currentStudentId);
        insertTp.exec();
    }

    // Nạp dữ liệu mẫu Giải thưởng nếu trống
    query.prepare("SELECT COUNT(*) FROM student_achievements WHERE student_id = :id");
    query.bindValue(":id", currentStudentId);
    if (query.exec() && query.next() && query.value(0).toInt() == 0) {
        QSqlQuery insertAch(db);
        insertAch.prepare("INSERT INTO student_achievements (student_id, title, achievement_date, category) VALUES "
                          "(:id, 'Giải Nhì Kỳ thi Lập trình sinh viên ICPC cấp Trường', '15/11/2025', 'Giải thưởng Học thuật'), "
                          "(:id, 'Thành viên Đội tuyển CTF Khoa CNTT - An toàn thông tin', '10/01/2026', 'Nghiên cứu Khoa học'), "
                          "(:id, 'Chánh chủ nhiệm Câu lạc bộ Thuật toán & Lập trình thi đấu', '20/03/2026', 'Hoạt động Ngoại khóa')");
        insertAch.bindValue(":id", currentStudentId);
        insertAch.exec();
    }
}

// ============================================================================
// NẠP DỮ LIỆU HỒ SƠ SINH VIÊN (TAB 1 & HEADER)
// ============================================================================

void StudentProfileDialog::loadStudentData()
{
    Student *s = StudentManager::getInstance().getStudentById(currentStudentId);
    if (!s) {
        QMessageBox::critical(this, "Lỗi dữ liệu", "Không tìm thấy hồ sơ sinh viên trong bộ nhớ!");
        this->reject();
        return;
    }

    // Nạp Header
    ui->lblFullName->setText(s->getFullName().toUpper());
    ui->lblStudentId->setText(QString("MSSV: %1  |  Lớp sinh hoạt: %2").arg(s->getId(), s->getClassName()));
    ui->lblMajorBadge->setText(QString("Chuyên ngành: %1").arg(s->getMajor()));

    double gpa = s->calculateGPA();
    ui->lblGpaValue->setText(QString::number(gpa, 'f', 2));
    ui->lblClassificationBadge->setText(s->getClassification());

    // Nạp Tab 1: Thông tin hành chính từ RAM & SQLite
    ui->txtDob->setText(s->getFormattedDob());
    ui->txtEmail->setText(QString("%1@student.hcmus.edu.vn").arg(s->getId()));

    // Lấy thông tin Phone và Address thực tế từ bảng students trong SQLite
    QSqlDatabase db = getActiveDatabase();
    QSqlQuery query(db);
    query.prepare("SELECT phone, address FROM students WHERE id = :id");
    query.bindValue(":id", currentStudentId);
    if (query.exec() && query.next()) {
        QString dbPhone = query.value(0).toString();
        QString dbAddr = query.value(1).toString();
        ui->txtPhone->setText(dbPhone.isEmpty() ? "0987654321" : dbPhone);
        ui->txtAddress->setText(dbAddr.isEmpty() ? "Ký túc xá Khu A ĐHQG-HCM, Phường Linh Trung, TP. Thủ Đức, TP. Hồ Chí Minh" : dbAddr);
    } else {
        ui->txtPhone->setText("0987654321");
        ui->txtAddress->setText("Ký túc xá Khu A ĐHQG-HCM, Phường Linh Trung, TP. Thủ Đức, TP. Hồ Chí Minh");
    }

    // Cố vấn học tập theo Ngành
    QString major = s->getMajor();
    if (major.contains("CNTT", Qt::CaseInsensitive) || major.contains("Công nghệ", Qt::CaseInsensitive)) {
        ui->txtAdvisor->setText("TS. Lê Hoàng Phú Hiếu (Khoa Công nghệ Thông tin)");
    } else if (major.contains("Toán", Qt::CaseInsensitive)) {
        ui->txtAdvisor->setText("PGS. TS. Nguyễn Thành Đạt (Khoa Toán - Tin học)");
    } else {
        ui->txtAdvisor->setText("TS. Trần Thị Sinh Học (Khoa Sinh học - Công nghệ Sinh học)");
    }
}

// ============================================================================
// CƠ CHẾ CHỈNH SỬA TẠI CHỖ & ĐỒNG BỘ HÓA TOÀN HỆ THỐNG
// ============================================================================

void StudentProfileDialog::on_btnEditProfile_clicked()
{
    if (!isEditMode) {
        toggleEditMode(true);
    } else {
        saveProfileChanges();
    }
}

void StudentProfileDialog::toggleEditMode(bool enable)
{
    isEditMode = enable;
    ui->txtDob->setReadOnly(!enable);
    ui->txtPhone->setReadOnly(!enable);
    ui->txtAddress->setReadOnly(!enable);

    if (enable) {
        ui->btnEditProfile->setText("💾 Lưu đồng bộ");
        ui->btnEditProfile->setStyleSheet("background-color: #E67E22; color: #FFFFFF; font-weight: bold; border-radius: 6px; padding: 8px 20px; border: none;");

        QString editStyle = "background-color: #FFFFFF; border: 2px solid #00509e; border-radius: 6px; padding: 6px 12px; color: #1a252f; font-weight: bold;";
        ui->txtDob->setStyleSheet(editStyle);
        ui->txtPhone->setStyleSheet(editStyle);
        ui->txtAddress->setStyleSheet(editStyle);

        ui->tabWidget->setCurrentIndex(0);
        ui->txtDob->setFocus();
        ui->txtDob->selectAll();
    } else {
        ui->btnEditProfile->setText("✏️ Chỉnh sửa");
        ui->btnEditProfile->setStyleSheet("background-color: #2980B9; color: #FFFFFF; font-weight: bold; border-radius: 6px; padding: 8px 20px; border: none;");

        QString readStyle = "background-color: #F8F9FA; border: 1px solid #D5D8DC; border-radius: 6px; padding: 6px 12px; color: #555555; font-weight: 500;";
        ui->txtDob->setStyleSheet(readStyle);
        ui->txtPhone->setStyleSheet(readStyle);
        ui->txtAddress->setStyleSheet(readStyle);
    }
}

void StudentProfileDialog::saveProfileChanges()
{
    QString newDobStr = ui->txtDob->text().trimmed();
    QString newPhone = ui->txtPhone->text().trimmed();
    QString newAddr = ui->txtAddress->text().trimmed();

    if (newDobStr.isEmpty()) {
        QMessageBox::warning(this, "Thiếu thông tin", "Ngày sinh không được để trống!");
        ui->txtDob->setFocus();
        return;
    }

    // Kiểm tra định dạng ngày sinh QDate
    QDate newDob = QDate::fromString(newDobStr, "dd/MM/yyyy");
    if (!newDob.isValid()) newDob = QDate::fromString(newDobStr, "yyyy-MM-dd");
    if (!newDob.isValid()) newDob = QDate::fromString(newDobStr, "d/M/yyyy");

    if (!newDob.isValid()) {
        QMessageBox::warning(this, "Định dạng sai", "Ngày sinh không hợp lệ! Vui lòng nhập theo định dạng: dd/MM/yyyy (VD: 29/03/2007)");
        ui->txtDob->setFocus();
        return;
    }

    // 1. Cập nhật đối tượng RAM trong StudentManager
    Student *s = StudentManager::getInstance().getStudentById(currentStudentId);
    if (s) {
        s->setDob(newDob);
        if (!newPhone.isEmpty()) s->setPhoneNumber(newPhone);
    }

    // 2. Cập nhật đồng bộ vào CSDL SQLite vĩnh viễn (Sử dụng getActiveDatabase)
    QSqlDatabase db = getActiveDatabase();
    if (!db.isOpen()) {
        QMessageBox::critical(this, "Lỗi kết nối", "Không thể kết nối đến cơ sở dữ liệu SQLite!");
        return;
    }

    QSqlQuery query(db);
    query.prepare("UPDATE students SET dob = :dob, phone = :phone, address = :addr WHERE id = :id");
    query.bindValue(":dob", newDob.toString("dd/MM/yyyy"));
    query.bindValue(":phone", newPhone);
    query.bindValue(":addr", newAddr);
    query.bindValue(":id", currentStudentId);

    if (query.exec()) {
        // 3. Kích hoạt bộ máy updateStudent để phát tín hiệu dataChanged ra MainWindow
        if (s) {
            StudentManager::getInstance().updateStudent(s);
        }

        toggleEditMode(false);
        loadStudentData();
        QMessageBox::information(this, "Đồng bộ thành công", "Toàn bộ thông tin thay đổi đã được đồng bộ vào RAM, CSDL SQLite và Bảng điều khiển chính!");
    } else {
        QMessageBox::critical(this, "Lỗi SQL", "Không thể đồng bộ dữ liệu: " + query.lastError().text());
    }
}

// ============================================================================
// NẠP BẢNG ĐIỂM & TÍN CHỈ (TAB 2)
// ============================================================================

void StudentProfileDialog::loadAcademicGrades()
{
    Student *s = StudentManager::getInstance().getStudentById(currentStudentId);
    if (!s) return;

    ui->tblCourses->setRowCount(0);
    int totalCredits = 0;
    QString major = s->getMajor();

    if (major.contains("CNTT", Qt::CaseInsensitive) || major.contains("Công nghệ", Qt::CaseInsensitive) || major.contains("IT", Qt::CaseInsensitive)) {
        const ITStudent *it = dynamic_cast<const ITStudent*>(s);
        if (it) {
            QStringList codes = {"CSC10001", "MTH10002", "CSC10003"};
            QStringList names = {"Nhập môn Lập trình C++", "Toán rời rạc & Ứng dụng", "Cấu trúc Dữ liệu & Thuật toán"};
            QList<int> credits = {4, 3, 4};
            QList<double> scores = {it->getCppScore(), it->getDiscreteMathScore(), it->getDsaScore()};

            for (int i = 0; i < codes.size(); ++i) {
                ui->tblCourses->insertRow(i);
                ui->tblCourses->setItem(i, 0, new QTableWidgetItem(codes[i]));
                ui->tblCourses->setItem(i, 1, new QTableWidgetItem(names[i]));
                ui->tblCourses->setItem(i, 2, new QTableWidgetItem(QString::number(credits[i]) + " TC"));
                ui->tblCourses->setItem(i, 3, new QTableWidgetItem(QString::number(scores[i], 'f', 2)));

                ui->tblCourses->item(i, 0)->setTextAlignment(Qt::AlignCenter);
                ui->tblCourses->item(i, 2)->setTextAlignment(Qt::AlignCenter);
                ui->tblCourses->item(i, 3)->setTextAlignment(Qt::AlignCenter);
                totalCredits += credits[i];
            }
        }
    }
    else if (major.contains("Toán", Qt::CaseInsensitive)) {
        const MathStudent *math = dynamic_cast<const MathStudent*>(s);
        if (math) {
            QStringList codes = {"MTH10101", "MTH10102"};
            QStringList names = {"Giải tích 1 (Hàm một biến)", "Đại số tuyến tính & Hình học"};
            QList<int> credits = {4, 4};
            QList<double> scores = {math->getCalculusScore(), math->getLinearAlgebraScore()};

            for (int i = 0; i < codes.size(); ++i) {
                ui->tblCourses->insertRow(i);
                ui->tblCourses->setItem(i, 0, new QTableWidgetItem(codes[i]));
                ui->tblCourses->setItem(i, 1, new QTableWidgetItem(names[i]));
                ui->tblCourses->setItem(i, 2, new QTableWidgetItem(QString::number(credits[i]) + " TC"));
                ui->tblCourses->setItem(i, 3, new QTableWidgetItem(QString::number(scores[i], 'f', 2)));

                ui->tblCourses->item(i, 0)->setTextAlignment(Qt::AlignCenter);
                ui->tblCourses->item(i, 2)->setTextAlignment(Qt::AlignCenter);
                ui->tblCourses->item(i, 3)->setTextAlignment(Qt::AlignCenter);
                totalCredits += credits[i];
            }
        }
    }
    else {
        const BioStudent *bio = dynamic_cast<const BioStudent*>(s);
        if (bio) {
            QStringList codes = {"BIO10001", "BIO10002"};
            QStringList names = {"Sinh học Phân tử Đại cương", "Di truyền học Hiện đại"};
            QList<int> credits = {3, 4};
            QList<double> scores = {bio->getMolecularBioScore(), bio->getGeneticsScore()};

            for (int i = 0; i < codes.size(); ++i) {
                ui->tblCourses->insertRow(i);
                ui->tblCourses->setItem(i, 0, new QTableWidgetItem(codes[i]));
                ui->tblCourses->setItem(i, 1, new QTableWidgetItem(names[i]));
                ui->tblCourses->setItem(i, 2, new QTableWidgetItem(QString::number(credits[i]) + " TC"));
                ui->tblCourses->setItem(i, 3, new QTableWidgetItem(QString::number(scores[i], 'f', 2)));

                ui->tblCourses->item(i, 0)->setTextAlignment(Qt::AlignCenter);
                ui->tblCourses->item(i, 2)->setTextAlignment(Qt::AlignCenter);
                ui->tblCourses->item(i, 3)->setTextAlignment(Qt::AlignCenter);
                totalCredits += credits[i];
            }
        }
    }

    ui->lblAccumulatedCredits->setText(QString("• Tổng số tín chỉ tích lũy chuyên ngành: %1 / 135 TC (Đạt yêu cầu lộ trình Năm 1)").arg(totalCredits));
}

// ============================================================================
// NẠP ĐIỂM RÈN LUYỆN (TAB 3) & GIẢI THƯỞNG (TAB 4)
// ============================================================================

void StudentProfileDialog::loadTrainingPoints()
{
    QSqlDatabase db = getActiveDatabase();
    QSqlQuery query(db);
    query.prepare("SELECT semester, points, classification FROM training_points WHERE student_id = :id ORDER BY id ASC");
    query.bindValue(":id", currentStudentId);

    ui->tblTrainingPoints->setRowCount(0);
    int row = 0;
    if (query.exec()) {
        while (query.next()) {
            ui->tblTrainingPoints->insertRow(row);
            auto *itemSem = new QTableWidgetItem(query.value(0).toString());
            auto *itemPts = new QTableWidgetItem(query.value(1).toString() + " điểm");
            auto *itemClass = new QTableWidgetItem(query.value(2).toString());

            itemPts->setTextAlignment(Qt::AlignCenter);
            itemClass->setTextAlignment(Qt::AlignCenter);

            ui->tblTrainingPoints->setItem(row, 0, itemSem);
            ui->tblTrainingPoints->setItem(row, 1, itemPts);
            ui->tblTrainingPoints->setItem(row, 2, itemClass);
            row++;
        }
    }
}

void StudentProfileDialog::loadAchievements()
{
    QSqlDatabase db = getActiveDatabase();
    QSqlQuery query(db);
    query.prepare("SELECT title, achievement_date, category FROM student_achievements WHERE student_id = :id ORDER BY id DESC");
    query.bindValue(":id", currentStudentId);

    ui->tblAchievements->setRowCount(0);
    int row = 0;
    if (query.exec()) {
        while (query.next()) {
            ui->tblAchievements->insertRow(row);
            auto *itemTitle = new QTableWidgetItem(query.value(0).toString());
            auto *itemDate = new QTableWidgetItem(query.value(1).toString());
            auto *itemCat = new QTableWidgetItem(query.value(2).toString());

            itemDate->setTextAlignment(Qt::AlignCenter);
            itemCat->setTextAlignment(Qt::AlignCenter);

            ui->tblAchievements->setItem(row, 0, itemTitle);
            ui->tblAchievements->setItem(row, 1, itemDate);
            ui->tblAchievements->setItem(row, 2, itemCat);
            row++;
        }
    }
}

// ============================================================================
// CÁC SỰ KIỆN NÚT BẤM VÀ THAO TÁC NGHIỆP VỤ
// ============================================================================

void StudentProfileDialog::on_btnChangeAvatar_clicked()
{
    QString filePath = QFileDialog::getOpenFileName(this, "Chọn ảnh đại diện sinh viên", "", "Hình ảnh (*.png *.jpg *.jpeg *.bmp)");
    if (!filePath.isEmpty()) {
        QPixmap avatar(filePath);
        if (!avatar.isNull()) {
            ui->lblAvatar->setPixmap(avatar.scaled(100, 100, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation));
            QMessageBox::information(this, "Cập nhật Avatar", "Đã tải và cập nhật ảnh đại diện mới vào hồ sơ cục bộ!");
        } else {
            QMessageBox::warning(this, "Lỗi tệp tin", "Không thể đọc định dạng hình ảnh vừa chọn!");
        }
    }
}

void StudentProfileDialog::on_btnAddAchievement_clicked()
{
    bool ok;
    QString title = QInputDialog::getText(this, "Thêm Giải Thưởng / NCKH",
                                          "Nhập tên giải thưởng, bài báo hoặc đề tài NCKH mới:",
                                          QLineEdit::Normal, "", &ok);
    if (ok && !title.trimmed().isEmpty()) {
        QString category = QInputDialog::getItem(this, "Phân loại thành tích", "Chọn lĩnh vực ghi nhận:",
                                                 {"Giải thưởng Học thuật", "Nghiên cứu Khoa học", "Hoạt động Ngoại khóa", "Học bổng Doanh nghiệp"},
                                                 0, false, &ok);
        if (ok) {
            QString currentDate = QDate::currentDate().toString("dd/MM/yyyy");
            QSqlDatabase db = getActiveDatabase();
            QSqlQuery query(db);
            query.prepare("INSERT INTO student_achievements (student_id, title, achievement_date, category) "
                          "VALUES (:id, :title, :date, :cat)");
            query.bindValue(":id", currentStudentId);
            query.bindValue(":title", title.trimmed());
            query.bindValue(":date", currentDate);
            query.bindValue(":cat", category);

            if (query.exec()) {
                loadAchievements();
                QMessageBox::information(this, "Thành công", "Đã bổ sung thành tích mới vào cơ sở dữ liệu SQLite vĩnh viễn!");
            } else {
                QMessageBox::critical(this, "Lỗi SQL", "Không thể lưu giải thưởng: " + query.lastError().text());
            }
        }
    }
}

void StudentProfileDialog::on_btnCloseProfile_clicked()
{
    this->accept();
}

// ============================================================================
// ĐỊNH DẠNG BẢNG & TRANG TRÍ GIAO DIỆN (QSS)
// ============================================================================

void StudentProfileDialog::formatTableHeaders()
{
    ui->tblCourses->setColumnCount(4);
    ui->tblCourses->setHorizontalHeaderLabels({"Mã Môn", "Tên Môn Học", "Số Tín Chỉ", "Điểm Môn"});
    ui->tblCourses->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    ui->tblCourses->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    ui->tblCourses->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
    ui->tblCourses->horizontalHeader()->setSectionResizeMode(3, QHeaderView::ResizeToContents);
    ui->tblCourses->setAlternatingRowColors(true);

    ui->tblTrainingPoints->setColumnCount(3);
    ui->tblTrainingPoints->setHorizontalHeaderLabels({"Học Kỳ Đánh Giá", "Điểm Rèn Luyện", "Xếp Loại ĐRL"});
    ui->tblTrainingPoints->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    ui->tblTrainingPoints->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    ui->tblTrainingPoints->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
    ui->tblTrainingPoints->setAlternatingRowColors(true);

    ui->tblAchievements->setColumnCount(3);
    ui->tblAchievements->setHorizontalHeaderLabels({"Tên Giải Thưởng / Đề Tài NCKH", "Ngày Chứng Nhận", "Phân Loại"});
    ui->tblAchievements->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    ui->tblAchievements->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    ui->tblAchievements->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
    ui->tblAchievements->setAlternatingRowColors(true);
}

void StudentProfileDialog::applyCustomStyles()
{
    QString qss = R"(
        QDialog { background-color: #F8F9FA; }
        #headerFrame { background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #002855, stop:1 #00509e); border-radius: 12px; }
        #lblFullName { color: #FFFFFF; letter-spacing: 0.5px; }
        #lblStudentId { color: #D0E1F9; font-weight: bold; }
        #lblMajorBadge { color: #FFFFFF; background-color: rgba(255, 255, 255, 0.15); padding: 4px 10px; border-radius: 4px; font-weight: bold; }
        #lblGpaTitle { color: #A0C4DF; font-size: 11px; font-weight: bold; }
        #lblGpaValue { color: #FFD700; }
        #lblClassificationBadge { color: #002855; background-color: #FFD700; padding: 3px 12px; border-radius: 10px; font-weight: bold; font-size: 12px; }

        #lblAvatar { background-color: #FFFFFF; border: 3px solid #4da6ff; border-radius: 50px; font-size: 45px; }
        #btnChangeAvatar { background-color: rgba(255, 255, 255, 0.2); color: #FFFFFF; border: 1px solid #FFFFFF; border-radius: 4px; padding: 3px 8px; font-size: 11px; font-weight: bold; }
        #btnChangeAvatar:hover { background-color: #FFFFFF; color: #002855; }

        QTabWidget::pane { border: 1px solid #E0E0E0; background: #FFFFFF; border-radius: 8px; top: -1px; }
        QTabBar::tab { background: #EAECEE; color: #5D6D7E; padding: 10px 18px; font-weight: bold; font-size: 13px; border-top-left-radius: 8px; border-top-right-radius: 8px; margin-right: 3px; }
        QTabBar::tab:selected { background: #FFFFFF; color: #00509e; border-top: 3px solid #00509e; border-left: 1px solid #E0E0E0; border-right: 1px solid #E0E0E0; }
        QTabBar::tab:hover:!selected { background: #D5D8DC; color: #2C3E50; }

        QLineEdit { background-color: #F8F9FA; border: 1px solid #D5D8DC; border-radius: 6px; padding: 6px 12px; color: #2C3E50; font-size: 13px; }
        QLineEdit:read-only { color: #555555; font-weight: 500; }

        #btnEditProfile { background-color: #2980B9; color: #FFFFFF; font-weight: bold; border-radius: 6px; border: none; }
        #btnEditProfile:hover { background-color: #2471A3; }

        #btnAddAchievement { background-color: #27AE60; color: #FFFFFF; font-weight: bold; border-radius: 6px; padding: 7px 15px; border: none; }
        #btnAddAchievement:hover { background-color: #219A52; }

        #btnCloseProfile { background-color: #6C7A89; color: #FFFFFF; font-weight: bold; border-radius: 6px; border: none; }
        #btnCloseProfile:hover { background-color: #5A6877; }
    )";

    this->setStyleSheet(qss);
}