#pragma once

#include <iostream>
#include <string>
#include <random>

// random device engine, usually based on /dev/random on UNIX-like systems
inline static std::random_device rd;
// initialize Mersennes' twister using rd to generate the seed
inline static std::mt19937 rng{rd()};

class Generator {
public:
    Generator() = delete;
    Generator(int row_lenght_min, int row_lenght_max, int row_count);
    // генерирует файл необходимых параметров
    void generateFile(std::ostream& out) const;
private:
    std::string generateString() const;
    int row_lenght_min_;
    int row_lenght_max_;
    int row_count_;
};
