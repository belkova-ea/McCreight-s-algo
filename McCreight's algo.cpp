//#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <new> 
#include <string>
#include <cstddef> 
#include <algorithm>  
#include <cstdio>
#include <time.h>
#include <fstream>

#define MAX_CHAR 27 // размер алфавита 
#define MARKER '$' // знак, который показывает, что позже у узла выведенного в файл не будет дочерних узлов

using namespace std;


struct treenode
{
    struct treenode* children[MAX_CHAR];
    int start;
    int end;
    struct treenode* parent;
    struct treenode* suffixlink;
    treenode() { //constructor
        for (int s = 0; s < MAX_CHAR; ++s) {
            children[s] = NULL;
        }
        parent = NULL;
        suffixlink = NULL;
        start = 0;
        end = 0;
    }
    ~treenode() {} //destructor
};

struct node_point
{
    int pstart;
    int pend;
    node_point(struct treenode* node) {
        pstart = node->start;
        pend = node->end;
    }
    ~node_point() {}
};


treenode* jumpdown(treenode* node, node_point beta, const string&T) {
    treenode* current = node;
    int i = 0;
    while (beta.pstart <= beta.pend) {
        i = 0;
        while (T[current->children[i]->start] != T[beta.pstart]) {  // выбирает путь, куда вставить новый узел
            ++i;
        }
        current = current->children[i];
        beta.pstart = beta.pstart + (current->end - current->start) + 1; 
                                                                       
    }
    if (beta.pstart == beta.pend + 1) {
        return current;
    }
    else { // переход  остнавливается на ребре, необходимо разбить ребро и вставить новый узел 
        treenode* w = new treenode();
        w->start = current->start;
        w->end = current->end - (beta.pstart - beta.pend) + 1;
        w->parent = current->parent;
        current->parent->children[i] = w;
        current->parent = w;
        current->start = w->end + 1;
        w->children[0] = current;
        return w;
    }
}

// Функция возвращает узел, где завершился спуск по дереву
treenode* walkdown(treenode* node, node_point& gamma, int& p, const string& T) {  
                                                                                   
    treenode* current = node;
    treenode* return_node;
    while (gamma.pstart <= gamma.pend) {
        int i = 0;
        while (current->children[i] != NULL && T[current->children[i]->start] != T[gamma.pstart]) { // выбираем путь спуска по дереву
            ++i;
        }
        if (!current->children[i]) { // current->children[i] == NULL, дошли до конца и нашли пустой узел
            cout << "NODE" << "\t\t";
            p = i;
            return_node = current;
            //break;
            return return_node; // завершаем цикл и возвращаем узел, на котором остановились
        }
        else {

            int intermediate = gamma.pstart + (current->children[i]->end - current->children[i]->start);
            int dist = 0;
            while (T[current->children[i]->start + dist] == T[gamma.pstart] && gamma.pstart <= min(intermediate, gamma.pend)) {
                ++dist;
                ++(gamma.pstart);
            }
            if (gamma.pstart > min(intermediate, gamma.pend)) { 
                current = current->children[i];
            }
            else { 
                cout << "EDGE" << "\t\t";
                treenode* new_node_v = new treenode();
                new_node_v->start = current->children[i]->start;
                new_node_v->end = current->children[i]->start + dist - 1;
                current->children[i]->start = current->children[i]->start + dist;
                current->children[i]->parent = new_node_v;
                new_node_v->children[0] = current->children[i];
                new_node_v->parent = current;
                p = 1;
                current->children[i] = new_node_v;
                return_node = new_node_v;
                //break;
                return return_node;
            }
        }
    }
   
}

treenode* build_suffix_tree(const string& T, int len) {

    cout << "j" << "\t" << "jd" << "\t" << "jdstops" << "\t" << "jdstart" << "\t" <<
        "jdend" << "\t" << "wd" << "\t" << "wdstops" << "\t" << "wdstart" << "\t" <<
        "wdend" << "\t" << "leafstart" << "\t" << "leafend" << endl;

    /*
    j - index
    jd - jumpdown переход по дереву 
    jdstops - где остановился переход по дереву (ребро или узел)
    jdstart - индекс узла, где начался переход по дереву
    jdend - индекс узла, где закончился переход по дереву
    wd - walkdown спуск по дереву
    wdstops - где остановился спуск по дереву (ребро или узел)
    wdstart - индекс узла, откуда начался спуск
    wdend - индекс узла, где закончился спуск
    leafstart - показывает стартовый индекс листа
    leafend - показыывает последний индекс листа
    */

    
    treenode* root = new treenode();
    root->parent = root;
    root->suffixlink = root;
    treenode* leafnode0 = new treenode();
    root->children[0] = leafnode0;
    leafnode0->parent = root;
    leafnode0->start = 0;
    leafnode0->end = len;

    
    treenode* node_v_j;
    node_v_j = root;
    treenode* w;

    node_point beta = node_point(root);
    node_point gamma = node_point(leafnode0);

    for (int j = 1; j <= len; ++j) { // вставка суффикса j 

        cout << j << "\t";
        treenode* current_leafnode = new treenode(); // "current_leafnode" это узел куда будет вставлен суффикс на этом шаге
        current_leafnode->start = j;
        current_leafnode->end = len;

        int I = 1; 
        if (node_v_j == root) {// не нужен переход по дереву
            ++gamma.pstart;
            I = 0;
        }
        else if (node_v_j->parent == root) {
            if (beta.pstart == beta.pend) {
                I = 0;
            }
            else {// нужно перейти по дереву и найти куда вставить суффикс
                ++beta.pstart;
            }
        }


        if (I == 0) {
            // Переход не нужен, можно прости пройти по дереву
            cout << "NO" << "\t" << "NA" << "\t\t" << "NA" << "\t" << "NA" << "\t" << "YES" << "\t";
            int p = 0;
            treenode* new_node_v;
            new_node_v = walkdown(root, gamma, p, T);
            new_node_v->children[p] = current_leafnode;
            current_leafnode->parent = new_node_v;
            current_leafnode->start = gamma.pstart;
            if (beta.pstart == beta.pend && beta.pstart != -1) {
                node_v_j->suffixlink = root;
            }

            cout << new_node_v->start << "\t" << new_node_v->end << "\t" << current_leafnode->start << "\t\t" << current_leafnode->end << endl;

        }
        else { // Нужен переход по суффиксной ссылке
            cout << "YES" << "\t";
            w = jumpdown(node_v_j->parent->suffixlink, beta, T);
            node_v_j->suffixlink = w;

            if (!w->children[1]) { // w->children[1] = NULL; w - узел где переход завершился пустой, вставляем суффикс
                w->children[1] = current_leafnode;
                current_leafnode->start = gamma.pstart;
                current_leafnode->parent = w;
                cout << "EDGE" << "\t\t" << w->start << "\t" << w->end << "\t" <<
                    "NO" << "\t" << "NA" << "\t\t" << "NA" << "\t" << "NA" << "\t" << current_leafnode->start << "\t\t"
                    << current_leafnode->end << endl;
            }
            else {  // переход завершился в сушествующем узле, нужно спустится по дереву и добавить новый узел
                cout << "NODE" << "\t\t" << w->start << "\t" << w->end << "\t" << "YES" << "\t";
                int p = 0;
                treenode* new_node_v;
                new_node_v = walkdown(w, gamma, p, T);
                new_node_v->children[p] = current_leafnode;
                current_leafnode->parent = new_node_v;
                current_leafnode->start = gamma.pstart;
                cout << new_node_v->start << "\t" << new_node_v->end << "\t" << current_leafnode->start << "\t\t" << current_leafnode->end << endl;
            }
        }

        gamma = node_point(current_leafnode);
        beta = node_point(current_leafnode->parent);
        node_v_j = current_leafnode->parent;
    }
    return root;
}

//вывод результатов индексирования в файл
void serialize(treenode* root, FILE* fp)
{
    if (root == NULL) return;

    
    fprintf(fp, "%i %i ", root->start, root->end);
    for (int i = 0; i < MAX_CHAR && root->children[i] != NULL; i++) { serialize(root->children[i], fp); }

    fprintf(fp, "%c ", MARKER);
}

int main(int argc, char* argv[])
{
    clock_t start, end;
    double duration;
    start = clock(); // получаем стартовое время

    string T;

    ifstream f1;
    f1.open("filename.txt");

    if (!f1.is_open()) {
        cout << "Can't open file" << endl; // если не открылся
        return EXIT_FAILURE;
    }
    else if (f1.peek() == EOF) {
        cout << "File is empty" << endl; // если первый символ конец файла
        return EXIT_FAILURE;
    }

    f1 >> T;

    int len = T.length();
    T.push_back('$');

    

    treenode* root = build_suffix_tree(T, len);

    end = clock(); // получаем время по окончании построения дерева
    duration = ((double)end - start) / CLOCKS_PER_SEC; // подсчитываем время выполнения алгоритма

    /* время выполнения алгоритма в худшем случае линейно O(n), где n - количество символов в строке без учёта $ */
    ofstream f2;
    f2.open("info.txt");
    f2 << "Длина исходной строки: " << len << endl << "Исходная строка со знаком $: " << T << endl;
    f2 << "Время построения суффиксного дерева: " << duration << " seconds" << endl;

    FILE* fp;
    fopen_s(&fp, "tree.txt", "w");

    serialize(root, fp);
}
