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

private:
    QSqlQueryModel *model{};
    QTableView *tableView{};

    QComboBox *deptBox;
    QComboBox *teacherBox;

    QPushButton *commitButton{};

    int currentTeacherId;

    void setupUI();

private slots:
    void showGrades() const;
    void showAverageAndRate() const;
    void commitGrades();

};



#endif //TEACHERGRADEWINDOW_H
