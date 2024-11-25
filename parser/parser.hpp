//
// Created by zyn66 on 11/23/2024.
//

#ifndef PARSER_H
#define PARSER_H

#include <string>
#include <vector>
#include <map>
#include <iostream>
#include "../src/scanner.h"

using namespace std;

class Node {
public:
    string token_value;
    string code_value;
    string shape;
    vector<Node*> children;
    Node* sibling = nullptr;
    int index = -1;

    Node(string t, string c, string s);

    void setChildren(vector<Node*> y);
    void setSibling(Node* y);
};

class Parser {
public:
    vector<Token> tokens_list;
    int tmp_index = 0;
    Node* parse_tree = nullptr;
    map<int, vector<string>> nodes_table;
    vector<pair<int, int>> edges_table;
    vector<pair<int, int>> same_rank_nodes;

    Token token{};

    Parser();
    void setTokensList(vector<Token> tokens);
    bool nextToken();
    bool match(int expected_type);
    Node* statement();
    Node* stmtSequence();
    Node* factor();
    Node* term();
    Node* simpleExp();
    Node* exp();
    Node* ifStmt();
    Node* repeatStmt();
    Node* assignStmt();
    Node* readStmt();
    Node* writeStmt();
    void createNodesTable(Node* node = nullptr);
    void createEdgesTable(Node* node = nullptr);
    void run();
    void clearTables();
    void printParseTree(Node* node, int depth = 0);
    void outputTables();
};



#endif //PARSER_H
