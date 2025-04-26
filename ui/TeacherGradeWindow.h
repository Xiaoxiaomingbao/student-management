#ifndef TEACHERGRADEWINDOW_H
#define TEACHERGRADEWINDOW_H

#include <QTableView>
#include <QSqlTableModel>
#include <QPushButton>
#include <QComboBox>

class TeacherGradeWindow final : public QWidget {
    Q_OBJECT

public:
    explicit TeacherGradeWindow(QWidget *parent = nullptr);

    ~TeacherGradeWindow() override;

    void refresh() const;

private:
    QSqlQueryModel *model{};
    QTableView *tableView{};

    QComboBox *deptBox;
    QComboBox *teacherBox;

    QPushButton *addButton{};

    int currentTeacherId;

    void setupUI();

signals:
    void databaseChanged();

private slots:
    void showGrades() const;
    void addGrade();

};



#endif //TEACHERGRADEWINDOW_H
