//
// Created by JiamingShan on 2021/2/16.
//

#ifndef CODE_FILEMANIP_H
#define CODE_FILEMANIP_H

#include <string>
#include <fstream>

typedef string FileName;
typedef long Address;

template <class T>
void fwrite(ostream &_file, const T &t) {
    _file.write(reinterpret_cast<const char *>(&t), sizeof(t));
}

template <class T>
void fread(istream &_file, T &t) {
    _file.read(reinterpret_cast<char *>(&t), sizeof(t));
}

#endif //CODE_FILEMANIP_H
