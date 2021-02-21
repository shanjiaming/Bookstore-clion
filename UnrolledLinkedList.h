//#define Template
//#define IO
//------------------------------------
//
// Created by JiamingShan on 2021/1/28.
//

#ifndef SRC_BPLUSTREE_HPP
#define SRC_BPLUSTREE_HPP

#include <tuple>
#include <vector>
#include <iostream>
#include <algorithm>
#include <map>
#include <memory.h>
#include <typeinfo>
#include "filemanip.h"


//------------------------------------
class NotFound {
};

class NoErase {
};

class MultipleElement {
};


#ifdef Template
//this version is for <int, int>, or those who has const length and has operator=,<. If not,you also should redefine hash() in .cpp
using TKey = Address;
using TKM = Address;
using TValue = Address;
#else
//this version is for <string, int>, working with the whole bookstore
using TKey = string;
using TKM = tuple<int, int, int>;
using TValue = Address;
#endif
class UnrolledLinkedList {
private:
    FileName fileName;
    fstream file;


private:
    using Node = pair<TKM, TValue>;

    TKM hash(TKey arg);

    static const int Nmax = 400;

    struct Block {
        friend UnrolledLinkedList;
        Address next = -1;      //next means the address of next Block in the file.
        int num = 0;            //num means how many valid nodes are there in this Block.
        Node nodes[Nmax];       //This means 0 <= num <= Nmax
    };

    void getblock(Address x, Block &b);

    void putblock(Address x, const Block &b);

    void putblockend(const Block &b);


public:

    UnrolledLinkedList(const FileName &_fileName);

    TValue find(const TKey &key);

    void insert(const TKey &_key, const TValue &o);

    void erase(const TKey &_key, const TValue &o);

    vector<TValue> findVector(const TKey &_key);

    vector<TValue> findAll();

# ifdef IO

public:

    void printVector(TKey k);

    void printAll();

#endif

};


#endif //SRC_BPLUSTREE_HPP