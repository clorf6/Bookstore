//
// Created by 屋顶上的小丑 on 2022/12/11.
//

#ifndef BOOKSTORE_BLOCKLINKEDLIST_CPP
#define BOOKSTORE_BLOCKLINKEDLIST_CPP

#include "BlockLinkedList.h"

BlockLinkedList::BlockLinkedList(const std::string &file_name) {
    file.open(file_name, std::ios::in | std::ios::out | std::ios::binary);
    if (!file.is_open()) {
        std::ofstream create;
        create.open(file_name);
        file.open(file_name, std::ios::in | std::ios::out | std::ios::binary);
    }
}

Node BlockLinkedList::ReadNode(int pos, Node &now) {
    file.seekg((pos - 1) * kSizeofNode);
    file.read(reinterpret_cast<char *>(&now), kSizeofNode);
    now.pos = pos;
}

void BlockLinkedList::WriteNode(int pos, Node &now) {
    now.pos = pos;
    file.seekp((pos - 1) * kSizeofNode);
    file.write(reinterpret_cast<char *>(&now), kSizeofNode);
}

void BlockLinkedList::Split(int pos) {
    ReadNode(pos, now);
    file.seekp(0, std::ios::end);
    int new_pos = file.tellp() / kSizeofNode + 1;
    Node new_now;
    new_now.nex = now.nex;
    new_now.pre = pos;
    if (now.nex) {
        ReadNode(now.nex, nex);
        nex.pre = new_pos;
        WriteNode(nex.pos, nex);
    }
    now.nex = new_pos;
    memcpy(new_now.data, now.data + kNodeSize, kSizeofElement * (now.size - kNodeSize));
    new_now.size = now.size - kNodeSize;
    now.size = kNodeSize;
    WriteNode(pos, now);
    WriteNode(new_pos, new_now);
}

void BlockLinkedList::Merge(int pos) {
    ReadNode(pos, now);
    ReadNode(now.nex, nex);
    if (nex.nex) {
        ReadNode(nex.nex, nex_nex);
        nex_nex.pre = pos;
        WriteNode(nex.nex, nex_nex);
    }
    memcpy(now.data + now.size, nex.data, kSizeofElement * nex.size);
    now.size += nex.size;
    now.nex = nex.nex;
    WriteNode(pos, now);
}

void BlockLinkedList::insert(const Element &x) {
    file.seekg(0, std::ios::end);
    int delta = file.tellg();
    if (!delta) {
        now.size = 1;
        now.data[0] = x;
        WriteNode(1, now);
        return ;
    }
    ReadNode(1, now);
    if (x < now.data[0]) {
        for (int i = now.size - 1; i >= 0; i--) {
            now.data[i + 1] = now.data[i];
        }
        now.data[0] = x;
        now.size++;
        WriteNode(1, now);
        if (now.size == (kNodeSize << 1)) {
            Split(now.pos);
        }
        return ;
    }
    bool flag = 0;
    while (x >= now.data[0]) {
        if (now.nex) {
            ReadNode(now.nex, now);
        } else {
            flag = 1;
            break;
        }
    }
    if (!now.pre || flag) {
        las = now;
    } else {
        ReadNode(now.pre, las);
    }
    /* find the node by two divede
    int i = std::upper_bound(las.data, las.data + las.size, x) - las.data;
    for (int j = las.size - 1; j >= i; j--) {
        las.data[j + 1] = las.data[j];
    }
    las.data[i] = x;
    las.size++;
    file.seekp((las.pos - 1) * kSizeofNode);
    file.write(reinterpret_cast<char *>(&las), kSizeofNode);
    WriteNode(las.pos, las);
    if (las.size == (kNodeSize << 1)) {
        Split(las.pos);
    } */
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
                 Split(las.pos);
             }
             break;
         }
     }
}

void BlockLinkedList::erase(const Element &x) {
    ReadNode(1, now);
    if (x < now.data[0]) {
        return ;
    }
    bool flag = 0;
    while (x >= now.data[0]) {
        if (now.nex) {
            ReadNode(now.nex, now);
        } else {
            flag = 1;
            break;
        }
    }
    if (!now.pre || flag) {
        las = now;
    } else {
        ReadNode(now.pre, las);
    }
    /* find the node by two divede.
    int i = std::lower_bound(las.data, las.data + las.size, x) - las.data;
    if (las.data[i] == x) {
        for (int j = i; j < las.size - 1; j++) {
            las.data[j] = las.data[j + 1];
        }
        las.size--;
        WriteNode(las.pos, las);
        if (las.nex) {
            if (now.size + las.size <= kNodeSize) {
                Merge(las.pos);
            }
        }
    } */
     for (int i = las.size - 1; i >= 0; i--) {
         if (las.data[i] == x) {
             for (int j = i; j < las.size - 1; j++) {
                 las.data[j] = las.data[j + 1];
             }
             las.size--;
             WriteNode(las.pos, las);
             if (las.nex) {
                 if (now.size + las.size <= kNodeSize) {
                     Merge(las.pos);
                 }
             }
             break;
         }
     }
}

void BlockLinkedList::find(const std::string &index, std::vector<int> &ret) {
    buffer = (char *)index.data();
    ret.clear();
    ReadNode(1, now);
    if (strcmp(buffer, now.data[0].index) < 0) {
        return ;
    }
    bool flag = 0;
    while (strcmp(buffer, now.data[0].index) > 0) {
        if (now.nex) {
            ReadNode(now.nex, now);
        } else {
            flag = 1;
            break;
        }
    }
    if (!now.pre || flag) {
        las = now;
    } else {
        ReadNode(now.pre, las);
    }
    for (int i = 0; i < las.size; i++) {
        int comp = strcmp(buffer, las.data[i].index);
        if (!comp) {
            ret.push_back(las.data[i].value);
        } else if (comp < 0) {
            return ;
        }
    }
    if (las.nex) {
        ReadNode(las.nex, now);
        for (int i = 0; i < now.size; i++) {
            int comp = strcmp(buffer, now.data[i].index);
            if (!comp) {
                ret.push_back(now.data[i].value);
            } else if (comp < 0) {
                return ;
            }
        }
        while (now.nex) {
            ReadNode(now.nex, now);
            if (now.data[0].index > index) {
                return ;
            }
            for (int i = 0; i < now.size; i++) {
                int comp = strcmp(buffer, now.data[i].index);
                if (!comp) {
                    ret.push_back(now.data[i].value);
                } else if (comp < 0) {
                    return ;
                }
            }
        }
    }
}

BlockLinkedList::~BlockLinkedList() {
    file.close();
}

int main() {
    BlockLinkedList list("test");
    int T;
    std::cin >> T;
    while (T--) {
        std::string opt;
        std::cin >> opt;
        if (opt == "insert") {
            std::string s;
            int data;
            std::cin >> s >> data;
            list.insert(Element{s, data});
        } else if (opt == "find") {
            std::string s;
            std::cin >> s;
            list.find(s, x);
            if (!x.size())
                std::cout << "null";
            else {
                for (auto i : x)
                    std::cout << i << ' ';
            }
            std::cout << '\n';
        } else if (opt == "delete") {
            std::string s;
            int data;
            std::cin >> s >> data;
            list.erase(Element{s, data});
        }
    }
    return 0;
}

#endif //BOOKSTORE_BLOCKLINKEDLIST_CPP
