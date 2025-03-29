#include <QApplication>
#include <QMessageBox>

#include "Database.h"
#include "ui/ManagerStudentWindow.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    const QMessageBox::StandardButton reply = QMessageBox::question(nullptr, "初始化数据库",
                                                              "是否需要创建数据库？（首次运行请选择“是”）",
                                                              QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::Yes) {
        if (Database::connect()) {
            Database::createTables();
        }
    } else {
        if (!Database::connect()) {
            return -1;
        }
    }

    ManagerStudentWindow window;
    window.setWindowTitle("学生管理系统");
    window.resize(600, 400);
    window.show();

    return QApplication::exec();
}
