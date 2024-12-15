//
// Created by zyn66 on 11/23/2024.
//

#include "parser.h"
#include "scanner.h"

#include <cstddef>
#include <iostream>
#include <ostream>
#include <string>
#include <utility>

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

bool Parser::validateIdentifier(const Token& token) {
    // Check if token type is IDENTIFIER
    if (token.token_type != TokenType::IDENTIFIER) {
        cerr << "Syntax Error: Expected an identifier, but found '"
             << token.string_val << "'." << endl;
        return false;
    }

    // Validate the token value (e.g., no numbers at the start)
    const std::string& value = token.string_val;

    // Identifier must start with a letter or underscore
    if (!isalpha(value[0]) && value[0] != '_') {
        cerr << "Syntax Error: Identifier must start with a letter or '_', but found '"
             << value[0] << "' in '" << value << "'." << endl;
        return false;
    }

    // Check the rest of the identifier for valid characters
    for (size_t i = 1; i < value.length(); ++i) {
        if (!isalnum(value[i]) && value[i] != '_') {
            cerr << "Syntax Error: Invalid character '" << value[i]
                 << "' in identifier '" << value << "'." << endl;
            return false;
        }
    }

    return true; // Valid identifier
}

void Parser::setTokensList(Token* tok_list, size_t num) {
    tokens_list = tok_list;
    tokens_num = num;
    tmp_index = 0;
    if (tokens_num != 0) {
        token = tokens_list[tmp_index];
    }
}

bool Parser::nextToken() {
    if (tmp_index < tokens_num) {
        tmp_index++;
        token = tokens_list[tmp_index];
        return true;
    }
    return false;
}
bool Parser::match(int expected_type) {
    if (tmp_index >= tokens_num) {
        cerr << "Error: Token index out of bounds!" << endl;
        return false;
    }
    if (token.token_type == expected_type) {
        nextToken();
        return true;
    }
    return false;
}

Node* Parser::statement() {
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
        return nullptr;
    }
    cout << "Exiting statement()" << endl; // Debugging statement
    return t;
}

Node* Parser::stmtSequence() {
    cout << "Entering stmtSequence()" << endl;
    //vector<Node*> stmts;

    Node* temp = statement();
    Node* first = temp;
    while (token.token_type != TokenType::END && token.token_type != TokenType::ELSE && token.token_type != TokenType::UNTIL) {
        if(!match(TokenType::SEMICOLON))
            break;
        Node* t = statement(); // Attempt to parse a single statement

        //if (t != nullptr) {
        //    stmts.push_back(t); // Add the valid statement to the list
        //}

        if (t == nullptr) {
            cerr << "Warning: Empty or invalid statement in stmtSequence." << endl;
            return nullptr;
        }

        // Check if the next token is a semicolon
        //if (token.token_type == TokenType::SEMICOLON) {
        //    match(TokenType::SEMICOLON); // Consume the semicolon
        //    cout << "Matched semicolon." << endl; // Debugging output
        //}

        //if (token.token_type != TokenType::IDENTIFIER &&
        //    token.token_type != TokenType::READ &&
        //    token.token_type != TokenType::WRITE &&
        //    token.token_type != TokenType::IF &&
        //    token.token_type != TokenType::REPEAT &&
        //    token.token_type != TokenType::END) {
        //    break;
        //    }
        temp->sibling = t;
        temp = t;
    }

    //Node* stmt_seq = new Node("stmt_sequence", "", "rectangle");
    //stmt_seq->setChildren(stmts);

    cout << "Exiting stmtSequence()" << endl;
    return first;
}

Node* Parser::factor() {
    Node* t = nullptr;
    if (token.token_type == TokenType::IDENTIFIER) {
        std::string val = "id (" + std::string(token.string_val) + ")";
        t = new Node(val, "ID", "ellipse");
        nextToken();
    } else if (token.token_type == TokenType::NUMBER) {
        std::string val = "const (" + to_string(token.num_val) + ")";
        t = new Node(val, "CONST", "ellipse");
        nextToken();
    } else if (match(TokenType::OPENBRACKET)) {
        t = exp();
        match(TokenType::CLOSEDBRACKET);
    } else {
        cout << "Syntax Error in factor()" << endl; // Debugging statement
        return nullptr;
    }
    cout << "Exiting factor()" << endl; // Debugging statement
    return t;
}

Node* Parser::term() {
    cout << "Entering term()" << endl; // Debugging statement
    Node* t = factor();
    while (token.token_type == TokenType::MULT || token.token_type == TokenType::DIV) {
        std::string val = "OP (" + std::string(token.string_val) + ")";
        Node* p = new Node(val, "OP", "ellipse");
        //p->setChildren({t, nullptr});
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

    cout << "Exiting ifStmt()" << endl;
    return ifNode;
}

Node* Parser::repeatStmt() {
    // Create the root node for the repeat statement
    Node* repeatNode = new Node("repeat", "REPEAT", "rectangle");

    // Match the 'REPEAT' keyword
    match(TokenType::REPEAT);

    // Parse the statement sequence inside the 'REPEAT' block
    Node* stmtNode = stmtSequence();
    if (!stmtNode) {
        cerr << "Syntax Error: Invalid statement sequence in repeat statement." << endl;
        delete repeatNode; // Clean up allocated memory
        return nullptr;    // Exit gracefully on error
    }

    // Ensure the 'UNTIL' keyword is present
    if (token.token_type != TokenType::UNTIL) {
        cerr << "Syntax Error: Expected 'UNTIL' in repeat statement." << endl;
        delete repeatNode; // Clean up allocated memory
        delete stmtNode;   // Clean up allocated memory
        return nullptr;    // Exit gracefully on error
    }

    // Match the 'UNTIL' keyword
    match(TokenType::UNTIL);

    // Parse the expression following 'UNTIL'
    Node* expNode = exp();
    if (!expNode) {
        cerr << "Syntax Error: Invalid expression following 'UNTIL' in repeat statement." << endl;
        delete repeatNode; // Clean up allocated memory
        delete stmtNode;   // Clean up allocated memory
        return nullptr;    // Exit gracefully on error
    }

    // Set the children of the repeat node
    repeatNode->setChildren({stmtNode, expNode});

    return repeatNode; // Return the constructed repeat node
}


Node* Parser::assignStmt() {
    cout << "Entering assignStmt()" << endl; // Debugging statement
    if (!validateIdentifier(token)) {
        return nullptr;
    }
    std::string assign_val = "assign (" + std::string(token.string_val) + ")";
    Node* t = new Node(assign_val, "ID", "rectangle");
    match(TokenType::IDENTIFIER);
    match(TokenType::ASSIGN);
    Node* expNode = exp();
    t->setChildren({expNode});
    cout << "Exiting assignStmt()" << endl; // Debugging statement
    return t;
}

Node* Parser::readStmt() {
    match(TokenType::READ);

    // Validate and handle the identifier
    if (validateIdentifier(token)) {
        std::string read_val = "read (" + std::string(token.string_val) + ")";
        Node* readNode = new Node(read_val, "READ", "rectangle");
        std::cout << read_val << std::endl;
        match(TokenType::IDENTIFIER); // Consume the identifier
        return readNode;
    }

    return nullptr;
}

Node* Parser::writeStmt() {
    cout << "Entering writeStmt()" << endl; // Debugging statement
    Node* t = new Node("write", "WRITE", "rectangle");
    match(TokenType::WRITE);
    t->setChildren({exp()});
    cout << "Exiting writeStmt()" << endl; // Debugging statement
    return t;
}

void Parser::createNodesTable(Node* node) {
    if (node == nullptr) { // Check for null node
        cerr << "Error: Null node encountered in createNodesTable." << endl;
        return;
    }

    // Debug: Print the node being processed
    cout << "Processing node with value: " << node->token_value
         << ", code: " << node->code_value
         << ", shape: " << node->shape
         << endl;

    // Assign index if it doesn't exist
    if (node->index == -1) {
        node->index = nodes_table.size(); // Assign a new index
        nodes_table[node->index] = {node->token_value, node->code_value, node->shape};
    }

    // Debug: Print table size after insertion
    cout << "Nodes table size: " << nodes_table.size() << endl;

    // Process children
    for (auto child : node->children) {
        if (child == nullptr) {
            continue;
        }
        createNodesTable(child);
    }

    // Process sibling
    if (node->sibling != nullptr) {
        createNodesTable(node->sibling);
    }
}



void Parser::createEdgesTable(Node* node) {
    if (node == nullptr) {
        if (parse_tree == nullptr) {
            cerr << "Error: Parse tree is not initialized." << endl;
            return;
        }
        node = parse_tree; // Start from the root of the parse tree
    }

    cout << "Processing node index: " << node->index << endl;

    // Process children
    for (auto child : node->children) {
        if (child != nullptr) {
            cout << "Adding edge: (" << node->index << ", " << child->index << ")" << endl;
            edges_table.emplace_back(node->index, child->index); // Add edge from current node to child
            createEdgesTable(child); // Recur on child
        }
    }

    // Process sibling
    if (node->sibling != nullptr) {
        cout << "Adding edge: (" << node->index << ", " << node->sibling->index << ")" << endl;
        edges_table.emplace_back(node->index, node->sibling->index); // Add edge from current node to sibling
        createEdgesTable(node->sibling); // Recur on sibling
    }
}



void Parser::run() {
    cout << "Running parser..." << endl; // Debugging statement
    parse_tree = stmtSequence();
    if (parse_tree == nullptr) {
        cerr << "Error: Failed to parse input." << endl;
        return;
    }
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
