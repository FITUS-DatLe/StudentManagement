#include "biostudent.h"
#include <QDebug>

// ============================================================================
// KHỞI TẠO & KIỂM TRA DỮ LIỆU
// ============================================================================

BioStudent::BioStudent(const QString &id, const QString &fullName, const QDate &dob,
                       const QString &className, const QString &phone,
                       double molBio, double genetics)
    : Student(id, fullName, dob, className, phone) // Khởi tạo lớp cha
{
    setAllScores(molBio, genetics);
}

bool BioStudent::validateScore(double score)
{
    // Thang điểm chuẩn đại học từ 0.0 đến 10.0
    return (score >= 0.0 && score <= 10.0);
}

// ============================================================================
// GETTERS
// ============================================================================

double BioStudent::getMolecularBioScore() const { return molecularBioScore; }
double BioStudent::getGeneticsScore() const { return geneticsScore; }

int BioStudent::getMolecularBioCredits() { return CREDITS_MOL_BIO; }
int BioStudent::getGeneticsCredits() { return CREDITS_GENETICS; }

// ============================================================================
// SETTERS (VALIDATION & EXCEPTION THROWING)
// ============================================================================

void BioStudent::setMolecularBioScore(double score)
{
    if (!validateScore(score)) {
        throw std::invalid_argument("Điểm Sinh học phân tử không hợp lệ! Vui lòng nhập giá trị từ 0.0 đến 10.0.");
    }
    molecularBioScore = score;
}

void BioStudent::setGeneticsScore(double score)
{
    if (!validateScore(score)) {
        throw std::invalid_argument("Điểm Di truyền học không hợp lệ! Vui lòng nhập giá trị từ 0.0 đến 10.0.");
    }
    geneticsScore = score;
}

void BioStudent::setAllScores(double molBio, double genetics)
{
    setMolecularBioScore(molBio);
    setGeneticsScore(genetics);
}

// ============================================================================
// TRIỂN KHAI PHƯƠNG THỨC ĐA HÌNH (POLYMORPHISM IMPLEMENTATIONS)
// ============================================================================

double BioStudent::calculateGPA() const
{
    // Công thức tính GPA theo tín chỉ: (Điểm * Tín chỉ) / Tổng tín chỉ
    double totalPoints = (molecularBioScore * CREDITS_MOL_BIO) +
                         (geneticsScore * CREDITS_GENETICS);

    return totalPoints / static_cast<double>(getTotalCredits());
}

int BioStudent::getTotalCredits() const
{
    return CREDITS_MOL_BIO + CREDITS_GENETICS; // Tổng = 8 tín chỉ
}

QString BioStudent::getMajor() const
{
    return "Sinh Học";
}

QString BioStudent::getClassification() const
{
    QString baseStanding = evaluateAcademicStanding();

    // Gắn nhãn danh hiệu đặc biệt cho sinh viên Sinh học xuất sắc
    if (isLabResearcherCandidate()) {
        return "🧬 Ứng viên Lab (" + baseStanding + ")";
    }
    if (isScholarshipEligible()) {
        return "🌟 " + baseStanding + " (Học Bổng Sinh Học)";
    }
    return baseStanding;
}

bool BioStudent::isScholarshipEligible() const
{
    // Điều kiện học bổng khoa Sinh Học:
    // 1. Điểm trung bình tích lũy >= 8.5
    // 2. Không môn cốt lõi nào dưới 8.0
    double gpa = calculateGPA();
    return (gpa >= 8.5 && molecularBioScore >= 8.0 && geneticsScore >= 8.0);
}

// ============================================================================
// GIAO DIỆN CHUYỂN ĐỔI DỮ LIỆU (SERIALIZATION)
// ============================================================================

QJsonObject BioStudent::toJsonObject() const
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

    // Thông tin điểm chuyên ngành Sinh
    QJsonObject scores;
    scores["molecularBio"] = molecularBioScore;
    scores["genetics"] = geneticsScore;
    scores["gpa"] = QString::number(calculateGPA(), 'f', 2).toDouble();

    json["academicPerformance"] = scores;
    json["classification"] = getClassification();
    json["scholarship"] = isScholarshipEligible();

    return json;
}

QString BioStudent::toCsvRow() const
{
    // Sử dụng multi-arg call trong .arg() duy nhất để tối ưu hiệu năng và tránh cảnh báo linter
    // Cột thứ 3 để trống (0.00) nhằm đồng bộ số lượng cột trên bảng tổng hợp toàn trường
    return QString("%1,\"%2\",%3,%4,%5,\"%6\",%7,%8,0.00,%9,\"%10\"")
        .arg(id,
             fullName,
             getFormattedDob(),
             className,
             phoneNumber,
             getMajor(),
             QString::number(molecularBioScore, 'f', 2),
             QString::number(geneticsScore, 'f', 2),
             QString::number(calculateGPA(), 'f', 2),
             getClassification());
}

// ============================================================================
// PHƯƠNG THỨC NGHIỆP VỤ MỞ RỘNG
// ============================================================================

void BioStudent::printSummary() const
{
    qDebug() << "----------------------------------------";
    qDebug() << "[KHOA SINH HỌC - PHIẾU ĐIỂM CHUYÊN NGÀNH]";
    qDebug() << "MSSV:" << id << "| Họ tên:" << fullName << "| Lớp:" << className;
    qDebug() << "Sinh học phân tử (4 TC):" << molecularBioScore << "| Di truyền học (4 TC):" << geneticsScore;
    qDebug() << "=> GPA Tín chỉ:" << QString::number(calculateGPA(), 'f', 2) << "| Xếp loại:" << getClassification();
    qDebug() << "----------------------------------------";
}

QString BioStudent::getStrongestSubject() const
{
    if (qAbs(molecularBioScore - geneticsScore) < 0.001) {
        return "Cân bằng SH Phân tử & Di truyền (" + QString::number(molecularBioScore) + ")";
    }
    if (molecularBioScore > geneticsScore) {
        return "Sinh học phân tử (" + QString::number(molecularBioScore) + ")";
    }
    return "Di truyền học (" + QString::number(geneticsScore) + ")";
}

bool BioStudent::isLabResearcherCandidate() const
{
    // Tiêu chuẩn chọn Trợ lý nghiên cứu Lab: Cả hai môn chuyên sâu đều đạt từ 9.0 trở lên
    return (molecularBioScore >= 9.0 && geneticsScore >= 9.0);
}