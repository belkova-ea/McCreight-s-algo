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


//
//int main() {
//
//    srand((unsigned)time(NULL)); // ������ ��������� �������� ��� ���������� ��������������� �����, ������������� rand ��������.
//
//    for (int i = 0; i < 1; i++) {
//        int n = rand() % 100 + 1; // rand - ������� ��������� ��������� �����, �� ������ 100, �� ������ 1
//        auto str = random_string::GenerateString(n);
//        cout << str << endl;
//    }
//
//    return 0;
//}