#include <iostream>
#include <new> 
#include <string>
#include <cstddef> 
#include <algorithm>  
#include <cstdio>
#include <fstream>
#include <iomanip>
#include <chrono>
#include "BuildTree.hpp"
#include "StringGenerator.hpp"


using namespace std;


// объявление узла в графике
string print_node(treenode* node, string T) {
    string s = to_string((unsigned long)node); // указатель id узла
    s += " [label = \"" + node->chars(T) + "\"];"; // подстрока
    return s;
}

// вывод результатов в файл
void serialize_(treenode* root, FILE* fp, string T)
{
    if (root == NULL) return; // если дерево пустое, выходим

    for (int i = 0; i < MAX_CHAR && root->children[i] != NULL; i++) // обход детей в дереве
    { 
        fprintf(
            fp,
            "%s\n", // суффикс
            print_node(root->children[i], T).c_str()
        );
        fprintf( // ребро
            fp,
            "%lu -> %lu;\n", // id ноды - id ребенка ноды
            (unsigned long)root,
            (unsigned long)root->children[i]
        );
        serialize_(root->children[i], fp, T); 
    }

}

// графическое построение дерева (dot)
void serialize(treenode* root, FILE* fp, string T) {
    fprintf(fp, "digraph G {\n"); // необходимо для графического построения дерева
    fprintf(
        fp,
        "%s\n",
        print_node(root, T).c_str() //вывод корня дерева
    );
    serialize_(root, fp, T);
    fprintf(fp, "}\n");
}

// тестирование. сравниваем подстроки из строки и то, что получилось в дереве
bool test_(int i, treenode* node, string T) {
    for (int j = 0; j < node->chars(T).length(); i++, j++) {
        if (T[i] != node->chars(T)[j]) { // если нашли несовпадение выходим, дерево построено неверно
            return false;
        }
        if (T[i] == '$') { // если дошли до конца строки и не зашли в первый if, значит дерево построено верно
            return true;
        }
    }
    for (int k = 0; k < MAX_CHAR && node->children[k] != NULL; k++) {
        if (test_(i, node->children[k], T)) { // вызываем функцию
            return true;
        }
    }
    return false;
}

bool test(string T) {
    int len = T.length();
    T.push_back('$');
    treenode* root = build_suffix_tree(T, len);

    for (int i = 0; i < len; i++) {
        if (!test_(i, root, T)) {
            return false;
        }
    }
    return true;
}

int main(/*int argc, char* argv[]*/)
{
    setlocale(LC_ALL, "Russian");

    string T;
    unsigned long sum = 0;
    
    int command;
    cout << "1. Запустить тест из файла" << endl;
    cout << "2. Запустить ряд тестов из файла" << endl;
    cout << "3. Запустить генератор тестов" << endl << endl;
    cout << "Введите команду: ";
    cin >> command;

    if (command == 1) {

        ifstream f1;
        f1.open("single_str_file.txt");

        if (!f1.is_open()) {
            cout << "Can't open file" << endl; // если не открылся
            return EXIT_FAILURE;
        }
        else if (f1.peek() == EOF) {
            cout << "File is empty" << endl; // если первый символ конец файла
            return EXIT_FAILURE;
        }

        f1 >> T;

        auto t_start = chrono::high_resolution_clock::now(); // получаем стартовое время

        int len = T.length();
        T.push_back('$');

        treenode* root = build_suffix_tree(T, len);

        auto t_end = chrono::high_resolution_clock::now(); // получаем время по окончании построения дерева
        double elapsed_time_ms = chrono::duration<double, milli>(t_end - t_start).count(); // подсчитываем время выполнения алгоритма
        /* время выполнения алгоритма в худшем случае линейно O(n), где n - количество символов в строке*/

        ofstream f2;
        f2.open("single_test_res.txt");

        f2 << "Длина исходной строки: " << len << endl << "Исходная строка со знаком $: " << T << endl;
        f2 << "Время построения суффиксного дерева: " << elapsed_time_ms << " ms" << endl;
        f2 << "Затраты по памяти: " << treenode::nodes_allocated * sizeof(treenode) << " байт" << endl; 
        // затраты по памяти линейны и зависят от количества узлов, так как память выделяется под узлы дерева


        if (test(T)) {
            f2 << "Прохождение теста: " << "true" << endl;
        }
        else f2 << "Прохождение теста: " << "false" << endl;

        f2.close();

        FILE* fp;
        fopen_s(&fp, "tree.txt", "w");

        serialize(root, fp, T);
    }
    else if (command == 2) {
       
    // тесты из файла
        ifstream f1;
        f1.open("test_strings.txt");

        if (!f1.is_open()) {
            cout << "Can't open file" << endl; // если не открылся
            return EXIT_FAILURE;
        }
        else if (f1.peek() == EOF) {
            cout << "File is empty" << endl; // если первый символ конец файла
            return EXIT_FAILURE;
        }
        
        ofstream f_test;
        f_test.open("info_tests.txt");

        while (getline(f1, T)) { // пока не достигнут конец файла класть очередную строку в переменную T

            auto t_start = chrono::high_resolution_clock::now(); // получаем стартовое время

            int len = T.length();
            T.push_back('$');


            treenode* root = build_suffix_tree(T, len);


            auto t_end = chrono::high_resolution_clock::now(); // получаем время по окончании построения дерева
            double elapsed_time_ms = chrono::duration<double, milli>(t_end - t_start).count(); // подсчитываем время выполнения алгоритма
            /* время выполнения алгоритма в худшем случае линейно O(n), где n - количество символов в строке*/

            f_test << "Длина исходной строки: " << len << endl << "Исходная строка со знаком $: " << T << endl;
            f_test << "Время построения суффиксного дерева: " << elapsed_time_ms << " ms" << endl;
            f_test << "Затраты по памяти: " << (treenode::nodes_allocated * sizeof(treenode)) - sum << " байт" << endl;
            // затраты по памяти линейны и зависят от количества узлов, так как память выделяется под узлы дерева

            sum += treenode::nodes_allocated * sizeof(treenode) - sum;

            if (test(T)) {
                f_test << "Прохождение теста: " << "true" << endl;
            }
            else f_test << "Прохождение теста: " "false" << endl;

            f_test << endl;

        }
    }
    else if (command == 3) {
        // генератор тестов

        ofstream auto_t_file;
        auto_t_file.open("auto_tests_result.txt");

        srand((unsigned)time(NULL)); // Задает начальное значение для генератора псевдослучайных чисел, используемого rand функцией.
        
        int amount;
        amount = rand() % 100 + 10;

        // Можно раскомментировать и вводить количество тестов с консоли
        //
        /*cout << "Сколько тестов вы хотите запустить: ";
        cin >> amount;
        if (amount <= 0) {
            cout << "Пожалуйста введите число больше 0";
            cin >> amount;
        }*/

        for (int i = 0; i < amount; i++) {

            int n = rand() % 100 + 1; // rand - функция генерации случайных чисел, не больше 100, не меньше 1
            T = GenerateString(n);

            auto t_start = chrono::high_resolution_clock::now(); // получаем стартовое время

            int len = T.length();
            T.push_back('$');

            treenode* root = build_suffix_tree(T, len);

            auto t_end = chrono::high_resolution_clock::now(); // получаем время по окончании построения дерева
            double elapsed_time_ms = chrono::duration<double, milli>(t_end - t_start).count(); // подсчитываем время выполнения алгоритма
            /* время выполнения алгоритма в худшем случае линейно O(n), где n - количество символов в строке*/

            auto_t_file << "Длина исходной строки: " << len << endl << "Исходная строка со знаком $: " << T << endl;
            auto_t_file << "Время построения суффиксного дерева: " << elapsed_time_ms << " ms" << endl;
            auto_t_file << "Затраты по памяти: " << treenode::nodes_allocated * sizeof(treenode) - sum << " байт" << endl;
            // затраты по памяти линейны и зависят от количества узлов, так как память выделяется под узлы дерева

            sum += treenode::nodes_allocated * sizeof(treenode) - sum;

            if (test(T)) {
                auto_t_file << "Прохождение теста: " << "true" << endl << endl;
            }
            else auto_t_file << "Прохождение теста: " << "false" << endl << endl;

        }

        auto_t_file.close();
    }
}
