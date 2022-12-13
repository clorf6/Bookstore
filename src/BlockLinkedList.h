//
// Created by 屋顶上的小丑 on 2022/12/11.
//

#ifndef BOOKSTORE_BLOCKLINKEDLIST
#define BOOKSTORE_BLOCKLINKEDLIST

#include<fstream>
#include<string>
#include<cstring>
#include<vector>
#include<iostream>
#include<algorithm>
#include<utility>

const int kNodeSize = 350;
const int kMaxIndexLength = 64;
const size_t kSizeofIndex = 64;
const size_t kSizeofElement = 68;
const size_t kSizeofNode = 16 + (kNodeSize << 1) * kSizeofElement;
static char *buffer;
static std::vector<int> x;

struct Element {
    char index[kMaxIndexLength];
    int value;

    Element(std::string Index = "",int Value = 0) : value(Value) {
        memset(index, 0, kSizeofIndex);
        strcpy(index, Index.c_str());
    };

    bool operator==(const Element &x) const {
        return (!strcmp(index, x.index)) && value == x.value;
    }

    bool operator<(const Element &x) const {
        int comp = strcmp(index, x.index);
        return (!comp) ? value < x.value : comp < 0;
    }

    bool operator>(const Element &x) const {
        int comp = strcmp(index, x.index);
        return (!comp) ? value > x.value : comp > 0;
    }

    bool operator<=(const Element &x) const {
        int comp = strcmp(index, x.index);
        return (!comp) ? value <= x.value : comp <= 0;
    }

    bool operator>=(const Element &x) const {
        int comp = strcmp(index, x.index);
        return (!comp) ? value >= x.value : comp >= 0;
    }

    bool operator!=(const Element &x) const {
        return strcmp(index, x.index) || value != x.value;
    }
};

struct Node {
    int size, pos, nex, pre;
    Element data[kNodeSize << 1];

    Node() : size(0), pos(0), nex(0), pre(0) {};
};

static Node now, nex, nex_nex, las;

class BlockLinkedList {
public:
    std::fstream file;
    BlockLinkedList(const std::string &);
    void ReadNode(int, Node &);
    void WriteNode(int, Node &);
    void Split(int);
    void Merge(int);
    void insert(const Element &);
    void erase(const Element &);
    void find(const std::string &, std::vector<int> &);
    ~BlockLinkedList();
};

#endif //BOOKSTORE_BLOCKLINKEDLIST
