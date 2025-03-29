#include <QSqlQuery>
#include <QSqlError>
#include <QInputDialog>
#include <QVBoxLayout>
#include <QComboBox>
#include <QHeaderView>
#include <QDebug>

#include "ManagerTeacherWindow.h"

ManagerTeacherWindow::ManagerTeacherWindow(QWidget *parent) : QWidget(parent) {
    setupUI();
    setupDatabase();
}

ManagerTeacherWindow::~ManagerTeacherWindow() {
    delete model;
    delete tableView;
    delete addButton;
    delete deleteButton;
    delete updateButton;
}

void ManagerTeacherWindow::setupUI() {
    auto *mainLayout = new QVBoxLayout();
    auto *buttonLayout = new QHBoxLayout();

    tableView = new QTableView(this);

    addButton = new QPushButton("添加教师", this);
    deleteButton = new QPushButton("删除教师", this);
    updateButton = new QPushButton("更新教师", this);

    addButton->setAttribute(Qt::WA_LayoutUsesWidgetRect);
    deleteButton->setAttribute(Qt::WA_LayoutUsesWidgetRect);
    updateButton->setAttribute(Qt::WA_LayoutUsesWidgetRect);

    mainLayout->addWidget(tableView);
    buttonLayout->addWidget(addButton);
    buttonLayout->addWidget(deleteButton);
    buttonLayout->addWidget(updateButton);
    mainLayout->addLayout(buttonLayout);

    connect(addButton, &QPushButton::clicked, this, &ManagerTeacherWindow::addTeacher);
    connect(deleteButton, &QPushButton::clicked, this, &ManagerTeacherWindow::deleteTeacher);
    connect(updateButton, &QPushButton::clicked, this, &ManagerTeacherWindow::updateTeacher);

    this->setLayout(mainLayout);
}

void ManagerTeacherWindow::setupDatabase() {
    model = new QSqlTableModel(this);
    model->setTable("teachers");

    // 设置表格的列名
    model->setHeaderData(0, Qt::Horizontal, "教工号");
    model->setHeaderData(1, Qt::Horizontal, "姓名");
    model->setHeaderData(2, Qt::Horizontal, "部门");

    model->select();

    tableView->setModel(model);

    // 不显示行号
    tableView->verticalHeader()->setVisible(false);
}

void ManagerTeacherWindow::addTeacher() {
    const QString name = QInputDialog::getText(this, "添加教师", "输入姓名:");
    if (name.isEmpty()) return;

    const QString department = QInputDialog::getText(this, "添加教师", "输入部门:");
    if (department.isEmpty()) return;

    QSqlQuery query;
    query.prepare("INSERT INTO teachers (name, department) VALUES (:name, :department)");
    query.bindValue(":name", name);
    query.bindValue(":department", department);

    if (!query.exec()) {
        qDebug() << "Add teacher failed: " << query.lastError().text();
    } else {
        model->select();
    }
}

void ManagerTeacherWindow::deleteTeacher() {
    int teacherId = 0;
    teacherId = QInputDialog::getInt(this, "删除教师", "输入教工号:");
    if (teacherId == 0) return;

    QSqlQuery query;
    query.prepare("DELETE FROM teachers WHERE teacher_id = :teacher_id");
    query.bindValue(":teacher_id", teacherId);

    if (!query.exec()) {
        qDebug() << "Delete teacher failed: " << query.lastError().text();
    } else {
        model->select();
    }
}

void ManagerTeacherWindow::updateTeacher() {
    int teacherId = 0;
    teacherId = QInputDialog::getInt(this, "更新教师", "输入教工号:");
    if (teacherId == 0) return;

    const QString newDepartment = QInputDialog::getText(this, "更新教师", "输入新部门:");

    QSqlQuery query;
    query.prepare("UPDATE teachers SET department = :department WHERE teacher_id = :teacher_id");
    query.bindValue(":teacher_id", teacherId);
    query.bindValue(":department", newDepartment);

    if (!query.exec()) {
        qDebug() << "Update teacher failed: " << query.lastError().text();
    } else {
        model->select();
    }
}