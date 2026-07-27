#ifndef STUDENTPROFILEDIALOG_H
#define STUDENTPROFILEDIALOG_H

#include <QDialog>
#include <QString>

namespace Ui {
class StudentProfileDialog;
}

/**
 * @brief Lớp StudentProfileDialog - Quản lý Hồ sơ Sinh viên 360°
 * Tích hợp cơ chế Inline Toggle Editing và đồng bộ hóa RAM/SQLite vĩnh viễn.
 */
class StudentProfileDialog : public QDialog
{
    Q_OBJECT

public:
    explicit StudentProfileDialog(const QString &studentId, QWidget *parent = nullptr);
    ~StudentProfileDialog() override;

private slots:
    void on_btnChangeAvatar_clicked();
    void on_btnAddAchievement_clicked();
    void on_btnEditProfile_clicked(); // Slot xử lý bật/tắt chế độ chỉnh sửa & lưu đồng bộ
    void on_btnCloseProfile_clicked();

private:
    Ui::StudentProfileDialog *ui;
    QString currentStudentId;
    bool isEditMode; // Cờ theo dõi trạng thái xem hay đang chỉnh sửa

    void initDatabaseTables();
    void loadStudentData();
    void loadAcademicGrades();
    void loadTrainingPoints();
    void loadAchievements();
    void applyCustomStyles();
    void formatTableHeaders();

    // Bộ động cơ chỉnh sửa tại chỗ (Inline Editing Engine)
    void toggleEditMode(bool enable);
    void saveProfileChanges();
};

#endif // STUDENTPROFILEDIALOG_H