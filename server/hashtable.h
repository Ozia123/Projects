#ifndef HASHTABLE_H
#define HASHTABLE_H

#include<QObject>
#include<QFile>
#include<QDebug>

class HashTable {
private:
    struct HashTableElement {
        QString hash;
        QString        username;
        HashTableElement*  next;
    };
    int NumberOfUsers = 0;
    HashTableElement* begin;
    bool IsHashTableSaved;
public:
    bool IsCoincidesHash(QString &hash);
    bool IsUsernameAvailable(QString &username);
    HashTable();
    HashTable(QString &path);
    bool Add(QString &hash, QString &username);
    bool SearchFor(QString &hash, QString &username);
    void Save(QString &path);
    void Load(QString &path);
    QString GetHashByID(int index);
    QString GetUserNameByID(int index);
    int GetNumberOfUsers() { return NumberOfUsers; }
};

#endif // HASHTABLE_H
