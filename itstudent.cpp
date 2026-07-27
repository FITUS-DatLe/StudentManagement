#include "itstudent.h"
#include <algorithm>
#include <QDebug>
#include <QJsonArray>

// ============================================================================
// KHỞI TẠO & KIỂM TRA DỮ LIỆU
// ============================================================================

ITStudent::ITStudent(const QString &id, const QString &fullName, const QDate &dob,
                     const QString &className, const QString &phone,
                     double cpp, double discreteMath, double dsa)
    : Student(id, fullName, dob, className, phone) // Gọi constructor lớp cha
{
    // Sử dụng hàm setAllScores để đảm bảo dữ liệu đi qua bộ lọc Validation
    setAllScores(cpp, discreteMath, dsa);
}

bool ITStudent::validateScore(double score)
{
    // Điểm số đại học hợp lệ phải nằm trong thang điểm 10 (từ 0.0 đến 10.0)
    return (score >= 0.0 && score <= 10.0);
}

// ============================================================================
// GETTERS
// ============================================================================

double ITStudent::getCppScore() const { return cppScore; }
double ITStudent::getDiscreteMathScore() const { return discreteMathScore; }
double ITStudent::getDsaScore() const { return dsaScore; }

int ITStudent::getCppCredits() { return CREDITS_CPP; }
int ITStudent::getDiscreteMathCredits() { return CREDITS_MATH; }
int ITStudent::getDsaCredits() { return CREDITS_DSA; }

// ============================================================================
// SETTERS (VALIDATION & EXCEPTION THROWING)
// ============================================================================

void ITStudent::setCppScore(double score)
{
    if (!validateScore(score)) {
        throw std::invalid_argument("Điểm Lập trình C++ không hợp lệ! Vui lòng nhập từ 0.0 đến 10.0.");
    }
    cppScore = score;
}

void ITStudent::setDiscreteMathScore(double score)
{
    if (!validateScore(score)) {
        throw std::invalid_argument("Điểm Toán rời rạc không hợp lệ! Vui lòng nhập từ 0.0 đến 10.0.");
    }
    discreteMathScore = score;
}

void ITStudent::setDsaScore(double score)
{
    if (!validateScore(score)) {
        throw std::invalid_argument("Điểm Cấu trúc dữ liệu & Thuật toán (DSA) không hợp lệ! Vui lòng nhập từ 0.0 đến 10.0.");
    }
    dsaScore = score;
}

void ITStudent::setAllScores(double cpp, double discreteMath, double dsa)
{
    setCppScore(cpp);
    setDiscreteMathScore(discreteMath);
    setDsaScore(dsa);
}

// ============================================================================
// TRIỂN KHAI PHƯƠNG THỨC ĐA HÌNH (POLYMORPHISM IMPLEMENTATIONS)
// ============================================================================

double ITStudent::calculateGPA() const
{
    // Công thức GPA trọng số tín chỉ: (Điểm * Tín chỉ) / Tổng tín chỉ
    double totalPoints = (cppScore * CREDITS_CPP) +
                         (discreteMathScore * CREDITS_MATH) +
                         (dsaScore * CREDITS_DSA);

    return totalPoints / static_cast<double>(getTotalCredits());
}

int ITStudent::getTotalCredits() const
{
    return CREDITS_CPP + CREDITS_MATH + CREDITS_DSA; // Tổng = 11 tín chỉ
}

QString ITStudent::getMajor() const
{
    return "Công nghệ Thông tin";
}

QString ITStudent::getClassification() const
{
    QString baseStanding = evaluateAcademicStanding();

    // Gắn nhãn danh hiệu đặc biệt cho sinh viên CNTT xuất sắc
    if (isCodingElite()) {
        return "🔥 Top Coder (" + baseStanding + ")";
    }
    if (isScholarshipEligible()) {
        return "🌟 " + baseStanding + " (Học Bổng CNTT)";
    }
    return baseStanding;
}

bool ITStudent::isScholarshipEligible() const
{
    // Điều kiện học bổng ngành CNTT:
    // 1. GPA tổng >= 8.5
    // 2. Hai môn lập trình cốt lõi (C++ và DSA) bắt buộc phải từ 8.0 trở lên
    // 3. Không có môn nào dưới 7.0 (Không bị điểm liệt học bổng)
    double gpa = calculateGPA();
    return (gpa >= 8.5 && cppScore >= 8.0 && dsaScore >= 8.0 && discreteMathScore >= 7.0);
}

// ============================================================================
// GIAO DIỆN CHUYỂN ĐỔI DỮ LIỆU (SERIALIZATION)
// ============================================================================

QJsonObject ITStudent::toJsonObject() const
{
    QJsonObject json;
    // Thông tin hành chính từ lớp cha
    json["id"] = id;
    json["fullName"] = fullName;
    json["dob"] = dob.toString("yyyy-MM-dd");
    json["className"] = className;
    json["email"] = email;
    json["phoneNumber"] = phoneNumber;
    json["major"] = getMajor();

    // Thông tin điểm số chuyên ngành
    QJsonObject scores;
    scores["cpp"] = cppScore;
    scores["discreteMath"] = discreteMathScore;
    scores["dsa"] = dsaScore;
    scores["gpa"] = QString::number(calculateGPA(), 'f', 2).toDouble();

    json["academicPerformance"] = scores;
    json["classification"] = getClassification();
    json["scholarship"] = isScholarshipEligible();

    return json;
}

QString ITStudent::toCsvRow() const
{
    // Định dạng CSV chuẩn: ID,FullName,DOB,Class,Phone,Major,Cpp,Math,DSA,GPA,Classification
    return QString("%1,\"%2\",%3,%4,%5,\"%6\",%7,%8,%9,%10,\"%11\"")
        .arg(id)
        .arg(fullName)
        .arg(getFormattedDob())
        .arg(className)
        .arg(phoneNumber)
        .arg(getMajor())
        .arg(cppScore, 0, 'f', 2)
        .arg(discreteMathScore, 0, 'f', 2)
        .arg(dsaScore, 0, 'f', 2)
        .arg(calculateGPA(), 0, 'f', 2)
        .arg(getClassification());
}

// ============================================================================
// PHƯƠNG THỨC NGHIỆP VỤ MỞ RỘNG
// ============================================================================

void ITStudent::printSummary() const
{
    qDebug() << "----------------------------------------";
    qDebug() << "[KHOA CÔNG NGHỆ THÔNG TIN - PHIẾU ĐIỂM]";
    qDebug() << "MSSV:" << id << "| Họ tên:" << fullName << "| Lớp:" << className;
    qDebug() << "Điểm C++ (4 TC):" << cppScore << "| Toán RR (3 TC):" << discreteMathScore << "| DSA (4 TC):" << dsaScore;
    qDebug() << "=> GPA Tín chỉ:" << QString::number(calculateGPA(), 'f', 2) << "| Xếp loại:" << getClassification();
    qDebug() << "----------------------------------------";
}

QString ITStudent::getStrongestSubject() const
{
    double maxScore = std::max({cppScore, discreteMathScore, dsaScore});
    if (qAbs(maxScore - cppScore) < 0.001) return "Lập trình C++ (" + QString::number(cppScore) + ")";
    if (qAbs(maxScore - dsaScore) < 0.001) return "Cấu trúc dữ liệu & Thuật toán (" + QString::number(dsaScore) + ")";
    return "Toán rời rạc (" + QString::number(discreteMathScore) + ")";
}

bool ITStudent::isCodingElite() const
{
    // Đạt danh hiệu Top Coder nếu 2 môn chuyên sâu thuật toán và lập trình đều từ 9.0 trở lên
    return (cppScore >= 9.0 && dsaScore >= 9.0);
}