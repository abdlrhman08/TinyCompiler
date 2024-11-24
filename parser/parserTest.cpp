//
// Created by zyn66 on 11/23/2024.
//

#include "parser.hpp"

// Node Implementation
Node::Node(string t, string c, string s) : token_value(t), code_value(c), shape(s) {}

void Node::setChildren(std::vector<Node*> y) {
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
    cout << "Entering statement() with token: " << token.string_val << endl; // Debugging statement
    Node* t = nullptr;
    if (token.token_type == TokenType::IF) {
        cout << "IF" << endl;
        t = ifStmt();
    } else if (token.token_type == TokenType::REPEAT) {
        cout << "REPEAT" << endl;
        t = repeatStmt();
    } else if (token.token_type == TokenType::IDENTIFIER) {
        cout << "ASSIGN" << endl;
        t = assignStmt();
    } else if (token.token_type == TokenType::READ) {
        cout << "READ" << endl;
        t = readStmt();
    } else if (token.token_type == TokenType::WRITE) {
        cout << "WRITE" << endl;
        t = writeStmt();
    } else {
        cout << "Syntax Error" << endl;
    }
    cout << "Exiting statement()" << endl; // Debugging statement
    return t;
}

Node* Parser::stmtSequence() {
    cout << "Entering stmtSequence()" << endl;
    vector<Node*> stmts;

    while (token.token_type != TokenType::END && token.token_type != TokenType::ELSE && token.token_type != TokenType::UNTIL) {
        Node* t = statement(); // Parse a single statement
        if (t != nullptr) {
            stmts.push_back(t);
        }

        // Match semicolon between statements, but don't require it at the end
        if (token.token_type == TokenType::SEMICOLON) {
            match(TokenType::SEMICOLON);
        } else {
            break; // Exit loop if no semicolon is found
        }
    }

    Node* stmt_seq = new Node("stmt_sequence", "", "ellipse");
    stmt_seq->setChildren(stmts);

    cout << "Exiting stmtSequence()" << endl;
    return stmt_seq;
}



Node* Parser::factor() {
    cout << "Entering factor() with token: " << token.string_val << endl; // Debugging statement
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
        cout << "Syntax Error in factor()" << endl; // Debugging statement
    }
    cout << "Exiting factor()" << endl; // Debugging statement
    return t;
}

Node* Parser::term() {
    cout << "Entering term()" << endl; // Debugging statement
    Node* t = factor();
    while (token.token_type == TokenType::MULT || token.token_type == TokenType::DIV) {
        Node* p = new Node(token.string_val, "OP", "ellipse");
        p->setChildren({t, nullptr});
        nextToken();
        p->setChildren({t, factor()});
        t = p;
    }
    cout << "Exiting term()" << endl; // Debugging statement
    return t;
}

Node* Parser::simpleExp() {
    cout << "Entering simpleExp()" << endl; // Debugging statement

    // Parse the first term
    Node* left = term();

    // Handle additive or relational operators
    while (token.token_type == TokenType::PLUS || token.token_type == TokenType::MINUS ||
           token.token_type == TokenType::LESSTHAN || token.token_type == TokenType::EQUAL) {
        // Create a node for the operator
        Node* operatorNode = new Node(token.string_val, "OP", "ellipse");

        // Advance the token
        nextToken();

        // Parse the next term
        Node* right = term();

        // Set children for the operator node
        operatorNode->setChildren({left, right});

        // Update the current tree
        left = operatorNode;
           }

    cout << "Exiting simpleExp()" << endl; // Debugging statement
    return left;
}
Node* Parser::exp() {
    cout << "Entering exp()" << endl; // Debugging statement

    // Parse the left-hand side
    Node* left = simpleExp();

    // Handle relational operators
    if (token.token_type == TokenType::LESSTHAN || token.token_type == TokenType::EQUAL) {
        // Create a node for the operator
        Node* operatorNode = new Node(token.string_val, "OP", "ellipse");

        // Advance the token
        nextToken();

        // Parse the right-hand side
        Node* right = simpleExp();

        // Set children for the operator node
        operatorNode->setChildren({left, right});

        // Update the expression tree
        left = operatorNode;
    }

    cout << "Exiting exp()" << endl; // Debugging statement
    return left;
}

Node* Parser::ifStmt() {
    cout << "Entering ifStmt()" << endl;

    Node* ifNode = new Node("ifStmt", "", "rectangle");

    match(TokenType::IF); // Match 'if' token
    Node* condition = exp(); // Parse the condition

    match(TokenType::THEN); // Match 'then' token
    Node* stmtSeq = stmtSequence(); // Parse the statement sequence inside 'if'

    if (token.token_type == TokenType::END) {
        match(TokenType::END); // Match 'end' token to conclude the block
    } else {
        cerr << "Syntax Error: Expected 'END' after if statement." << endl;
    }

    // Setting children
    vector<Node*> children = {condition, stmtSeq};
    ifNode->setChildren(children);

    cout << "Exiting ifStmt()" << endl;
    return ifNode;
}


Node* Parser::repeatStmt() {
    cout << "Entering repeatStmt()" << endl; // Debugging statement
    Node* t = new Node("repeat", "REPEAT", "ellipse");
    match(TokenType::REPEAT);
    Node* stmt_node = stmtSequence();
    match(TokenType::UNTIL);
    Node* exp_node = exp();
    t->setChildren({stmt_node, exp_node});
    cout << "Exiting repeatStmt()" << endl; // Debugging statement
    return t;
}

Node* Parser::assignStmt() {
    cout << "Entering assignStmt()" << endl; // Debugging statement
    Node* t = new Node("assign", "ID", "ellipse");
    Node* idNode = new Node(token.string_val, "ID", "ellipse");
    match(TokenType::IDENTIFIER);
    match(TokenType::ASSIGN);
    Node* expNode = exp();
    t->setChildren({idNode, expNode});
    cout << "Exiting assignStmt()" << endl; // Debugging statement
    return t;
}

Node* Parser::readStmt() {
    cout << "Entering readStmt()" << endl; // Debugging statement
    Node* t = new Node("read", "READ", "ellipse");
    match(TokenType::READ);
    t->setChildren({new Node(token.string_val, "ID", "ellipse"), nullptr});
    match(TokenType::IDENTIFIER);
    cout << "Exiting readStmt()" << endl; // Debugging statement
    return t;
}

Node* Parser::writeStmt() {
    cout << "Entering writeStmt()" << endl; // Debugging statement
    Node* t = new Node("write", "WRITE", "ellipse");
    match(TokenType::WRITE);
    t->setChildren({exp(), nullptr});
    cout << "Exiting writeStmt()" << endl; // Debugging statement
    return t;
}

void Parser::createNodesTable(Node* node) {
    if (node == nullptr) { // Check if node is null
        if (parse_tree == nullptr) { // Ensure parse_tree is also initialized
            cerr << "Error: Parse tree is not initialized." << endl;
            return;
        }
        node = parse_tree; // Start from the root of the parse tree
    }

    // Process the current node only if it hasn't been added to the table
    if (node->index == -1) {
        node->index = nodes_table.size(); // Assign a new index
        nodes_table[node->index] = {node->token_value, node->code_value, node->shape};
    }

    // Traverse children recursively
    for (auto child : node->children) {
        if (child != nullptr) { // Check for null children
            createNodesTable(child);
        }
    }

    // Traverse sibling recursively
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
    cout << "Running parser..." << endl; // Debugging statement
    parse_tree = stmtSequence();
    //print parse tree
    cout << "Parse tree:" << endl;
    printParseTree(parse_tree, 0);
    createNodesTable(parse_tree);
    createEdgesTable(parse_tree);
    cout << "Finished parsing" << endl; // Debugging statement
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

void Parser::clearTables() {
    nodes_table.clear();
    edges_table.clear();
    same_rank_nodes.clear();
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

int main(int argc, char** argv) {
    // use static data token to test parser
    vector<Token> tokens = {{TokenType::IF, "if", 0},
        {TokenType::IDENTIFIER, "a", 0},
        {TokenType::LESSTHAN, "<", 0},
        {TokenType::NUMBER, "5", 5},
        {TokenType::THEN, "then", 0},
        {TokenType::IDENTIFIER, "b", 0},
        {TokenType::ASSIGN, ":=", 0},
        {TokenType::NUMBER, "1", 1},
        {TokenType::SEMICOLON, ";", 0},
        {TokenType::END, "end", 0},
    };

    Parser parser;
    parser.setTokensList(tokens);
    parser.run();
    parser.outputTables();
    parser.printParseTree(parser.parse_tree, 0);
    parser.clearTables();

    return 0;
}
