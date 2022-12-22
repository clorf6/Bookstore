//
// Created by 屋顶上的小丑 on 2022/12/22.
//

#ifndef BOOKSTORE_BOOKSTORE_CPP
#define BOOKSTORE_BOOKSTORE_CPP

#include "Bookstore.h"

int GetPrivilege() {
    if (online.empty()) {
        return 0;
    }
    return online.back().user_privilege;
}

Bookstore::Bookstore() {
    finance.open("finance", std::ios::in | std::ios::out | std::ios::binary);
    log_file.open("log", std::ios::in | std::ios::out | std::ios::binary);
    if (!finance.is_open()) {
        create.open("finance");
        finance.open("finance", std::ios::in | std::ios::out | std::ios::binary);
    }
    if (!log_file.is_open()) {
        create.open("log");
        finance.open("log", std::ios::in | std::ios::out | std::ios::binary);
    }
}

void Bookstore::Run() {
    while (std::getline(std::cin, op)) {
        if (op == "quit" || op == "exit") {
            break;
        }
        DivideOperation(op);
        if (ops[0] == "su") {

        }
    }
}

Bookstore::~Bookstore() {
    finance.close();
    log_file.close();
}

#endif //BOOKSTORE_BOOKSTORE_CPP