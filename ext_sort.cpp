#include "ext_sort.hpp"
#include <filesystem>
#include <algorithm>

namespace ext {

std::filesystem::path operator""_p(const char* data, std::size_t sz) {
    return std::filesystem::path(data, data + sz);
}

void printToFile(std::ofstream& out, const std::vector<std::string>& array) {
    for (const std::string& s : array) {
        out << s << "\n";
    }
}

std::vector<std::string> readNRowsWithoutSort(std::ifstream& in, int row_count) {
    std::string s;
    std::vector<std::string> array;
    array.reserve(row_count);
    while (array.size() < row_count && getline(in, s)) {
        array.push_back(std::move(s));
    }
    return array;
}

std::vector<std::string> readNRows(std::ifstream& in, int row_count) {
    std::vector<std::string> array = readNRowsWithoutSort(in, row_count);
    std::sort(array.begin(), array.end());
    return array;
}

size_t splitFile(std::ifstream& in, int row_count) {
    size_t count_files = 0;
    std::vector<std::string> array = readNRows(in, row_count);
    while (!array.empty()) {
        std::ofstream out("temp/temp_0_" + std::to_string(count_files));
        printToFile(out, array);
        ++count_files;
        array = readNRows(in, row_count);
    }
    return count_files;
}

void merge(std::ifstream& in1, std::ifstream& in2, std::ofstream& out) {
    // сливаем два файла в один
    // реализуем простенькую буферизацию чтобы не гонять туда-сюда потоки ввода вывода
    // реализация без буферизации ниже
    std::vector<std::string> buffin1 = readNRowsWithoutSort(in1, 10000);
    std::vector<std::string> buffin2 = readNRowsWithoutSort(in2, 10000);
    std::vector<std::string> buffout;
    buffout.reserve(20000);
    std::vector<std::string>::iterator it1 = buffin1.begin();
    std::vector<std::string>::iterator it2 = buffin2.begin();
    while (!(buffin1.empty() && buffin2.empty())) {
        if (it1 != buffin1.end() && it2 != buffin2.end()) {
            if (*it1 < *it2) {
                buffout.push_back(std::move(*it1));
                ++it1;
            } else {
                buffout.push_back(std::move(*it2));
                ++it2;
            }
        } else if (it1 == buffin1.end() && it2 != buffin2.end()) {
            // пишем из буфера 2, так как первый уже закончился
            buffout.push_back(std::move(*it2));
            ++it2;
        } else if (it1 != buffin1.end() && it2 == buffin2.end()) {
            // пишем из буфера 1, так как второй уже закончился
            buffout.push_back(std::move(*it1));
            ++it1;
        }
        if (buffout.size() == 20000) {
            printToFile(out, buffout);
            buffout.clear();
        }
        if (it1 == buffin1.end()) {
            buffin1 = readNRowsWithoutSort(in1, 10000);
            it1 = buffin1.begin();
        }
        if (it2 == buffin2.end()) {
            buffin2 = readNRowsWithoutSort(in2, 10000);
            it2 = buffin2.begin();
        }
    }
    printToFile(out, buffout);
    /*
    std::string s1;
    std::string s2;
    getline(in1, s1);
    getline(in2, s2);
    while (!(s1.empty() && s2.empty())) {
        if (!s1.empty() && (s2.empty() || s1 < s2)) {
            out << s1 << std::endl;
            if (!getline(in1, s1)) {
                s1.clear();
            }
        } else {
            out << s2 << std::endl;
            if (!getline(in2, s2)) {
                s2.clear();
            }
        }
    }
    */
}

void recurMerge(size_t count_files, int level, std::ofstream& out) {
    using namespace std::literals;
    // не забудем удалить предыдущие файлы (чтобы не засорять память)
    for (size_t i = 0; i < count_files * 2; ++i) {
        std::filesystem::remove("temp"_p / ("temp_"s + std::to_string(level - 1) + "_"s + std::to_string(i)));
    }
    // условие выхода из рекурсивной функции - осталось только два чанка
    if (count_files < 3) {
        std::ifstream in1("temp/temp_"s + std::to_string(level) + "_0"s);
        std::ifstream in2("temp/temp_"s + std::to_string(level) + "_1"s);
        merge(in1, in2, out);
        return;
    }
    // сливаем по два чанка
    for (size_t i = 0; i < count_files; i += 2) {
        std::ifstream in1("temp/temp_"s + std::to_string(level) + "_"s + std::to_string(i));
        std::ifstream in2("temp/temp_"s + std::to_string(level) + "_"s + std::to_string(i + 1));
        std::ofstream out1("temp/temp_"s + std::to_string(level + 1) + "_"s + std::to_string(i / 2));
        merge(in1, in2, out1);
    }
    // рекурсивно запускаем
    recurMerge((count_files + 1)/ 2, level + 1, out);
}

void sort(std::ifstream& in, std::ofstream& out) {
    using namespace std::literals;
    // создадим директорию для временного хранилища (в ОЗУ то не влезает)
    std::filesystem::create_directory("temp"s);
    // нарезаем исходный файл на count_files файлов по 100 строк
    size_t count_files = splitFile(in, 10000);
    recurMerge(count_files, 0, out);
    // вычищаем папку сборки от временных файлов со слиянием
    std::filesystem::remove_all("temp"s);
}

} // namespace ext
