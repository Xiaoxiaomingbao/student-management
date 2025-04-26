#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTreeWidget>
#include <QStackedWidget>

#include "StudentCourseWindow.h"
#include "StudentGradeWindow.h"
#include "TeacherGradeWindow.h"

class MainWindow final : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private slots:
    void onItemClicked(const QTreeWidgetItem *item, int column);

private:
    QTreeWidget *treeWidget;     // 左侧分级菜单
    QStackedWidget *stackedWidget; // 右侧内容区域

    // 不同界面的关联数据动态调整
    StudentCourseWindow *scw;
    StudentGradeWindow *sgw;
    TeacherGradeWindow *tgw;
    bool sgwNeedRefresh;
    bool tgwNeedRefresh;
};



#endif //MAINWINDOW_H
