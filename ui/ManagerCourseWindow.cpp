#include <QSqlQuery>
#include <QSqlError>
#include <QInputDialog>
#include <QVBoxLayout>
#include <QComboBox>
#include <QHeaderView>
#include <QDebug>

#include "ManagerCourseWindow.h"
#include "NameSqlTableModel.h"

ManagerCourseWindow::ManagerCourseWindow(QWidget *parent) : QWidget(parent) {
    setupUI();
    setupDatabase();
}

ManagerCourseWindow::~ManagerCourseWindow() {
    delete model;
    delete tableView;
    delete addButton;
    delete deleteButton;
}

void ManagerCourseWindow::setupUI() {
    auto *mainLayout = new QVBoxLayout();
    auto *buttonLayout = new QHBoxLayout();

    tableView = new QTableView(this);

    addButton = new QPushButton("添加课程", this);
    deleteButton = new QPushButton("删除课程", this);

    addButton->setAttribute(Qt::WA_LayoutUsesWidgetRect);
    deleteButton->setAttribute(Qt::WA_LayoutUsesWidgetRect);

    mainLayout->addWidget(tableView);
    buttonLayout->addWidget(addButton);
    buttonLayout->addWidget(deleteButton);
    mainLayout->addLayout(buttonLayout);

    connect(addButton, &QPushButton::clicked, this, &ManagerCourseWindow::addCourse);
    connect(deleteButton, &QPushButton::clicked, this, &ManagerCourseWindow::deleteCourse);

    this->setLayout(mainLayout);
}

void ManagerCourseWindow::setupDatabase() {
    QSqlQuery query;
    query.prepare("SELECT teacher_id, name FROM teachers");
    if (!query.exec()) {
        qDebug() << "Query teachers failed: " << query.lastError().text();
        return;
    }
    while (query.next()) {
        teacher_map.insert(query.value(0).toInt(), query.value(1).toString());
    }

    model = new NameSqlTableModel(teacher_map, 3, this);
    model->setTable("courses");

    // 设置表格的列名
    model->setHeaderData(0, Qt::Horizontal, "课程号");
    model->setHeaderData(1, Qt::Horizontal, "课程名");
    model->setHeaderData(2, Qt::Horizontal, "学分");
    model->setHeaderData(3, Qt::Horizontal, "教师");

    model->select();

    tableView->setModel(model);

    // 不显示行号
    tableView->verticalHeader()->setVisible(false);
}

void ManagerCourseWindow::addCourse() {
    const QString name = QInputDialog::getText(this, "添加课程", "输入课程名称:");
    if (name.isEmpty()) return;

    const int credit = QInputDialog::getInt(this, "添加课程", "输入学分:");
    if (credit <= 0) return;

    bool ok;
    const QStringList teacher_names = teacher_map.values();
    const QString selectedTeacherName = QInputDialog::getItem(
        this, "选择课程", "选择授课教师:", teacher_names, 0, false, &ok);
    if (!ok || selectedTeacherName.isEmpty()) return;

    const int teacher_id = teacher_map.key(selectedTeacherName);

    QSqlQuery query;
    query.prepare("INSERT INTO courses (name, credit, teacher_id) VALUES (:name, :credit, :teacher_id)");
    query.bindValue(":name", name);
    query.bindValue(":credit", credit);
    query.bindValue(":teacher_id", teacher_id);

    if (!query.exec()) {
        qDebug() << "Add course failed: " << query.lastError().text();
    } else {
        model->select();
    }
}

void ManagerCourseWindow::deleteCourse() {
    int courseId = 0;
    courseId = QInputDialog::getInt(this, "删除课程", "输入课程号:");
    if (courseId == 0) return;

    QSqlQuery query;
    query.prepare("DELETE FROM courses WHERE course_id = :course_id");
    query.bindValue(":course_id", courseId);

    if (!query.exec()) {
        qDebug() << "Delete course failed: " << query.lastError().text();
    } else {
        model->select();
    }
}
