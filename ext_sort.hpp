#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <string>

// принцип сортировки НЕ в оперативной памяти
// делим файл на подфайлы, читая и сохраняя на диск подфайлы по row_count строк
// при сохранении подфайлов размера row_count используется встроенная сортировка std::sort
// при сортировке частей (чанков) используется сортировка слиянием (ну а какая же ещё)
// рекурсивно сливаем чанки во всё большие и большие
// получаем конечный файл
// временные чанки вместе с директорией удаляются

namespace ext {
void sort(std::ifstream& in, std::ofstream& out);
} // namespace ext



