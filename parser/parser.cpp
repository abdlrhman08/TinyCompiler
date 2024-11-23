//
// Created by zyn66 on 11/23/2024.
//

#include "parser.hpp"

// Node Implementation
Node::Node(string t, string c, string s) : token_value(t), code_value(c), shape(s) {}

void Node::setChildren(vector<Node*> y) {
    children.insert(children.end(), y.begin(), y.end());
}

void Node::setSibling(Node* y) {
    sibling = y;
}

// Parser Implementation
Parser::Parser() {}

void Parser::setTokensList(vector<Token> tokens) {
    tokens_list = tokens;
    tmp_index = 0;
    if (!tokens_list.empty()) {
        token = tokens_list[tmp_index];
    }
}

bool Parser::nextToken() {
    if (tmp_index < tokens_list.size() - 1) {
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
    Node* t = statement();
    stmts.push_back(t);
    while (token.token_type != TokenType::END && token.token_type != TokenType::ELSE && token.token_type != TokenType::UNTIL ) {
        if (match(TokenType::SEMICOLON)) {
            Node* t = statement();
            stmts.push_back(t);
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
    } else {
        cout << "Syntax Error" << endl;
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
    Node* t = term();
    while (token.token_type == TokenType::PLUS || token.token_type == TokenType::MINUS || token.token_type == TokenType::LESSTHAN || token.token_type == TokenType::EQUAL) {
        Node* p = new Node(token.string_val, "OP", "ellipse");
        p->setChildren({t, nullptr});
        nextToken();
        p->setChildren({t, term()});
        t = p;
    }
    return t;
}

Node* Parser::exp() {
    Node* t = simpleExp();
    if (token.token_type == TokenType::LESSTHAN || token.token_type == TokenType::EQUAL) {
        Node* p = new Node(token.string_val, "OP", "ellipse");
        p->setChildren({t, nullptr});
        nextToken();
        p->setChildren({t, simpleExp()});
        t = p;
    }
    return t;
}

Node* Parser::ifStmt() {
    Node* t = new Node("if", "IF", "ellipse");
    match(TokenType::IF);
    Node* exp_node = exp();
    match(TokenType::THEN);
    Node* stmt_node = stmtSequence();
    t->setChildren({exp_node, stmt_node});
    if (token.token_type == TokenType::ELSE) {
        match(TokenType::ELSE);
        Node* stmt_node = stmtSequence();
        t->setChildren({exp_node, stmt_node});
    }
    match(TokenType::END);
    return t;
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
    t->setChildren({new Node(token.string_val, "ID", "ellipse"), nullptr});
    match(TokenType::IDENTIFIER);
    match(TokenType::ASSIGN);
    t->setChildren({t, exp()});
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
        node = parse_tree;
    }
    if (node->index == -1) {
        node->index = nodes_table.size();
        nodes_table[node->index] = {node->token_value, node->code_value, node->shape};
    }
    for (auto child : node->children) {
        createNodesTable(child);
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
        edges_table.push_back({node->index, child->index});
        createEdgesTable(child);
    }
    if (node->sibling != nullptr) {
        edges_table.push_back({node->index, node->sibling->index});
        createEdgesTable(node->sibling);
    }
}

void Parser::run() {
    parse_tree = stmtSequence();
    createNodesTable();
    createEdgesTable();
}

void Parser::clearTables() {
    nodes_table.clear();
    edges_table.clear();
    same_rank_nodes.clear();
}