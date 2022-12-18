//
// Created by 屋顶上的小丑 on 2022/12/11.
//

#ifndef BOOKSTORE_BLOCKLINKEDLIST_H
#define BOOKSTORE_BLOCKLINKEDLIST_H

#include<fstream>
#include<string>
#include<cstring>
#include<vector>
#include<iostream>
#include<algorithm>
#include<utility>

const int kNodeSize = 316;
const int kMaxIndexLength = 64;
const size_t kSizeofIndex = 64;
static char *buffer;

template<class T>
class Element {
public:
    char index[kMaxIndexLength];
    T value;

    Element(std::string Index = "",T Value = 0) : value(Value) {
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

    ~Element() = default;
};

template<class T>
class NodeInfo {
public:
    int size, pos, nex, pre;
    Element<T> min_element;

    NodeInfo() : size(0), pos(0), nex(0), pre(0) {};
    ~NodeInfo() = default;
};

template<class T>
class Node {
public:
    int size, pos, nex, pre;
    Element<T> min_element;
    Element<T> data[kNodeSize << 1];

    Node() : size(0), pos(0), nex(0), pre(0) {};
    ~Node() = default;
};

template<class T>
class BlockLinkedList {
private:
    std::fstream file;
public:
    const size_t kSizeofElement = 64 + sizeof(T);
    const size_t kSizeofNodeInfo = 16 + kSizeofElement;
    const size_t kSizeofNode = kSizeofNodeInfo + (kNodeSize << 1) * kSizeofElement;
    Node<T> now, nex, las;
    NodeInfo<T> now_info, nex_info, nex_nex_info;
    std::vector<T> x;
    BlockLinkedList(const std::string &);
    void ReadNode(int, Node<T> &);
    void ReadNodeInfo(int, NodeInfo<T> &);
    void WriteNode(int, Node<T> &);
    void WriteNodeInfo(int, NodeInfo<T> &);
    void Split(Node<T> &);
    void Merge(Node<T> &, Node<T> &);
    void insert(const Element<T> &);
    void erase(const Element<T> &);
    void find(const std::string &);
    ~BlockLinkedList();
};

#endif //BOOKSTORE_BLOCKLINKEDLIST
