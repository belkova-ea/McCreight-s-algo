#include <numeric>
#include <random>
#include <vector>
#include <iostream>
#include <iterator>
#include "StringGenerator.hpp"

using namespace std;



// функция генерации строки, на вход получает число - длину генерируемой строки
string GenerateString(size_t len) {
    string str;
    auto rand_char = []() { return impl::alphabet[impl::randomer()]; };
    generate_n(back_inserter(str), len, rand_char);
    return str;
}


//
//int main() {
//
//    srand((unsigned)time(NULL)); // Задает начальное значение для генератора псевдослучайных чисел, используемого rand функцией.
//
//    for (int i = 0; i < 1; i++) {
//        int n = rand() % 100 + 1; // rand - функция генерации случайных чисел, не больше 100, не меньше 1
//        auto str = random_string::GenerateString(n);
//        cout << str << endl;
//    }
//
//    return 0;
//}