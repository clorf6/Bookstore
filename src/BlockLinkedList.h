//
// Created by 屋顶上的小丑 on 2022/12/11.
//

#ifndef BOOKSTORE_BLOCKLINKEDLIST_H
#define BOOKSTORE_BLOCKLINKEDLIST_H

#include <fstream>
#include <string>
#include <cstring>
#include <vector>
#include <iostream>
#include <algorithm>
#include <utility>

const int kNodeSize = 316;
const int kMaxIndexLength = 64;
const size_t kSizeofIndex = 64;
static char *buffer;

template<class T>
class Element {
public:
    char index[kMaxIndexLength]{};
    T value;

    explicit Element(const std::string& Index = "",const T& Value = 0) : value(Value) {
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
    std::vector<T> ans;

    explicit BlockLinkedList(const std::string &);

    BlockLinkedList(const BlockLinkedList<T> &);

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

template<class T>
BlockLinkedList<T>::BlockLinkedList(const std::string &file_name) {
    file.open(file_name, std::ios::in | std::ios::out | std::ios::binary);
    if (!file.is_open()) {
        std::ofstream create;
        create.open(file_name);
        file.open(file_name, std::ios::in | std::ios::out | std::ios::binary);
    }
}

template<class T>
BlockLinkedList<T>::BlockLinkedList(const BlockLinkedList<T> &x) {
    file = x.file;
}

template<class T>
void BlockLinkedList<T>::ReadNodeInfo(int pos,
                                      NodeInfo<T> &ret_info) {
    file.seekg((pos - 1) * kSizeofNode);
    file.read(reinterpret_cast<char *>(&ret_info), kSizeofNodeInfo);
    ret_info.pos = pos;
}

template<class T>
void BlockLinkedList<T>::ReadNode(int pos,
                                  Node<T> &ret) {
    file.seekg((pos - 1) * kSizeofNode);
    file.read(reinterpret_cast<char *>(&ret), kSizeofNode);
    ret.pos = pos;
}

template<class T>
void BlockLinkedList<T>::WriteNodeInfo(int pos,
                                       NodeInfo<T> &ret_info) {
    ret_info.pos = pos;
    file.seekp((pos - 1) * kSizeofNode);
    file.write(reinterpret_cast<char *>(&ret_info), kSizeofNodeInfo);
}

template<class T>
void BlockLinkedList<T>::WriteNode(int pos,
                                   Node<T> &ret) {
    ret.pos = pos;
    file.seekp((pos - 1) * kSizeofNode);
    file.write(reinterpret_cast<char *>(&ret), kSizeofNode);
}

template<class T>
void BlockLinkedList<T>::Split(Node<T> &x) {
    file.seekp(0, std::ios::end);
    int new_pos = file.tellp() / kSizeofNode + 1;
    Node<T> new_now;
    new_now.nex = x.nex;
    new_now.pre = x.pos;
    if (x.nex) {
        ReadNodeInfo(x.nex, nex_info);
        nex_info.pre = new_pos;
        WriteNodeInfo(x.nex, nex_info);
    }
    x.nex = new_pos;
    memcpy(new_now.data, x.data + kNodeSize, kSizeofElement * (x.size - kNodeSize));
    new_now.size = x.size - kNodeSize;
    x.size = kNodeSize;
    new_now.min_element = new_now.data[0];
    WriteNode(x.pos, x);
    WriteNode(new_pos, new_now);
}

template<class T>
void BlockLinkedList<T>::Merge(Node<T> &x,
                               Node<T> &y) {
    if (y.nex) {
        ReadNodeInfo(y.nex, nex_nex_info);
        nex_nex_info.pre = x.pos;
        WriteNodeInfo(y.nex, nex_nex_info);
    }
    memcpy(x.data + x.size, y.data, kSizeofElement * y.size);
    if (!x.size) {
        x.min_element = y.min_element;
    }
    x.size += y.size;
    x.nex = y.nex;
    WriteNode(x.pos, x);
}

template<class T>
void BlockLinkedList<T>::insert(const Element<T> &x) {
    file.seekg(0, std::ios::end);
    int delta = file.tellg();
    if (!delta) {
        now.size = 1;
        now.data[0] = now.min_element = x;
        WriteNode(1, now);
        return ;
    }
    ReadNodeInfo(1, now_info);
    if (x < now_info.min_element) {
        ReadNode(1, now);
        for (int i = now.size - 1; i >= 0; i--) {
            now.data[i + 1] = now.data[i];
        }
        now.data[0] = now.min_element = x;
        now.size++;
        WriteNode(1, now);
        if (now.size == (kNodeSize << 1)) {
            Split(now);
        }
        return ;
    }
    bool flag = false;
    while (x >= now_info.min_element) {
        if (now_info.nex) {
            ReadNodeInfo(now_info.nex, now_info);
        } else {
            flag = true;
            break;
        }
    }
    if (!now_info.pre || flag) {
        ReadNode(now_info.pos, las);
    } else {
        ReadNode(now_info.pre, las);
    }
    for (int i = las.size - 1; i >= 0; i--) {
        if (las.data[i] <= x) {
            for (int j = las.size - 1; j >= i + 1; j--) {
                las.data[j + 1] = las.data[j];
            }
            las.data[i + 1] = x;
            las.size++;
            file.seekp((las.pos - 1) * kSizeofNode);
            file.write(reinterpret_cast<char *>(&las), kSizeofNode);
            WriteNode(las.pos, las);
            if (las.size == (kNodeSize << 1)) {
                Split(las);
            }
            break;
        }
    }
}

template<class T>
void BlockLinkedList<T>::erase(const Element<T> &x) {
    ReadNodeInfo(1, now_info);
    if (x < now_info.min_element) {
        return ;
    }
    bool flag = false;
    while (x >= now_info.min_element) {
        if (now_info.nex) {
            ReadNodeInfo(now_info.nex, now_info);
        } else {
            flag = true;
            break;
        }
    }
    if (!now_info.pre || flag) {
        ReadNode(now_info.pos, las);
    } else {
        ReadNode(now_info.pre, las);
    }
    for (int i = las.size - 1; i >= 0; i--) {
        if (las.data[i] == x) {
            for (int j = i; j < las.size - 1; j++) {
                las.data[j] = las.data[j + 1];
            }
            las.size--;
            if (!i) {
                las.min_element = las.data[0];
            }
            WriteNode(las.pos, las);
            if (las.nex) {
                if (now_info.size + las.size <= kNodeSize) {
                    ReadNode(las.nex, now);
                    Merge(las, now);
                }
            }
            break;
        }
    }
}

template<class T>
void BlockLinkedList<T>::find(const std::string &index) {
    buffer = (char *)index.data();
    ans.clear();
    ReadNodeInfo(1, now_info);
    if (strcmp(buffer, now_info.min_element.index) < 0) {
        return ;
    }
    bool flag = false;
    while (strcmp(buffer, now_info.min_element.index) > 0) {
        if (now_info.nex) {
            ReadNodeInfo(now_info.nex, now_info);
        } else {
            flag = true;
            break;
        }
    }
    if (!now_info.pre || flag) {
        ReadNode(now_info.pos, las);
    } else {
        ReadNode(now_info.pre, las);
    }
    for (int i = 0; i < las.size; i++) {
        int comp = strcmp(buffer, las.data[i].index);
        if (!comp) {
            ans.push_back(las.data[i].value);
        } else if (comp < 0) {
            return ;
        }
    }
    if (las.nex) {
        ReadNode(las.nex, now);
        for (int i = 0; i < now.size; i++) {
            int comp = strcmp(buffer, now.data[i].index);
            if (!comp) {
                ans.push_back(now.data[i].value);
            } else if (comp < 0) {
                return ;
            }
        }
        while (now.nex) {
            ReadNode(now.nex, now);
            if (now.min_element.index > index) {
                return ;
            }
            for (int i = 0; i < now.size; i++) {
                int comp = strcmp(buffer, now.data[i].index);
                if (!comp) {
                    ans.push_back(now.data[i].value);
                } else if (comp < 0) {
                    return ;
                }
            }
        }
    }
}

template<class T>
BlockLinkedList<T>::~BlockLinkedList() {
    file.close();
}

#endif //BOOKSTORE_BLOCKLINKEDLIST_H
