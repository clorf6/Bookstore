//
// Created by 屋顶上的小丑 on 2022/12/22.
//

#ifndef BOOKSTORE_BOOKSTORE_CPP
#define BOOKSTORE_BOOKSTORE_CPP

#include "Bookstore.h"

void Bookstore::Run() {
    while (std::getline(std::cin, op)) {
        try {
            if (op == "quit" || op == "exit") {
                break;
            }
            DivideOperation(op);
            if (ops[0] == "su") {
                if (ops.size() != 2 && ops.size() != 3) throw Exception("Invalid");
                if (!JudgeUserIDAndPasswd(ops[1])) throw Exception("Invalid");
                if (ops.size() == 2) {
                    account_system.LoginAccount(ops[1], "");
                } else {
                    if (!JudgeUserIDAndPasswd(ops[2])) throw Exception("Invalid");
                    account_system.LoginAccount(ops[1], ops[2]);
                }
            } else if (ops[0] == "logout") {
                if (ops.size() != 1) throw Exception("Invalid");
                account_system.LogoutAccount();
            } else if (ops[0] == "register") {
                if (ops.size() != 4) throw Exception("Invalid");
                if (!JudgeUserIDAndPasswd(ops[1])) throw Exception("Invalid");
                if (!JudgeUserIDAndPasswd(ops[2])) throw Exception("Invalid");
                if (!JudgeUsername(ops[3])) throw Exception("Invalid");
                account_system.RegisterAccount(ops[1], ops[2], ops[3]);
            } else if (ops[0] == "passwd") {
                if (ops.size() != 3 && ops.size() != 4) throw Exception("Invalid");
                if (!JudgeUserIDAndPasswd(ops[1])) throw Exception("Invalid");
                if (!JudgeUserIDAndPasswd(ops[2])) throw Exception("Invalid");
                if (ops.size() == 3) {
                    account_system.ChangePassword(ops[1], "", ops[2]);
                } else {
                    if (!JudgeUserIDAndPasswd(ops[3])) throw Exception("Invalid");
                    account_system.ChangePassword(ops[1], ops[2], ops[3]);
                }
            } else if (ops[0] == "useradd") {
                if (ops.size() != 5) throw Exception("Invalid");
                if (!JudgeUserIDAndPasswd(ops[1])) throw Exception("Invalid");
                if (!JudgeUserIDAndPasswd(ops[2])) throw Exception("Invalid");
                if (!JudgePrivilege(ops[3])) throw Exception("Invalid");
                if (!JudgeUsername(ops[4])) throw Exception("Invalid");
                account_system.AddAccount(ops[1], ops[2], std::stoi(ops[3]), ops[4]);
            } else if (ops[0] == "delete") {
                if (ops.size() != 2) throw Exception("Invalid");
                if (!JudgeUserIDAndPasswd(ops[1])) throw Exception("Invalid");
                account_system.DelAccount(ops[1]);
            } else if (ops[0] == "show") {
                if (ops[1] == "finance") {
                    if (ops.size() != 2 && ops.size() != 3) throw Exception("Invalid");
                    if (ops.size() == 2) book_system.NowFinance();
                    else {
                        if (!JudgeCount(ops[2])) throw Exception("Invalid");
                        book_system.QueryFinance(std::stoi(ops[2]));
                    }
                } else {
                    if (ops.size() != 1 && ops.size() != 2) throw Exception("Invalid");
                    if (ops.size() == 1) book_system.PrintAllBook();
                    if (ops[1].substr(0, 6) == "-ISBN=") {
                        auto&& ISBN = ops[1].substr(6);
                        if (!JudgeISBN(ISBN)) throw Exception("Invalid");
                        book_system.SearchBookByISBN(ISBN);
                    } else if (ops[1].substr(0, 6) == "-name=") {
                        auto&& book_name = ops[1].substr(6);
                        if (!JudgeBooknameAndAuthor(book_name)) throw Exception("Invalid");
                        book_system.SearchBookByBookName(book_name.substr(1, book_name.length() - 2));
                    } else if (ops[1].substr(0, 8) == "-author=") {
                        auto&& author = ops[1].substr(8);
                        if (!JudgeBooknameAndAuthor(author)) throw Exception("Invalid");
                        book_system.SearchBookByAuthor(author.substr(1, author.length() - 2));
                    } else if (ops[1].substr(0, 9) == "-keyword=") {
                        auto&& keyword = ops[1].substr(9);
                        if (!JudgeKeyword(keyword)) throw Exception("Invalid");
                        book_system.SearchBookByKeyword(keyword.substr(1, keyword.length() - 2));
                    } else throw Exception("Invalid");
                }
            } else if (ops[0] == "buy") {
                if (ops.size() != 3) throw Exception("Invalid");
                if (!JudgeISBN(ops[1])) throw Exception("Invalid");
                if (!JudgeQuantity(ops[2])) throw Exception("Invalid");
                int Quantity = std::stoi(ops[2]);
                book_system.BuyBook(ops[1], Quantity);
            } else if (ops[0] == "select") {
                if (ops.size() != 2) throw Exception("Invalid");
                if (!JudgeISBN(ops[1])) throw Exception("Invalid");
                book_system.SelectBook(ops[1]);
            } else if (ops[0] == "modify") {
                if (ops.size() < 2 || ops.size() > 6) throw Exception("Invalid");
                if (!book_system.JudgeModify()) throw Exception("Invalid");
                bool type[5]{};
                for (int i = 1; i < ops.size(); i++) {
                    if (ops[i].substr(0, 6) == "-ISBN=") {
                        if (type[0]) throw Exception("Invalid");
                        if (!JudgeISBN(ops[i].substr(6))) throw Exception("Invalid");
                        type[0] = true;
                    } else if (ops[i].substr(0, 6) == "-name=") {
                        if (type[1]) throw Exception("Invalid");
                        if (!JudgeBooknameAndAuthor(ops[i].substr(6))) throw Exception("Invalid");
                        type[1] = true;
                    } else if (ops[i].substr(0, 8) == "-author=") {
                        if (type[2]) throw Exception("Invalid");
                        if (!JudgeBooknameAndAuthor(ops[i].substr(8))) throw Exception("Invalid");
                        type[2] = true;
                    } else if (ops[i].substr(0, 9) == "-keyword=") {
                        if (type[3]) throw Exception("Invalid");
                        if (!JudgeKeyword(ops[i].substr(9))) throw Exception("Invalid");
                        type[3] = true;
                    } else if (ops[i].substr(0, 7) == "-price=") {
                        if (type[4]) throw Exception("Invalid");
                        if (!JudgePrice(ops[i].substr(7))) throw Exception("Invalid");
                        type[4] = true;
                    } else throw Exception("Invalid");
                }
            } else if (ops[0] == "import") {
                if (ops.size() != 3) throw Exception("Invalid");
                if (!JudgeQuantity(ops[1])) throw Exception("Invalid");
                if (!JudgeCost(ops[2])) throw Exception("Invalid");
                int Quantity = std::stoi(ops[1]);
                double Cost = std::stod(ops[2]);
                book_system.ImportBook(Quantity, Cost);
            } else throw Exception("Invalid");
        }
        catch (Exception &error) {
            std::cout << "Invalid\n";
        }
    }
}


#endif //BOOKSTORE_BOOKSTORE_CPP