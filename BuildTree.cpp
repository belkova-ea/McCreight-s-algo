#include "BuildTree.hpp"


unsigned long treenode::nodes_allocated = 0;

// ������� ���������� ����, ��� ���������� ������� �� ������ (fastfind)
// �� ������� �����, ��� ���� ������ ��������� ���������� � �����-�� ������������ ���� � �� ����� ������ �� ����������
treenode* jumpdown(treenode* node, node_point beta, const string& T) {
    treenode* current = node;
    int i = 0;
    while (beta.pstart <= beta.pend) {
        i = 0;
        while (T[current->children[i]->start] != T[beta.pstart]) {  // �������� ����, ���� �������� ����� ����
            ++i;
        }
        current = current->children[i]; // ��������� � ������ �������� ����
        beta.pstart = beta.pstart + (current->end - current->start) + 1;

    }
    if (beta.pstart == beta.pend + 1) { // ������ ����� ���������
        return current; // ���������� ������� ����
    }
    else { // �������  �������������� �� �����, ���������� ������� ����� � �������� ����� ���� 
        treenode* w = new treenode(); // ��������� ����� ���� ������
        w->start = current->start;
        w->end = current->end - (beta.pstart - beta.pend) + 1;
        w->parent = current->parent;
        current->parent->children[i] = w;
        current->parent = w;
        current->start = w->end + 1;
        w->children[0] = current; // ������� ���� ���������� ������� ������ ����
        return w; // ���������� ��������� ����
    }
}

// ������� ���������� ����, ��� ���������� ����� �� ������ (slowfind)
treenode* walkdown(treenode* node, node_point& gamma, int& p, const string& T) {

    treenode* current = node;
    treenode* return_node;
    while (gamma.pstart <= gamma.pend) {
        int i = 0;
        while (current->children[i] != NULL && T[current->children[i]->start] != T[gamma.pstart]) { // �������� ���� ������ �� ������
            ++i;
        }
        if (!current->children[i]) { // current->children[i] == NULL, ����� �� ����� � ����� ������ ����

            p = i;
            return_node = current;

            return return_node; // ��������� ���� � ���������� ����, �� ������� ������������
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

                return return_node;
            }
        }
    }

}

treenode* build_suffix_tree(const string& T, int len) {

    treenode* root = new treenode(); // ������ ������
    root->parent = root;
    root->suffixlink = root;
    treenode* leafnode0 = new treenode(); // ������ ������ ����
    root->children[0] = leafnode0;
    leafnode0->parent = root;
    leafnode0->start = 0;
    leafnode0->end = len;


    treenode* node_v_j;
    node_v_j = root;
    treenode* w;

    node_point beta = node_point(root); 
    node_point gamma = node_point(leafnode0);

    for (int j = 1; j <= len; ++j) { // ������� �������� j 

        treenode* current_leafnode = new treenode(); // "current_leafnode" ��� ���� ���� ����� �������� ������� �� ���� ����
        current_leafnode->start = j;
        current_leafnode->end = len;

        int I = 1;
        if (node_v_j == root) {// �� ����� ������� �� ������
            ++gamma.pstart;
            I = 0;
        }
        else if (node_v_j->parent == root) { 
            if (beta.pstart == beta.pend) { 
                I = 0;
            }
            else { // ����� ������� �� ������ � ����� ���� �������� �������
                ++beta.pstart;
            }
        }


        if (I == 0) { // ������� �� �����, ����� ������ ������ �� ������

            int p = 0;
            treenode* new_node_v;
            new_node_v = walkdown(root, gamma, p, T);
            new_node_v->children[p] = current_leafnode;
            current_leafnode->parent = new_node_v;
            current_leafnode->start = gamma.pstart;
            if (beta.pstart == beta.pend && beta.pstart != -1) {
                node_v_j->suffixlink = root;
            }

        }
        else { // ����� ������� �� ���������� ������
            
            w = jumpdown(node_v_j->parent->suffixlink, beta, T);
            node_v_j->suffixlink = w;

            if (!w->children[1]) { // w->children[1] = NULL; w - ���� ��� ������� ���������� ������, ��������� �������
                w->children[1] = current_leafnode;
                current_leafnode->start = gamma.pstart;
                current_leafnode->parent = w;
                
            }
            else {  // ������� ���������� � ������������ ����, ����� ��������� �� ������ � �������� ����� ����
                
                int p = 0;
                treenode* new_node_v;
                new_node_v = walkdown(w, gamma, p, T);
                new_node_v->children[p] = current_leafnode;
                current_leafnode->parent = new_node_v;
                current_leafnode->start = gamma.pstart;
            }
        }

        gamma = node_point(current_leafnode);
        beta = node_point(current_leafnode->parent);
        node_v_j = current_leafnode->parent;
    }
    return root;
}