#ifndef STUDENTGRADEWINDOW_H
#define STUDENTGRADEWINDOW_H

#include <QTableView>
#include <QSqlTableModel>
#include <QComboBox>

class StudentGradeWindow final : public QWidget {
    Q_OBJECT

public:
    explicit StudentGradeWindow(QWidget *parent = nullptr);

    ~StudentGradeWindow() override;

    void refresh() const;

private:
    QSqlQueryModel *model{};
    QTableView *tableView{};

    QComboBox *classBox;
    QComboBox *studentBox;

    QLineEdit *avgLineEdit;
    QLineEdit *weightedAvgLineEdit;

    int currentStudentId;

    void setupUI();

private slots:
    void showGrades() const;
    void showAverage() const;

};



#endif //STUDENTGRADEWINDOW_H
