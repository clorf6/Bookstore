//
// Created by 屋顶上的小丑 on 2022/12/20.
//
#ifndef BOOKSTORE_ACCOUNTSYSTEM_CPP
#define BOOKSTORE_ACCOUNTSYSTEM_CPP

#include "AccountSystem.h"

AccountSystem::AccountSystem() {
    account_data.open("account_data", std::ios::in | std::ios::out | std::ios::binary);
    if (!account_data.is_open()) {
        create.open("account_data");
        account_data.open("account_data", std::ios::in | std::ios::out | std::ios::binary);
    }
    Account root("root", "sjtu", "root", 7);
    WriteAccount(1, root);
    account_pos.insert(Element<int>{"root", 1});
    online.clear();
    online.emplace_back(1, 0, 7);
}

void AccountSystem::ReadAccount(int pos,
                                Account &ret) {
    account_data.seekg((pos - 1) * kSizeofAccount);
    account_data.read(reinterpret_cast<char *>(&ret), kSizeofAccount);
}

void AccountSystem::WriteAccount(int pos,
                                 Account &ret) {
    account_data.seekp((pos - 1) * kSizeofAccount);
    account_data.write(reinterpret_cast<char *>(&ret), kSizeofAccount);
}

void AccountSystem::LoginAccount(const std::string &User_ID,
                                 const std::string &Password = "") {
    account_pos.find(User_ID);
    if (account_pos.ans.empty()) {
        std::cout << "Invalid\n";
        return ;
    }
    int pos = account_pos.ans[0];
    ReadAccount(pos, now_account);
    if (now_account.password != Password && GetPrivilege() <= now_account.privilege) {
        std::cout << "Invalid\n";
        return ;
    }
    online.emplace_back(pos, 0, now_account.privilege);
    is_login[pos]++;
}

void AccountSystem::LogoutAccount() {
    if (online.empty()) {
        std::cout << "Invalid\n";
        return ;
    }
    is_login[online.back().user_pos]--;
    online.pop_back();
}

void AccountSystem::RegisterAccount(const std::string &User_ID,
                                    const std::string &Password,
                                    const std::string &User_name) {
    account_pos.find(User_ID);
    if (!account_pos.ans.empty()) {
        std::cout << "Invalid\n";
        return ;
    }
    Account new_account(User_ID, Password, User_name, 1);
    account_data.seekp(0, std::ios::end);
    int new_pos = account_data.tellp() / kSizeofAccount + 1;
    account_pos.insert(Element<int>{User_ID, new_pos});
    WriteAccount(new_pos, new_account);
}

void AccountSystem::ChangePassword(const std::string &User_ID,
                                   const std::string &CurPasswd,
                                   const std::string &NewPasswd) {
    if (online.empty()) {
        std::cout << "Invalid\n";
        return ;
    }
    account_pos.find(User_ID);
    if (account_pos.ans.empty()) {
        std::cout << "Invalid\n";
        return ;
    }
    int pos = account_pos.ans[0];
    ReadAccount(pos, now_account);
    if (now_account.password != CurPasswd && GetPrivilege() != 7) {
        std::cout << "Invalid\n";
        return ;
    }
    memset(now_account.password, 0, 30);
    strcpy(now_account.password, NewPasswd.c_str());
    WriteAccount(pos, now_account);
}

void AccountSystem::AddAccount(const std::string &User_ID,
                               const std::string &Password,
                               const int &Privilege,
                               const std::string &User_name) {
    if (GetPrivilege() < 3) {
        std::cout << "Invalid\n";
        return ;
    }
    if (GetPrivilege() <= Privilege) {
        std::cout << "Invalid\n";
        return ;
    }
    account_pos.find(User_ID);
    if (!account_pos.ans.empty()) {
        std::cout << "Invalid\n";
        return ;
    }
    Account new_account(User_ID, Password, User_name, Privilege);
    account_data.seekp(0, std::ios::end);
    int new_pos = account_data.tellp() / kSizeofAccount + 1;
    account_pos.insert(Element<int>{User_ID, new_pos});
    WriteAccount(new_pos, new_account);
}

void AccountSystem::DelAccount(const std::string &User_ID) {
    if (GetPrivilege() < 7) {
        std::cout << "Invalid\n";
        return ;
    }
    account_pos.find(User_ID);
    if (account_pos.ans.empty()) {
        std::cout << "Invalid\n";
        return ;
    }
    int pos = account_pos.ans[0];
    if (is_login[pos]) {
        std::cout << "Invalid\n";
        return ;
    }
    account_pos.erase(Element<int>{User_ID, pos});
}

//void AccountSystem::GetTop() {
//    ReadAccount(online.back().user_pos, top);
//    std::cout << top.user_ID << ' ' << top.password << ' ' << top.user_name << ' ' << top.privilege << '\n';
//}

AccountSystem::~AccountSystem() {
    account_data.close();
    online.clear();
    is_login.clear();
}

#endif //BOOKSTORE_ACCOUNTSYSTEM_CPP

