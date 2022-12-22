//
// Created by 屋顶上的小丑 on 2022/12/22.
//
#ifndef BOOKSTORE_UTILS_CPP
#define BOOKSTORE_UTILS_CPP

#include "Utils.h"

void DivideOperation(const std::string& Op) {
    now_op.clear();
    ops.clear();
    for (auto& i : Op) {
        if (i == ' ') {
            ops.push_back(now_op);
            now_op.clear();
        } else {
            now_op += i;
        }
    }
    ops.push_back(now_op);
}

#endif //BOOKSTORE_UTILS_CPP