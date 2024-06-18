typedef struct {
    Token *current_token;
    Lexer *lexer;
} Parser;

Parser* init_parser(Lexer *lexer) {
    Parser *parser = (Parser*)malloc(sizeof(Parser));
    parser->lexer = lexer;
    parser->current_token = get_next_token(lexer);
    return parser;
}

void eat(Parser *parser, TokenType type) {
    if (parser->current_token->type == type) {
        parser->current_token = get_next_token(parser->lexer);
    } else {
        printf("Error: Expected token type %d but got %d\n", type, parser->current_token->type);
        exit(1);
    }
}

ASTNode* parse_function_definition(Parser *parser) {
    eat(parser, TOKEN_F);
    ASTNode *header = parse_function_header(parser);
    ASTNode *body = parse_function_body(parser);
    ASTNode *function_node = create_ast_node(NODE_FUNCTION, NULL);
    function_node->left = header;
    function_node->right = body;
    return function_node;
}

ASTNode* parse_function_header(Parser *parser) {
    ASTNode *name = parse_function_name(parser);
    eat(parser, TOKEN_LPAREN);
    ASTNode *params = parse_parameter_list(parser);
    eat(parser, TOKEN_RPAREN);
    eat(parser, TOKEN_ARROW);
    ASTNode *return_type = parse_return_type(parser);
    ASTNode *header_node = create_ast_node(NODE_FUNCTION, NULL);
    header_node->left = name;
    header_node->right = create_ast_node(NODE_PARAMETER, NULL);
    header_node->right->left = params;
    header_node->right->right = return_type;
    return header_node;
}

ASTNode* parse_function_name(Parser *parser) {
    ASTNode *name_node = create_ast_node(NODE_IDENTIFIER, parser->current_token->value);
    eat(parser, TOKEN_IDENTIFIER);
    return name_node;
}

ASTNode* parse_parameter_list(Parser *parser) {
    ASTNode *param = parse_parameter(parser);
    ASTNode *param_list = param;
    while (parser->current_token->type == TOKEN_COMMA) {
        eat(parser, TOKEN_COMMA);
        ASTNode *next_param = parse_parameter(parser);
        ASTNode *new_param_list = create_ast_node(NODE_PARAMETER, NULL);
        new_param_list->left = param_list;
        new_param_list->right = next_param;
        param_list = new_param_list;
    }
    return param_list;
}

ASTNode* parse_parameter(Parser *parser) {
    ASTNode *type_node = parse_type(parser);
    eat(parser, TOKEN_COLON);
    ASTNode *id_node = create_ast_node(NODE_IDENTIFIER, parser->current_token->value);
    eat(parser, TOKEN_IDENTIFIER);
    ASTNode *param_node = create_ast_node(NODE_PARAMETER, NULL);
    param_node->left = type_node;
    param_node->right = id_node;
    return param_node;
}

ASTNode* parse_return_type(Parser *parser) {
    return parse_type(parser);
}

ASTNode* parse_type(Parser *parser) {
    ASTNode *type_node = create_ast_node(NODE_TYPE, parser->current_token->value);
    eat(parser, TOKEN_TYPE);
    return type_node;
}

ASTNode* parse_function_body(Parser *parser) {
    eat(parser, TOKEN_LBRACE);
    ASTNode *statements = parse_statements(parser);
    eat(parser, TOKEN_RBRACE);
    return statements;
}

ASTNode* parse_statements(Parser *parser) {
    ASTNode *statement = parse_statement(parser);
    ASTNode *statement_list = statement;
    while (parser->current_token->type != TOKEN_RBRACE) {
        ASTNode *next_statement = parse_statement(parser);
        ASTNode *new_statement_list = create_ast_node(NODE_STATEMENT, NULL);
        new_statement_list->left = statement_list;
        new_statement_list->right = next_statement;
        statement_list = new_statement_list;
    }
    return statement_list;
}

ASTNode* parse_statement(Parser *parser) {
    ASTNode *statement = NULL;
    if (parser->current_token->type == TOKEN_TYPE) {
        statement = parse_variable_declaration(parser);
        eat(parser, TOKEN_SEMICOLON);
    } else if (parser->current_token->type == TOKEN_IDENTIFIER) {
        statement = parse_assignment(parser);
        eat(parser, TOKEN_SEMICOLON);
    } else if (parser->current_token->type == TOKEN_RETURN) {
        statement = parse_return_statement(parser);
        eat(parser, TOKEN_SEMICOLON);
    } else {
        printf("Error: Unknown statement\n");
        exit(1);
    }
    return statement;
}

ASTNode* parse_variable_declaration(Parser *parser) {
    ASTNode *type_node = parse_type(parser);
    eat(parser, TOKEN_COLON);
    ASTNode *id_node = create_ast_node(NODE_IDENTIFIER, parser->current_token->value);
    eat(parser, TOKEN_IDENTIFIER);
    ASTNode *decl_node = create_ast_node(NODE_VARIABLE_DECLARATION, NULL);
    decl_node->left = type_node;
    decl_node->right = id_node;
    if (parser->current_token->type == TOKEN_ASSIGN) {
        eat(parser, TOKEN_ASSIGN);
        ASTNode *expr_node = parse_expression(parser);
        ASTNode *assign_node = create_ast_node(NODE_ASSIGNMENT, NULL);
        assign_node->left = decl_node;
        assign_node->right = expr_node;
        return assign_node;
    }
    return decl_node;
}

ASTNode* parse_assignment(Parser *parser) {
    ASTNode *id_node = create_ast_node(NODE_IDENTIFIER, parser->current_token->value);
    eat(parser, TOKEN_IDENTIFIER);
    eat(parser, TOKEN_ASSIGN);
    ASTNode *expr_node = parse_expression(parser);
    ASTNode *assign_node = create_ast_node(NODE_ASSIGNMENT, NULL);
    assign_node->left = id_node;
    assign_node->right = expr_node;
    return assign_node;
}

ASTNode* parse_return_statement(Parser *parser) {
    eat(parser, TOKEN_RETURN);
    ASTNode *expr_node = parse_expression(parser);
    ASTNode *return_node = create_ast_node(NODE_RETURN, NULL);
    return_node->left = expr_node;
    return return_node;
}

ASTNode* parse_expression(Parser *parser) {
    ASTNode *term = parse_term(parser);
    while (parser->current_token->type == TOKEN_OPERATOR) {
        ASTNode *op_node = create_ast_node(NODE_OPERATOR, parser->current_token->value);
        eat(parser, TOKEN_OPERATOR);
        op_node->left = term;
        op_node->right = parse_term(parser);
        term = op_node;
    }
    return term;
}

ASTNode* parse_term(Parser *parser) {
    ASTNode *term = NULL;
    if (parser->current_token->type == TOKEN_IDENTIFIER) {
        term = create_ast_node(NODE_IDENTIFIER, parser->current_token->value);
        eat(parser, TOKEN_IDENTIFIER);
    } else if (parser->current_token->type == TOKEN_INTEGER) {
        term = create_ast_node(NODE_INTEGER, parser->current_token->value);
        eat(parser, TOKEN_INTEGER);
    } else if (parser->current_token->type == TOKEN_LPAREN) {
        eat(parser, TOKEN_LPAREN);
        term = parse_expression(parser);
        eat(parser, TOKEN_RPAREN);
    } else {
        printf("Error: Unknown term\n");
        exit(1);
    }
    return term;
}

