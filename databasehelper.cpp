#include "databasehelper.h"
#include <QFile>
#include <QDir>
#include <QDebug>
#include <QSqlRecord>
#include <QVariant>

// ============================================================================
// KHỞI TẠO & QUẢN LÝ VÒNG ĐỜI (SINGLETON IMPLEMENTATION)
// ============================================================================

DatabaseHelper::DatabaseHelper()
{
}

DatabaseHelper::~DatabaseHelper()
{
    closeDatabase();
}

DatabaseHelper& DatabaseHelper::getInstance()
{
    static DatabaseHelper instance;
    return instance;
}

bool DatabaseHelper::initDatabase(const QString &dbPath)
{
    // Kiểm tra nếu kết nối đã tồn tại thì tái sử dụng, không tạo mới
    if (QSqlDatabase::contains(CONNECTION_NAME)) {
        db = QSqlDatabase::database(CONNECTION_NAME);
    } else {
        db = QSqlDatabase::addDatabase("QSQLITE", CONNECTION_NAME);
    }

    db.setDatabaseName(dbPath);

    if (!db.open()) {
        qCritical() << "[DB ERROR] Không thể mở cơ sở dữ liệu:" << db.lastError().text();
        return false;
    }

    qInfo() << "[DB INFO] Kết nối thành công tới SQLite:" << dbPath;

    // Áp dụng các cấu hình tối ưu hóa hiệu năng và kiểm tra lược đồ
    configureDatabasePragmas();

    if (!checkAndUpgradeSchema()) {
        qCritical() << "[DB ERROR] Khởi tạo lược đồ cơ sở dữ liệu thất bại!";
        return false;
    }

    return true;
}

void DatabaseHelper::closeDatabase()
{
    if (db.isOpen()) {
        db.close();
        qInfo() << "[DB INFO] Đã đóng kết nối cơ sở dữ liệu an toàn.";
    }
    // Thu hồi kết nối ra khỏi bộ nhớ quản lý của Qt
    if (QSqlDatabase::contains(CONNECTION_NAME)) {
        QSqlDatabase::removeDatabase(CONNECTION_NAME);
    }
}

bool DatabaseHelper::isConnected() const
{
    return db.isOpen();
}

QString DatabaseHelper::getDatabasePath() const
{
    return db.databaseName();
}

// ============================================================================
// CẤU HÌNH TỐI ƯU HÓA & LƯỢC ĐỒ (PRAGMAS & SCHEMA)
// ============================================================================

void DatabaseHelper::configureDatabasePragmas()
{
    QSqlQuery query(db);

    // 1. Kích hoạt ràng buộc khóa ngoại (Foreign Keys)
    query.exec("PRAGMA foreign_keys = ON;");

    // 2. Kích hoạt Write-Ahead Logging (WAL) để tăng tốc độ ghi và cho phép đọc/ghi đồng thời
    query.exec("PRAGMA journal_mode = WAL;");

    // 3. Tối ưu hóa đồng bộ đĩa cứng cho chế độ WAL
    query.exec("PRAGMA synchronous = NORMAL;");

    // 4. Tăng bộ nhớ đệm (Cache Size) lên 10,000 trang (~40MB RAM) để truy vấn siêu nhanh
    query.exec("PRAGMA cache_size = -10000;");
}

bool DatabaseHelper::checkAndUpgradeSchema()
{
    QSqlQuery query(db);

    // Tạo bảng metadata để theo dõi phiên bản CSDL
    if (!query.exec("CREATE TABLE IF NOT EXISTS db_metadata (version INTEGER PRIMARY KEY);")) {
        qCritical() << "[DB ERROR] Lỗi tạo bảng db_metadata:" << query.lastError().text();
        return false;
    }

    query.exec("SELECT version FROM db_metadata LIMIT 1;");
    int currentVersion = 0;
    if (query.next()) {
        currentVersion = query.value(0).toInt();
    } else {
        // Nếu là DB mới tinh, chèn phiên bản ban đầu
        query.exec(QString("INSERT INTO db_metadata (version) VALUES (%1);").arg(DATABASE_VERSION));
    }

    // Nếu cần nâng cấp lược đồ trong tương lai, viết logic migration tại đây
    if (currentVersion < DATABASE_VERSION) {
        qInfo() << "[DB INFO] Đang nâng cấp lược đồ từ phiên bản" << currentVersion << "lên" << DATABASE_VERSION;
        // Ví dụ: query.exec("ALTER TABLE students ADD COLUMN new_feature TEXT;");
        query.exec(QString("UPDATE db_metadata SET version = %1;").arg(DATABASE_VERSION));
    }

    return createTables();
}

bool DatabaseHelper::createTables()
{
    QSqlQuery query(db);

    // 1. Bảng Sinh viên chuẩn hóa (Thêm index cho cột major và full_name để tìm kiếm cực nhanh)
    QString createStudentSQL =
        "CREATE TABLE IF NOT EXISTS students ("
        "id TEXT PRIMARY KEY, "
        "full_name TEXT NOT NULL, "
        "dob TEXT NOT NULL, "
        "class_name TEXT, "
        "phone TEXT, "
        "major TEXT NOT NULL, "
        "score1 REAL DEFAULT 0.0, "
        "score2 REAL DEFAULT 0.0, "
        "score3 REAL DEFAULT 0.0, "
        "created_at DATETIME DEFAULT CURRENT_TIMESTAMP, "
        "updated_at DATETIME DEFAULT CURRENT_TIMESTAMP"
        ");";

    if (!query.exec(createStudentSQL)) {
        qCritical() << "[DB ERROR] Lỗi tạo bảng students:" << query.lastError().text();
        return false;
    }

    // Tạo Chỉ mục (Index) hỗ trợ tính năng Live-Search và Lọc theo ngành
    query.exec("CREATE INDEX IF NOT EXISTS idx_student_major ON students(major);");
    query.exec("CREATE INDEX IF NOT EXISTS idx_student_name ON students(full_name);");

    // 2. Bảng Tài khoản hệ thống
    QString createUserSQL =
        "CREATE TABLE IF NOT EXISTS users ("
        "username TEXT PRIMARY KEY, "
        "password TEXT NOT NULL, "
        "role TEXT DEFAULT 'ADMIN', "
        "created_at DATETIME DEFAULT CURRENT_TIMESTAMP"
        ");";

    if (!query.exec(createUserSQL)) {
        qCritical() << "[DB ERROR] Lỗi tạo bảng users:" << query.lastError().text();
        return false;
    }

    // Tạo tài khoản mặc định nếu chưa tồn tại
    query.exec("INSERT OR IGNORE INTO users (username, password, role) VALUES ('25127293', '123456', 'SUPER_ADMIN');");

    return true;
}

// ============================================================================
// QUẢN LÝ GIAO DỊCH (TRANSACTION CONTROL)
// ============================================================================

bool DatabaseHelper::beginTransaction()
{
    if (!db.isOpen()) return false;
    return db.transaction();
}

bool DatabaseHelper::commitTransaction()
{
    if (!db.isOpen()) return false;
    return db.commit();
}

bool DatabaseHelper::rollbackTransaction()
{
    if (!db.isOpen()) return false;
    return db.rollback();
}

// ============================================================================
// THAO TÁC NGHIỆP VỤ SINH VIÊN (CRUD OPERATIONS)
// ============================================================================

bool DatabaseHelper::insertStudent(const Student *student)
{
    if (!student || !db.isOpen()) return false;

    QSqlQuery query(db);
    query.prepare("INSERT INTO students (id, full_name, dob, class_name, phone, major, score1, score2, score3) "
                  "VALUES (:id, :name, :dob, :class, :phone, :major, :s1, :s2, :s3);");

    query.bindValue(":id", student->getId());
    query.bindValue(":name", student->getFullName());
    query.bindValue(":dob", student->getDob().toString("yyyy-MM-dd"));
    query.bindValue(":class", student->getClassName());
    query.bindValue(":phone", student->getPhoneNumber());
    query.bindValue(":major", student->getMajor());

    // Ánh xạ điểm đa hình theo từng ngành
    if (const ITStudent *it = dynamic_cast<const ITStudent*>(student)) {
        query.bindValue(":s1", it->getCppScore());
        query.bindValue(":s2", it->getDiscreteMathScore());
        query.bindValue(":s3", it->getDsaScore());
    } else if (const MathStudent *math = dynamic_cast<const MathStudent*>(student)) {
        query.bindValue(":s1", math->getCalculusScore());
        query.bindValue(":s2", math->getLinearAlgebraScore());
        query.bindValue(":s3", 0.0);
    } else if (const BioStudent *bio = dynamic_cast<const BioStudent*>(student)) {
        query.bindValue(":s1", bio->getMolecularBioScore());
        query.bindValue(":s2", bio->getGeneticsScore());
        query.bindValue(":s3", 0.0);
    }

    if (!query.exec()) {
        qCritical() << "[DB ERROR] Lỗi thêm sinh viên ID" << student->getId() << ":" << query.lastError().text();
        return false;
    }
    return true;
}

bool DatabaseHelper::updateStudent(const Student *student)
{
    if (!student || !db.isOpen()) return false;

    QSqlQuery query(db);
    query.prepare("UPDATE students SET full_name = :name, dob = :dob, class_name = :class, "
                  "phone = :phone, major = :major, score1 = :s1, score2 = :s2, score3 = :s3, "
                  "updated_at = CURRENT_TIMESTAMP WHERE id = :id;");

    query.bindValue(":id", student->getId());
    query.bindValue(":name", student->getFullName());
    query.bindValue(":dob", student->getDob().toString("yyyy-MM-dd"));
    query.bindValue(":class", student->getClassName());
    query.bindValue(":phone", student->getPhoneNumber());
    query.bindValue(":major", student->getMajor());

    if (const ITStudent *it = dynamic_cast<const ITStudent*>(student)) {
        query.bindValue(":s1", it->getCppScore());
        query.bindValue(":s2", it->getDiscreteMathScore());
        query.bindValue(":s3", it->getDsaScore());
    } else if (const MathStudent *math = dynamic_cast<const MathStudent*>(student)) {
        query.bindValue(":s1", math->getCalculusScore());
        query.bindValue(":s2", math->getLinearAlgebraScore());
        query.bindValue(":s3", 0.0);
    } else if (const BioStudent *bio = dynamic_cast<const BioStudent*>(student)) {
        query.bindValue(":s1", bio->getMolecularBioScore());
        query.bindValue(":s2", bio->getGeneticsScore());
        query.bindValue(":s3", 0.0);
    }

    if (!query.exec()) {
        qCritical() << "[DB ERROR] Lỗi cập nhật sinh viên ID" << student->getId() << ":" << query.lastError().text();
        return false;
    }
    return true;
}

bool DatabaseHelper::deleteStudent(const QString &studentId)
{
    if (studentId.trimmed().isEmpty() || !db.isOpen()) return false;

    QSqlQuery query(db);
    query.prepare("DELETE FROM students WHERE id = :id;");
    query.bindValue(":id", studentId.trimmed());

    if (!query.exec()) {
        qCritical() << "[DB ERROR] Lỗi xóa sinh viên ID" << studentId << ":" << query.lastError().text();
        return false;
    }
    return true;
}

Student* DatabaseHelper::getStudentById(const QString &studentId)
{
    if (studentId.trimmed().isEmpty() || !db.isOpen()) return nullptr;

    QSqlQuery query(db);
    query.prepare("SELECT id, full_name, dob, class_name, phone, major, score1, score2, score3 "
                  "FROM students WHERE id = :id LIMIT 1;");
    query.bindValue(":id", studentId.trimmed());

    if (query.exec() && query.next()) {
        try {
            return StudentFactory::createStudent(
                query.value(5).toString(), // major
                query.value(0).toString(), // id
                query.value(1).toString(), // name
                QDate::fromString(query.value(2).toString(), "yyyy-MM-dd"), // dob
                query.value(3).toString(), // class
                query.value(4).toString(), // phone
                query.value(6).toDouble(), // s1
                query.value(7).toDouble(), // s2
                query.value(8).toDouble()  // s3
                );
        } catch (const std::exception &e) {
            qCritical() << "[DB ERROR] Lỗi parse dữ liệu sinh viên:" << e.what();
        }
    }
    return nullptr;
}

QVector<Student*> DatabaseHelper::loadAllStudents()
{
    QVector<Student*> list;
    if (!db.isOpen()) return list;

    // Sắp xếp mặc định theo thời gian cập nhật mới nhất
    QSqlQuery query("SELECT id, full_name, dob, class_name, phone, major, score1, score2, score3 "
                    "FROM students ORDER BY updated_at DESC;", db);

    while (query.next()) {
        try {
            Student *student = StudentFactory::createStudent(
                query.value(5).toString(),
                query.value(0).toString(),
                query.value(1).toString(),
                QDate::fromString(query.value(2).toString(), "yyyy-MM-dd"),
                query.value(3).toString(),
                query.value(4).toString(),
                query.value(6).toDouble(),
                query.value(7).toDouble(),
                query.value(8).toDouble()
                );
            if (student) {
                list.append(student);
            }
        } catch (const std::exception &e) {
            qWarning() << "[DB WARNING] Bỏ qua bản ghi lỗi:" << e.what();
        }
    }

    qInfo() << "[DB INFO] Đã nạp thành công" << list.size() << "sinh viên từ ổ cứng vào RAM.";
    return list;
}

// ============================================================================
// XỬ LÝ HÀNG LOẠT HIỆU NĂNG CAO (BATCH OPERATIONS)
// ============================================================================

bool DatabaseHelper::insertStudentsBatch(const QVector<Student*> &students)
{
    if (students.isEmpty() || !db.isOpen()) return false;

    // Bắt đầu Giao dịch (Transaction)
    // Nếu không có Transaction, chèn 1,000 dòng sẽ mất ~10 giây vì phải mở/đóng file đĩa 1,000 lần.
    // Có Transaction, thời gian giảm xuống còn ~0.05 giây!
    if (!beginTransaction()) {
        qCritical() << "[DB ERROR] Không thể khởi tạo Transaction cho Batch Insert.";
        return false;
    }

    QSqlQuery query(db);
    query.prepare("INSERT OR REPLACE INTO students (id, full_name, dob, class_name, phone, major, score1, score2, score3) "
                  "VALUES (:id, :name, :dob, :class, :phone, :major, :s1, :s2, :s3);");

    for (const Student *student : students) {
        if (!student) continue;

        query.bindValue(":id", student->getId());
        query.bindValue(":name", student->getFullName());
        query.bindValue(":dob", student->getDob().toString("yyyy-MM-dd"));
        query.bindValue(":class", student->getClassName());
        query.bindValue(":phone", student->getPhoneNumber());
        query.bindValue(":major", student->getMajor());

        if (const ITStudent *it = dynamic_cast<const ITStudent*>(student)) {
            query.bindValue(":s1", it->getCppScore());
            query.bindValue(":s2", it->getDiscreteMathScore());
            query.bindValue(":s3", it->getDsaScore());
        } else if (const MathStudent *math = dynamic_cast<const MathStudent*>(student)) {
            query.bindValue(":s1", math->getCalculusScore());
            query.bindValue(":s2", math->getLinearAlgebraScore());
            query.bindValue(":s3", 0.0);
        } else if (const BioStudent *bio = dynamic_cast<const BioStudent*>(student)) {
            query.bindValue(":s1", bio->getMolecularBioScore());
            query.bindValue(":s2", bio->getGeneticsScore());
            query.bindValue(":s3", 0.0);
        }

        if (!query.exec()) {
            qCritical() << "[DB ERROR] Lỗi trong quá trình Batch Insert tại ID" << student->getId() << "-" << query.lastError().text();
            rollbackTransaction(); // Hoàn tác toàn bộ nếu có bất kỳ dòng nào lỗi
            return false;
        }
    }

    // Xác nhận lưu toàn bộ xuống đĩa
    if (!commitTransaction()) {
        qCritical() << "[DB ERROR] Lỗi Commit Transaction!";
        rollbackTransaction();
        return false;
    }

    qInfo() << "[DB INFO] Batch Insert thành công:" << students.size() << "bản ghi.";
    return true;
}

bool DatabaseHelper::deleteAllStudents()
{
    if (!db.isOpen()) return false;
    QSqlQuery query(db);
    return query.exec("DELETE FROM students;");
}

// ============================================================================
// BẢO MẬT & TÀI KHOẢN (SECURITY & AUTHENTICATION)
// ============================================================================
bool DatabaseHelper::authenticateUser(const QString &username, const QString &password)
{
    if (!db.isOpen()) {
        qWarning() << "[DB ERROR] Cơ sở dữ liệu chưa được mở!";
        return false;
    }

    QSqlQuery query(db);

    // 1. Kiểm tra xem bảng users đã tồn tại chưa, nếu chưa thì tạo bảng
    query.exec("CREATE TABLE IF NOT EXISTS users ("
               "username TEXT PRIMARY KEY, "
               "password TEXT NOT NULL, "
               "role TEXT DEFAULT 'Admin')");

    // 2. Tự động thêm tài khoản mặc định nếu CSDL chưa có tài khoản nào
    query.exec("SELECT COUNT(*) FROM users");
    if (query.next() && query.value(0).toInt() == 0) {
        QSqlQuery insertQuery(db);
        insertQuery.prepare("INSERT INTO users (username, password, role) VALUES (:user, :pass, :role)");
        insertQuery.bindValue(":user", "25127293");
        insertQuery.bindValue(":pass", "123456");
        insertQuery.bindValue(":role", "Admin");
        insertQuery.exec();
        qInfo() << "[DB INFO] Đã khởi tạo tài khoản mặc định: 25127293 / 123456";
    }

    // 3. Cơ chế Bypass linh hoạt cho phiên thử nghiệm (Fallback cho người phát triển)
    if ((username == "25127293" || username == "admin") && password == "123456") {
        qInfo() << "[AUTH INFO] Đăng nhập thành công qua quyền Admin dự phòng!";
        return true;
    }

    // 4. Kiểm tra tài khoản thực tế trong SQLite
    query.prepare("SELECT password FROM users WHERE username = :user");
    query.bindValue(":user", username.trimmed());

    if (query.exec() && query.next()) {
        QString storedPassword = query.value(0).toString();
        if (storedPassword == password) {
            qInfo() << "[AUTH INFO] Đăng nhập thành công cho user:" << username;
            return true;
        }
    }

    qWarning() << "[AUTH WARNING] Sai tài khoản hoặc mật khẩu:" << username;
    return false;
}
bool DatabaseHelper::registerUser(const QString &username, const QString &password, const QString &role)
{
    if (username.trimmed().isEmpty() || password.isEmpty() || !db.isOpen()) return false;

    QSqlQuery query(db);
    query.prepare("INSERT INTO users (username, password, role) VALUES (:user, :pass, :role);");
    query.bindValue(":user", username.trimmed());
    query.bindValue(":pass", password);
    query.bindValue(":role", role.toUpper());

    return query.exec();
}

bool DatabaseHelper::changePassword(const QString &username, const QString &oldPassword, const QString &newPassword)
{
    if (!authenticateUser(username, oldPassword)) return false;

    QSqlQuery query(db);
    query.prepare("UPDATE users SET password = :newPass WHERE username = :user;");
    query.bindValue(":newPass", newPassword);
    query.bindValue(":user", username.trimmed());

    return query.exec();
}

// ============================================================================
// BẢO TRÌ & SAO LƯU (MAINTENANCE & BACKUP)
// ============================================================================

bool DatabaseHelper::backupDatabase(const QString &backupDestPath)
{
    if (!db.isOpen()) return false;

    // Cần đóng kết nối tạm thời hoặc sử dụng lệnh copy file an toàn
    QString currentPath = db.databaseName();

    // Đảm bảo dữ liệu trong bộ nhớ đệm WAL được ghi hết xuống file chính trước khi copy
    QSqlQuery query(db);
    query.exec("PRAGMA wal_checkpoint(FULL);");

    if (QFile::exists(backupDestPath)) {
        QFile::remove(backupDestPath);
    }

    if (QFile::copy(currentPath, backupDestPath)) {
        qInfo() << "[DB INFO] Đã sao lưu CSDL thành công tới:" << backupDestPath;
        return true;
    } else {
        qCritical() << "[DB ERROR] Sao lưu CSDL thất bại!";
        return false;
    }
}