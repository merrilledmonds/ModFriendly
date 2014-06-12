#ifndef SCRIPTLAYER_H
#define SCRIPTLAYER_H

#include "stdafx.h"
#include <deque>
#include <iostream>
#include <fstream>
#include <algorithm>


enum SymbolType{
	SYMBOL_SEMICOLON,
	SYMBOL_COLON,
	SYMBOL_COMMA,
	SYMBOL_DOT,
	SYMBOL_PREPROCESSOR,
	SYMBOL_LBRACE,
	SYMBOL_RBRACE,
	SYMBOL_LBRACKET,
	SYMBOL_RBRACKET,
	SYMBOL_DOUBLEQUOTE,
	SYMBOL_LPARENTHESIS,
	SYMBOL_RPARENTHESIS,
	SYMBOL_PLUS,
	SYMBOL_MINUS,
	SYMBOL_MULTIPLY,
	SYMBOL_DIVIDE,
	SYMBOL_POWER,
	SYMBOL_MOD,
	SYMBOL_EQUALS,
	SYMBOL_PLUSEQUALS,
	SYMBOL_MINUSEQUALS,
	SYMBOL_DIVIDEEQUALS,
	SYMBOL_MULTIPLYEQUALS,
	SYMBOL_MODEQUALS,
	SYMBOL_POWEREQUALS,
	SYMBOL_INCREMENT,
	SYMBOL_DECREMENT,
	SYMBOL_COMMENTSINGLE,
	SYMBOL_COMMENTMULTISTART,
	SYMBOL_COMMENTMULTIEND,
	SYMBOL_STRINGDELIM,
	SYMBOL_LITERAL,
	SYMBOL_KEYWORD,
	SYMBOL_IDENTIFIER,
	SYMBOL_ENDLINE,
	SYMBOL_INVALID
};

struct Token{
	int line;
	int column;
	SymbolType type;
	std::string value;
	Token(){
		line=0;
		column=0;
		type=SYMBOL_INVALID;
		value="";
	};
};

struct TokenStack{
	std::vector<Token> stack;
};

enum KeywordType{
	KEYWORD_REQUIRE,
	KEYWORD_IMPLY,
	KEYWORD_PEEK,
	KEYWORD_DEFINE,
	KEYWORD_GLOBAL,
	KEYWORD_LOCAL,
	KEYWORD_RETURN,
	KEYWORD_INVALID
};

struct Keyword{
	KeywordType t;
};

enum LiteralType{
	LITERAL_INT,
	LITERAL_STRING
};

struct Literal{
	union data{
		int i;
		char* s;
	};
	LiteralType t;
};

struct Operator{
	SymbolType t;
};

enum IdentifierType{
	IDENTIFIER_VAR,
	IDENTIFIER_FUN
};

struct Identifier{
	IdentifierType t;
	char* i;
};

union ExpressionP{
	Literal l;
	Operator o;
	Identifier i;
	Keyword k;
};

struct Expression{
	std::deque<ExpressionP> stack;
};

struct Statement{
	std::deque<Expression> stack;
};

struct StatementStack{
	std::vector<Statement> stack;
};

struct Script{
	std::string path;
	std::string name;

	StatementStack stack;
};



class Lexer{
public:
	Lexer();
	~Lexer();

	void addSymbol(std::string symbol, SymbolType type);
	void addKeyword(std::string symbol, KeywordType type);
	TokenStack analyze(std::ifstream& ss);
	void printStack();

private:
	Token currentToken();
	Token peekToken();
	Token extractNextToken(std::string& s, std::string& cutpart);
	//s is the string to extract from
	//cutpart is the string to store what was removed

	bool isToken(std::string& s, char next);
	bool isOperator(std::string& s);
	bool isKeyword(std::string& s);
	bool isLiteral(std::string& s);
	bool isCompleteLiteral(std::string& s);

	SymbolType getOperatorType(std::string& s);
	void setOperator(SymbolType t);
	void setOperator(std::string& s);

	KeywordType getPreprocessorType(std::string& s);

	std::map<SymbolType, std::string> mTypeToString;
	std::map<std::string, SymbolType> mStringToType;
	std::map<SymbolType, bool> mOperatorMap;
	std::map<std::string, SymbolType> mOperatorTypes;
	std::map<KeywordType, bool> mKeywordMap;
	std::map<std::string, KeywordType> mKeywordTypes;
	std::deque<Token> mTokens;
	std::string mLine;
	std::string mNextLine;
	TokenStack stack;
};

class Parser{
public:
	Parser();
	~Parser();

	void preprocess();
	void pass1();
	void pass2();
	Script parse(TokenStack ts);

};


class Interpreter{
public:
	Interpreter();
	~Interpreter();



};





class ScriptLayer{
public:
	ScriptLayer();
	~ScriptLayer();

	void startTests();

	void loadFile(std::string path, std::string name="");
	void runFile(std::string path, std::string name="");
	void runScript(std::string name);
private:
	std::map<std::string, Script> mScripts;
	Lexer l;
	Parser p;
	Interpreter i;
};

#endif