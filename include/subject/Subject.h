#pragma once
#include <iostream>
#include <string>

class Subject {
public:
    int f3(int arg1, int arg2) {
        return arg1 + arg2;
    }

    void print_data(const std::string& label, double value) {
        std::cout << "Subject: " << label << " = " << value << std::endl;
    }
};