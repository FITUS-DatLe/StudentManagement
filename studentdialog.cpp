#include "studentdialog.h"
#include "ui_studentdialog.h"
#include "studentfactory.h"
#include "itstudent.h"
#include "mathstudent.h"
#include "biostudent.h"
#include <QMessageBox>
#include <QRegularExpression>
#include <QDebug>

// ============================================================================
// KHỞI TẠO & HỦY DIỆT
// ============================================================================

StudentDialog::StudentDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::StudentDialog),
    createdStudent(nullptr),
    isEditMode(false)
{
    ui->setupUi(this);

    // Cấu hình giao diện mặc định
    ui->stackedScores->setCurrentIndex(0); // Hiển thị trang nhập điểm CNTT
    ui->txtDob->setText(QDate::currentDate().addYears(-18).toString("dd/MM/yyyy")); // Mặc định DD/MM/YYYY
}

StudentDialog::~StudentDialog()
{
    if (this->result() != QDialog::Accepted && createdStudent != nullptr) {
        delete createdStudent;
        createdStudent = nullptr;
    }
    delete ui;
}

// ============================================================================
// HÀM HỖ TRỢ KIỂM TRA DỮ LIỆU ĐẦU VÀO
// ============================================================================

bool StudentDialog::parseScore(const QString &scoreStr, double &outScore, const QString &fieldName)
{
    QString trimmed = scoreStr.trimmed();
    if (trimmed.isEmpty()) {
        QMessageBox::warning(this, "Lỗi nhập liệu", QString("Vui lòng không để trống điểm môn %1!").arg(fieldName));
        return false;
    }

    bool ok = false;
    outScore = trimmed.toDouble(&ok);

    if (!ok || outScore < 0.0 || outScore > 10.0) {
        QMessageBox::warning(this, "Điểm không hợp lệ",
                             QString("Điểm môn [%1] phải là con số hợp lệ nằm trong khoảng từ 0.0 đến 10.0!").arg(fieldName));
        return false;
    }
    return true;
}

bool StudentDialog::validateInputs()
{
    // 1. Kiểm tra Mã số sinh viên
    QString id = ui->txtId->text().trimmed();
    if (id.isEmpty()) {
        QMessageBox::warning(this, "Thiếu thông tin", "Vui lòng nhập Mã số sinh viên!");
        ui->txtId->setFocus();
        return false;
    }

    QRegularExpression idRegex("^[0-9]{6,10}$");
    if (!idRegex.match(id).hasMatch()) {
        QMessageBox::warning(this, "Sai định dạng", "Mã số sinh viên chỉ được chứa từ 6 đến 10 chữ số!");
        ui->txtId->selectAll();
        ui->txtId->setFocus();
        return false;
    }

    // 2. Kiểm tra Họ và tên
    QString name = ui->txtName->text().trimmed();
    if (name.isEmpty()) {
        QMessageBox::warning(this, "Thiếu thông tin", "Vui lòng nhập Họ và tên sinh viên!");
        ui->txtName->setFocus();
        return false;
    }

    // 3. Kiểm tra Ngày sinh (Định dạng DD/MM/YYYY)
    QString dobStr = ui->txtDob->text().trimmed();
    QDate dob = QDate::fromString(dobStr, "dd/MM/yyyy");
    if (!dob.isValid()) {
        dob = QDate::fromString(dobStr, "yyyy-MM-dd");
    }

    if (!dob.isValid()) {
        QMessageBox::warning(this, "Sai định dạng", "Ngày sinh không hợp lệ! Định dạng chuẩn: DD/MM/YYYY (VD: 15/05/2005)");
        ui->txtDob->setFocus();
        return false;
    }

    int age = QDate::currentDate().year() - dob.year();
    if (age < 16 || age > 60) {
        QMessageBox::warning(this, "Tuổi không hợp lệ", "Sinh viên nằm trong độ tuổi từ 16 đến 60!");
        ui->txtDob->setFocus();
        return false;
    }

    // 4. Kiểm tra Tên lớp
    QString className = ui->txtClass->text().trimmed();
    if (className.isEmpty()) {
        QMessageBox::warning(this, "Thiếu thông tin", "Vui lòng nhập Lớp sinh hoạt!");
        ui->txtClass->setFocus();
        return false;
    }

    return true;
}

// ============================================================================
// CHẾ ĐỘ CHỈNH SỬA
// ============================================================================

void StudentDialog::setStudentData(const Student *student)
{
    if (!student) return;

    isEditMode = true;
    this->setWindowTitle("Chỉnh Sửa Thông Tin Sinh Viên");

    // Đổ dữ liệu hành chính vào form (Chuyển QLineEdit)
    ui->txtId->setText(student->getId());
    ui->txtId->setEnabled(false); // Khóa MSSV khi sửa
    ui->txtName->setText(student->getFullName());
    ui->txtDob->setText(student->getFormattedDob());
    ui->txtClass->setText(student->getClassName());

    // Xác định chuyên ngành
    QString major = student->getMajor();
    if (major.contains("Công nghệ Thông tin", Qt::CaseInsensitive) || major.contains("IT", Qt::CaseInsensitive)) {
        ui->cboMajor->setCurrentIndex(0);
        ui->cboMajor->setEnabled(false);
        const ITStudent *it = dynamic_cast<const ITStudent*>(student);
        if (it) {
            ui->txtScoreIT1->setText(QString::number(it->getCppScore(), 'f', 2));
            ui->txtScoreIT2->setText(QString::number(it->getDiscreteMathScore(), 'f', 2));
            ui->txtScoreIT3->setText(QString::number(it->getDsaScore(), 'f', 2));
        }
    }
    else if (major.contains("Toán", Qt::CaseInsensitive) || major.contains("Math", Qt::CaseInsensitive)) {
        ui->cboMajor->setCurrentIndex(1);
        ui->cboMajor->setEnabled(false);
        const MathStudent *math = dynamic_cast<const MathStudent*>(student);
        if (math) {
            ui->txtScoreMath1->setText(QString::number(math->getCalculusScore(), 'f', 2));
            ui->txtScoreMath2->setText(QString::number(math->getLinearAlgebraScore(), 'f', 2));
        }
    }
    else if (major.contains("Sinh", Qt::CaseInsensitive) || major.contains("Bio", Qt::CaseInsensitive)) {
        ui->cboMajor->setCurrentIndex(2);
        ui->cboMajor->setEnabled(false);
        const BioStudent *bio = dynamic_cast<const BioStudent*>(student);
        if (bio) {
            ui->txtScoreBio1->setText(QString::number(bio->getMolecularBioScore(), 'f', 2));
            ui->txtScoreBio2->setText(QString::number(bio->getGeneticsScore(), 'f', 2));
        }
    }
}

Student* StudentDialog::getStudent() const
{
    return createdStudent;
}

// ============================================================================
// XỬ LÝ SỰ KIỆN GIAO DIỆN
// ============================================================================

void StudentDialog::on_cboMajor_currentIndexChanged(int index)
{
    ui->stackedScores->setCurrentIndex(index);
}

void StudentDialog::on_btnSave_clicked()
{
    if (!validateInputs()) {
        return;
    }

    QString id = ui->txtId->text().trimmed();
    QString name = ui->txtName->text().trimmed();

    // Parse QDate từ QLineEdit txtDob
    QString dobStr = ui->txtDob->text().trimmed();
    QDate dob = QDate::fromString(dobStr, "dd/MM/yyyy");
    if (!dob.isValid()) {
        dob = QDate::fromString(dobStr, "yyyy-MM-dd");
    }

    QString className = ui->txtClass->text().trimmed().toUpper();

    int majorIndex = ui->cboMajor->currentIndex();
    double s1 = 0.0, s2 = 0.0, s3 = 0.0;
    QString majorType;

    if (majorIndex == 0) { // CNTT
        majorType = "IT";
        if (!parseScore(ui->txtScoreIT1->text(), s1, "Lập trình C++") ||
            !parseScore(ui->txtScoreIT2->text(), s2, "Toán rời rạc") ||
            !parseScore(ui->txtScoreIT3->text(), s3, "Cấu trúc dữ liệu & Thuật toán")) {
            return;
        }
    }
    else if (majorIndex == 1) { // Toán
        majorType = "MATH";
        if (!parseScore(ui->txtScoreMath1->text(), s1, "Giải tích") ||
            !parseScore(ui->txtScoreMath2->text(), s2, "Đại số tuyến tính")) {
            return;
        }
    }
    else { // Sinh
        majorType = "BIO";
        if (!parseScore(ui->txtScoreBio1->text(), s1, "Sinh học phân tử") ||
            !parseScore(ui->txtScoreBio2->text(), s2, "Di truyền học")) {
            return;
        }
    }

    try {
        if (createdStudent != nullptr) {
            delete createdStudent;
            createdStudent = nullptr;
        }

        // Tạo đối tượng qua Factory (sử dụng chuỗi phone rỗng nếu UI không có)
        createdStudent = StudentFactory::createStudent(majorType, id, name, dob, className, "", s1, s2, s3);

        if (createdStudent) {
            this->accept();
        } else {
            QMessageBox::critical(this, "Lỗi hệ thống", "Không thể khởi tạo đối tượng sinh viên từ Factory!");
        }
    }
    catch (const std::exception &e) {
        QMessageBox::critical(this, "Lỗi Nghiệp Vụ", QString("Hệ thống từ chối dữ liệu:\n") + e.what());
    }
}

void StudentDialog::on_btnCancel_clicked()
{
    this->reject();
}