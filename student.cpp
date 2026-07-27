#include "student.h"
#include <QRegularExpression>
#include <QDebug>

Student::Student(const QString &id, const QString &fullName, const QDate &dob, const QString &className, const QString &phone)
{
    // Cài đặt thông qua Setter để tận dụng cơ chế kiểm tra lỗi (Validation)
    if (!id.isEmpty()) setId(id);
    if (!fullName.isEmpty()) setFullName(fullName);
    if (dob.isValid() && dob != QDate::currentDate()) setDob(dob);
    setClassName(className);
    setPhoneNumber(phone);
}

// ============================================================================
// HÀM HỖ TRỢ CHUẨN HÓA & KIỂM TRA (STATIC HELPERS)
// ============================================================================

QString Student::sanitizeName(const QString &rawName)
{
    QString trimmed = rawName.trimmed();
    if (trimmed.isEmpty()) return "";

    // Xóa khoảng trắng kép ở giữa chuỗi (VD: "Nguyễn    Văn   An" -> "Nguyễn Văn An")
    trimmed.replace(QRegularExpression("\\s+"), " ");

    // Chuyển thành chữ thường toàn bộ, sau đó viết hoa chữ cái đầu mỗi từ
    QStringList words = trimmed.toLower().split(" ");
    for (int i = 0; i < words.size(); ++i) {
        if (!words[i].isEmpty()) {
            QString word = words[i];
            word[0] = word[0].toUpper();
            words[i] = word;
        }
    }
    return words.join(" ");
}

bool Student::validateId(const QString &rawId)
{
    // Kiểm tra mã số sinh viên: Bắt buộc từ 6 đến 10 ký tự số
    QRegularExpression regex("^[0-9]{6,10}$");
    return regex.match(rawId.trimmed()).hasMatch();
}

bool Student::validateDob(const QDate &rawDob)
{
    if (!rawDob.isValid()) return false;

    int age = QDate::currentDate().year() - rawDob.year();
    // Sinh viên đại học hợp lệ trong độ tuổi từ 16 đến 60
    return (age >= 16 && age <= 60);
}

void Student::generateEmail()
{
    if (!id.isEmpty()) {
        email = id + "@student.hcmus.edu.vn";
    }
}

// ============================================================================
// GETTERS
// ============================================================================

QString Student::getId() const { return id; }
QString Student::getFullName() const { return fullName; }
QDate Student::getDob() const { return dob; }
QString Student::getClassName() const { return className; }
QString Student::getEmail() const { return email; }
QString Student::getPhoneNumber() const { return phoneNumber; }

int Student::getAge() const
{
    if (!dob.isValid()) return 0;
    int age = QDate::currentDate().year() - dob.year();
    if (QDate::currentDate().dayOfYear() < dob.dayOfYear()) {
        age--;
    }
    return age;
}

QString Student::getFormattedDob() const
{
    return dob.toString("dd/MM/yyyy");
}

// ============================================================================
// SETTERS (TÍCH HỢP VALIDATION & EXCEPTION HANDLING)
// ============================================================================

void Student::setId(const QString &newId)
{
    QString cleanedId = newId.trimmed();
    if (!validateId(cleanedId)) {
        throw std::invalid_argument("Mã số sinh viên không hợp lệ! Bắt buộc từ 6 đến 10 chữ số.");
    }
    id = cleanedId;
    generateEmail(); // Tự động cập nhật lại email trường khi đổi ID
}

void Student::setFullName(const QString &newFullName)
{
    if (newFullName.trimmed().isEmpty()) {
        throw std::invalid_argument("Họ và tên sinh viên không được để trống!");
    }
    fullName = sanitizeName(newFullName);
}

void Student::setDob(const QDate &newDob)
{
    if (!validateDob(newDob)) {
        throw std::invalid_argument("Ngày sinh không hợp lệ! Tuổi sinh viên nằm trong khoảng 16 - 60.");
    }
    dob = newDob;
}

void Student::setClassName(const QString &newClassName)
{
    className = newClassName.trimmed().toUpper();
}

void Student::setPhoneNumber(const QString &newPhone)
{
    // Chỉ giữ lại chữ số trong số điện thoại
    QString cleanedPhone = newPhone.trimmed();
    cleanedPhone.remove(QRegularExpression("[^0-9]"));
    phoneNumber = cleanedPhone;
}

// ============================================================================
// PHƯƠNG THỨC NGHIỆP VỤ CHUNG
// ============================================================================

QString Student::evaluateAcademicStanding() const
{
    double gpa = calculateGPA();
    if (gpa >= 9.0) return "Xuất sắc";
    if (gpa >= 8.0) return "Giỏi";
    if (gpa >= 7.0) return "Khá";
    if (gpa >= 5.0) return "Trung bình";
    return "Yếu";
}

void Student::printSummary() const
{
    qDebug() << "========================================";
    qDebug() << "ID:" << id << "| Name:" << fullName << "| Major:" << getMajor();
    qDebug() << "Class:" << className << "| DOB:" << getFormattedDob() << "(" << getAge() << "t)";
    qDebug() << "Email:" << email << "| Phone:" << phoneNumber;
    qDebug() << "GPA:" << QString::number(calculateGPA(), 'f', 2) << "| Standing:" << evaluateAcademicStanding();
    qDebug() << "========================================";
}

// ============================================================================
// NẠP CHỒNG TOÁN TỬ (OPERATOR OVERLOADING)
// ============================================================================

bool Student::operator==(const Student &other) const
{
    // Hai sinh viên được xem là một nếu trùng Mã số sinh viên (ID)
    return this->id == other.id;
}

bool Student::operator<(const Student &other) const
{
    // Mặc định sắp xếp sinh viên theo Điểm trung bình (GPA) giảm dần
    // Nếu GPA bằng nhau, ưu tiên sắp xếp theo Mã số sinh viên tăng dần
    double thisGpa = this->calculateGPA();
    double otherGpa = other.calculateGPA();

    if (qAbs(thisGpa - otherGpa) > 0.001) {
        return thisGpa > otherGpa; // GPA cao hơn xếp trước
    }
    return this->id < other.id;
}