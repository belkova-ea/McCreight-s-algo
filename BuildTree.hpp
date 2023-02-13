#pragma once

#include <iostream>
#include <new> 
#include <string>
#include <cstddef> 
#include <algorithm>  
#include <cstdio>
#include <time.h>
#include <fstream>
#include <chrono>

#define MAX_CHAR 27 // размер алфавита 

using namespace std;

struct treenode
{
    static unsigned long nodes_allocated; // счётчик вершин
    struct treenode* children[MAX_CHAR];
    int start; 
    int end; 
    struct treenode* parent; // указатель на родителя
    struct treenode* suffixlink; // указатель на суффиксную ссылку
     
    treenode() { // конструктор класса
        treenode::nodes_allocated++;
        for (int s = 0; s < MAX_CHAR; ++s) {
            children[s] = NULL; // создаём массив детей
        }
        parent = NULL;
        suffixlink = NULL;
        start = -1; // начальный индекс
        end = -1; // конечный индекс
    }

    string chars(string T) {
        if (start != -1) { // если не корень дерева
            return T.substr(start, end - start + 1); // substr предопределенная функция, возвращает подстроку (позиция 1 символа, длина подстроки)
        }
        else {
            return ""; // если корень возвращаем пустую строку
        }
    }

    ~treenode() {} // деструктор класса
};

struct node_point
{
    int pstart;
    int pend;
    node_point(struct treenode* node) { // конструктор класса ... node - вершина в дереве
        pstart = node->start;   
        pend = node->end;
    }
    ~node_point() {} // деструктор класса
};

// Функция возвращает узел, где завершился переход по дереву
treenode* jumpdown(treenode* node, node_point beta, const string& T);

// Функция возвращает узел, где завершился спуск по дереву
treenode* walkdown(treenode* node, node_point& gamma, int& p, const string& T);

// Функция построения дерева
treenode* build_suffix_tree(const string& T, int len);