#ifndef NAMESQLTABLEMODEL_H
#define NAMESQLTABLEMODEL_H

#include <QSqlTableModel>
#include <QVariant>


class NameSqlTableModel final : public QSqlTableModel {
    // ID -> 名字
    const QMap<int, QString> map;
    // 列索引
    const int columnIndex;

public:
    explicit NameSqlTableModel(const QMap<int, QString> &map, const int columnIndex,
                              QObject *parent = nullptr) : QSqlTableModel(parent),
                                                           map(map), columnIndex(columnIndex) {
    }

    QVariant data(const QModelIndex &index, const int role = Qt::DisplayRole) const override {
        if (role == Qt::DisplayRole && index.column() == columnIndex) {
            const int id = QSqlTableModel::data(index, Qt::DisplayRole).toInt();
            return map.value(id, QString());
        }
        return QSqlTableModel::data(index, role);
    }
};



#endif //NAMESQLTABLEMODEL_H
