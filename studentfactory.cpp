#include "studentfactory.h"
#include <QRegularExpression>
#include <QStringList>
#include <QJsonValue>
#include <QDebug>

// ============================================================================
// HÀM HỖ TRỢ CHUẨN HÓA (STATIC HELPER)
// ============================================================================

QString StudentFactory::normalizeMajorType(const QString &rawMajor)
{
    QString clean = rawMajor.trimmed().toLower();

    // Gom nhóm các từ khóa đồng nghĩa của Ngành CNTT
    if (clean.contains("cntt") || clean == "it" || clean.contains("công nghệ thông tin") || clean.contains("information")) {
        return "IT";
    }
    // Gom nhóm từ khóa Ngành Toán Học
    if (clean.contains("toán") || clean.contains("math") || clean.contains("toán học")) {
        return "MATH";
    }
    // Gom nhóm từ khóa Ngành Sinh Học
    if (clean.contains("sinh") || clean.contains("bio") || clean.contains("sinh học")) {
        return "BIO";
    }
    return "UNKNOWN";
}

// ============================================================================
// 1. KHỞI TẠO TỪ THAM SỐ RỜI RẠC
// ============================================================================

Student* StudentFactory::createStudent(const QString &majorType,
                                       const QString &id,
                                       const QString &fullName,
                                       const QDate &dob,
                                       const QString &className,
                                       const QString &phone,
                                       double score1,
                                       double score2,
                                       double score3)
{
    QString type = normalizeMajorType(majorType);

    if (type == "IT") {
        // score1: C++, score2: Toán rời rạc, score3: Cấu trúc dữ liệu (DSA)
        return new ITStudent(id, fullName, dob, className, phone, score1, score2, score3);
    }
    else if (type == "MATH") {
        // score1: Giải tích, score2: Đại số tuyến tính (score3 bị bỏ qua)
        return new MathStudent(id, fullName, dob, className, phone, score1, score2);
    }
    else if (type == "BIO") {
        // score1: Sinh học phân tử, score2: Di truyền học (score3 bị bỏ qua)
        return new BioStudent(id, fullName, dob, className, phone, score1, score2);
    }

    throw std::invalid_argument("Lỗi Factory: Chuyên ngành [" + majorType.toStdString() + "] không được hệ thống hỗ trợ!");
}

// ============================================================================
// 2. KHỞI TẠO TỪ ĐỐI TƯỢNG JSON
// ============================================================================

Student* StudentFactory::createFromJson(const QJsonObject &json)
{
    // Trích xuất các trường định danh chung
    QString id = json["id"].toString();
    QString fullName = json["fullName"].toString();
    QDate dob = QDate::fromString(json["dob"].toString(), "yyyy-MM-dd");
    QString className = json["className"].toString();
    QString phone = json["phoneNumber"].toString();
    QString major = json["major"].toString();

    // Trích xuất khối điểm số chuyên ngành
    QJsonObject scores = json["academicPerformance"].toObject();
    QString type = normalizeMajorType(major);

    if (type == "IT") {
        double cpp = scores["cpp"].toDouble();
        double math = scores["discreteMath"].toDouble();
        double dsa = scores["dsa"].toDouble();
        return new ITStudent(id, fullName, dob, className, phone, cpp, math, dsa);
    }
    else if (type == "MATH") {
        double calculus = scores["calculus"].toDouble();
        double linear = scores["linearAlgebra"].toDouble();
        return new MathStudent(id, fullName, dob, className, phone, calculus, linear);
    }
    else if (type == "BIO") {
        double molBio = scores["molecularBio"].toDouble();
        double genetics = scores["genetics"].toDouble();
        return new BioStudent(id, fullName, dob, className, phone, molBio, genetics);
    }

    throw std::invalid_argument("Lỗi Factory (JSON): Ngành [" + major.toStdString() + "] không hợp lệ!");
}

// ============================================================================
// 3. KHỞI TẠO TỪ DÒNG DỮ LIỆU CSV
// ============================================================================

Student* StudentFactory::createFromCsvRow(const QString &csvRow)
{
    QString trimmedRow = csvRow.trimmed();
    if (trimmedRow.isEmpty()) {
        throw std::invalid_argument("Lỗi Factory (CSV): Dòng dữ liệu rỗng!");
    }

    // Tách chuỗi theo dấu phẩy, có xử lý ngoại lệ cho trường hợp chuỗi nằm trong dấu ngoặc kép
    // Cấu trúc chuẩn 11 cột: ID,FullName,DOB,Class,Phone,Major,Score1,Score2,Score3,GPA,Classification
    QStringList fields;
    QString currentField;
    bool inQuotes = false;

    for (int i = 0; i < trimmedRow.length(); ++i) {
        QChar c = trimmedRow[i];
        if (c == '"') {
            inQuotes = !inQuotes; // Đảo trạng thái khi gặp dấu ngoặc kép
        } else if (c == ',' && !inQuotes) {
            fields.append(currentField.trimmed());
            currentField.clear();
        } else {
            currentField.append(c);
        }
    }
    fields.append(currentField.trimmed()); // Thêm trường cuối cùng

    if (fields.size() < 8) {
        throw std::invalid_argument("Lỗi Factory (CSV): Dòng dữ liệu thiếu cột thông tin chuẩn!");
    }

    QString id = fields[0];
    QString fullName = fields[1];
    QDate dob = QDate::fromString(fields[2], "dd/MM/yyyy");
    if (!dob.isValid()) {
        dob = QDate::fromString(fields[2], "yyyy-MM-dd"); // Thử định dạng ISO nếu định dạng VN thất bại
    }
    QString className = fields[3];
    QString phone = fields[4];
    QString major = fields[5];

    double score1 = fields[6].toDouble();
    double score2 = fields[7].toDouble();
    double score3 = (fields.size() > 8) ? fields[8].toDouble() : 0.0;

    return createStudent(major, id, fullName, dob, className, phone, score1, score2, score3);
}

// ============================================================================
// TIỆN ÍCH HỆ THỐNG
// ============================================================================

QStringList StudentFactory::getSupportedMajors()
{
    return {
        "💻 Ngành Công Nghệ Thông Tin",
        "📐 Ngành Toán Học",
        "🧬 Ngành Sinh Học"
    };
}