#ifndef TEACHERCOURSEWINDOW_H
#define TEACHERCOURSEWINDOW_H

#include <QTableView>
#include <QPushButton>
#include <QSqlTableModel>
#include <QComboBox>

class TeacherCourseWindow final : public QWidget {
    Q_OBJECT

public:
    explicit TeacherCourseWindow(QWidget *parent = nullptr);

    ~TeacherCourseWindow() override;

private:
    QSqlQueryModel *model{};
    QTableView *tableView{};

    QComboBox *deptBox;
    QComboBox *teacherBox;

    // departmentName -> {(teacherId, teacherName), ...}
    QMap<QString, QList<QPair<int, QString>>> deptTeacher;

    void setupUI();

private slots:
    void updateTableView(int teacherId) const;
};



#endif //TEACHERCOURSEWINDOW_H
