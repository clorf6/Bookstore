//
// Created by 屋顶上的小丑 on 2022/12/20.
//
#ifndef BOOKSTORE_BOOKSYSTEM_CPP
#define BOOKSTORE_BOOKSYSTEM_CPP

#include "BookSystem.h"

BookSystem::BookSystem() {
    book_data.open("book_data", std::ios::in | std::ios::out | std::ios::binary);
    if (!book_data.is_open()) {
        std::ofstream create;
        create.open("book_data");
        book_data.open("book_data", std::ios::in | std::ios::out | std::ios::binary);
    }
}

void BookSystem::ReadBook(int pos,
                          Book &ret) {
    book_data.seekg((pos - 1) * kSizeofBook);
    book_data.read(reinterpret_cast<char *>(&ret), kSizeofBook);
}

void BookSystem::WriteBook(int pos,
                           Book &ret) {
    book_data.seekp((pos - 1) * kSizeofBook);
    book_data.write(reinterpret_cast<char *>(&ret), kSizeofBook);
}

void BookSystem::PrintAllBook() {
    if (online.empty()) {
        throw Exception("Invalid");
    }
    isbn_pos.getall();
    if (isbn_pos.ans.empty()) {
        std::cout << '\n';
        return ;
    }
    ans_book.clear();
    for (int& pos : isbn_pos.ans) {
        ReadBook(pos, now_book);
        ans_book.push_back(now_book);
    }
    for (auto& now : ans_book) {
        std::cout << now.ISBN << '\t'
                  << now.book_name << '\t'
                  << now.author << '\t'
                  << now.keyword << '\t'
                  << now.price << '\t'
                  << now.quantity << '\n';
    }
}

void BookSystem::SearchBookByISBN(const std::string &isbn) {
    if (online.empty()) {
        throw Exception("Invalid");
    }
    isbn_pos.find(isbn);
    if (isbn_pos.ans.empty()) {
        std::cout << '\n';
        return ;
    }
    ans_book.clear();
    for (int& pos : isbn_pos.ans) {
        ReadBook(pos, now_book);
        ans_book.push_back(now_book);
    }
    std::sort(ans_book.begin(), ans_book.end());
    for (auto& now : ans_book) {
        std::cout << now.ISBN << '\t'
                  << now.book_name << '\t'
                  << now.author << '\t'
                  << now.keyword << '\t'
                  << now.price << '\t'
                  << now.quantity << '\n';
    }
}

void BookSystem::SearchBookByBookName(const std::string &_book_name) {
    if (online.empty()) {
        throw Exception("Invalid");
    }
    book_name_pos.find(_book_name);
    if (book_name_pos.ans.empty()) {
        std::cout << '\n';
        return ;
    }
    ans_book.clear();
    for (int& pos : book_name_pos.ans) {
        ReadBook(pos, now_book);
        ans_book.push_back(now_book);
    }
    std::sort(ans_book.begin(), ans_book.end());
    for (auto& now : ans_book) {
        std::cout << now.ISBN << '\t'
                  << now.book_name << '\t'
                  << now.author << '\t'
                  << now.keyword << '\t'
                  << now.price << '\t'
                  << now.quantity << '\n';
    }
}

void BookSystem::SearchBookByAuthor(const std::string &_author) {
    if (online.empty()) {
        throw Exception("Invalid");
    }
    author_pos.find(_author);
    if (author_pos.ans.empty()) {
        std::cout << '\n';
        return ;
    }
    ans_book.clear();
    for (int& pos : author_pos.ans) {
        ReadBook(pos, now_book);
        ans_book.push_back(now_book);
    }
    std::sort(ans_book.begin(), ans_book.end());
    for (auto& now : ans_book) {
        std::cout << now.ISBN << '\t'
                  << now.book_name << '\t'
                  << now.author << '\t'
                  << now.keyword << '\t'
                  << now.price << '\t'
                  << now.quantity << '\n';
    }
}

void BookSystem::SearchBookByKeyword(const std::string &_keyword) {
    if (online.empty()) {
        throw Exception("Invalid");
    }
    for (auto& i : _keyword) {
        if (i == '|') {
            throw Exception("Invalid");
        }
    }
    keyword_pos.find(_keyword);
    if (keyword_pos.ans.empty()) {
        std::cout << '\n';
        return ;
    }
    ans_book.clear();
    for (int& pos : keyword_pos.ans) {
        ReadBook(pos, now_book);
        ans_book.push_back(now_book);
    }
    std::sort(ans_book.begin(), ans_book.end());
    for (auto& now : ans_book) {
        std::cout << now.ISBN << '\t'
                  << now.book_name << '\t'
                  << now.author << '\t'
                  << now.keyword << '\t'
                  << now.price << '\t'
                  << now.quantity << '\n';
    }
}

void BookSystem::BuyBook(const std::string &isbn, const int &_quantity) {
    if (online.empty()) {
        throw Exception("Invalid");
    }
    isbn_pos.find(isbn);
    if (isbn_pos.ans.empty()) {
        throw Exception("Invalid");
    }
    int pos = isbn_pos.ans[0];
    ReadBook(pos, now_book);
    if (_quantity > now_book.quantity) {
        throw Exception("Invalid");
    }
    now_book.quantity -= _quantity;
    double income = now_book.price * _quantity;
    ReadDeal(count, now_deal);
    now_deal.income += income;
    WriteDeal(++count, now_deal);
    std::cout << std::fixed << std::setprecision(2) << income << '\n';
    WriteBook(pos, now_book);
}

int BookSystem::AddBook(const std::string &isbn) {
    Book new_book(isbn);
    book_data.seekp(0, std::ios::end);
    int new_pos = book_data.tellp() / kSizeofBook + 1;
    WriteBook(new_pos, new_book);
    isbn_pos.insert(Element<int>{isbn, new_pos});
    book_name_pos.insert(Element<int>{"", new_pos});
    author_pos.insert(Element<int>{"", new_pos});
    keyword_pos.insert(Element<int>{"", new_pos});
    return new_pos;
}

void BookSystem::SelectBook(const std::string &isbn) {
    if (GetPrivilege() < 3) {
        throw Exception("Invalid");
    }
    isbn_pos.find(isbn);
    if (isbn_pos.ans.empty()) {
        online.back().book_pos = AddBook(isbn);
        return ;
    }
    online.back().book_pos = isbn_pos.ans[0];
}

bool BookSystem::JudgeModify() {
    if (GetPrivilege() < 3) {
        throw Exception("Invalid");
    }
    if (!online.back().book_pos) {
        throw Exception("Invalid");
    }
    return true;
}

void BookSystem::ModifyBookISBN(const std::string &isbn) {
    int pos = online.back().book_pos;
    ReadBook(pos, now_book);
    isbn_pos.erase(Element<int>{now_book.ISBN, pos});
    memset(now_book.ISBN, 0, 20);
    strcpy(now_book.ISBN, isbn.c_str());
    isbn_pos.insert(Element<int>{isbn, pos});
    WriteBook(pos, now_book);
}

void BookSystem::ModifyBookName(const std::string &_book_name) {
    int pos = online.back().book_pos;
    ReadBook(pos, now_book);
    book_name_pos.erase(Element<int>{now_book.book_name, pos});
    memset(now_book.book_name, 0, 60);
    strcpy(now_book.book_name, _book_name.c_str());
    book_name_pos.insert(Element<int>{_book_name, pos});
    WriteBook(pos, now_book);
}

void BookSystem::ModifyBookAuthor(const std::string &_author) {
    int pos = online.back().book_pos;
    ReadBook(pos, now_book);
    author_pos.erase(Element<int>{now_book.author, pos});
    memset(now_book.author, 0, 60);
    strcpy(now_book.author, _author.c_str());
    author_pos.insert(Element<int>{_author, pos});
    WriteBook(pos, now_book);
}

void BookSystem::ModifyBookKeyword(const std::string &_keyword) {
    int pos = online.back().book_pos;
    ReadBook(pos, now_book);
    DivideKeyword(now_book.keyword, ans_keyword);
    for (auto& i : ans_keyword) {
        keyword_pos.erase(Element<int>{i, pos});
    }
    memset(now_book.keyword, 0, 60);
    strcpy(now_book.keyword, _keyword.c_str());
    DivideKeyword(_keyword, ans_keyword);
    for (auto& i : ans_keyword) {
        keyword_pos.insert(Element<int>{i, pos});
    }
    WriteBook(pos, now_book);
}

void BookSystem::ModifyBookPrice(const double &_price) {
    int pos = online.back().book_pos;
    ReadBook(pos, now_book);
    now_book.price = _price;
    WriteBook(pos, now_book);
}

void BookSystem::ImportBook(const int &_quantity, const double &_totalcost) {
    if (GetPrivilege() < 3) {
        throw Exception("Invalid");
    }
    int pos = online.back().book_pos;
    if (!pos) {
        throw Exception("Invalid");
    }
    ReadBook(pos, now_book);
    now_book.quantity += _quantity;
    ReadDeal(count, now_deal);
    now_deal.outcome += _totalcost;
    WriteDeal(++count, now_deal);
    WriteBook(pos, now_book);
}

BookSystem::~BookSystem() {
    book_data.close();
}

#endif //BOOKSTORE_BOOKSYSTEM_CPP