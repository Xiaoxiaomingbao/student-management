#include <QVBoxLayout>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

#include "TeacherCourseWindow.h"

TeacherCourseWindow::TeacherCourseWindow(QWidget *parent) : QWidget(parent) {
    setupUI();
}

TeacherCourseWindow::~TeacherCourseWindow() {
    delete model;
    delete tableView;
    delete deptBox;
    delete teacherBox;
}

void TeacherCourseWindow::setupUI() {
    // 创建布局
    const auto layout = new QVBoxLayout(this);

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
    connect(deptBox, &QComboBox::currentTextChanged, this, [this]() {
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
            updateTableView(pair.first);
            break;
        }
    }

    // 绑定信号槽：用户选择教师下拉框时更新表格视图
    connect(teacherBox, &QComboBox::currentTextChanged, this, [this]() {
    for (const auto &pair : deptTeacher[deptBox->currentText()]) {
        if (pair.second == teacherBox->currentText()) {
            updateTableView(pair.first);
            break;
        }
    }
    });
}

void TeacherCourseWindow::updateTableView(const int teacherId) const {
    QSqlQuery query;
    query.prepare("SELECT course_id AS 课程号, name AS 课程, credit AS 学分 FROM courses WHERE teacher_id = :teacher_id");
    query.bindValue(":teacher_id", teacherId);
    query.exec();

    model->setQuery(query);
}
