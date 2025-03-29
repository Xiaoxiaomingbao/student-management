#ifndef STUDENTSQLTABLEMODEL_H
#define STUDENTSQLTABLEMODEL_H

#include <QSqlTableModel>
#include <QVariant>


class StudentSqlTableModel final : public QSqlTableModel {
public:
    explicit StudentSqlTableModel(QObject *parent = nullptr) : QSqlTableModel(parent) {}

    QVariant data(const QModelIndex &index, const int role = Qt::DisplayRole) const override {
        // 性别在第三列
        if (role == Qt::DisplayRole && index.column() == 2) {
            const int gender = QSqlTableModel::data(index, Qt::DisplayRole).toInt();
            return gender == 0 ? "女" : "男";
        }
        return QSqlTableModel::data(index, role);
    }
};



#endif //STUDENTSQLTABLEMODEL_H
