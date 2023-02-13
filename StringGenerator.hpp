#pragma once
#include <numeric>
#include <random>
#include <vector>
#include <iostream>
#include <iterator>

using namespace std;

namespace impl {

    class Randomer {
        // random seed
        mt19937 gen_; // является очень эффективным генератором псевдослучайных чисел
        uniform_int_distribution<size_t> dist_; // функция-которая выдает случайные целые числа или дискретные значения из заданного входного диапазона с одинаковой вероятностью.
        // size_t — беззнаковый тип для представления размера любого объекта
    public:
        Randomer() : Randomer(0, numeric_limits<size_t>::max(), random_device{}()) {  // конструктор класса и список инициализации элементов

        }
        // numeric_limits Шаблон класса описывает арифметические свойства встроенных числовых типов.
        // random_device формирует случайную последовательность с помощью внешнего устройства.

        explicit Randomer(size_t max) //Конструкторы преобразования определяют преобразование из пользовательских или встроенных типов в пользовательские типы. 
            : Randomer(0, max, random_device{}()) {
        }

        Randomer(size_t min, size_t max, unsigned int seed = random_device{}())
            : gen_{ seed }, dist_{ min, max } {
        }

        size_t operator()() {
            return dist_(gen_);
        }
    };

    // Алфавит из которого генерируются строки
    static vector<char> alphabet{ 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j',
                                            'k', 'l', 'm', 'n','o','p', 'q', 'r', 's', 't', 'u', 'v',
                                            'w', 'x', 'y', 'z' };
    // static - класс хранения

    static Randomer randomer{ alphabet.size() - 1 }; // экземпляр класса

    
}  // namespace impl

string GenerateString(size_t len);
   