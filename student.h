#ifndef STUDENT_H
#define STUDENT_H

#include <QString>
#include <QDate>
#include <QJsonObject>
#include <stdexcept>

/**
 * @brief Lớp trừu tượng Student (Abstract Base Class)
 * Đại diện cho thực thể sinh viên trong hệ thống quản lý trường Đại học.
 * Tuân thủ nghiêm ngặt tính Đóng gói (Encapsulation) và Trừu tượng (Abstraction).
 */
class Student
{
protected:
    QString id;          // Mã số sinh viên (Chuẩn 8 ký tự số, VD: 25127001)
    QString fullName;    // Họ và tên (Tự động chuẩn hóa viết hoa chữ cái đầu)
    QDate dob;           // Ngày sinh (Kiểm tra hợp lệ từ 17 đến 50 tuổi)
    QString className;   // Lớp sinh hoạt (VD: 25DTH1)
    QString email;       // Email trường cấp (Tự động sinh theo ID: <id>@student.hcmus.edu.vn)
    QString phoneNumber; // Số điện thoại liên lạc (Tùy chọn)

    // Các hàm hỗ trợ nội bộ (Helper functions)
    static QString sanitizeName(const QString &rawName);
    static bool validateId(const QString &rawId);
    static bool validateDob(const QDate &rawDob);
    void generateEmail();

public:
    // Khởi tạo và Hủy
    explicit Student(const QString &id = "",
                     const QString &fullName = "",
                     const QDate &dob = QDate::currentDate(),
                     const QString &className = "",
                     const QString &phone = "");

    virtual ~Student() = default; // Virtual destructor đảm bảo hủy đúng đối tượng lớp con

    // --- GETTERS (Cung cấp truy cập chỉ đọc) ---
    QString getId() const;
    QString getFullName() const;
    QDate getDob() const;
    QString getClassName() const;
    QString getEmail() const;
    QString getPhoneNumber() const;
    int getAge() const;
    QString getFormattedDob() const; // Trả về dạng DD/MM/YYYY

    // --- SETTERS (Đóng gói & Kiểm tra tính hợp lệ của dữ liệu đầu vào) ---
    void setId(const QString &newId);
    void setFullName(const QString &newFullName);
    void setDob(const QDate &newDob);
    void setClassName(const QString &newClassName);
    void setPhoneNumber(const QString &newPhone);

    // --- CÁC PHƯƠNG THỨC THUẦN ẢO (Pure Virtual Functions) ---
    // Bắt buộc các lớp con chuyên ngành (IT, Math, Bio) phải triển khai (Đa hình)
    virtual double calculateGPA() const = 0;
    virtual int getTotalCredits() const = 0;
    virtual QString getMajor() const = 0;
    virtual QString getClassification() const = 0;
    virtual bool isScholarshipEligible() const = 0;

    // Giao diện chuyển đổi dữ liệu (Serialization) để lưu file / giao tiếp API
    virtual QJsonObject toJsonObject() const = 0;
    virtual QString toCsvRow() const = 0;

    // --- PHƯƠNG THỨC NGHIỆP VỤ CHUNG ---
    virtual QString evaluateAcademicStanding() const;
    virtual void printSummary() const;

    // --- NẠP CHỒNG TOÁN TỬ (Operator Overloading) ---
    // Hỗ trợ tìm kiếm, so sánh và sắp xếp trong các container (QVector, std::sort)
    bool operator==(const Student &other) const;
    bool operator<(const Student &other) const; // Mặc định sắp xếp theo GPA giảm dần
};

#endif // STUDENT_H