#include "mathstudent.h"
#include <QDebug>

// ============================================================================
// KHỞI TẠO & KIỂM TRA DỮ LIỆU
// ============================================================================

MathStudent::MathStudent(const QString &id, const QString &fullName, const QDate &dob,
                         const QString &className, const QString &phone,
                         double calculus, double linearAlgebra)
    : Student(id, fullName, dob, className, phone) // Khởi tạo lớp cha
{
    setAllScores(calculus, linearAlgebra);
}

bool MathStudent::validateScore(double score)
{
    // Thang điểm chuẩn đại học từ 0.0 đến 10.0
    return (score >= 0.0 && score <= 10.0);
}

// ============================================================================
// GETTERS
// ============================================================================

double MathStudent::getCalculusScore() const { return calculusScore; }
double MathStudent::getLinearAlgebraScore() const { return linearAlgebraScore; }

int MathStudent::getCalculusCredits() { return CREDITS_CALCULUS; }
int MathStudent::getLinearAlgebraCredits() { return CREDITS_LINEAR_ALGEBRA; }

// ============================================================================
// SETTERS (VALIDATION & EXCEPTION THROWING)
// ============================================================================

void MathStudent::setCalculusScore(double score)
{
    if (!validateScore(score)) {
        throw std::invalid_argument("Điểm Giải tích không hợp lệ! Vui lòng nhập giá trị từ 0.0 đến 10.0.");
    }
    calculusScore = score;
}

void MathStudent::setLinearAlgebraScore(double score)
{
    if (!validateScore(score)) {
        throw std::invalid_argument("Điểm Đại số tuyến tính không hợp lệ! Vui lòng nhập giá trị từ 0.0 đến 10.0.");
    }
    linearAlgebraScore = score;
}

void MathStudent::setAllScores(double calculus, double linearAlgebra)
{
    setCalculusScore(calculus);
    setLinearAlgebraScore(linearAlgebra);
}

// ============================================================================
// TRIỂN KHAI PHƯƠNG THỨC ĐA HÌNH (POLYMORPHISM IMPLEMENTATIONS)
// ============================================================================

double MathStudent::calculateGPA() const
{
    // Công thức tính GPA theo trọng số tín chỉ: (Điểm * Tín chỉ) / Tổng tín chỉ
    double totalPoints = (calculusScore * CREDITS_CALCULUS) +
                         (linearAlgebraScore * CREDITS_LINEAR_ALGEBRA);

    return totalPoints / static_cast<double>(getTotalCredits());
}

int MathStudent::getTotalCredits() const
{
    return CREDITS_CALCULUS + CREDITS_LINEAR_ALGEBRA; // Tổng = 9 tín chỉ
}

QString MathStudent::getMajor() const
{
    return "Toán Học";
}

QString MathStudent::getClassification() const
{
    QString baseStanding = evaluateAcademicStanding();

    // Gắn nhãn danh hiệu đặc biệt cho sinh viên Toán xuất sắc
    if (isMathOlympiadCandidate()) {
        return "🏆 Ứng viên Olympic (" + baseStanding + ")";
    }
    if (isScholarshipEligible()) {
        return "🌟 " + baseStanding + " (Học Bổng Toán Học)";
    }
    return baseStanding;
}

bool MathStudent::isScholarshipEligible() const
{
    // Điều kiện học bổng khoa Toán Học:
    // 1. Điểm trung bình tích lũy >= 8.5
    // 2. Cả hai môn lý thuyết cốt lõi đều phải đạt từ 8.0 trở lên
    double gpa = calculateGPA();
    return (gpa >= 8.5 && calculusScore >= 8.0 && linearAlgebraScore >= 8.0);
}

// ============================================================================
// GIAO DIỆN CHUYỂN ĐỔI DỮ LIỆU (SERIALIZATION)
// ============================================================================

QJsonObject MathStudent::toJsonObject() const
{
    QJsonObject json;
    // Thông tin định danh từ lớp cha
    json["id"] = id;
    json["fullName"] = fullName;
    json["dob"] = dob.toString("yyyy-MM-dd");
    json["className"] = className;
    json["email"] = email;
    json["phoneNumber"] = phoneNumber;
    json["major"] = getMajor();

    // Thông tin điểm chuyên ngành Toán
    QJsonObject scores;
    scores["calculus"] = calculusScore;
    scores["linearAlgebra"] = linearAlgebraScore;
    scores["gpa"] = QString::number(calculateGPA(), 'f', 2).toDouble();

    json["academicPerformance"] = scores;
    json["classification"] = getClassification();
    json["scholarship"] = isScholarshipEligible();

    return json;
}

QString MathStudent::toCsvRow() const
{
    // Sử dụng multi-arg call trong .arg() duy nhất để sửa cảnh báo clazy-qstring-arg
    return QString("%1,\"%2\",%3,%4,%5,\"%6\",%7,%8,0.00,%9,\"%10\"")
        .arg(id,
             fullName,
             getFormattedDob(),
             className,
             phoneNumber,
             getMajor(),
             QString::number(calculusScore, 'f', 2),
             QString::number(linearAlgebraScore, 'f', 2),
             QString::number(calculateGPA(), 'f', 2),
             getClassification());
}

// ============================================================================
// PHƯƠNG THỨC NGHIỆP VỤ MỞ RỘNG
// ============================================================================

void MathStudent::printSummary() const
{
    qDebug() << "----------------------------------------";
    qDebug() << "[KHOA TOÁN HỌC - PHIẾU ĐIỂM CHUYÊN NGÀNH]";
    qDebug() << "MSSV:" << id << "| Họ tên:" << fullName << "| Lớp:" << className;
    qDebug() << "Giải tích (5 TC):" << calculusScore << "| Đại số tuyến tính (4 TC):" << linearAlgebraScore;
    qDebug() << "=> GPA Tín chỉ:" << QString::number(calculateGPA(), 'f', 2) << "| Xếp loại:" << getClassification();
    qDebug() << "----------------------------------------";
}

QString MathStudent::getStrongestSubject() const
{
    if (qAbs(calculusScore - linearAlgebraScore) < 0.001) {
        return "Cân bằng Giải tích & Đại số (" + QString::number(calculusScore) + ")";
    }
    if (calculusScore > linearAlgebraScore) {
        return "Giải tích (" + QString::number(calculusScore) + ")";
    }
    return "Đại số tuyến tính (" + QString::number(linearAlgebraScore) + ")";
}

bool MathStudent::isMathOlympiadCandidate() const
{
    // Tiêu chuẩn chọn đội tuyển Olympic Toán: Cả Giải tích và Đại số tuyến tính từ 9.0 trở lên
    return (calculusScore >= 9.0 && linearAlgebraScore >= 9.0);
}