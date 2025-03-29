#ifndef MANAGERTEACHERWINDOW_H
#define MANAGERTEACHERWINDOW_H

#include <QTableView>
#include <QPushButton>
#include <QSqlTableModel>

class ManagerTeacherWindow final : public QWidget {
    Q_OBJECT

public:
    explicit ManagerTeacherWindow(QWidget *parent = nullptr);
    ~ManagerTeacherWindow() override;

private slots:
    void addTeacher();
    void deleteTeacher();
    void updateTeacher();

private:
    QSqlTableModel *model{};
    QTableView *tableView{};
    QPushButton *addButton{};
    QPushButton *deleteButton{};
    QPushButton *updateButton{};

    void setupUI();
    void setupDatabase();
};



#endif //MANAGERTEACHERWINDOW_H
