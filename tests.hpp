#pragma once

#include <cassert>
#include <fstream>
#include <algorithm>
#include "generator.hpp"
#include "ext_sort.hpp"

using namespace std::literals;

namespace tests {

std::vector<std::string> readFile(std::ifstream& in) {
    std::string s;
    std::vector<std::string> array;
    while (getline(in, s)) {
        array.push_back(std::move(s));
    }
    return array;
}

void simple() {
    // простой тест где проверяем с сортировкой
    Generator g(100, 1000, 976);
    {
        std::ofstream out("test_in.txt"s);
        g.generateFile(out);
    }
    {
        std::ifstream in("test_in.txt"s);
        std::ofstream out("test_out.txt"s);
        ext::sort(in, out);
    }
    {
        std::ifstream in("test_in.txt"s);
        std::ifstream out("test_out.txt"s);
        auto in_str = readFile(in);
        std::sort(in_str.begin(),in_str.end());
        auto out_str = readFile(out);
        assert(in_str == out_str);
    }
}

void medium() {
    // более нагруженный тест тест где проверяем с сортировкой
    Generator g(100, 1000, 97865);
    {
        std::ofstream out("test_in_medium.txt"s);
        g.generateFile(out);
    }
    {
        std::ifstream in("test_in_medium.txt"s);
        std::ofstream out("test_out_medium.txt"s);
        ext::sort(in, out);
    }
    {
        std::ifstream in("test_in_medium.txt"s);
        std::ifstream out("test_out_medium.txt"s);
        auto in_str = readFile(in);
        std::sort(in_str.begin(),in_str.end());
        auto out_str = readFile(out);
        assert(in_str == out_str);
    }
}

void hard() {
    // самый нагруженный тест тест. Не сравниваем результаты
    // (так как на это уйдёт ОЧЕНЬ много времени), а просто смотрим, что ничего не падает
    // и не выбрасываются исключения
    Generator g(10, 20, 100000000);
    {
        std::ofstream out("test_in_hard.txt"s);
        g.generateFile(out);
    }
    {
        std::ifstream in("test_in_hard.txt"s);
        std::ofstream out("test_out_hard.txt"s);
        try {
            ext::sort(in, out);
        }  catch (...) {
            assert(false);
        }
    }
}

}


