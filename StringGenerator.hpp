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
        mt19937 gen_; // �������� ����� ����������� ����������� ��������������� �����
        uniform_int_distribution<size_t> dist_; // �������-������� ������ ��������� ����� ����� ��� ���������� �������� �� ��������� �������� ��������� � ���������� ������������.
        // size_t � ����������� ��� ��� ������������� ������� ������ �������
    public:
        Randomer() : Randomer(0, numeric_limits<size_t>::max(), random_device{}()) {  // ����������� ������ � ������ ������������� ���������

        }
        // numeric_limits ������ ������ ��������� �������������� �������� ���������� �������� �����.
        // random_device ��������� ��������� ������������������ � ������� �������� ����������.

        explicit Randomer(size_t max) //������������ �������������� ���������� �������������� �� ���������������� ��� ���������� ����� � ���������������� ����. 
            : Randomer(0, max, random_device{}()) {
        }

        Randomer(size_t min, size_t max, unsigned int seed = random_device{}())
            : gen_{ seed }, dist_{ min, max } {
        }

        size_t operator()() {
            return dist_(gen_);
        }
    };

    // ������� �� �������� ������������ ������
    static vector<char> alphabet{ 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j',
                                            'k', 'l', 'm', 'n','o','p', 'q', 'r', 's', 't', 'u', 'v',
                                            'w', 'x', 'y', 'z' };
    // static - ����� ��������

    static Randomer randomer{ alphabet.size() - 1 }; // ��������� ������

    
}  // namespace impl

string GenerateString(size_t len);
   