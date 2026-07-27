#ifndef STUDENTDIALOG_H
#define STUDENTDIALOG_H

#include <QDialog>
#include <QString>
#include <QDate>
#include "student.h"

namespace Ui {
class StudentDialog;
}

/**
 * @brief Lớp StudentDialog - Form giao diện Thêm mới và Chỉnh sửa Sinh viên
 * Đóng vai trò View tương tác, hỗ trợ validation dữ liệu đầu vào và chuyển đổi giao diện nhập điểm động.
 */
class StudentDialog : public QDialog
{
    Q_OBJECT

public:
    // Khởi tạo Dialog với chế độ mặc định là Thêm mới (Add Mode)
    explicit StudentDialog(QWidget *parent = nullptr);
    ~StudentDialog() override;

    // Thiết lập dữ liệu sinh viên cần chỉnh sửa (Kích hoạt Edit Mode)
    void setStudentData(const Student *student);

    // Lấy con trỏ đối tượng Sinh viên đã khởi tạo thành công (Trả về nullptr nếu hủy)
    Student* getStudent() const;

private slots:
    // Tự động chuyển đổi giao diện nhập điểm khi chọn ngành trong QComboBox
    void on_cboMajor_currentIndexChanged(int index);

    // Xử lý sự kiện bấm nút Lưu (Validate và khởi tạo đối tượng)
    void on_btnSave_clicked();

    // Xử lý sự kiện bấm nút Hủy
    void on_btnCancel_clicked();

private:
    Ui::StudentDialog *ui;
    Student *createdStudent; // Lưu trữ con trỏ đối tượng tạo bởi StudentFactory
    bool isEditMode;         // Cờ xác định trạng thái Add hay Edit

    // Các hàm trợ giúp kiểm tra tính hợp lệ dữ liệu đầu vào (Validation Helpers)
    bool validateInputs();
    bool parseScore(const QString &scoreStr, double &outScore, const QString &fieldName);
};

#endif // STUDENTDIALOG_H