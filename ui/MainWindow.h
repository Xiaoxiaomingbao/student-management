#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTreeWidget>
#include <QStackedWidget>

class MainWindow final : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

    private slots:
        void onItemClicked(const QTreeWidgetItem *item, int column) const;

private:
    QTreeWidget *treeWidget;     // 左侧分级菜单
    QStackedWidget *stackedWidget; // 右侧内容区域
};



#endif //MAINWINDOW_H
