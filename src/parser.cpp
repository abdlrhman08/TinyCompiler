//
// Created by zyn66 on 11/23/2024.
//

#include "parser.hpp"

#include <utility>
#include <iostream>

// Node Implementation
Node::Node(string t, string c, string s) : token_value(std::move(t)), code_value(std::move(c)), shape(std::move(s)) {}

void Node::setChildren(std::vector<Node*> y) {
    children.insert(children.end(), y.begin(), y.end());
}

void Node::setSibling(Node* y) {
    sibling = y;
}

// Parser Implementation
Parser::Parser() = default;

void Parser::setTokensList(Token* tok_list, size_t num) {
    tokens_list = tok_list;
    tokens_num = num;
    tmp_index = 0;
    if (num != 0) {
        token = tokens_list[tmp_index];
    }
}

bool Parser::nextToken() {
    if (tmp_index < tokens_num - 1) {
        tmp_index++;
        token = tokens_list[tmp_index];
        return true;
    }
    return false;
}

bool Parser::match(int expected_type) {
    if (token.token_type == expected_type) {
        nextToken();
        return true;
    }
    return false;
}

Node* Parser::statement() {
    Node* t = nullptr;
    cout << token.string_val << endl;
    if (token.token_type == TokenType::IF) {
        t = ifStmt();
    } else if (token.token_type == TokenType::REPEAT) {
        t = repeatStmt();
    } else if (token.token_type == TokenType::IDENTIFIER) {
        t = assignStmt();
    } else if (token.token_type == TokenType::READ) {
        t = readStmt();
    } else if (token.token_type == TokenType::WRITE) {
        t = writeStmt();
    } else {
        cout << "Syntax Error" << endl;
    }
    return t;
}

Node* Parser::stmtSequence() {
    vector<Node*> stmts;

    while (token.token_type != TokenType::END && token.token_type != TokenType::ELSE && token.token_type != TokenType::UNTIL) {
        Node* t = statement();
        if (t != nullptr) {
            stmts.push_back(t);
        }

        if (token.token_type == TokenType::SEMICOLON) {
            match(TokenType::SEMICOLON);
        } else {
            break;
        }
    }

    Node* stmt_seq = new Node("stmt_sequence", "", "ellipse");
    stmt_seq->setChildren(stmts);

    return stmt_seq;
}

Node* Parser::factor() {
    Node* t = nullptr;
    if (token.token_type == TokenType::IDENTIFIER) {
        t = new Node(token.string_val, "ID", "ellipse");
        nextToken();
    } else if (token.token_type == TokenType::NUMBER) {
        t = new Node(to_string(token.num_val), "CONST", "ellipse");
        nextToken();
    } else if (match(TokenType::OPENBRACKET)) {
        t = exp();
        match(TokenType::CLOSEDBRACKET);
    }
    return t;
}

Node* Parser::term() {
    Node* t = factor();
    while (token.token_type == TokenType::MULT || token.token_type == TokenType::DIV) {
        Node* p = new Node(token.string_val, "OP", "ellipse");
        p->setChildren({t, nullptr});
        nextToken();
        p->setChildren({t, factor()});
        t = p;
    }
    return t;
}

Node* Parser::simpleExp() {
    Node* left = term();

    while (token.token_type == TokenType::PLUS || token.token_type == TokenType::MINUS ||
           token.token_type == TokenType::LESSTHAN || token.token_type == TokenType::EQUAL) {
        Node* operatorNode = new Node(token.string_val, "OP", "ellipse");
        nextToken();
        Node* right = term();
        operatorNode->setChildren({left, right});
        left = operatorNode;
           }

    return left;
}

Node* Parser::exp() {
    Node* left = simpleExp();

    if (token.token_type == TokenType::LESSTHAN || token.token_type == TokenType::EQUAL) {
        Node* operatorNode = new Node(token.string_val, "OP", "ellipse");
        nextToken();
        Node* right = simpleExp();
        operatorNode->setChildren({left, right});
        left = operatorNode;
    }

    return left;
}


Node* Parser::ifStmt() {
    Node* ifNode = new Node("ifStmt", "", "rectangle");

    match(TokenType::IF);
    Node* condition = exp();

    match(TokenType::THEN);
    Node* stmtSeq = stmtSequence();

    vector<Node*> children = {condition, stmtSeq};

    if (token.token_type == TokenType::ELSE) {
        match(TokenType::ELSE);
        Node* elseStmtSeq = stmtSequence();
        children.push_back(elseStmtSeq);
    }

    if (token.token_type == TokenType::END) {
        match(TokenType::END);
    } else {
        cerr << "Syntax Error: Expected 'END' after if statement." << endl;
    }

    // Set children
    ifNode->setChildren(children);

    return ifNode;
}

Node* Parser::repeatStmt() {
    Node* t = new Node("repeat", "REPEAT", "ellipse");
    match(TokenType::REPEAT);
    Node* stmt_node = stmtSequence();
    match(TokenType::UNTIL);
    Node* exp_node = exp();
    t->setChildren({stmt_node, exp_node});
    return t;
}

Node* Parser::assignStmt() {
    Node* t = new Node("assign", "ID", "ellipse");
    Node* idNode = new Node(token.string_val, "ID", "ellipse");
    match(TokenType::IDENTIFIER);
    match(TokenType::ASSIGN);
    Node* expNode = exp();
    t->setChildren({idNode, expNode});
    return t;
}


Node* Parser::readStmt() {
    Node* t = new Node("read", "READ", "ellipse");
    match(TokenType::READ);
    t->setChildren({new Node(token.string_val, "ID", "ellipse"), nullptr});
    match(TokenType::IDENTIFIER);
    return t;
}

Node* Parser::writeStmt() {
    Node* t = new Node("write", "WRITE", "ellipse");
    match(TokenType::WRITE);
    t->setChildren({exp(), nullptr});
    return t;
}

void Parser::createNodesTable(Node* node) {
    if (node == nullptr) {
        if (parse_tree == nullptr) {
            return;
        }
        node = parse_tree;
    }

    if (node->index == -1) {
        node->index = nodes_table.size();
        nodes_table[node->index] = {node->token_value, node->code_value, node->shape};
    }

    for (auto child : node->children) {
        if (child != nullptr) {
            createNodesTable(child);
        }
    }

    if (node->sibling != nullptr) {
        createNodesTable(node->sibling);
    }
}


void Parser::createEdgesTable(Node* node) {
    if (node == nullptr) {
        node = parse_tree;
    }
    for (auto child : node->children) {
        std::cout << "Current CHild: " <<child << " Current node: " << node << std::endl;
        edges_table.emplace_back(node->index, child->index);
        createEdgesTable(child);
    }
    if (node->sibling != nullptr) {
        edges_table.emplace_back(node->index, node->sibling->index);
        createEdgesTable(node->sibling);
    }
}

void Parser::run() {
    parse_tree = stmtSequence();
    createNodesTable(parse_tree);
    createEdgesTable(parse_tree);
}

void Parser::clearTables() {
    nodes_table.clear();
    edges_table.clear();
    same_rank_nodes.clear();
}

void Parser::outputTables() {
    cout << "Nodes table:" << endl;
    for (const auto& node : nodes_table) {
        cout << node.first << ": " << node.second[0] << ", " << node.second[1] << ", " << node.second[2] << endl;
    }
    cout << "Edges table:" << endl;
    for (const auto& edge : edges_table) {
        cout << edge.first << " -> " << edge.second << endl;
    }
}

void Parser::printParseTree(Node* node, int depth) {
    if (node == nullptr) {
        return;
    }
    for (int i = 0; i < depth; i++) {
        cout << "  ";
    }
    cout << node->token_value << endl;
    for (auto child : node->children) {
        printParseTree(child, depth + 1);
    }
    if (node->sibling != nullptr) {
        printParseTree(node->sibling, depth);
    }
}
