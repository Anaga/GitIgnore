#include "databasemanager.h"
#include <QDebug>
#include <QSqlError>

DatabaseManager& DatabaseManager::instance()
{
    static DatabaseManager instance;
    return instance;
}

DatabaseManager::DatabaseManager()
{
    db = QSqlDatabase::addDatabase("QSQLITE");
}

DatabaseManager::~DatabaseManager()
{
    closeDatabase();
}

bool DatabaseManager::openDatabase(const QString& path)
{
    db.setDatabaseName(path);
    if (!db.open()) {
        qDebug() << "Error: connection with database failed";
        qDebug() << "Error details:" << db.lastError().text();
        return false;
    }
    qDebug() << "Database opened successfully";
    return true;
}

void DatabaseManager::closeDatabase()
{
    db.close();
}

QSqlDatabase& DatabaseManager::database()
{
    if (!db.isOpen()) {
        qDebug() << "Warning: Attempting to use a closed database connection";
    }
    return db;
}
