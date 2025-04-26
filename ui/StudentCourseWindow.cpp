#include <QVBoxLayout>
#include <QSqlQuery>
#include <QSqlError>
#include <QInputDialog>
#include <QDebug>

#include "StudentCourseWindow.h"

StudentCourseWindow::StudentCourseWindow(QWidget *parent) : QWidget(parent) {
    setupUI();
}

StudentCourseWindow::~StudentCourseWindow() {
    delete model;
    delete tableView;
    delete classBox;
    delete studentBox;
}

void StudentCourseWindow::setupUI() {
    auto *mainLayout = new QVBoxLayout();
    auto *buttonLayout = new QHBoxLayout();

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

    addButton = new QPushButton("添加选课", this);
    deleteButton = new QPushButton("删除选课", this);

    addButton->setAttribute(Qt::WA_LayoutUsesWidgetRect);
    deleteButton->setAttribute(Qt::WA_LayoutUsesWidgetRect);

    model = new QSqlQueryModel(this);
    tableView->setModel(model);

    mainLayout->addWidget(classBox);
    mainLayout->addWidget(studentBox);
    mainLayout->addWidget(tableView);
    buttonLayout->addWidget(addButton);
    buttonLayout->addWidget(deleteButton);
    mainLayout->addLayout(buttonLayout);

    this->setLayout(mainLayout);

    connect(classBox, &QComboBox::currentTextChanged, this, [this, classStudent](const QString &className) {
        studentBox->clear();
        for (const auto &pair : classStudent[className]) {
            studentBox->addItem(pair.second);
        }
        showSelections();
    });

    // 初始界面展示表格
    for (const auto &pair : classStudent[classBox->currentText()]) {
        if (pair.second == studentBox->currentText()) {
            currentStudentId = pair.first;
            showSelections();
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
        showSelections();
    });

    connect(addButton, &QPushButton::clicked, this, &StudentCourseWindow::addSelection);

    connect(deleteButton, &QPushButton::clicked, this, &StudentCourseWindow::deleteSelection);
}

void StudentCourseWindow::showSelections() const {
    QSqlQuery query;
    query.prepare("SELECT grades.course_id AS 课程号, courses.name AS 课程, credit AS 学分, "
                  "teachers.name AS 教师, department AS 部门 FROM grades "
                  "INNER JOIN courses ON courses.course_id = grades.course_id "
                  "INNER JOIN teachers ON teachers.teacher_id = courses.teacher_id "
                  "WHERE student_id = :student_id");
    query.bindValue(":student_id", currentStudentId);

    if (!query.exec()) {
        qDebug() << "Query selections failed: " << query.lastError().text();
        return;
    }

    model->setQuery(query);
}

void StudentCourseWindow::addSelection(){
    QSqlQuery query;

    const QString name = QInputDialog::getText(this, "添加选课", "输入课程名:");
    if (name.isEmpty()) return;

    const QString teacher = QInputDialog::getText(this, "添加选课", "输入教师名:");
    if (teacher.isEmpty()) return;

    query.prepare("SELECT course_id FROM courses INNER JOIN teachers ON courses.teacher_id = teachers.teacher_id "
                  "WHERE courses.name = :name AND teachers.name = :teacher");
    query.bindValue(":name", name);
    query.bindValue(":teacher", teacher);
    if (!query.exec()) {
        qDebug() << "Query course_id failed: " << query.lastError().text();
        return;
    }
    int courseId = 0;
    if (query.next()) {
        courseId = query.value(0).toInt();
    } else {
        qDebug() << "Course not found";
        return;
    }

    query.prepare("INSERT INTO grades (student_id, course_id) VALUES (:student_id, :course_id)");
    query.bindValue(":student_id", currentStudentId);
    query.bindValue(":course_id", courseId);

    if (!query.exec()) {
        qDebug() << "Add selection failed: " << query.lastError().text();
    } else {
        showSelections();
    }

    emit databaseChanged();
}

void StudentCourseWindow::deleteSelection(){
    const int courseId = QInputDialog::getInt(this, "删除选课", "输入课程号:");
    if (courseId <= 0) return;

    QSqlQuery query;
    query.prepare("DELETE FROM grades WHERE student_id = :student_id AND course_id = :course_id AND score IS NULL");
    query.bindValue(":student_id", currentStudentId);
    query.bindValue(":course_id", courseId);

    if (!query.exec()) {
        qDebug() << "Delete selection failed: " << query.lastError().text();
    } else {
        showSelections();
    }

    emit databaseChanged();
}
