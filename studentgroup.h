#ifndef STUDENTGROUP_H
#define STUDENTGROUP_H

#include <QString>
#include <QVector>
#include <QMap>
#include <QJsonArray>
#include <QJsonObject>
#include <stdexcept>
#include "student.h"

/**
 * @brief Lớp StudentGroup - Quản lý tập hợp sinh viên theo mô hình Bao gộp (Composition)
 * Quản lý vòng đời (lifecycle) của các đối tượng Student thông qua con trỏ đa hình.
 */
class StudentGroup
{
private:
    QString groupName;          // Tên nhóm / Lớp học (VD: Lớp CNTT K2025)
    QString groupCode;          // Mã lớp hành chính (VD: 25DTH1)
    QVector<Student*> students; // Danh sách con trỏ đa hình quản lý sinh viên

public:
    // Khởi tạo và Hủy
    explicit StudentGroup(const QString &name = "Default Group", const QString &code = "GENERAL");
    ~StudentGroup(); // Destructor tự động giải phóng toàn bộ bộ nhớ cấp phát động

    // Vô hiệu hóa Copy Constructor và Assignment Operator để tránh lỗi Double-Free con trỏ
    StudentGroup(const StudentGroup&) = delete;
    StudentGroup& operator=(const StudentGroup&) = delete;

    // --- GETTERS & SETTERS ---
    QString getGroupName() const;
    QString getGroupCode() const;
    void setGroupName(const QString &name);
    void setGroupCode(const QString &code);

    // --- CÁC THAO TÁC QUẢN LÝ DANH SÁCH (CRUD OPERATIONS) ---
    void addStudent(Student *student);                   // Thêm sinh viên (Kiểm tra trùng ID)
    bool removeStudentById(const QString &studentId);    // Xóa sinh viên theo ID và giải phóng RAM
    Student* findStudentById(const QString &studentId) const; // Tìm kiếm sinh viên theo ID
    void clearAll();                                     // Xóa sạch danh sách và giải phóng bộ nhớ

    // --- TÌM KIẾM & LỌC DỮ LIỆU NÂNG CAO ---
    QVector<Student*> getAllStudents() const;
    QVector<Student*> getStudentsByMajor(const QString &major) const;
    QVector<Student*> getScholarshipCandidates() const;
    QVector<Student*> searchStudentsByName(const QString &keyword) const;
    QVector<Student*> getTopPerformers(int count = 5) const; // Lấy Top N sinh viên điểm cao nhất

    // --- SẮP XẾP DANH SÁCH ---
    void sortByGpaDescending();
    void sortByIdAscending();
    void sortByNameAscending();

    // --- THỐNG KÊ HỌC THUẬT (STATISTICS) ---
    int getTotalStudentCount() const;
    double calculateAverageGpa() const;
    int getScholarshipCount() const;
    QMap<QString, int> countStudentsByMajor() const;
    QMap<QString, int> getAcademicStandingDistribution() const; // Thống kê số lượng theo xếp loại

    // --- GIAO DIỆN CHUYỂN ĐỔI DỮ LIỆU (SERIALIZATION) ---
    QJsonArray toJsonArray() const;
    QString toCsvString() const;
    void printGroupReport() const;
};

#endif // STUDENTGROUP_H