#include"Hash.h"
#include <QDebug>

// CREATE STRING'S HASH
unsigned long long Hash::Generate(QString &buffer) {
    std::string str = buffer.toStdString();
    unsigned long long HASH = GetHash(str, 0);
    TransformHash(HASH);
    hash = HASH;
    qDebug() << "HASH::Get - Successfully generated hash: " << HASH;
    return HASH;
}

// MAKES KEY'S HASH
unsigned long long Hash::GetHash(std::string &buffer, int i) {
    if (i == buffer.length()) return buffer[i]*pow(5, i);
    return buffer[i]*pow(5, i) + GetHash(buffer, i + 1);
}

// TRANSFORMS HASH TO LENGTH: const unsigned HASH_SIZE
void Hash::TransformHash(unsigned long long &hash) {
    int i = 0;
    unsigned long long max_hash_length = 0;
    while (i < HASH_SIZE) {
        max_hash_length += 9 * pow(10, i);
        i++;
    }
    while (hash > max_hash_length) {
        hash /= 10;
    }
    return;
}
