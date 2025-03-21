#ifndef DATABASE_H
#define DATABASE_H

#include <QSqlQuery>
#include <QSqlError>
#include <QString>
#include <QDebug>
#include <filesystem>
#include <fstream>
#include <map>
#include <string>

class Database {
public:
    static bool connect() {
        QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
        db.setDatabaseName("../db/sqlite.db");

        if (!db.open()) {
            qDebug() << "Error connecting database: " << db.lastError().text();
            return false;
        }
        qDebug() << "Connected database";
        return true;
    }

    static bool createTables() {
        std::map<std::string, std::string> contents;
        if (!readFiles("../sql", contents)) {
            qDebug() << "Error reading files";
        }
        for (auto &[fst, snd] : contents) {
            if (QSqlQuery query; !query.exec(QString::fromStdString(snd))) {
                qDebug() << "Error creating table " << QString::fromStdString(fst) << " : " << query.lastError().text();
                return false;
            }
            qDebug() << "Table " << QString::fromStdString(fst) << "created";
        }
        return true;
    }

private:
    static bool readFiles(const std::string& dir_path, std::map<std::string, std::string>& contents) {
        try {
            // 检查目录有效性
            if (!std::filesystem::exists(dir_path) || !std::filesystem::is_directory(dir_path)) {
                return false;
            }

            // 遍历一级目录
            for (const auto& entry : std::filesystem::directory_iterator(dir_path)) {

                // 排除子目录
                if (!entry.is_regular_file())  continue;

                // 读取文件内容
                std::ifstream file(entry.path());
                if (!file.is_open())  continue;

                // 高效读取全部内容
                std::string content{
                    std::istreambuf_iterator<char>(file),
                    std::istreambuf_iterator<char>()
                };

                contents[entry.path().filename().string()] = std::move(content);
            }
            return true;
        }
        catch (const std::filesystem::filesystem_error& e) {
            // 处理文件系统异常
            return false;
        }
    }
};



#endif //DATABASE_H
