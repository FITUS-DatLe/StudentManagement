#ifndef STUDENTMANAGER_H
#define STUDENTMANAGER_H

#include <QObject>
#include <QVector>
#include <QMap>
#include <QString>
#include "student.h"
#include "studentgroup.h"
#include "databasehelper.h"

/**
 * @brief Lớp StudentManager - Controller trung tâm (Design Pattern #2: Singleton)
 * Điều phối dữ liệu giữa RAM (StudentGroup) và ổ cứng SQLite (DatabaseHelper).
 * Kế thừa QObject để hỗ trợ cơ chế giao tiếp Signals/Slots cực kỳ mạnh mẽ của Qt.
 */
class StudentManager : public QObject
{
    Q_OBJECT

private:
    StudentGroup mainGroup; // Bộ nhớ đệm RAM quản lý toàn bộ danh sách
    bool isInitialized;

    // Private Constructor & Destructor cho Singleton Pattern
    explicit StudentManager(QObject *parent = nullptr);
    ~StudentManager() override;

    // Vô hiệu hóa Copy/Move để tránh phá vỡ quy tắc Singleton
    StudentManager(const StudentManager&) = delete;
    StudentManager& operator=(const StudentManager&) = delete;

public:
    static StudentManager& getInstance();

    // Khởi tạo hệ thống & Đọc dữ liệu từ SQLite lên RAM
    bool initManager();
    void reloadFromDatabase();
    bool isReady() const;

    // --- CÁC THAO TÁC NGHIỆP VỤ CHÍNH (CRUD) ---
    bool addStudent(Student *student);
    bool updateStudent(Student *student);
    bool deleteStudent(const QString &studentId);
    Student* getStudentById(const QString &studentId) const;
    QVector<Student*> getAllStudents() const;

    // --- TÌM KIẾM & LỌC DỮ LIỆU ---
    QVector<Student*> searchStudents(const QString &keyword) const;
    QVector<Student*> filterByMajor(const QString &major) const;
    QVector<Student*> filterScholarshipCandidates() const;
    QVector<Student*> getTopPerformers(int count = 5) const;

    // --- SẮP XẾP DANH SÁCH ---
    void sortListByGpaDesc();
    void sortListByIdAsc();
    void sortListByNameAsc();

    // --- THỐNG KÊ HỌC THUẬT (STATISTICS) ---
    int getTotalCount() const;
    double getAverageGpa() const;
    int getScholarshipCount() const;
    QMap<QString, int> getMajorDistribution() const;
    QMap<QString, int> getStandingDistribution() const;

    // --- NHẬP / XUẤT DỮ LIỆU (IMPORT / EXPORT) ---
    bool exportToCsv(const QString &filePath) const;
    bool importFromCsv(const QString &filePath);
    bool exportToJson(const QString &filePath) const;
    bool importFromJson(const QString &filePath);

signals:
    // Các tín hiệu thông báo cho Tầng View tự động cập nhật Giao diện
    void dataChanged();                         // Phát ra khi danh sách có thay đổi bất kỳ
    void studentAdded(const QString &studentId);
    void studentUpdated(const QString &studentId);
    void studentDeleted(const QString &studentId);
    void operationError(const QString &errorMessage); // Báo lỗi lên UI để hiển thị QMessageBox
};

#endif // STUDENTMANAGER_H