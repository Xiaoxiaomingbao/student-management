#include <QHBoxLayout>

#include "MainWindow.h"

#include "ManagerCourseWindow.h"
#include "ManagerStudentWindow.h"
#include "ManagerTeacherWindow.h"
#include "StudentCourseWindow.h"
#include "StudentGradeWindow.h"
#include "TeacherCourseWindow.h"
#include "TeacherGradeWindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    // 设置主窗口
    setWindowTitle("教学管理系统");
    resize(800, 600);

    // 中心窗口
    const auto centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    // 创建左侧树形菜单
    treeWidget = new QTreeWidget(this);
    treeWidget->setHeaderLabel("功能列表");

    // 添加分级菜单项
    const auto item1 = new QTreeWidgetItem(treeWidget, QStringList("管理员界面"));
    auto subItem1 = new QTreeWidgetItem(item1, QStringList("学生信息管理"));
    auto subItem2 = new QTreeWidgetItem(item1, QStringList("教师信息管理"));
    auto subItem3 = new QTreeWidgetItem(item1, QStringList("课程信息管理"));

    const auto item2 = new QTreeWidgetItem(treeWidget, QStringList("学生界面"));
    auto subItem4 = new QTreeWidgetItem(item2, QStringList("选择课程"));
    auto subItem5 = new QTreeWidgetItem(item2, QStringList("查看成绩"));

    const auto item3 = new QTreeWidgetItem(treeWidget, QStringList("教师界面"));
    auto subItem6 = new QTreeWidgetItem(item3, QStringList("查看课程"));
    auto subItem7 = new QTreeWidgetItem(item3, QStringList("管理成绩"));

    treeWidget->expandAll(); // 展开所有分类

    // 右侧堆叠窗口
    stackedWidget = new QStackedWidget(this);

    stackedWidget->addWidget(new ManagerStudentWindow(this)); // 0
    stackedWidget->addWidget(new ManagerTeacherWindow(this)); // 1
    stackedWidget->addWidget(new ManagerCourseWindow(this)); // 2
    stackedWidget->addWidget(new StudentCourseWindow(this)); // 3
    stackedWidget->addWidget(new StudentGradeWindow(this)); // 4
    stackedWidget->addWidget(new TeacherCourseWindow(this)); // 5
    stackedWidget->addWidget(new TeacherGradeWindow(this)); // 6

    // 布局
    const auto layout = new QHBoxLayout;
    layout->addWidget(treeWidget, 1);  // 左侧占比 1
    layout->addWidget(stackedWidget, 3); // 右侧占比 3
    centralWidget->setLayout(layout);

    // 绑定点击信号
    connect(treeWidget, &QTreeWidget::itemClicked, this, &MainWindow::onItemClicked);
}

MainWindow::~MainWindow() {}

void MainWindow::onItemClicked(const QTreeWidgetItem *item, const int column) const {
    // 获取选中的文本
    if (const QString text = item->text(column); text == "学生信息管理") {
        stackedWidget->setCurrentIndex(0);
    } else if (text == "教师信息管理") {
        stackedWidget->setCurrentIndex(1);
    } else if (text == "课程信息管理") {
        stackedWidget->setCurrentIndex(2);
    } else if (text == "选择课程") {
        stackedWidget->setCurrentIndex(3);
    } else if (text == "查看成绩") {
        stackedWidget->setCurrentIndex(4);
    } else if (text == "查看课程") {
        stackedWidget->setCurrentIndex(5);
    } else if (text == "管理成绩") {
        stackedWidget->setCurrentIndex(6);
    }
}
