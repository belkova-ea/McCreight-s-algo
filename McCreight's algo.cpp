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


//using namespace std;

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

// графическое построение дерева (dot graphviz)
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

// тестирование. сравниваем подстроки из исходной строки и то, что получилось в дереве
bool test_(int i, treenode* node, string T) {
    for (int j = 0; j < node->chars(T).length(); i++, j++) {
        if (T[i] != node->chars(T)[j]) { // если нашли несовпадение суффикса выходим, дерево построено неверно
            return false;
        }
        if (T[i] == '$') { // если дошли до конца строки и не зашли в первый if, значит дерево построено верно
            return true;
        }
    }
    for (int k = 0; k < MAX_CHAR && node->children[k] != NULL; k++) { // проходим по массиву детей
        if (test_(i, node->children[k], T)) { // вызываем функцию
            return true;
        }
    }
    return false;
}

// тестирование на правильное построение дерева
bool test(string T) {
    int len = T.length();
    T.push_back('$');
    treenode* root = build_suffix_tree(T, len);

    for (int i = 0; i < len; i++) {
        return test_(i, root, T);         
    }
    return true;
}

bool find_string_(int i, treenode* node, string T, string S) {
    for (int j = 0; j < node->chars(T).length(); i++, j++) {
        if (S[i] != node->chars(T)[j]) { // если нашли несовпадение выходим
            return false;
        }
        if (i == S.length()-1) { // если дошли до конца строки и не зашли в первый if, значит подстрока найдена
            return true;
        }
    }   
    for (int k = 0; k < MAX_CHAR && node->children[k] != NULL; k++) { // проходим по массиву детей
        if (find_string_(i, node->children[k], T, S)) { // вызываем функцию
            return true;
        }
    }
    return false;
}

// функция поиска подстроки в строке
bool find_string(treenode* node, string T, string S) {
    return find_string_(0, node, T, S);
}

int main()
{
    setlocale(LC_ALL, "Russian");

    string T, M;
    
    int command;
    cout << "1. Запустить тест из файла" << endl;
    cout << "2. Запустить генератор тестов" << endl;
    cout << "3. Тесты поиска подстроки" << endl << endl;
    
    cout << "Введите команду: ";
    cin >> command;

    if (command == 1) { // single_str_file одна строка и поиск подстроки для неё

        ifstream f1;
        f1.open("single_str_file.txt");

        ofstream f2;
        f2.open("single_test_res.txt");

        if (!f1.is_open()) {
            cout << "Can't open file" << endl; // если не открылся
            return EXIT_FAILURE;
        }
        else if (f1.peek() == EOF) {
            cout << "File is empty" << endl; // если первый символ конец файла
            return EXIT_FAILURE;
        }

        f1 >> T;

        f1.close();

        auto t_start = chrono::high_resolution_clock::now(); // получаем стартовое время

        int len = T.length();
        T.push_back('$');

        treenode* root = build_suffix_tree(T, len);

        auto t_end = chrono::high_resolution_clock::now(); // получаем время по окончании построения дерева
        double elapsed_time_ms = chrono::duration<double, milli>(t_end - t_start).count(); // подсчитываем время выполнения алгоритма
        /* время выполнения алгоритма O(n), где n - количество символов в строке*/

        f2 << "Длина исходной строки: " << len << endl << "Исходная строка со знаком $: " << T << endl;
        f2 << "Время построения суффиксного дерева: " << elapsed_time_ms << " ns" << endl;
        f2 << "Затраты по памяти: " << treenode::nodes_allocated * sizeof(treenode) << " байт" << endl; 
        // затраты по памяти линейны и зависят от количества узлов, так как память выделяется под узлы дерева

        if (test(T)) {
            f2 << "Прохождение теста на правильное построение дерева: " << "true" << endl;
        }
        else f2 << "Прохождение теста на правильное построение дерева: " << "false" << endl;

        f2.close();

        FILE* fp;
        fopen_s(&fp, "tree.txt", "w");

        serialize(root, fp, T);
        
        cout << "\n" << "Построение дерева завершено" << "\n" << "Результаты в файле single_test_res.txt" << "\n\n";


        cout << "Введите строку, которую нужно найти: ";
        cin >> M;

        auto time_s = chrono::high_resolution_clock::now();

        if (find_string(root, T, M)) {
            cout << "\n" << "Подстрока найдена" << endl;
            auto time_e = chrono::high_resolution_clock::now(); // получаем время по окончании построения дерева
            double elapsed_time = chrono::duration<double, milli>(time_e - time_s).count(); // подсчитываем время выполнения алгоритма
            cout << "Время поиска: " << elapsed_time << "ns" << endl;
        }
        else cout << "Подстроки в тексте нет" << endl;

    }
    else if (command == 2) { // генератор тестов

        ofstream auto_t_file;
        auto_t_file.open("auto_tests_result.txt");

        mt19937 rnd(4321);
        
        int amount = 50;
      
        auto_t_file << "РЕЗУЛЬТАТЫ АВТОМАТИЧЕСКИХ ТЕСТОВ" << "\n\n";

        for (int i = 0; i < amount; i++) {

            int n = rnd() % 100 + 1; // rnd - генерирует случайное число, не больше 100, не меньше 1
            T = GenerateString(n);

            auto t_start = chrono::high_resolution_clock::now(); // получаем стартовое время

            int len = T.length();
            T.push_back('$');

            treenode* root = build_suffix_tree(T, len);

            auto t_end = chrono::high_resolution_clock::now(); // получаем время по окончании построения дерева
            double elapsed_time_ms = chrono::duration<double, milli>(t_end - t_start).count(); // подсчитываем время выполнения алгоритма

            /* время выполнения алгоритма O(n), где n - количество символов в строке*/

            auto_t_file << "Длина исходной строки: " << len << endl << "Исходная строка со знаком $: " << T << endl;
            auto_t_file << "Время построения суффиксного дерева: " << elapsed_time_ms << " ns" << endl;
            auto_t_file << "Количество вершин дерева: " << treenode::nodes_allocated << endl;
            auto_t_file << "Память на 1 вершину: " << sizeof(treenode) << endl;
            auto_t_file << "Затраты по памяти: " << treenode::nodes_allocated * sizeof(treenode) << " байт" << endl;
 
            /*затраты по памяти линейны и зависят от количества узлов, так как память выделяется под узлы дерева*/

            if (test(T)) {
                auto_t_file << "Прохождение теста на правильное построение дерева: " << "true" << "\n\n";
            }
            else auto_t_file << "Прохождение теста на правильное построение дерева: " << "false" << "\n\n";

            treenode::nodes_allocated = 0;
        }

        auto_t_file.close();

        cout << "\n" << "Построение деревьев завершено" << "\n" << "Результаты в файле auto_tests_result.txt" << "\n";
    }
    else if (command == 3) { // найти подстроки из файла find_strings в строке из файла _str_file
        
        ifstream file_;
        file_.open("_str_file.txt");

        ifstream file_find;
        file_find.open("find_strings.txt");

        ofstream find_res;
        find_res.open("find_str_results.txt");

        find_res << "РЕЗУЛЬТАТЫ ТЕСТОВ ПОИСКА ПОДСТРОК" << "\n\n";

        file_ >> T;

        auto t_start = chrono::high_resolution_clock::now(); // получаем стартовое время

        int len = T.length();
        find_res << '\n' << "Исходная строка: " << T << endl;
        T.push_back('$');

        treenode* root = build_suffix_tree(T, len);

        auto t_end = chrono::high_resolution_clock::now(); // получаем время по окончании построения дерева
        double elapsed_time_ms = chrono::duration<double, milli>(t_end - t_start).count(); // подсчитываем время выполнения алгоритма
        /* время выполнения алгоритма O(n), где n - количество символов в строке*/

        find_res << "Время построения суффиксного дерева: " << elapsed_time_ms << " ns" << endl;
        find_res << "Затраты по памяти: " << treenode::nodes_allocated * sizeof(treenode) << " байт" << "\n\n";

        if (test(T)) {
            find_res << "Прохождение теста на правильное построение дерева: " << "true" << endl;
        }
        else find_res << "Прохождение теста на правильное построение дерева: " << "false" << endl;

 
        while (getline(file_find, M)) {
            auto time_s = chrono::high_resolution_clock::now();
            find_res << '\n' << "Подстрока: " << M << endl;
            if (find_string(root, T, M)) {
                find_res << "Подстрока найдена" << endl;
                auto time_e = chrono::high_resolution_clock::now(); // получаем время по окончании поиска
                double elapsed_time = chrono::duration<double, milli>(time_e - time_s).count(); // подсчитываем время выполнения
                find_res << "Время поиска: " << elapsed_time << "ns" << endl;
            }
            else
            {
                find_res << "Подстроки в тексте нет" << endl;
                auto time_e = chrono::high_resolution_clock::now(); // получаем время по окончании поиска
                double elapsed_time = chrono::duration<double, milli>(time_e - time_s).count(); // подсчитываем время выполнения
                find_res << "Время поиска: " << elapsed_time << " ns" << endl;
            }
        }

        cout << "\n" << "Построение дерева и поиск подстрок завершены" << "\n" << "Результаты в файле find_str_results.txt" << "\n";

    }
}
