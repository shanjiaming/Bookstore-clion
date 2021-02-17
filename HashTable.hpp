#define stub
//#define treedebug
//------------------------------------
//
// Created by JiamingShan on 2021/1/28.
//

#ifndef SRC_BPLUSTREE_HPP
#define SRC_BPLUSTREE_HPP

#include <vector>
#include <iostream>
#include <algorithm>
#include <map>
#include <memory.h>
#include "filemanip.h"



//------------------------------------
class NotFound{};
class MultipleElement{};

template <class TKey ,class TValue>
class HashTable {
private:
    FileName fileName;
    fstream file;
#ifdef stub
    multimap<TKey, TValue> Map;
#endif

#ifndef stub
    TValue find(const TKey &key) const{
        auto v = findVector(key);
        auto sz = v.size();
        if (sz == 0) throw NotFound();
        if (sz == 1) return v[0];
        throw MultipleElement();
    }

    void insert(const TKey &key, const TValue &o){
        int b_a = hash(key);
    }


    void erase(const TKey &key, const TValue &o){
    }

    vector<TValue> findVector(const TKey &key) const{
    }

    vector<TValue> findAll() const{
    }
#endif
public:
    HashTable<TKey, TValue>(const FileName &_fileName) : fileName(_fileName){}

#ifndef stub

private:

    const int BlockSize =
            NodeSize =
                    R =
                    NodeNum =

    using Node = pair<Address, pair<TKey, TValue>>;

    int hash(const Node &n){
        return n.second.first % R;
    }

//
//    class Block{
//    private:
//        Address startAddress;
//        Address sas[B];
//    public:
//        Block(){
//            memset(sas, -1, sizeof(sas) * R);
//        }
//        void write(size_t x, writethings){
//
//        }
//
//
//    };
//


    class Pool{
    private:

    public:

    };






#endif

#ifdef stub

    TValue find(const TKey &key) const{
        auto v = findVector(key);
        auto sz = v.size();
        if (sz == 0) throw NotFound();
        if (sz == 1) return v[0];
        throw MultipleElement();
    }

    void insert(const TKey &key, const TValue &o){
        Map.insert(make_pair(key, o));
    }


    void erase(const TKey &key, const TValue &o){
        int i = Map.count(key);
        auto it = Map.find(key);
        while(i--){
            if (it->second == o){
                auto itassert = ++it;
                --it;
                assert(itassert->first != key || itassert->second != o);
                Map.erase(it);
                return;
            }
            ++it;
        }
        assert(false);
    }

    vector<TValue> findVector(const TKey &key) const{
        vector<TValue> v;
        int i = Map.count(key);
        auto it = Map.find(key);
        while(i--){
            v.push_back(it++->second);
        }
        return v;
    }

    vector<TValue> findAll() const{
        vector<TValue> v;
        //TODO
        for (auto i:Map){
            v.push_back(i.second);
        }
        return v;
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


















#endif //SRC_BPLUSTREE_HPP


//1.树的设计：双类型模板？
//2.怎么索引结构设计？

