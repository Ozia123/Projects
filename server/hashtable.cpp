#include "hashtable.h"
#include <QDebug>

HashTable::HashTable() {
    begin = nullptr;
    IsHashTableSaved = true;
}

HashTable::HashTable(QString &path) {
    Load(path);
}

bool HashTable::SearchFor(QString &hash, QString &username){
    HashTableElement *p = begin;

    while (p){
        if (username == p->username)
            if (hash == p->hash) return true;
        p = p->next;
    }
    return false;

}

bool HashTable::IsCoincidesHash(QString &hash){
    HashTableElement *p = begin;

    while (p){
        if (hash == p->hash) return true;
        p = p->next;
    }
    return false;
}

bool  HashTable::IsUsernameAvailable(QString &username){
    HashTableElement *p = begin;

    while (p){
        if (username == p->username) return false;
        p = p->next;
    }

    return true;
}

bool HashTable::Add(QString &hash, QString &username) {
    HashTableElement *p = begin;
    NumberOfUsers++;

    if (!begin) {
        begin = new HashTableElement;
        begin->next = nullptr;
        begin->hash = hash;
        begin->username = username;
        return true;
    }

    if(!IsUsernameAvailable(username)) {
        NumberOfUsers--;
        return false;
    }

    if (IsCoincidesHash(hash)) {
        do {
            p = p->next;
        } while(p->hash != hash);
    
        if (p->next){
            HashTableElement *z;
            z = new HashTableElement;
            z->hash = hash;
            z->username = username;
            z->next = p->next;
            p->next = z;
        }
        else {
            while (p->next)
                p = p->next;
        
            p->next = new HashTableElement;
            p = p->next;
            p->next = NULL;
            begin->hash = hash;
            begin->username = username;
        }
    }

    else {
        while (p->next)
            p = p->next;

        p->next = new HashTableElement;
        p = p->next;
        p->next = NULL;
        p->hash = hash;
        p->username = username;
    }

    return true;
}

void HashTable::Save(QString &path){
    QFile mFile(path);

    if (!mFile.open(QFile::WriteOnly | QFile::Text)) {
        qDebug() << "Cannot to werite in file";
        return;
    }

    QTextStream stream(&mFile);

    HashTableElement* t = begin;

    while (t) {
    stream << t->username << "\n" << t->hash << "\n";
    t = t->next;
    }
}

void HashTable::Load(QString &path){
    QFile mFile(path);

    if (!mFile.open(QFile::ReadOnly | QFile::Text)) {
       qDebug() << "Cannot to write in file";
       return;
    }

    QTextStream stream(&mFile);
    int AA = 1;
    QString buffer;
    QString username;

    do {
        buffer = stream.readLine(20);
        if (AA & 1) {
            username = buffer;
        }
        else {
            Add(buffer, username);
        }
        AA++;
    } while(!buffer.isEmpty());
}

QString HashTable::GetHashByID(int index){
    HashTableElement *p = begin;
    for (int i = 0; i < index ; i++){
        p = p->next;
    }
    return p->hash;
}

QString HashTable::GetUserNameByID(int index){
    HashTableElement *p = begin;
    for (int i = 0; i < index ; i++){
        p = p->next;
    }
    return p->username;
}



