//
// Created by JiamingShan on 2021/2/16.
//

#ifndef CODE_FILEMANIP_H
#define CODE_FILEMANIP_H

#include <string>
#include <fstream>
#include <cassert>
using namespace std;

typedef string FileName;
typedef int Address;

inline void fcreate(FileName fn){//FIXME 这里并没有用二进制打开，但是我猜测无关紧要。
    ifstream fin(fn);
    if (fin) {
        fin.close();
        return;
    }
    ofstream fooo(fn);
    assert(fooo);
    fooo.close();
}

#define openfile file.open(fileName, ios::in | ios::out | ios::binary);assert(file);

#define closefile file.close();

template <class T>
void fwrite(ostream &_file, const T &t) {
    _file.write(reinterpret_cast<const char *>(&t), sizeof(t));
}

template <class T>
void fread(istream &_file, T &t) {
    _file.read(reinterpret_cast<char *>(&t), sizeof(t));
}

#endif //CODE_FILEMANIP_H
