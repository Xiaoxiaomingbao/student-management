#ifndef MANAGERCOURSEWINDOW_H
#define MANAGERCOURSEWINDOW_H

#include <QTableView>
#include <QPushButton>
#include <QSqlTableModel>

class ManagerCourseWindow final : public QWidget {
    Q_OBJECT

public:
    explicit ManagerCourseWindow(QWidget *parent = nullptr);
    ~ManagerCourseWindow() override;

private slots:
    void addCourse();
    void deleteCourse();

private:
    QSqlTableModel *model{};
    QTableView *tableView{};
    QPushButton *addButton{};
    QPushButton *deleteButton{};

    // 教师 ID -> 教师姓名
    QMap<int, QString> teacher_map;

    void setupUI();
    void setupDatabase();
};


#endif //MANAGERCOURSEWINDOW_H
