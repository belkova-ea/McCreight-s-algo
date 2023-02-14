#include <numeric>
#include <random>
#include <vector>
#include <iostream>
#include <iterator>
#include "StringGenerator.hpp"

using namespace std;

// ������� ��������� ������, �� ���� �������� ����� - ����� ������������ ������
string GenerateString(size_t len) {
    string str;
    auto rand_char = []() { return impl::alphabet[impl::randomer()]; };
    generate_n(back_inserter(str), len, rand_char);
    return str;
}