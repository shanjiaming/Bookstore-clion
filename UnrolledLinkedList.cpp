//
// Created by JiamingShan on 2021/2/20.
//
#include "UnrolledLinkedList.h"

UnrolledLinkedList::UnrolledLinkedList(const FileName &_fileName) : fileName(_fileName) {
    ifstream fin(_fileName);
    if (fin) return;
    ofstream fout(_fileName);
    assert(fout);
    fout.close();
    file.open(_fileName);
    file.seekp(0);
    fwrite(file, Block());
    file.close();
}


#ifdef Template

TKM UnrolledLinkedList::hash(TKey arg) {
    return arg;
}

#else

const int KEY[3] = {393241, 786433, 19260817};
const int MOD = 998244353;
TKM UnrolledLinkedList::hash(TKey arg) {
    int results[3];
    for (int num = 0; num < 3; ++num) {
        results[num] = 0;
        for (int i = 0; i < arg.length(); ++i) {
            if (arg[i] == '\0') break;
            results[num] = int(((long long) results[num] * KEY[num] + arg[i]) % MOD);
        }
    }
    return {results[0], results[1], results[2]};
}

#endif


void UnrolledLinkedList::getblock(Address x, Block &b) {
    file.seekp(x);
    fread(file, b);
}

void UnrolledLinkedList::putblock(Address x, Block &b) {
    file.seekp(x);
    fwrite(file, b);
};

void UnrolledLinkedList::putblockend(Block &b) {
    file.seekp(0, ios::end);
    fwrite(file, b);
}


TValue UnrolledLinkedList::find(const TKey &key) {
    vector<TValue> v = findVector(key);
    auto sz = v.size();
    if (sz == 0) throw NotFound();
    if (sz == 1) return v[0];
    throw MultipleElement();
}

void UnrolledLinkedList::insert(const TKey &_key, const Address &o) {
    TKM key = hash(_key);
    openfile
    Block bl;
    Node insert_node{key, o};
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

void UnrolledLinkedList::erase(const TKey &_key, const Address &o) {
    TKM key = hash(_key);
    openfile
    Block bl;
    Node erase_node{key, o};
    getblock(0, bl);
    Address block_pos = 0;
    while (!bl.num || bl.nodes[bl.num - 1] < erase_node) {
        if (bl.next == -1) break;
        assert(bl.num);
        block_pos = bl.next;
        getblock(bl.next, bl);
    }
    int erase_pos = lower_bound(bl.nodes, bl.nodes + bl.num, erase_node) - bl.nodes;
    if (bl.nodes[erase_pos] != erase_node) throw NoErase();
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

vector<TValue> UnrolledLinkedList::findVector(const TKey &_key) {
    TKM key = hash(_key);
    openfile
    vector<TValue> v;
    Block bl;
    getblock(0, bl);
    while (bl.num && bl.nodes[bl.num - 1].first < key) {
        if (bl.next == -1) break;
        getblock(bl.next, bl);
    }
    while (true) {
        vector<TKM> onlykey;
        for (int i = 0; i < bl.num; ++i) {
            onlykey.push_back(bl.nodes[i].first);
        }
        int find_pos = lower_bound(onlykey.begin(), onlykey.end(), key) - onlykey.begin();
        int find_max = upper_bound(onlykey.begin(), onlykey.end(), key) - onlykey.begin();
        for (int i = find_pos; i < find_max; ++i) {
            v.push_back(bl.nodes[i].second);
        }
        if (bl.next == -1 || find_max != bl.num) {
            closefile
            return v;
        }
        getblock(bl.next, bl);
    }
}

vector<TValue> UnrolledLinkedList::findAll() {
    openfile
    vector<TValue> v;
    Block bl;
    getblock(0, bl);
    while (true) {
        for (int i = 0; i < bl.num; ++i) {
            v.push_back(bl.nodes[i].second);
        }
        if (bl.next == -1) break;
        getblock(bl.next, bl);
    };
    closefile
    return v;
}

# ifdef IO

void UnrolledLinkedList::printVector(TKey k) {
    vector<TValue> v = findVector(k);
    for (auto i : v)
        cout << i << " ";
    cout << '\n';
}

void UnrolledLinkedList::printAll() {
    vector<TValue> v = findAll();
    for (auto i : v)
        cout << i << " ";
    cout << '\n';
}

#endif
