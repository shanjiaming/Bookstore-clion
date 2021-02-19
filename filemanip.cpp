//
// Created by JiamingShan on 2021/2/20.
//
#include "filemanip.h"

void fcreate(FileName fn){//FIXME 这里并没有用二进制打开，但是我猜测无关紧要。
    ifstream fin(fn);
    if (fin) return;
    ofstream fooo(fn);
    assert(fooo);
    fooo.close();
}