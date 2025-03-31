#ifndef SEXSQLTABLEMODEL_H
#define SEXSQLTABLEMODEL_H

#include <QSqlTableModel>
#include <QVariant>


class SexSqlTableModel final : public QSqlTableModel {
    // 性别所在列的编号（从 0 开始计数）
    const int columnIndex;

public:
    explicit SexSqlTableModel(const int columnIndex, QObject *parent = nullptr) : QSqlTableModel(parent),
        columnIndex(columnIndex) {
    }

    QVariant data(const QModelIndex &index, const int role = Qt::DisplayRole) const override {
        if (role == Qt::DisplayRole && index.column() == columnIndex) {
            const int gender = QSqlTableModel::data(index, Qt::DisplayRole).toInt();
            return gender == 0 ? "女" : "男";
        }
        return QSqlTableModel::data(index, role);
    }
};



#endif //SEXSQLTABLEMODEL_H
