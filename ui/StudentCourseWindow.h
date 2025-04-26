#ifndef STUDENTCOURSEWINDOW_H
#define STUDENTCOURSEWINDOW_H

#include <QTableView>
#include <QPushButton>
#include <QSqlTableModel>
#include <QComboBox>

class StudentCourseWindow final : public QWidget {
    Q_OBJECT

public:
    explicit StudentCourseWindow(QWidget *parent = nullptr);

    ~StudentCourseWindow() override;

private:
    QSqlQueryModel *model{};
    QTableView *tableView{};

    QComboBox *classBox;
    QComboBox *studentBox;

    QPushButton *addButton{};
    QPushButton *deleteButton{};

    int currentStudentId;

    void setupUI();

signals:
    void databaseChanged();

private slots:
    void showSelections() const;
    void addSelection();
    void deleteSelection();
};



#endif //STUDENTCOURSEWINDOW_H
