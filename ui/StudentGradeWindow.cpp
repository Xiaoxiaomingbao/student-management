#include <QVBoxLayout>
#include <QSqlQuery>
#include <QSqlError>
#include <QLineEdit>
#include <QDebug>

#include "StudentGradeWindow.h"

StudentGradeWindow::StudentGradeWindow(QWidget *parent) : QWidget(parent) {
    setupUI();
}

StudentGradeWindow::~StudentGradeWindow() {
    delete model;
    delete tableView;
    delete classBox;
    delete studentBox;
    delete avgLineEdit;
    delete weightedAvgLineEdit;
}

void StudentGradeWindow::setupUI() {
    auto *mainLayout = new QVBoxLayout();

    tableView = new QTableView(this);

    QMap<QString, QList<QPair<int, QString>>> classStudent;
    QSqlQuery query;
    query.prepare("SELECT student_id, name, class_name FROM students");
    if (!query.exec()) {
        qDebug() << "Query failed: " << query.lastError().text();
        return;
    }
    while (query.next()) {
        const int studentId = query.value(0).toInt();
        const QString studentName = query.value(1).toString();
        const QString className = query.value(2).toString();
        classStudent[className].append({studentId, studentName});
    }

    classBox = new QComboBox(this);
    classBox->setEditable(false);
    classBox->addItems(classStudent.keys());

    studentBox = new QComboBox(this);
    studentBox->setEditable(false);
    for (const auto &pair : classStudent[classBox->currentText()]) {
        studentBox->addItem(pair.second);
    }

    model = new QSqlQueryModel(this);
    tableView->setModel(model);

    avgLineEdit = new QLineEdit(this);
    avgLineEdit->setReadOnly(true);
    avgLineEdit->setPlaceholderText("算数平均分");

    weightedAvgLineEdit = new QLineEdit(this);
    weightedAvgLineEdit->setReadOnly(true);
    weightedAvgLineEdit->setPlaceholderText("加权平均分");

    mainLayout->addWidget(classBox);
    mainLayout->addWidget(studentBox);
    mainLayout->addWidget(tableView);
    mainLayout->addWidget(avgLineEdit);
    mainLayout->addWidget(weightedAvgLineEdit);

    this->setLayout(mainLayout);

    connect(classBox, &QComboBox::currentTextChanged, this, [this, classStudent](const QString &className) {
        studentBox->clear();
        for (const auto &pair : classStudent[className]) {
            studentBox->addItem(pair.second);
        }
        showGrades();
    });

    // 初始界面展示表格
    for (const auto &pair : classStudent[classBox->currentText()]) {
        if (pair.second == studentBox->currentText()) {
            currentStudentId = pair.first;
            showGrades();
            break;
        }
    }

    connect(studentBox, &QComboBox::currentTextChanged, this, [this, classStudent](const QString &studentName) {
        for (const auto &pair : classStudent[classBox->currentText()]) {
            if (pair.second == studentName) {
                currentStudentId = pair.first;
                break;
            }
        }
        showGrades();
    });

    showAverage();
}

void StudentGradeWindow::showGrades() const {
    QSqlQuery query;
    query.prepare("SELECT student_id AS 学号, courses.name AS 课程, IFNULL(score, '暂无') AS 分数, "
                  "credit AS 学分, department AS 部门, teachers.name AS 教师 FROM grades "
                  "INNER JOIN courses ON courses.course_id = grades.course_id "
                  "INNER JOIN teachers ON teachers.teacher_id = courses.teacher_id "
                  "WHERE student_id = :student_id");
    query.bindValue(":student_id", currentStudentId);

    if (!query.exec()) {
        qDebug() << "Query grades failed: " << query.lastError().text();
        return;
    }

    model->setQuery(query);
}

void StudentGradeWindow::showAverage() const {
    QSqlQuery query;
    query.prepare("SELECT AVG(score), SUM(score * credit) / SUM(credit) FROM grades "
                  "INNER JOIN courses ON courses.course_id = grades.course_id "
                  "WHERE student_id = :student_id AND score IS NOT NULL");
    query.bindValue(":student_id", currentStudentId);

    if (!query.exec()) {
        qDebug() << "Query average failed: " << query.lastError().text();
        return;
    }

    if (query.next()) {
        const QVariant avg = query.value(0);
        const QVariant weightedAvg = query.value(1);

        avgLineEdit->setText(avg.isNull() ? "暂无" : QString::number(avg.toDouble(), 'f', 2));
        weightedAvgLineEdit->setText(weightedAvg.isNull() ? "暂无" : QString::number(weightedAvg.toDouble(), 'f', 2));
    }
}