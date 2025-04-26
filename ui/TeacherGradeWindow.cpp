#include <QVBoxLayout>
#include <QSqlQuery>
#include <QInputDialog>
#include <QSqlError>
#include <QDebug>

#include "TeacherGradeWindow.h"

TeacherGradeWindow::TeacherGradeWindow(QWidget *parent) : QWidget(parent) {
    setupUI();
}

TeacherGradeWindow::~TeacherGradeWindow() {
    delete model;
    delete tableView;
    delete deptBox;
    delete teacherBox;
    delete addButton;
}

void TeacherGradeWindow::setupUI() {
    // 创建布局
    const auto layout = new QVBoxLayout(this);

    QMap<QString, QList<QPair<int, QString>>> deptTeacher;

    // 准备数据
    QSqlQuery query;
    query.prepare("SELECT teacher_id, name, department FROM teachers");
    if (!query.exec()) {
        qDebug() << "Query failed: " << query.lastError().text();
        return;
    }
    while (query.next()) {
        const int teacherId = query.value(0).toInt();
        const QString teacherName = query.value(1).toString();
        const QString departmentName = query.value(2).toString();
        deptTeacher[departmentName].append({teacherId, teacherName});
    }

    // 创建部门下拉框
    deptBox = new QComboBox();
    deptBox->setEditable(false);
    deptBox->addItems(deptTeacher.keys());
    layout->addWidget(deptBox);

    // 创建教师下拉框
    teacherBox = new QComboBox();
    teacherBox->setEditable(false);
    for (const auto &pair : deptTeacher[deptBox->currentText()]) {
        teacherBox->addItem(pair.second);
    }
    layout->addWidget(teacherBox);

    // 绑定信号槽：用户选择部门时更新教师下拉框的内容
    connect(deptBox, &QComboBox::currentTextChanged, this, [this, deptTeacher]() {
        teacherBox->clear();
        for (const auto &pair : deptTeacher[deptBox->currentText()]) {
            teacherBox->addItem(pair.second);
        }
    });

    // 创建表格视图
    tableView = new QTableView(this);
    model = new QSqlQueryModel(this);
    tableView->setModel(model);
    layout->addWidget(tableView);

    for (const auto &pair : deptTeacher[deptBox->currentText()]) {
        if (pair.second == teacherBox->currentText()) {
            currentTeacherId = pair.first;
            showGrades();
            break;
        }
    }

    // 绑定信号槽：用户选择教师下拉框时更新表格视图
    connect(teacherBox, &QComboBox::currentTextChanged, this, [this, deptTeacher]() {
        for (const auto &pair: deptTeacher[deptBox->currentText()]) {
            if (pair.second == teacherBox->currentText()) {
                currentTeacherId = pair.first;
                break;
            }
        }
        showGrades();
    });

    // 创建“设置成绩”按钮
    addButton = new QPushButton("设置成绩", this);
    layout->addWidget(addButton);

    connect(addButton, &QPushButton::clicked, this, &TeacherGradeWindow::addGrade);
}

void TeacherGradeWindow::showGrades() const {
    QSqlQuery query;
    query.prepare("SELECT student_id AS 学号, courses.course_id AS 课程号, courses.name AS 课程, "
                  "IFNULL(score, '暂无') AS 分数, credit AS 学分 FROM grades "
                  "INNER JOIN courses ON courses.course_id = grades.course_id "
                  "INNER JOIN teachers ON teachers.teacher_id = courses.teacher_id "
                  "WHERE teachers.teacher_id = :teacher_id");
    query.bindValue(":teacher_id", currentTeacherId);

    if (!query.exec()) {
        qDebug() << "Query grades failed: " << query.lastError().text();
        return;
    }

    model->setQuery(query);
}

void TeacherGradeWindow::addGrade() {
    const int courseId = QInputDialog::getInt(this, "添加成绩", "输入课程号:");
    if (courseId <= 0) return;

    const int studentId = QInputDialog::getInt(this, "添加成绩", "输入学号:");
    if (studentId <= 0) return;

    const double score = QInputDialog::getDouble(this, "添加成绩", "输入成绩:");
    if (score < 0 || score > 100) return;

    QSqlQuery query;
    query.prepare("UPDATE grades SET score = :score "
              "WHERE student_id = :student_id AND course_id = :course_id");
    query.bindValue(":student_id", studentId);
    query.bindValue(":course_id", courseId);
    query.bindValue(":score", score);

    if (!query.exec()) {
        qDebug() << "Add course failed: " << query.lastError().text();
    } else {
        showGrades();
    }

    emit databaseChanged();
}

void TeacherGradeWindow::refresh() const {
    showGrades();
}

