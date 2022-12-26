//
// Created by 屋顶上的小丑 on 2022/12/22.
//

#ifndef BOOKSTORE_BOOKSTORE_H
#define BOOKSTORE_BOOKSTORE_H

#include "AccountSystem.h"
#include "BookSystem.h"
#include "Exception.h"
#include "Utils.h"

static std::string op;

class Bookstore {
public:
    AccountSystem account_system;
    BookSystem book_system;
    Bookstore() = default;
    void Run();
    ~Bookstore() = default;
};

#endif //BOOKSTORE_BOOKSTORE_H
