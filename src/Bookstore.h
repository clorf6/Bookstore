//
// Created by 屋顶上的小丑 on 2022/12/22.
//

#ifndef BOOKSTORE_BOOKSTORE_H
#define BOOKSTORE_BOOKSTORE_H

#include "AccountSystem.h"
#include "BookSystem.h"
#include "LogSystem.h"
#include "Utils.h"

struct OnlineUser {
    int user_pos;
    int book_pos;
    int user_privilege;

    explicit OnlineUser(const int& User_pos = 0,
                        const int& Book_pos = 0,
                        const int& User_privilege = 0) :
            user_pos(User_pos),
            book_pos(Book_pos),
            user_privilege(User_privilege) {};
};

std::vector<OnlineUser> online;
static int GetPrivilege();
static std::ofstream create;
static std::string op;

class Bookstore {
private:
    AccountSystem account_system;
    BookSystem book_system;
public:
    Bookstore();
    static void Run();
    ~Bookstore();
};

#endif //BOOKSTORE_BOOKSTORE_H
