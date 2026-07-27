#ifndef STUDENTFACTORY_H
#define STUDENTFACTORY_H

#include "student.h"
#include "itstudent.h"
#include "mathstudent.h"
#include "biostudent.h"
#include <QString>
#include <QDate>
#include <QJsonObject>
#include <stdexcept>

/**
 * @brief Lớp StudentFactory - Triển khai Design Pattern: Factory Method
 * Chuyên trách việc tạo lập các đối tượng sinh viên chuyên ngành đa hình.
 */
class StudentFactory
{
private:
    // Hàm hỗ trợ nội bộ chuẩn hóa từ khóa chuyên ngành
    static QString normalizeMajorType(const QString &rawMajor);

public:
    // Vô hiệu hóa bộ dựng mặc định vì đây là lớp tiện ích tĩnh (Static Utility Class)
    StudentFactory() = delete;

    // 1. Khởi tạo từ danh sách tham số rời rạc (Dùng cho Form nhập liệu ở StudentDialog)
    static Student* createStudent(const QString &majorType,
                                  const QString &id,
                                  const QString &fullName,
                                  const QDate &dob,
                                  const QString &className,
                                  const QString &phone,
                                  double score1,
                                  double score2,
                                  double score3 = 0.0);

    // 2. Khởi tạo từ đối tượng JSON (Dùng khi đọc dữ liệu từ API hoặc file JSON)
    static Student* createFromJson(const QJsonObject &json);

    // 3. Khởi tạo từ dòng dữ liệu CSV (Dùng khi import/export dữ liệu từ bảng tính)
    static Student* createFromCsvRow(const QString &csvRow);

    // Hàm tiện ích lấy danh sách các ngành hỗ trợ trong hệ thống
    static QStringList getSupportedMajors();
};

#endif // STUDENTFACTORY_H