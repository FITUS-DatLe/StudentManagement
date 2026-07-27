#include "studentmanager.h"
#include "studentfactory.h"
#include <QFile>
#include <QTextStream>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QDebug>
#include <utility> // Bổ sung để dùng std::as_const

// ============================================================================
// KHỞI TẠO & SINGLETON IMPLEMENTATION
// ============================================================================

StudentManager::StudentManager(QObject *parent)
    : QObject(parent), mainGroup("HCMUS_MAIN_GROUP", "HCMUS_ALL"), isInitialized(false)
{
}

StudentManager::~StudentManager()
{
    mainGroup.clearAll();
}

StudentManager& StudentManager::getInstance()
{
    static StudentManager instance;
    return instance;
}

bool StudentManager::initManager()
{
    if (isInitialized) return true;

    qInfo() << "[MANAGER INFO] Đang khởi tạo Trình quản lý sinh viên...";

    if (!DatabaseHelper::getInstance().initDatabase()) {
        emit operationError("Không thể kết nối đến Cơ sở dữ liệu SQLite!");
        return false;
    }

    reloadFromDatabase();
    isInitialized = true;
    qInfo() << "[MANAGER INFO] Trình quản lý đã sẵn sàng hoạt động.";
    return true;
}

void StudentManager::reloadFromDatabase()
{
    mainGroup.clearAll();
    const QVector<Student*> dbStudents = DatabaseHelper::getInstance().loadAllStudents();

    // Sử dụng std::as_const để tránh clazy-range-loop-detach (Dòng 55)
    for (Student *s : std::as_const(dbStudents)) {
        try {
            mainGroup.addStudent(s);
        } catch (const std::exception &e) {
            qWarning() << "[MANAGER WARNING] Lỗi nạp sinh viên vào RAM:" << e.what();
            delete s;
        }
    }

    emit dataChanged();
}

bool StudentManager::isReady() const
{
    return isInitialized;
}

// ============================================================================
// CÁC THAO TÁC NGHIỆP VỤ CHÍNH (CRUD)
// ============================================================================

bool StudentManager::addStudent(Student *student)
{
    if (!student) {
        emit operationError("Dữ liệu sinh viên không hợp lệ (Null Pointer)!");
        return false;
    }

    if (mainGroup.findStudentById(student->getId()) != nullptr) {
        QString err = "Mã số sinh viên [" + student->getId() + "] đã tồn tại trong hệ thống!";
        qWarning() << "[MANAGER ERROR]" << err;
        emit operationError(err);
        return false;
    }

    if (!DatabaseHelper::getInstance().insertStudent(student)) {
        emit operationError("Lỗi ghi dữ liệu vào ổ cứng SQLite!");
        return false;
    }

    try {
        mainGroup.addStudent(student);
        qInfo() << "[MANAGER INFO] Đã thêm thành công sinh viên:" << student->getFullName();
        emit studentAdded(student->getId());
        emit dataChanged();
        return true;
    } catch (const std::exception &e) {
        emit operationError(QString("Lỗi RAM: ") + e.what());
        return false;
    }
}

bool StudentManager::updateStudent(Student *student)
{
    if (!student) {
        emit operationError("Dữ liệu cập nhật không hợp lệ!");
        return false;
    }

    Student *existingStudent = mainGroup.findStudentById(student->getId());
    if (!existingStudent) {
        emit operationError("Không tìm thấy sinh viên có ID [" + student->getId() + "] để cập nhật!");
        return false;
    }

    if (!DatabaseHelper::getInstance().updateStudent(student)) {
        emit operationError("Lỗi cập nhật xuống cơ sở dữ liệu SQLite!");
        return false;
    }

    if (existingStudent != student) {
        mainGroup.removeStudentById(student->getId());
        try {
            mainGroup.addStudent(student);
        } catch (const std::exception &e) {
            qCritical() << "[MANAGER CRITICAL] Lỗi thay thế con trỏ RAM:" << e.what();
            return false;
        }
    }

    qInfo() << "[MANAGER INFO] Đã cập nhật sinh viên ID:" << student->getId();
    emit studentUpdated(student->getId());
    emit dataChanged();
    return true;
}

bool StudentManager::deleteStudent(const QString &studentId)
{
    QString targetId = studentId.trimmed();
    if (targetId.isEmpty()) return false;

    if (!DatabaseHelper::getInstance().deleteStudent(targetId)) {
        emit operationError("Không thể xóa bản ghi khỏi ổ cứng SQLite!");
        return false;
    }

    if (mainGroup.removeStudentById(targetId)) {
        qInfo() << "[MANAGER INFO] Đã xóa sinh viên ID:" << targetId;
        emit studentDeleted(targetId);
        emit dataChanged();
        return true;
    } else {
        qWarning() << "[MANAGER WARNING] Đã xóa dưới DB nhưng không tìm thấy trên RAM ID:" << targetId;
        return false;
    }
}

Student* StudentManager::getStudentById(const QString &studentId) const
{
    return mainGroup.findStudentById(studentId);
}

QVector<Student*> StudentManager::getAllStudents() const
{
    return mainGroup.getAllStudents();
}

// ============================================================================
// TÌM KIẾM & LỌC DỮ LIỆU
// ============================================================================

QVector<Student*> StudentManager::searchStudents(const QString &keyword) const
{
    QString cleanKey = keyword.trimmed().toLower();
    if (cleanKey.isEmpty()) return getAllStudents();

    QVector<Student*> result;
    const QVector<Student*> all = mainGroup.getAllStudents();

    // Dùng std::as_const cho vòng lặp tìm kiếm (Dòng 312)
    for (Student *s : std::as_const(all)) {
        if (s->getId().toLower().contains(cleanKey) || s->getFullName().toLower().contains(cleanKey)) {
            result.append(s);
        }
    }
    return result;
}

QVector<Student*> StudentManager::filterByMajor(const QString &major) const
{
    if (major.contains("Tất cả", Qt::CaseInsensitive)) return getAllStudents();
    return mainGroup.getStudentsByMajor(major);
}

QVector<Student*> StudentManager::filterScholarshipCandidates() const
{
    return mainGroup.getScholarshipCandidates();
}

QVector<Student*> StudentManager::getTopPerformers(int count) const
{
    return mainGroup.getTopPerformers(count);
}

// ============================================================================
// SẮP XẾP DANH SÁCH
// ============================================================================

void StudentManager::sortListByGpaDesc()
{
    mainGroup.sortByGpaDescending();
    emit dataChanged();
}

void StudentManager::sortListByIdAsc()
{
    mainGroup.sortByIdAscending();
    emit dataChanged();
}

void StudentManager::sortListByNameAsc()
{
    mainGroup.sortByNameAscending();
    emit dataChanged();
}

// ============================================================================
// THỐNG KÊ HỌC THUẬT (STATISTICS)
// ============================================================================

int StudentManager::getTotalCount() const { return mainGroup.getTotalStudentCount(); }
double StudentManager::getAverageGpa() const { return mainGroup.calculateAverageGpa(); }
int StudentManager::getScholarshipCount() const { return mainGroup.getScholarshipCount(); }
QMap<QString, int> StudentManager::getMajorDistribution() const { return mainGroup.countStudentsByMajor(); }
QMap<QString, int> StudentManager::getStandingDistribution() const { return mainGroup.getAcademicStandingDistribution(); }

// ============================================================================
// NHẬP / XUẤT DỮ LIỆU (IMPORT / EXPORT)
// ============================================================================

bool StudentManager::exportToCsv(const QString &filePath) const
{
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        emit const_cast<StudentManager*>(this)->operationError("Không thể tạo file CSV tại: " + filePath);
        return false;
    }

    QTextStream out(&file);
    out.setEncoding(QStringConverter::Utf8);
    out << mainGroup.toCsvString();
    file.close();

    qInfo() << "[MANAGER INFO] Đã xuất file CSV thành công:" << filePath;
    return true;
}

bool StudentManager::importFromCsv(const QString &filePath)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        emit operationError("Không thể mở file CSV để đọc: " + filePath);
        return false;
    }

    QTextStream in(&file);
    in.setEncoding(QStringConverter::Utf8);

    if (!in.atEnd()) {
        in.readLine();
    }

    QVector<Student*> batchList;
    int lineCount = 0;

    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();
        if (line.isEmpty()) continue;

        try {
            Student *s = StudentFactory::createFromCsvRow(line);
            if (s && mainGroup.findStudentById(s->getId()) == nullptr) {
                batchList.append(s);
                lineCount++;
            } else {
                delete s;
            }
        } catch (const std::exception &e) {
            qWarning() << "[MANAGER WARNING] Lỗi parse CSV dòng:" << line << "->" << e.what();
        }
    }
    file.close();

    if (batchList.isEmpty()) {
        emit operationError("Không có bản ghi hợp lệ nào được tìm thấy trong file CSV!");
        return false;
    }

    if (DatabaseHelper::getInstance().insertStudentsBatch(batchList)) {
        // Dùng std::as_const khi duyệt qua batchList vừa đọc (Dòng 362)
        for (Student *s : std::as_const(batchList)) {
            mainGroup.addStudent(s);
        }
        qInfo() << "[MANAGER INFO] Import thành công" << lineCount << "sinh viên từ CSV.";
        emit dataChanged();
        return true;
    } else {
        qDeleteAll(batchList);
        emit operationError("Lỗi ghi dữ liệu import xuống SQLite!");
        return false;
    }
}

bool StudentManager::exportToJson(const QString &filePath) const
{
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        emit const_cast<StudentManager*>(this)->operationError("Không thể tạo file JSON!");
        return false;
    }

    QJsonDocument doc(mainGroup.toJsonArray());
    file.write(doc.toJson(QJsonDocument::Indented));
    file.close();

    qInfo() << "[MANAGER INFO] Đã xuất file JSON thành công:" << filePath;
    return true;
}

bool StudentManager::importFromJson(const QString &filePath)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        emit operationError("Không thể mở file JSON để đọc!");
        return false;
    }

    QByteArray data = file.readAll();
    file.close();

    QJsonParseError parseError;
    QJsonDocument doc = QJsonDocument::fromJson(data, &parseError);
    if (parseError.error != QJsonParseError::NoError || !doc.isArray()) {
        emit operationError("Định dạng file JSON không hợp lệ: " + parseError.errorString());
        return false;
    }

    const QJsonArray array = doc.array();
    QVector<Student*> batchList;

    // Dùng std::as_const cho QJsonArray (Dòng 377)
    for (const QJsonValue &val : std::as_const(array)) {
        if (!val.isObject()) continue;
        try {
            Student *s = StudentFactory::createFromJson(val.toObject());
            if (s && mainGroup.findStudentById(s->getId()) == nullptr) {
                batchList.append(s);
            } else {
                delete s;
            }
        } catch (const std::exception &e) {
            qWarning() << "[MANAGER WARNING] Lỗi parse JSON item:" << e.what();
        }
    }

    if (DatabaseHelper::getInstance().insertStudentsBatch(batchList)) {
        for (Student *s : std::as_const(batchList)) {
            mainGroup.addStudent(s);
        }
        emit dataChanged();
        return true;
    } else {
        qDeleteAll(batchList);
        emit operationError("Lỗi lưu dữ liệu JSON vào SQLite!");
        return false;
    }
}