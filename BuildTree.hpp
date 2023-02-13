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

#define MAX_CHAR 27 // ������ �������� 

using namespace std;

struct treenode
{
    static unsigned long nodes_allocated; // ������� ������
    struct treenode* children[MAX_CHAR];
    int start; 
    int end; 
    struct treenode* parent; // ��������� �� ��������
    struct treenode* suffixlink; // ��������� �� ���������� ������
     
    treenode() { // ����������� ������
        treenode::nodes_allocated++;
        for (int s = 0; s < MAX_CHAR; ++s) {
            children[s] = NULL; // ������ ������ �����
        }
        parent = NULL;
        suffixlink = NULL;
        start = -1; // ��������� ������
        end = -1; // �������� ������
    }

    string chars(string T) {
        if (start != -1) { // ���� �� ������ ������
            return T.substr(start, end - start + 1); // substr ���������������� �������, ���������� ��������� (������� 1 �������, ����� ���������)
        }
        else {
            return ""; // ���� ������ ���������� ������ ������
        }
    }

    ~treenode() {} // ���������� ������
};

struct node_point
{
    int pstart;
    int pend;
    node_point(struct treenode* node) { // ����������� ������ ... node - ������� � ������
        pstart = node->start;   
        pend = node->end;
    }
    ~node_point() {} // ���������� ������
};

// ������� ���������� ����, ��� ���������� ������� �� ������
treenode* jumpdown(treenode* node, node_point beta, const string& T);

// ������� ���������� ����, ��� ���������� ����� �� ������
treenode* walkdown(treenode* node, node_point& gamma, int& p, const string& T);

// ������� ���������� ������
treenode* build_suffix_tree(const string& T, int len);