#include <iostream>
#include <fstream>
#include "ext_sort.hpp"
#include "tests.hpp"

using namespace std::literals;

int main() {
    // тесты
    tests::simple();
    tests::medium();
    //tests::hard(); // этот тест проходится, но всё вешает к чертям на часик другой
    std::cout << "Tests finished correctly"s << std::endl;
    // для пользователя
    std::ifstream in("in.txt"s);
    std::ofstream out("out.txt"s);
    ext::sort(in, out);
    return 0;
}
