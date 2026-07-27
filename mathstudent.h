#ifndef MATHSTUDENT_H
#define MATHSTUDENT_H

#include "student.h"
#include <QJsonObject>

/**
 * @brief Lớp MathStudent - Đại diện cho sinh viên khoa Toán Học
 * Kế thừa (Inheritance) từ lớp trừu tượng Student.
 * Áp dụng trọng số tín chỉ cao cho các môn khoa học cơ bản và tư duy logic.
 */
class MathStudent : public Student
{
private:
    double calculusScore;      // Điểm Giải tích (Môn cốt lõi - Trọng số cao)
    double linearAlgebraScore; // Điểm Đại số tuyến tính (Môn cốt lõi)

    // Hằng số tín chỉ chuyên ngành Toán học
    static constexpr int CREDITS_CALCULUS = 5;
    static constexpr int CREDITS_LINEAR_ALGEBRA = 4;

    // Hàm kiểm tra tính hợp lệ của điểm số đầu vào
    static bool validateScore(double score);

public:
    // Khởi tạo với đầy đủ các giá trị mặc định
    explicit MathStudent(const QString &id = "",
                         const QString &fullName = "",
                         const QDate &dob = QDate::currentDate(),
                         const QString &className = "",
                         const QString &phone = "",
                         double calculus = 0.0,
                         double linearAlgebra = 0.0);

    ~MathStudent() override = default;

    // --- GETTERS ---
    double getCalculusScore() const;
    double getLinearAlgebraScore() const;
    static int getCalculusCredits();
    static int getLinearAlgebraCredits();

    // --- SETTERS (Tích hợp kiểm tra ngoại lệ) ---
    void setCalculusScore(double score);
    void setLinearAlgebraScore(double score);
    void setAllScores(double calculus, double linearAlgebra);

    // --- GHI ĐÈ PHƯƠNG THỨC ĐA HÌNH (Polymorphism Overrides) ---
    double calculateGPA() const override;
    int getTotalCredits() const override;
    QString getMajor() const override;
    QString getClassification() const override;
    bool isScholarshipEligible() const override;

    // --- GIAO DIỆN CHUYỂN ĐỔI DỮ LIỆU (Serialization) ---
    QJsonObject toJsonObject() const override;
    QString toCsvRow() const override;

    // --- PHƯƠNG THỨC NGHIỆP VỤ MỞ RỘNG ---
    void printSummary() const override;
    QString getStrongestSubject() const; // Xác định môn chuyên ngành thế mạnh
    bool isMathOlympiadCandidate() const; // Đánh giá tiêu chuẩn đội tuyển Olympic Toán
};

#endif // MATHSTUDENT_H