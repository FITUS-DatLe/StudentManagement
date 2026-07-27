#ifndef DATABASEHELPER_H
#define DATABASEHELPER_H

#include <QString>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QVector>
#include <QMutex>
#include <memory>
#include "student.h"
#include "studentfactory.h"

/**
 * @brief Lớp DatabaseHelper - Triển khai Singleton Pattern (Design Pattern #2)
 * Phiên bản Enterprise: Hỗ trợ Named Connection, WAL Mode, ACID Transactions và Batch Operations.
 */
class DatabaseHelper
{
private:
    QSqlDatabase db;
    const QString CONNECTION_NAME = "HCMUS_PRO_STUDENT_DB_CONN"; // Tên kết nối độc lập
    const int DATABASE_VERSION = 1; // Sử dụng cho Schema Migration sau này

    // Private Constructor & Destructor (Singleton Pattern)
    explicit DatabaseHelper();
    ~DatabaseHelper();

    // Vô hiệu hóa Copy và Move Constructor để tránh phá vỡ quy tắc Singleton
    DatabaseHelper(const DatabaseHelper&) = delete;
    DatabaseHelper& operator=(const DatabaseHelper&) = delete;
    DatabaseHelper(DatabaseHelper&&) = delete;
    DatabaseHelper& operator=(DatabaseHelper&&) = delete;

    // Các hàm quản lý nội bộ
    void configureDatabasePragmas(); // Tối ưu hóa hiệu năng SQLite (WAL mode, Foreign Keys)
    bool createTables();             // Tạo lược đồ cơ sở dữ liệu
    bool checkAndUpgradeSchema();    // Kiểm tra và nâng cấp phiên bản DB

public:
    // Truy cập thực thể duy nhất (Thread-safe trong C++11 trở lên)
    static DatabaseHelper& getInstance();

    // Khởi tạo và quản lý vòng đời kết nối
    bool initDatabase(const QString &dbPath = "hcmus_student_pro.db");
    void closeDatabase();
    bool isConnected() const;
    QString getDatabasePath() const;

    // --- QUẢN LÝ GIAO DỊCH (ACID TRANSACTION CONTROL) ---
    bool beginTransaction();
    bool commitTransaction();
    bool rollbackTransaction();

    // --- CÁC THAO TÁC NGHIỆP VỤ SINH VIÊN (CRUD OPERATIONS) ---
    bool insertStudent(const Student *student);
    bool updateStudent(const Student *student);
    bool deleteStudent(const QString &studentId);
    Student* getStudentById(const QString &studentId);
    QVector<Student*> loadAllStudents();

    // --- XỬ LÝ HÀNG LOẠT HIỆU NĂNG CAO (BATCH OPERATIONS) ---
    // Giúp import 10,000 sinh viên từ file CSV chỉ trong vài phần trăm giây
    bool insertStudentsBatch(const QVector<Student*> &students);
    bool deleteAllStudents(); // Reset toàn bộ bảng sinh viên

    // --- QUẢN LÝ TÀI KHOẢN NGƯỜI DÙNG (AUTHENTICATION & SECURITY) ---
    bool authenticateUser(const QString &username, const QString &password);
    bool registerUser(const QString &username, const QString &password, const QString &role = "ADMIN");
    bool changePassword(const QString &username, const QString &oldPassword, const QString &newPassword);

    // --- BẢO TRÌ HỆ THỐNG (MAINTENANCE) ---
    bool backupDatabase(const QString &backupDestPath);
};

#endif // DATABASEHELPER_H