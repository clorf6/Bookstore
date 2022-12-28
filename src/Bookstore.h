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
static std::string str;

class Bookstore {
private:
    AccountSystem account_system;
    BookSystem book_system;
    std::fstream log_file;
    int op_count;
public:
    Bookstore();

    void GenerateLog();

    void Run();

    ~Bookstore();
};

#endif //BOOKSTORE_BOOKSTORE_H