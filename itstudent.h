#ifndef ITSTUDENT_H
#define ITSTUDENT_H

#include "student.h"
#include <QJsonObject>

/**
 * @brief Lớp ITStudent - Đại diện cho sinh viên khoa Công nghệ Thông tin
 * Kế thừa (Inheritance) từ lớp trừu tượng Student.
 * Thể hiện tính Đa hình (Polymorphism) qua việc ghi đè các công thức tính toán và chuẩn dữ liệu.
 */
class ITStudent : public Student
{
private:
    double cppScore;          // Điểm Lập trình C++ (Môn cốt lõi)
    double discreteMathScore; // Điểm Toán rời rạc
    double dsaScore;          // Điểm Cấu trúc dữ liệu & Thuật toán (DSA - Môn cốt lõi)

    // Hằng số tín chỉ chuyên ngành (Sử dụng constexpr để tối ưu bộ nhớ dịch)
    static constexpr int CREDITS_CPP = 4;
    static constexpr int CREDITS_MATH = 3;
    static constexpr int CREDITS_DSA = 4;

    // Hàm hỗ trợ kiểm tra điểm hợp lệ nội bộ
    static bool validateScore(double score);

public:
    // Khởi tạo đầy đủ tham số với giá trị mặc định
    explicit ITStudent(const QString &id = "",
                       const QString &fullName = "",
                       const QDate &dob = QDate::currentDate(),
                       const QString &className = "",
                       const QString &phone = "",
                       double cpp = 0.0,
                       double discreteMath = 0.0,
                       double dsa = 0.0);

    ~ITStudent() override = default;

    // --- GETTERS (Đặc thù ngành CNTT) ---
    double getCppScore() const;
    double getDiscreteMathScore() const;
    double getDsaScore() const;
    static int getCppCredits();
    static int getDiscreteMathCredits();
    static int getDsaCredits();

    // --- SETTERS (Tích hợp Exception Handling) ---
    void setCppScore(double score);
    void setDiscreteMathScore(double score);
    void setDsaScore(double score);
    void setAllScores(double cpp, double discreteMath, double dsa);

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
    QString getStrongestSubject() const; // Phân tích môn học có điểm cao nhất
    bool isCodingElite() const;          // Đánh giá danh hiệu "Top Coder" (C++ & DSA >= 9.0)
};

#endif // ITSTUDENT_H