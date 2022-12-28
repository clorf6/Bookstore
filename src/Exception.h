//
// Created by 屋顶上的小丑 on 2022/12/24.
//

#ifndef BOOKSTORE_EXCEPTION_H
#define BOOKSTORE_EXCEPTION_H

#include <iostream>
#include <cstring>

class Exception {
private:
    const std::string message;
public:
    Exception(const std::string &str) : message(str) {}

    const std::string what() { return message; }
};

#endif //BOOKSTORE_EXCEPTION_H