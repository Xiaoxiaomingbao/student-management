#ifndef MANAGERSTUDENTWINDOW_H
#define MANAGERSTUDENTWINDOW_H

#include <QTableView>
#include <QPushButton>
#include <QSqlTableModel>


class ManagerStudentWindow final : public QWidget {
    Q_OBJECT

public:
    explicit ManagerStudentWindow(QWidget *parent = nullptr);
    ~ManagerStudentWindow() override;

private slots:
    void addStudent();
    void deleteStudent() const;
    void updateStudent();

private:
    QSqlTableModel *model{};
    QTableView *tableView{};
    QPushButton *addButton{};
    QPushButton *deleteButton{};
    QPushButton *updateButton{};

    void setupUI();
    void setupDatabase();
};



#endif //MANAGERSTUDENTWINDOW_H
