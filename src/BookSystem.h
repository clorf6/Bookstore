//
// Created by 屋顶上的小丑 on 2022/12/20.
//

#ifndef BOOKSTORE_BOOKSYSTEM_H
#define BOOKSTORE_BOOKSYSTEM_H

#include <string>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <cstdio>
#include <cstring>
#include <vector>
#include <cmath>
#include <iomanip>
#include <unordered_map>
#include "AccountSystem.h"
#include "BlockLinkedList.h"
#include "Exception.h"
#include "Utils.h"

struct Book {
    char ISBN[21]{};
    char book_name[61]{};
    char author[61]{};
    char keyword[61]{};
    int quantity;
    double price;

    Book(const std::string &isbn = "",
         const std::string &_book_name = "",
         const std::string &_author = "",
         const std::string &_keyword = "",
         const int &_quantity = 0,
         const double &_price = 0.0) :
            quantity(_quantity),
            price(_price) {
        memset(ISBN, 0, 21);
        memset(book_name, 0, 61);
        memset(author, 0, 61);
        memset(keyword, 0, 61);
        strcpy(ISBN, isbn.c_str());
        strcpy(book_name, _book_name.c_str());
        strcpy(author, _author.c_str());
        strcpy(keyword, _keyword.c_str());
    };

    bool operator<(const Book &x) const {
        return strcmp(ISBN, x.ISBN) < 0;
    }
};

const size_t kSizeofBook = sizeof(Book);
static Book now_book;
static std::vector<Book> ans_book;

struct Deal {
    double income;
    double outcome;

    Deal(const double &Income = 0.0,
         const double &Outcome = 0.0) :
            income(Income),
            outcome(Outcome) {};
};

const size_t kSizeofDeal = 16;
static Deal now_deal, pre_deal;

class BookSystem {
private:
    BlockLinkedList<int> isbn_pos;
    BlockLinkedList<int> book_name_pos;
    BlockLinkedList<int> author_pos;
    BlockLinkedList<int> keyword_pos;
    std::fstream book_data;
    std::fstream finance;
    int count;
public:
    BookSystem();

    void ReadBook(int, Book &);

    void WriteBook(int, Book &);

    void PrintAllBook();

    void SearchBookByISBN(const std::string &);

    void SearchBookByBookName(const std::string &);

    void SearchBookByAuthor(const std::string &);

    void SearchBookByKeyword(const std::string &);

    void BuyBook(const std::string &, const int &);

    int AddBook(const std::string &);

    void SelectBook(const std::string &);

    bool JudgeModify();

    void ModifyBookISBN(const std::string &);

    void ModifyBookName(const std::string &);

    void ModifyBookAuthor(const std::string &);

    void ModifyBookKeyword(const std::string &);

    void ModifyBookPrice(const double &);

    void ImportBook(const int &, const double &);

    void ReadDeal(int, Deal &);

    void WriteDeal(int, Deal &);

    void NowFinance();

    void QueryFinance(const int &);

    void PrintAllFinance();

    ~BookSystem();
};

#endif //BOOKSTORE_BOOKSYSTEM_H