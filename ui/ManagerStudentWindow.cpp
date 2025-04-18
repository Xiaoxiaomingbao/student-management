#include <QSqlQuery>
#include <QSqlError>
#include <QInputDialog>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QComboBox>
#include <QHeaderView>
#include <QDebug>

#include "ManagerStudentWindow.h"
#include "SexSqlTableModel.h"

ManagerStudentWindow::ManagerStudentWindow(QWidget *parent) : QWidget(parent) {
    setupUI();
    setupDatabase();
}

ManagerStudentWindow::~ManagerStudentWindow() {
    delete model;
    delete tableView;
    delete addButton;
    delete deleteButton;
    delete updateButton;
}

void ManagerStudentWindow::setupUI() {
    auto *mainLayout = new QVBoxLayout();
    auto *buttonLayout = new QHBoxLayout();

    tableView = new QTableView(this);

    addButton = new QPushButton("添加学生", this);
    deleteButton = new QPushButton("删除学生", this);
    updateButton = new QPushButton("更新学生", this);

    addButton->setAttribute(Qt::WA_LayoutUsesWidgetRect);
    deleteButton->setAttribute(Qt::WA_LayoutUsesWidgetRect);
    updateButton->setAttribute(Qt::WA_LayoutUsesWidgetRect);

    mainLayout->addWidget(tableView);
    buttonLayout->addWidget(addButton);
    buttonLayout->addWidget(deleteButton);
    buttonLayout->addWidget(updateButton);
    mainLayout->addLayout(buttonLayout);

    connect(addButton, &QPushButton::clicked, this, &ManagerStudentWindow::addStudent);
    connect(deleteButton, &QPushButton::clicked, this, &ManagerStudentWindow::deleteStudent);
    connect(updateButton, &QPushButton::clicked, this, &ManagerStudentWindow::updateStudent);

    this->setLayout(mainLayout);
}

void ManagerStudentWindow::setupDatabase() {
    model = new SexSqlTableModel(2, this);
    model->setTable("students");

    // 设置表格的列名
    model->setHeaderData(0, Qt::Horizontal, "学号");
    model->setHeaderData(1, Qt::Horizontal, "姓名");
    model->setHeaderData(2, Qt::Horizontal, "性别");
    model->setHeaderData(3, Qt::Horizontal, "班级");

    model->select();

    tableView->setModel(model);

    // 不显示行号
    tableView->verticalHeader()->setVisible(false);
}

void ManagerStudentWindow::addStudent() {
    const QString name = QInputDialog::getText(this, "添加学生", "输入姓名:");
    if (name.isEmpty()) return;

    int gender = 0; // 默认值（0 = 女）
    {
        QMessageBox msgBox;
        msgBox.setWindowTitle("添加学生");
        msgBox.setText("选择性别:");
        QComboBox comboBox;
        comboBox.addItems({"女", "男"});  // 显示给用户
        msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
        msgBox.setDefaultButton(QMessageBox::Ok);

        auto *layout = dynamic_cast<QGridLayout *>(msgBox.layout());
        layout->addWidget(&comboBox, 1, 1);

        if (msgBox.exec() == QMessageBox::Ok) {
            gender = (comboBox.currentText() == "男") ? 1 : 0;  // 转换为 0 或 1
        } else {
            return; // 用户取消
        }
    }

    const QString className = QInputDialog::getText(this, "添加学生", "输入班级:");
    if (className.isEmpty()) return;

    QSqlQuery query;
    query.prepare("INSERT INTO students (name, gender, class_name) VALUES (:name, :gender, :class_name)");
    query.bindValue(":name", name);
    query.bindValue(":gender", gender);
    query.bindValue(":class_name", className);

    if (!query.exec()) {
        qDebug() << "Add student failed: " << query.lastError().text();
    } else {
        model->select();
    }
}

void ManagerStudentWindow::deleteStudent() {
    int studentId = 0;
    studentId = QInputDialog::getInt(this, "删除学生", "输入学号:");
    if (studentId == 0) return;

    QSqlQuery query;
    query.prepare("DELETE FROM students WHERE student_id = :student_id");
    query.bindValue(":student_id", studentId);

    if (!query.exec()) {
        qDebug() << "Delete student failed: " << query.lastError().text();
    } else {
        model->select();
    }
}

void ManagerStudentWindow::updateStudent() {
    int studentId = 0;
    studentId = QInputDialog::getInt(this, "更新学生", "输入学号:");
    if (studentId == 0) return;

    const QString newClassName = QInputDialog::getText(this, "更新学生", "输入新班级:");

    QSqlQuery query;
    query.prepare("UPDATE students SET class_name = :class_name WHERE student_id = :student_id");
    query.bindValue(":student_id", studentId);
    query.bindValue(":class_name", newClassName);

    if (!query.exec()) {
        qDebug() << "Update student failed: " << query.lastError().text();
    } else {
        model->select();
    }
}