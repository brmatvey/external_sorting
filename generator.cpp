#include "generator.hpp"
#include <random>



Generator::Generator(int row_lenght_min, int row_lenght_max, int row_count)
    : row_lenght_min_(row_lenght_min)
    , row_lenght_max_(row_lenght_max)
    , row_count_(row_count)
{

}

void Generator::generateFile(std::ostream& out) const {
    for (int i = 0; i < row_count_; ++i) {
        out << generateString() << '\n';
    }
    out.flush();
}

std::string Generator::generateString() const {
    std::string str;
    std::uniform_int_distribution<int> uid(row_lenght_min_, row_lenght_max_); // random dice
    int string_size = uid(rng); // use rng as a generator
    for (int i = 0; i < string_size; ++i) {
        std::uniform_int_distribution<int> uid_char(96, 122); // random dice
        int char_num = uid_char(rng); // use rng as a generator
        if (char_num == 96) {
            str += ' ';
        } else {
            str += static_cast<char>(char_num);
        }
    }
    return str;
}
