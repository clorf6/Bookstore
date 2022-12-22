//
// Created by 屋顶上的小丑 on 2022/12/20.
//
#ifndef BOOKSTORE_ACCOUNTSYSTEM_H
#define BOOKSTORE_ACCOUNTSYSTEM_H

#include <string>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <cstdio>
#include <cstring>
#include <vector>
#include <unordered_map>
#include "BlockLinkedList.h"

struct Account {
    char user_id[30]{};
    char password[30]{};
    char user_name[30]{};
    int privilege;

    explicit Account(const std::string& _user_ID = "",
            const std::string& _password = "",
            const std::string& _user_name = "",
            const int& _privilege = 0) : privilege(_privilege) {
        memset(user_id, 0, 30);
        memset(password, 0, 30);
        memset(user_name, 0, 30);
        strcpy(user_id, _user_ID.c_str());
        strcpy(password, _password.c_str());
        strcpy(user_name,_user_name.c_str());
    };
};

const size_t kSizeofAccount = sizeof(Account);
static Account now_account;

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
static std::unordered_map<int, int> is_login;
static int GetPrivilege() {
    if (online.empty()) {
        return 0;
    }
    return online.back().user_privilege;
}

class AccountSystem {
private:
    BlockLinkedList<int> account_pos = BlockLinkedList<int>("account_pos");
    std::fstream account_data;
public:
    AccountSystem();
    void ReadAccount(int, Account &);
    void WriteAccount(int, Account &);
    void LoginAccount(const std::string &, const std::string &);
    static void LogoutAccount();
    void RegisterAccount(const std::string &, const std::string &, const std::string &);
    void ChangePassword(const std::string &, const std::string &, const std::string &);
    void AddAccount(const std::string &, const std::string &, const int &, const std::string &);
    void DelAccount(const std::string &);
//    void GetTop();
    ~AccountSystem();
};

#endif //BOOKSTORE_ACCOUNTSYSTEM_H
