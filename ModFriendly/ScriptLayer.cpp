#include "stdafx.h"
#include "ScriptLayer.h"
#include "lauxlib.h"


bool isnumber(std::string& s){
	return !s.empty() && s.find_first_not_of("0123456789")==s.npos;
};

ScriptLayer::ScriptLayer():
	l(),
	p(),
	i()
{

};
ScriptLayer::~ScriptLayer(){
};

void ScriptLayer::startTests(){
	loadFile("test.txt");
};

void ScriptLayer::loadFile(std::string path, std::string name){
	std::ifstream filein;
	filein.open("script/test.custom");
	if(filein){
		std::cout<<"            Load File: Success. (OK)"<<std::endl;
	}else{
		std::cout<<"            Load File: Failed. (ERROR)"<<std::endl;
	}
	l.analyze(filein);
};

void ScriptLayer::runFile(std::string path, std::string name){

};

void ScriptLayer::runScript(std::string name){

};


Lexer::Lexer(){
	mTypeToString;
	mStringToType;
	mOperatorMap;
	mOperatorTypes;
	mKeywordMap;
	mKeywordTypes;
	mTokens;
	mLine;
	mNextLine;
	stack;

	addSymbol("(",SymbolType::SYMBOL_LPARENTHESIS);
	addSymbol(")",SymbolType::SYMBOL_RPARENTHESIS);
	addSymbol("[",SymbolType::SYMBOL_LBRACKET);
	addSymbol("]",SymbolType::SYMBOL_RBRACKET);
	addSymbol("{",SymbolType::SYMBOL_LBRACE);
	addSymbol("}",SymbolType::SYMBOL_RBRACE);
	addSymbol(";",SymbolType::SYMBOL_SEMICOLON);
	addSymbol(".",SymbolType::SYMBOL_DOT);
	addSymbol(",",SymbolType::SYMBOL_COMMA);
	addSymbol(":",SymbolType::SYMBOL_COLON);
	addSymbol("+",SymbolType::SYMBOL_PLUS);
	addSymbol("-",SymbolType::SYMBOL_MINUS);
	addSymbol("/",SymbolType::SYMBOL_DIVIDE);
	addSymbol("*",SymbolType::SYMBOL_MULTIPLY);
	addSymbol("%",SymbolType::SYMBOL_MOD);
	addSymbol("^",SymbolType::SYMBOL_POWER);
	addSymbol("=",SymbolType::SYMBOL_EQUALS);
	addSymbol("+=",SymbolType::SYMBOL_PLUSEQUALS);
	addSymbol("-=",SymbolType::SYMBOL_MINUSEQUALS);
	addSymbol("/=",SymbolType::SYMBOL_DIVIDEEQUALS);
	addSymbol("*=",SymbolType::SYMBOL_MULTIPLYEQUALS);
	addSymbol("%=",SymbolType::SYMBOL_MODEQUALS);
	addSymbol("^=",SymbolType::SYMBOL_POWEREQUALS);
	addSymbol("++",SymbolType::SYMBOL_INCREMENT);
	addSymbol("--",SymbolType::SYMBOL_DECREMENT);
	addSymbol("//",SymbolType::SYMBOL_COMMENTSINGLE);
	addSymbol("/*",SymbolType::SYMBOL_COMMENTMULTISTART);
	addSymbol("*/",SymbolType::SYMBOL_COMMENTMULTIEND);
	addSymbol("#",SymbolType::SYMBOL_PREPROCESSOR);
	addSymbol("\"",SymbolType::SYMBOL_STRINGDELIM);

	setOperator(SymbolType::SYMBOL_COLON);
	setOperator(SymbolType::SYMBOL_COMMA);
	setOperator(SymbolType::SYMBOL_DIVIDE);
	setOperator(SymbolType::SYMBOL_DIVIDEEQUALS);
	setOperator(SymbolType::SYMBOL_DECREMENT);
	setOperator(SymbolType::SYMBOL_DOT);
	setOperator(SymbolType::SYMBOL_INCREMENT);
	setOperator(SymbolType::SYMBOL_LBRACE);
	setOperator(SymbolType::SYMBOL_LBRACKET);
	setOperator(SymbolType::SYMBOL_LPARENTHESIS);
	setOperator(SymbolType::SYMBOL_MINUS);
	setOperator(SymbolType::SYMBOL_MINUSEQUALS);
	setOperator(SymbolType::SYMBOL_MOD);
	setOperator(SymbolType::SYMBOL_MODEQUALS);
	setOperator(SymbolType::SYMBOL_MULTIPLY);
	setOperator(SymbolType::SYMBOL_MULTIPLYEQUALS);
	setOperator(SymbolType::SYMBOL_PLUS);
	setOperator(SymbolType::SYMBOL_PLUSEQUALS);
	setOperator(SymbolType::SYMBOL_POWER);
	setOperator(SymbolType::SYMBOL_POWEREQUALS);
	setOperator(SymbolType::SYMBOL_RBRACE);
	setOperator(SymbolType::SYMBOL_RBRACKET);
	setOperator(SymbolType::SYMBOL_RPARENTHESIS);
	setOperator(SymbolType::SYMBOL_SEMICOLON);

	addKeyword("require",KEYWORD_REQUIRE);
	addKeyword("imply", KEYWORD_IMPLY);
	addKeyword("peek", KEYWORD_PEEK);
	addKeyword("define", KEYWORD_DEFINE);
	addKeyword("global", KEYWORD_GLOBAL);
	addKeyword("local", KEYWORD_LOCAL);
	addKeyword("return", KEYWORD_RETURN);
};

Lexer::~Lexer(){
};

void Lexer::addSymbol(std::string symbol, SymbolType type){
	mTypeToString[type]=symbol;
	mStringToType[symbol]=type;
};
void Lexer::addKeyword(std::string symbol, KeywordType type){
	mKeywordMap[type]=true;
	mKeywordTypes[symbol]=type;
};
TokenStack Lexer::analyze(std::ifstream& ss){
	if(ss.is_open()){
		bool lookingForMultilineComment = false;
		bool lookingForStringDelim = false;
		bool lookingForPreprocessor = false;
		std::string stringmem = "";
		int stringstartcol = 0;
		std::string cutsection = "";
		int currentLine = 0;
		int currentColumn = 1;
		while(ss.good() && std::getline(ss,mNextLine)){
			currentLine++;
			currentColumn = 1;
			KeywordType preprocessorKeyword = KeywordType::KEYWORD_INVALID;
			bool singlelinecomment=false;
			mLine = mNextLine;
			std::replace(mLine.begin(), mLine.end(), '\t',' ');
			std::replace(mLine.begin(), mLine.end(), '\n',' ');
			std::replace(mLine.begin(), mLine.end(), '\v',' ');
			std::replace(mLine.begin(), mLine.end(), '\r',' ');
			while(mLine.length()>0){
				Token nextT = extractNextToken(mLine,cutsection);
				nextT.column = currentColumn;
				currentColumn += cutsection.length();
				nextT.line = currentLine;
				if(nextT.type==SYMBOL_INVALID){//mostly for empty lines
					continue;//continue with line
				}
				if(!lookingForMultilineComment){
					if(!lookingForStringDelim){
						if(!lookingForPreprocessor){
							if(nextT.type == SYMBOL_COMMENTSINGLE){
								break; //end line immediately, continue to next line
							}
							else if(nextT.type == SYMBOL_COMMENTMULTISTART){
								lookingForMultilineComment = true;
								continue; //continue with line, but we're done with this token
							}
							else if(nextT.type == SYMBOL_PREPROCESSOR){
								lookingForPreprocessor = true;
							}
							else if(nextT.type == SYMBOL_STRINGDELIM){
								lookingForStringDelim = true;
								stringstartcol = currentColumn;
								continue; //continue with line, but we're done with this token
							}
							else if(nextT.type == SYMBOL_KEYWORD){
								stack.stack.push_back(std::move(nextT));
							}
							else if(nextT.type == SYMBOL_IDENTIFIER){
								stack.stack.push_back(std::move(nextT));
							}
							else if(nextT.type == SYMBOL_SEMICOLON){
								Token endLine;
								endLine.type = SYMBOL_ENDLINE;
								endLine.line = currentLine;
								endLine.column = currentColumn;
								stack.stack.push_back(std::move(endLine));
								continue; //continue line, but we're done with this token
							}
							else if(nextT.type != SYMBOL_INVALID){
								stack.stack.push_back(std::move(nextT));
							}
						}else{//looking for preprocessor directive
							if(nextT.type != SYMBOL_KEYWORD){
								//skip line
								break;
							}else{
								preprocessorKeyword = getPreprocessorType(nextT.value);
								if(preprocessorKeyword!=KeywordType::KEYWORD_GLOBAL &&
									preprocessorKeyword!=KeywordType::KEYWORD_LOCAL &&
									preprocessorKeyword!=KeywordType::KEYWORD_INVALID){
										nextT.type = SYMBOL_PREPROCESSOR;
								}
								stack.stack.push_back(nextT);

								int trimupto = 0;
								//remove up to first comment, then let the lexer do the rest
								int singlelinecommentpos = mLine.find("//");
								int multilinecommentpos = mLine.find("/*");
								bool hassinglelinecomment = singlelinecommentpos != mLine.npos;
								bool hasmultilinecomment = multilinecommentpos != mLine.npos;
								if(hassinglelinecomment){//we need to at least remove up to a "//..."
									if(hasmultilinecomment){//we might need to remove up to a "/*..." instead
										if(singlelinecommentpos<multilinecommentpos){//nope, "//..." comes first, so doesn't matter
											trimupto = singlelinecommentpos;
										}else{//yes, "/*" comes first, so we'll trim up to that
											trimupto = multilinecommentpos;
										}
									}else{//we don't need to remove up to a "/*...", only a "//..."
										trimupto = singlelinecommentpos;
									}
								}else if(hasmultilinecomment){//we need to remove up to a "/*..."
									trimupto = multilinecommentpos;
								}else{
									//no comments, do the whole thing
									trimupto = mLine.length(); //everything goes
								}

								if(trimupto>mLine.length()){
									std::cout<<"Error processing preprocessor."<<std::endl;
								}
								std::string trimmed = mLine.substr(0,trimupto);
								mLine.erase(0, trimupto);
								int oldCol = currentColumn;
								currentColumn += trimupto; //move column count up before going through preprocessor

								while(trimmed.length()>0 && trimmed.at(0)==' '){
									trimmed.erase(0,1);
									oldCol++; //we only care about leading spaces for column recording
								}
								while(trimmed.length()>0 && trimmed.at(trimmed.length()-1)==' '){
									trimmed.erase(trimmed.length()-1,1);
								}
								switch(preprocessorKeyword){
								case KEYWORD_DEFINE:{
									Token identify;
									identify.type = SYMBOL_IDENTIFIER;
									identify.line = currentLine;
									Token replace;
									replace.type = SYMBOL_IDENTIFIER;
									replace.line = currentLine;
									int spacecount = std::count(trimmed.begin(), trimmed.end(), ' ');
									if(spacecount!=1){
										std::cout<<"Incorrect number of spaces: "<<trimmed<<std::endl;
									}else{
										int spacepos = trimmed.find(" ");
										if(spacepos!=trimmed.npos && spacepos!=(trimmed.length()-1)){
											std::string first = trimmed.substr(0,spacepos);
											std::string second = trimmed.substr(spacepos+1, trimmed.npos);
											identify.value = first;
											identify.column = oldCol;
											oldCol += first.length() + 1;
											replace.value = second;
											replace.column = oldCol;
											oldCol += second.length();
											if(isnumber(second)){
												replace.type = SYMBOL_LITERAL;
											}
											stack.stack.push_back(std::move(identify));
											stack.stack.push_back(std::move(replace));
										}else{
											std::cout<<"Bad argument list: "<<trimmed<<std::endl;
										}
									}
									break;
													}
								case KEYWORD_IMPLY:{
									Token implied;
									implied.type = SYMBOL_LITERAL;
									implied.value = trimmed;
									implied.line = currentLine;
									implied.column = oldCol;
									oldCol += trimmed.length();
									stack.stack.push_back(std::move(implied));
									break;
												   }
								case KEYWORD_PEEK:{
									Token peeked;
									peeked.type = SYMBOL_LITERAL;
									peeked.value = trimmed;
									peeked.line = currentLine;
									peeked.column = oldCol;
									oldCol += trimmed.length();
									stack.stack.push_back(std::move(peeked));
									break;
												  }
								case KEYWORD_REQUIRE:{
									Token required;
									required.type = SYMBOL_LITERAL;
									required.value = trimmed;
									required.line = currentLine;
									required.column = oldCol;
									oldCol += trimmed.length();
									stack.stack.push_back(std::move(required));
									break;
													 }
								default:{
									//error?
									std::cout<<"Not a valid preprocessor: "<<nextT.value<<std::endl;
									break;
										}
								};
							
								Token endLine;
								endLine.type = SYMBOL_ENDLINE;
								endLine.line = currentLine;
								endLine.column = oldCol;
								stack.stack.push_back(std::move(endLine));
								lookingForPreprocessor=false;
								continue; //continue with line
							}
						}
					}else{//looking for string end
						if(nextT.type == SYMBOL_STRINGDELIM){
							//found end of string
							Token stringLiteral;
							stringLiteral.type = SYMBOL_LITERAL;
							stringLiteral.value = stringmem;
							stringLiteral.column = stringstartcol-1;
							stringLiteral.line = currentLine;
							stack.stack.push_back(std::move(stringLiteral));
							lookingForStringDelim = false;
							stringmem = "";
							continue; //continue line, but we're done with this string
						}else{
							stringmem.append(cutsection);
						}
					}
				}else{//looking for multiline comment end
					if(nextT.type == SYMBOL_COMMENTMULTIEND){
						lookingForMultilineComment = false;
					}
				}
			}
		}
		ss.close();
	}
	printStack();
	return stack;
};

void Lexer::printStack(){
	for(auto it = stack.stack.begin(); it!= stack.stack.end(); it++){
		Token& t = *it;
		if(t.type==SYMBOL_ENDLINE){
			std::cout<<std::endl;
		}else if(t.type==SYMBOL_PREPROCESSOR){
			std::cout<<"#"<<t.value<<"   ";
		}else if(t.type==SYMBOL_INVALID){
			std::cout<<"INVALID";
		}else if(t.type==SYMBOL_SEMICOLON){
			std::cout<<"OOPS;;;";
		}else{
			std::cout<<t.value<<"   ";
		}
	}
};

Token Lexer::currentToken(){
	return mTokens.front();
};
Token Lexer::peekToken(){
	if(mTokens.size()>1){
		return mTokens.at(1);
	}else{
		Token invalid;
		invalid.type = SYMBOL_INVALID;
		return invalid;
	}
};
Token Lexer::extractNextToken(std::string& s, std::string& cutpart){
	Token result;
	result.column=0;
	result.line=0;
	result.type=SYMBOL_INVALID;
	result.value=s;

	cutpart = "";

	//get rid of leading whitespace
	int whitespaceend = 0;
	while(whitespaceend<s.length() && s.at(whitespaceend)==' '){
		whitespaceend++;
	}
	cutpart.append(s.substr(0,whitespaceend));
	s.erase(0,whitespaceend);

	int tokenend = 1;
	while(tokenend<s.length() && !isToken(s.substr(0,tokenend), s.at(tokenend))){
		tokenend++;
	}
	std::string tokenstr = s.substr(0,tokenend);
	if(isOperator(tokenstr)){
		result.type = getOperatorType(tokenstr);
	}else if(isKeyword(tokenstr)){
		result.type = SYMBOL_KEYWORD;
	}else if(isLiteral(tokenstr)){
		result.type = SYMBOL_LITERAL;
	}else{//identifier
		if(tokenstr=="//"){
			result.type = SYMBOL_COMMENTSINGLE;
		}else if(tokenstr=="/*"){
			result.type = SYMBOL_COMMENTMULTISTART;
		}else if(tokenstr=="*/"){
			result.type = SYMBOL_COMMENTMULTIEND;
		}else if(tokenstr=="#"){
			result.type = SYMBOL_PREPROCESSOR;
		}else if(tokenstr=="\""){
			result.type = SYMBOL_STRINGDELIM;
		}else{
			if(tokenstr.length()==0 || tokenstr.find_first_not_of(" ")==tokenstr.npos){
				result.type = SYMBOL_INVALID;
			}else{
				if(isnumber(tokenstr)){
					result.type = SYMBOL_LITERAL;
				}else{
					result.type = SYMBOL_IDENTIFIER;
				}
			}
		}
	}
	
	result.value = tokenstr;
	cutpart.append(tokenstr);

	//get rid of token and trailing whitespace
	whitespaceend = tokenend;
	while(whitespaceend<s.length() && s.at(whitespaceend)==' '){
		whitespaceend++;
	}
	if(tokenend<s.length()){
		if(whitespaceend<=s.length()){
			cutpart.append(s.substr(tokenend,(whitespaceend-tokenend)));
		}else{
			cutpart.append(s.substr(tokenend, s.npos-tokenend));
		}
	}
	s.erase(0,whitespaceend);

	return result;
};


bool Lexer::isToken(std::string& s, char next){
	//s1 = s + next
	//if s is in the token map AND s1 doesn't complete another token
	//OR next is whitespace
	if(next=='\0'){
		return mStringToType.find(s)!=mStringToType.end();
	}
	std::stringstream ss;
	ss<<s<<next;
	return (mStringToType.find(s)!=mStringToType.end() && !isToken(ss.str(),'\0')) || next==' ' ||
		(!isOperator(s) && isOperator(std::string(1,next)));
};
bool Lexer::isOperator(std::string& s){
	return mOperatorTypes.find(s)!=mOperatorTypes.end() && mOperatorMap.find(mOperatorTypes[s])!=mOperatorMap.end() &&
		mOperatorMap[mOperatorTypes[s]];
};
bool Lexer::isKeyword(std::string& s){
	return mKeywordTypes.find(s)!=mKeywordTypes.end() && mKeywordMap.find(mKeywordTypes[s])!=mKeywordMap.end() &&
		mKeywordMap[mKeywordTypes[s]];
};
bool Lexer::isLiteral(std::string& s){
	return false;

};
bool Lexer::isCompleteLiteral(std::string& s){
	return false;

};
SymbolType Lexer::getOperatorType(std::string& s){
	if(mStringToType.find(s)!=mStringToType.end()){
		return mStringToType[s];
	}else{
		return SYMBOL_INVALID;
	}
};

void Lexer::setOperator(std::string& s){
	if(mStringToType.find(s)!=mStringToType.end()){
		SymbolType t = mStringToType[s];
		mOperatorMap[t]=true;
		mOperatorTypes[s]=t;
	}
};
void Lexer::setOperator(SymbolType t){
	if(mTypeToString.find(t)!=mTypeToString.end()){
		std::string& s = mTypeToString[t];
		mOperatorMap[t]=true;
		mOperatorTypes[s]=t;
	}
};

KeywordType Lexer::getPreprocessorType(std::string& s){
	if(mKeywordTypes.find(s)!=mKeywordTypes.end()){
		return mKeywordTypes[s];
	}else{
		return KEYWORD_INVALID;
	}
};

Parser::Parser(){

};
Parser::~Parser(){

};
void Parser::preprocess(){

};
void Parser::pass1(){

};
void Parser::pass2(){

};
Script Parser::parse(TokenStack ts){
	return Script();
};

Interpreter::Interpreter(){

};
Interpreter::~Interpreter(){

};