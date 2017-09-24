#ifndef _HASH_H
#define _HASH_H

#include <QString>
#include <string>

class Hash {
private:
    const unsigned         HASH_SIZE = 20;
    unsigned long long     hash;
    unsigned long long     GetHash(std::string&, int);
    void                   TransformHash(unsigned long long&);

public:
    Hash()                 { hash = 0; }
    Hash(QString &buffer)  { hash = 0; this->Generate(buffer); }
    ~Hash()                {}
    unsigned long long     Get() { return hash; }
    unsigned long long     Generate(QString &buffer);
};

#endif
