#ifndef BIOSTUDENT_H
#define BIOSTUDENT_H

#include "student.h"
#include <QJsonObject>

/**
 * @brief Lớp BioStudent - Đại diện cho sinh viên khoa Sinh Học
 * Kế thừa (Inheritance) từ lớp trừu tượng Student.
 * Tích hợp trọng số tín chỉ cho các học phần chuyên sâu về sự sống và di truyền.
 */
class BioStudent : public Student
{
private:
    double molecularBioScore; // Điểm Sinh học phân tử (Môn cốt lõi)
    double geneticsScore;     // Điểm Di truyền học (Môn cốt lõi)

    // Hằng số tín chỉ chuyên ngành Sinh học
    static constexpr int CREDITS_MOL_BIO = 4;
    static constexpr int CREDITS_GENETICS = 4;

    // Hàm kiểm tra tính hợp lệ của điểm số đầu vào
    static bool validateScore(double score);

public:
    // Khởi tạo với các tham số mặc định
    explicit BioStudent(const QString &id = "",
                        const QString &fullName = "",
                        const QDate &dob = QDate::currentDate(),
                        const QString &className = "",
                        const QString &phone = "",
                        double molBio = 0.0,
                        double genetics = 0.0);

    ~BioStudent() override = default;

    // --- GETTERS ---
    double getMolecularBioScore() const;
    double getGeneticsScore() const;
    static int getMolecularBioCredits();
    static int getGeneticsCredits();

    // --- SETTERS (Tích hợp xử lý ngoại lệ) ---
    void setMolecularBioScore(double score);
    void setGeneticsScore(double score);
    void setAllScores(double molBio, double genetics);

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
    QString getStrongestSubject() const;       // Xác định môn học thế mạnh
    bool isLabResearcherCandidate() const;     // Tiêu chuẩn chọn Nghiên cứu sinh Lab
};

#endif // BIOSTUDENT_H