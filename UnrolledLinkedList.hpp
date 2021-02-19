//#define stub
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
#include <typeinfo>
#include "filemanip.h"


//------------------------------------
class NotFound {};

class MultipleElement {};

const int KEY[2] = {393241, 786433};
const int MOD = 998244353;
pair<int,int> hash(string arg) {
    int results[2];
    for (int num = 0; num < 2; ++num) {
        results[num] = 0;
        for (int i = 0; i < arg.length(); ++i) {
            if (arg[i] == '\0') break;
            results[num] = int(((long long) results[num] * KEY[num] + arg[i]) % MOD);
        }
    }
    return {results[0], results[1]};
}

template<class TKey = pair<int,int>, class TValue = long>
class UnrolledLinkedList {
private:
    FileName fileName;
    fstream file;
public:

#ifdef stub
    multimap<TKey, TValue> Map;
    UnrolledLinkedList<TKey, TValue>(const FileName &_fileName) : fileName(_fileName) {
    }
#endif

#ifndef stub

    UnrolledLinkedList<TKey, TValue>(const FileName &_fileName) : fileName(_fileName) {
        fcreate(_fileName);
        file.open(_fileName);
        file.seekp(0);
        fwrite(file, Block());
        file.close();
    }

    using Node = pair<TKey, TValue>;
   /* struct Node{//to make == and = ok
        TKey first;
        TValue second;
        Node() = default;
        Node(TKey _first, TValue _second){
            if (typeid(TKey) == typeid(char[])) strcpy(first, _first);
            else first = _first;
            if (typeid(TKey) == typeid(char[])) strcpy(second, _second);
            else second = _second;
        }

        bool operator==(const Node &rhs) const {
            return first == rhs.first &&
                   second == rhs.second;
        }

        bool operator!=(const Node &rhs) const {
            return !(rhs == *this);
        }

        bool operator<(const Node &rhs) const {
            if (first < rhs.first)
                return true;
            if (rhs.first < first)
                return false;
            return second < rhs.second;
        }

        bool operator>(const Node &rhs) const {
            return rhs < *this;
        }

        bool operator<=(const Node &rhs) const {
            return !(rhs < *this);
        }

        bool operator>=(const Node &rhs) const {
            return !(*this < rhs);
        }
    };*/
    static const int Nsize = sizeof(Node);
    static const int Nmax = 4;
    static const int Merge = Nmax;//不这样似乎会删出num=0块，下一个满块的bug

    struct Block {
        friend UnrolledLinkedList<TKey, TValue>;
        int next = -1, num = 0;
        Node nodes[Nmax];
    };

    void getblock(Address x, Block &b) {
        file.seekp(x);
        fread(file, b);
    }

    void putblock(Address x, Block &b) {
        file.seekp(x);
        fwrite(file, b);
    };

    void putblockend(Block &b) {
        file.seekp(0, ios::end);
        fwrite(file, b);
    }

public:
    TValue find(const TKey &key) {
        vector<TValue> v = findVector(key);
        auto sz = v.size();
        if (sz == 0) throw NotFound();
        if (sz == 1) return v[0];
        throw MultipleElement();
    }

    void insert(const TKey &key, const TValue &o) {
        openfile
        Block bl;
        Node insert_node {key, o};
        getblock(0, bl);
        Address block_pos = 0;
        while (bl.num && bl.nodes[bl.num - 1] < insert_node) {
            if (bl.next == -1) break;
            block_pos = bl.next;
            getblock(bl.next, bl);
        }
        int insert_pos = lower_bound(bl.nodes, bl.nodes + bl.num, insert_node) - bl.nodes;
        if (bl.num < Nmax) {//insert directly 0-base!
            if (bl.num == insert_pos) {
                bl.nodes[insert_pos] = insert_node;
            } else {
                Node temp = insert_node;//FIXME 严重怀疑char数组无法赋值，因此node无法赋值。
                Node record;
                for (int i = insert_pos; i <= bl.num; ++i) {
                    record = bl.nodes[i];
                    bl.nodes[i] = temp;
                    temp = record;
                }
            }
            ++bl.num;
            putblock(block_pos, bl);
        } else {// insert with split in insert pos
            Block bl_end;
            if (insert_pos != bl.num) {
                for (int i = insert_pos; i < bl.num; ++i) {
                    bl_end.nodes[i - insert_pos] = bl.nodes[i];
                }
                bl.nodes[insert_pos] = insert_node;
                bl_end.num = bl.num - insert_pos;
                bl.num = insert_pos + 1;
            } else {
                bl_end.nodes[0] = insert_node;
                bl_end.num = bl.num - insert_pos + (insert_pos == bl.num);
            }
            bl_end.next = bl.next;
            file.seekp(0, ios::end);
            bl.next = file.tellp();
            //this is change mark
            putblock(block_pos, bl);
            putblockend(bl_end);
        }
        closefile
    }
//FIXME 小心整块连挤现象爆内存或时间

    void erase(const TKey &key, const TValue &o) {
        openfile
        Block bl;
        Node erase_node {key, o};
        getblock(0, bl);
        Address block_pos = 0;
        while (!bl.num || bl.nodes[bl.num - 1] < erase_node) {
            if (bl.next == -1) break;
            assert(bl.num);
            block_pos = bl.next;
            getblock(bl.next, bl);
        }
        int erase_pos = lower_bound(bl.nodes, bl.nodes + bl.num, erase_node) - bl.nodes;
        assert(bl.nodes[erase_pos] == erase_node);
        Node temp = erase_node;
        Node record;
        for (int i = erase_pos + 1; i < bl.num; ++i) {
            bl.nodes[i - 1] = bl.nodes[i];
        }
        --bl.num;//
        if (bl.next != -1) {//这么写可读性很不好，然而是getblock没法直接返回值的锅。
            Block bl_next;
            getblock(bl.next, bl_next);
            if (bl.num + bl_next.num <= Merge) {//merge
                for (int i = 0; i < bl_next.num; ++i) {
                    bl.nodes[i + bl.num] = bl_next.nodes[i];
                }
                bl.num += bl_next.num;
                bl.next = bl_next.next;
            }
        }
        putblock(block_pos, bl);
        closefile
    }

    vector<TValue> findVector(const TKey &key) {
        openfile
        vector<TValue> v;
        Block bl;
        getblock(0, bl);
        while (bl.num && bl.nodes[bl.num - 1].first < key) {
            if (bl.next == -1) break;
            getblock(bl.next, bl);
        }

        vector<TKey> onlykey;
        for (int i = 0; i < bl.num; ++i) {
            onlykey.push_back(bl.nodes[i].first);
        }

        int find_pos = lower_bound(onlykey.begin(), onlykey.end(), key) - onlykey.begin();
        int find_max = upper_bound(onlykey.begin(), onlykey.end(), key) - onlykey.begin();
        for (int i = find_pos; i < find_max; ++i) {
            v.push_back(bl.nodes[i].second);
        }
        if (find_max != bl.num) {
            closefile
            return v;
        }
        Block bl_next;
        getblock(bl.next, bl_next);
        if (bl.next == -1) {
            closefile
            return v;
        }
        vector<TKey> onlykeynext;
        for (int i = 0; i < bl_next.num; ++i) {
            onlykeynext.push_back(bl_next.nodes[i].first);
        }
        int find_max_next = upper_bound(onlykeynext.begin(), onlykeynext.end(), key) - onlykeynext.begin();
        for (int i = 0; i < find_max_next; ++i) {
            v.push_back(bl_next.nodes[i].second);
        }
        closefile
        return v;
    }

    vector<TValue> findAll() {
        openfile
        vector<TValue> v;
        Block bl;
        getblock(0, bl);
        while(true) {
            for (int i = 0; i < bl.num; ++i) {
                v.push_back(bl.nodes[i].second);
            }
            if (bl.next == -1) break;
            getblock(bl.next, bl);
        };
        closefile
        return v;
    }

#endif


#ifndef stub

private:

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