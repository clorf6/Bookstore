//
// Created by 屋顶上的小丑 on 2022/12/11.
//

#ifndef BOOKSTORE_BLOCKLINKEDLIST_CPP
#define BOOKSTORE_BLOCKLINKEDLIST_CPP

#include "BlockLinkedList.h"

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
void BlockLinkedList<T>::ReadNodeInfo(int pos, NodeInfo<T> &now_info) {
    file.seekg((pos - 1) * kSizeofNode);
    file.read(reinterpret_cast<char *>(&now_info), kSizeofNodeInfo);
    now_info.pos = pos;
}

template<class T>
void BlockLinkedList<T>::ReadNode(int pos, Node<T> &now) {
    file.seekg((pos - 1) * kSizeofNode);
    file.read(reinterpret_cast<char *>(&now), kSizeofNode);
    now.pos = pos;
}

template<class T>
void BlockLinkedList<T>::WriteNodeInfo(int pos, NodeInfo<T> &now_info) {
    now_info.pos = pos;
    file.seekp((pos - 1) * kSizeofNode);
    file.write(reinterpret_cast<char *>(&now_info), kSizeofNodeInfo);
}

template<class T>
void BlockLinkedList<T>::WriteNode(int pos, Node<T> &now) {
    now.pos = pos;
    file.seekp((pos - 1) * kSizeofNode);
    file.write(reinterpret_cast<char *>(&now), kSizeofNode);
}

template<class T>
void BlockLinkedList<T>::Split(Node<T> &now) {
    file.seekp(0, std::ios::end);
    int new_pos = file.tellp() / kSizeofNode + 1;
    Node<T> new_now;
    new_now.nex = now.nex;
    new_now.pre = now.pos;
    if (now.nex) {
        ReadNodeInfo(now.nex, nex_info);
        nex_info.pre = new_pos;
        WriteNodeInfo(now.nex, nex_info);
    }
    now.nex = new_pos;
    memcpy(new_now.data, now.data + kNodeSize, kSizeofElement * (now.size - kNodeSize));
    new_now.size = now.size - kNodeSize;
    now.size = kNodeSize;
    new_now.min_element = new_now.data[0];
    WriteNode(now.pos, now);
    WriteNode(new_pos, new_now);
}

template<class T>
void BlockLinkedList<T>::Merge(Node<T> &now, Node<T> &nex) {
    if (nex.nex) {
        ReadNodeInfo(nex.nex, nex_nex_info);
        nex_nex_info.pre = now.pos;
        WriteNodeInfo(nex.nex, nex_nex_info);
    }
    memcpy(now.data + now.size, nex.data, kSizeofElement * nex.size);
    if (!now.size) {
        now.min_element = nex.min_element;
    }
    now.size += nex.size;
    now.nex = nex.nex;
    WriteNode(now.pos, now);
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
    bool flag = 0;
    while (x >= now_info.min_element) {
        if (now_info.nex) {
            ReadNodeInfo(now_info.nex, now_info);
        } else {
            flag = 1;
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
    bool flag = 0;
    while (x >= now_info.min_element) {
        if (now_info.nex) {
            ReadNodeInfo(now_info.nex, now_info);
        } else {
            flag = 1;
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
    x.clear();
    ReadNodeInfo(1, now_info);
    if (strcmp(buffer, now_info.min_element.index) < 0) {
        return ;
    }
    bool flag = 0;
    while (strcmp(buffer, now_info.min_element.index) > 0) {
        if (now_info.nex) {
            ReadNodeInfo(now_info.nex, now_info);
        } else {
            flag = 1;
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
            x.push_back(las.data[i].value);
        } else if (comp < 0) {
            return ;
        }
    }
    if (las.nex) {
        ReadNode(las.nex, now);
        for (int i = 0; i < now.size; i++) {
            int comp = strcmp(buffer, now.data[i].index);
            if (!comp) {
                x.push_back(now.data[i].value);
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
                    x.push_back(now.data[i].value);
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

#endif //BOOKSTORE_BLOCKLINKEDLIST_CPP
