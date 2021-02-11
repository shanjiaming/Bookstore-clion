//
// Created by JiamingShan on 2021/1/28.
//

#ifndef SRC_BPLUSTREE_H
#define SRC_BPLUSTREE_H

#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <map>
#include "Error.h"
using namespace std;

//------------------------------------
#define stub
//#define treedebug
//------------------------------------

typedef string FileName;
typedef string StringType;

template <class T>
class BPlusTree {
private:
    FileName filename;
    fstream fin, fout;
    fstream fin_next_tree;
#ifdef stub
    map<StringType, T> Map;
#endif
public:
    explicit BPlusTree<T>(const StringType &_filename) : filename(_filename){}

//    T find(const StringType &key);
//
//    void insert(const T &o);
//
//    void erase(const StringType &key);

#ifdef stub

    T find(const StringType &key){
        auto ret = Map.find(key);
        if (ret == Map.end())
            throw Not_Found();
        return (*ret).second;
    }

    void insert(const T &o){
        Map.insert(make_pair(o.key(), o));
    }

    void erase(const StringType &key){
        Map.erase(key);
    }

    vector<int> &findall(const StringType target){

    }


#ifdef treedebug
    void print(){
        for (auto i:Map){
            cout << i << endl;
        }
    }
#endif
#endif

};


#endif //SRC_BPLUSTREE_H
