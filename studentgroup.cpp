#include "studentgroup.h"
#include <algorithm>
#include <QDebug>
#include <QTextStream>

// ============================================================================
// KHỞI TẠO & QUẢN LÝ VÒNG ĐỜI (CONSTRUCTOR & DESTRUCTOR)
// ============================================================================

StudentGroup::StudentGroup(const QString &name, const QString &code)
    : groupName(name.trimmed()), groupCode(code.trimmed().toUpper())
{
}

StudentGroup::~StudentGroup()
{
    clearAll(); // Đảm bảo không xảy ra rò rỉ bộ nhớ (Memory Leak) khi hủy lớp
}

// ============================================================================
// GETTERS & SETTERS
// ============================================================================

QString StudentGroup::getGroupName() const { return groupName; }
QString StudentGroup::getGroupCode() const { return groupCode; }

void StudentGroup::setGroupName(const QString &name)
{
    if (name.trimmed().isEmpty()) {
        throw std::invalid_argument("Tên nhóm/lớp không được để trống!");
    }
    groupName = name.trimmed();
}

void StudentGroup::setGroupCode(const QString &code)
{
    groupCode = code.trimmed().toUpper();
}

// ============================================================================
// CÁC THAO TÁC QUẢN LÝ DANH SÁCH (CRUD OPERATIONS)
// ============================================================================

void StudentGroup::addStudent(Student *student)
{
    if (!student) {
        throw std::invalid_argument("Lỗi: Không thể thêm đối tượng sinh viên rỗng (Null Pointer)!");
    }

    // Kiểm tra trùng lặp Mã số sinh viên
    if (findStudentById(student->getId()) != nullptr) {
        throw std::runtime_error("Mã số sinh viên [" + student->getId().toStdString() + "] đã tồn tại trong lớp!");
    }

    students.append(student);
}

bool StudentGroup::removeStudentById(const QString &studentId)
{
    QString targetId = studentId.trimmed();
    for (int i = 0; i < students.size(); ++i) {
        if (students[i]->getId() == targetId) {
            delete students[i]; // Thu hồi vùng nhớ trên RAM
            students.removeAt(i);
            return true; // Xóa thành công
        }
    }
    return false; // Không tìm thấy sinh viên
}

Student* StudentGroup::findStudentById(const QString &studentId) const
{
    QString targetId = studentId.trimmed();
    for (Student *s : students) {
        if (s->getId() == targetId) {
            return s;
        }
    }
    return nullptr;
}

void StudentGroup::clearAll()
{
    qDeleteAll(students); // Hàm tiện ích của Qt xóa toàn bộ con trỏ trong container
    students.clear();
}

// ============================================================================
// TÌM KIẾM & LỌC DỮ LIỆU NÂNG CAO
// ============================================================================

QVector<Student*> StudentGroup::getAllStudents() const
{
    return students;
}

QVector<Student*> StudentGroup::getStudentsByMajor(const QString &major) const
{
    QVector<Student*> result;
    QString targetMajor = major.trimmed().toLower();

    for (Student *s : students) {
        if (s->getMajor().toLower().contains(targetMajor)) {
            result.append(s);
        }
    }
    return result;
}

QVector<Student*> StudentGroup::getScholarshipCandidates() const
{
    QVector<Student*> result;
    for (Student *s : students) {
        if (s->isScholarshipEligible()) {
            result.append(s);
        }
    }
    return result;
}

QVector<Student*> StudentGroup::searchStudentsByName(const QString &keyword) const
{
    QVector<Student*> result;
    QString targetKeyword = keyword.trimmed().toLower();

    for (Student *s : students) {
        if (s->getFullName().toLower().contains(targetKeyword)) {
            result.append(s);
        }
    }
    return result;
}

QVector<Student*> StudentGroup::getTopPerformers(int count) const
{
    QVector<Student*> sortedList = students;

    // Sắp xếp giảm dần theo GPA
    std::sort(sortedList.begin(), sortedList.end(), [](Student *a, Student *b) {
        return a->calculateGPA() > b->calculateGPA();
    });

    if (count >= sortedList.size()) {
        return sortedList;
    }
    return sortedList.mid(0, count);
}

// ============================================================================
// SẮP XẾP DANH SÁCH
// ============================================================================

void StudentGroup::sortByGpaDescending()
{
    // Tận dụng toán tử < đã nạp chồng trong lớp Student
    std::sort(students.begin(), students.end(), [](Student *a, Student *b) {
        return (*b) < (*a); // Sắp xếp giảm dần
    });
}

void StudentGroup::sortByIdAscending()
{
    std::sort(students.begin(), students.end(), [](Student *a, Student *b) {
        return a->getId() < b->getId();
    });
}

void StudentGroup::sortByNameAscending()
{
    std::sort(students.begin(), students.end(), [](Student *a, Student *b) {
        // Tách tên riêng để sắp xếp chuẩn Việt Nam (nếu cần), ở đây sắp xếp theo chuỗi Họ Tên
        return a->getFullName() < b->getFullName();
    });
}

// ============================================================================
// THỐNG KÊ HỌC THUẬT (STATISTICS)
// ============================================================================

int StudentGroup::getTotalStudentCount() const
{
    return students.size();
}

double StudentGroup::calculateAverageGpa() const
{
    if (students.isEmpty()) return 0.0;

    double totalGpa = 0.0;
    for (const Student *s : students) {
        totalGpa += s->calculateGPA(); // Gọi đa hình
    }
    return totalGpa / static_cast<double>(students.size());
}

int StudentGroup::getScholarshipCount() const
{
    int count = 0;
    for (const Student *s : students) {
        if (s->isScholarshipEligible()) {
            count++;
        }
    }
    return count;
}

QMap<QString, int> StudentGroup::countStudentsByMajor() const
{
    QMap<QString, int> distribution;
    for (const Student *s : students) {
        QString major = s->getMajor();
        distribution[major]++;
    }
    return distribution;
}

QMap<QString, int> StudentGroup::getAcademicStandingDistribution() const
{
    QMap<QString, int> distribution;
    for (const Student *s : students) {
        // Lấy từ khóa xếp loại chính (Xuất sắc, Giỏi, Khá...)
        QString standing = s->evaluateAcademicStanding();
        distribution[standing]++;
    }
    return distribution;
}

// ============================================================================
// GIAO DIỆN CHUYỂN ĐỔI DỮ LIỆU (SERIALIZATION)
// ============================================================================

QJsonArray StudentGroup::toJsonArray() const
{
    QJsonArray array;
    for (const Student *s : students) {
        array.append(s->toJsonObject()); // Tự động gọi toJsonObject() đa hình của từng ngành
    }
    return array;
}

QString StudentGroup::toCsvString() const
{
    QString csvData;
    QTextStream stream(&csvData);

    // Ghi dòng Tiêu đề (Header)
    stream << "ID,FullName,DOB,Class,Phone,Major,Score1,Score2,Score3,GPA,Classification\n";

    // Ghi từng dòng dữ liệu sinh viên
    for (const Student *s : students) {
        stream << s->toCsvRow() << "\n";
    }
    return csvData;
}

void StudentGroup::printGroupReport() const
{
    qDebug() << "============================================================";
    qDebug() << "BÁO CÁO TỔNG HỢP LỚP/NHÓM:" << groupName << "(Mã:" << groupCode << ")";
    qDebug() << "Tổng số sinh viên:" << getTotalStudentCount() << "| Điểm TB chung:" << QString::number(calculateAverageGpa(), 'f', 2);
    qDebug() << "Số lượng đạt học bổng:" << getScholarshipCount();

    qDebug() << "--- Phân bố theo chuyên ngành ---";
    QMap<QString, int> majors = countStudentsByMajor();
    for (auto it = majors.begin(); it != majors.end(); ++it) {
        qDebug() << " +" << it.key() << ":" << it.value() << "sinh viên";
    }

    qDebug() << "--- Phân bố học lực ---";
    QMap<QString, int> standings = getAcademicStandingDistribution();
    for (auto it = standings.begin(); it != standings.end(); ++it) {
        qDebug() << " +" << it.key() << ":" << it.value() << "sinh viên";
    }
    qDebug() << "============================================================";
}