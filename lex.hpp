//
//  lex.hpp
//  sparrowJs
//
//  Created by yang on 17/1/19.
//  Copyright © 2017年 yang. All rights reserved.
//

#ifndef lex_hpp
#define lex_hpp

#include <string>
#include "utils.hpp"
enum LEX_TYPE{
    L_EOF=0,
    L_ID=256,// avoid starting from 226 and Ascii code coincidence
    L_IF,
    L_ELSE,
    L_ELSEIF,
    L_DO,
    L_WHILE,
    L_FOR,
    L_BREAK,
    L_CONTINUE,
    L_FUNCTION,
    L_RETURN,
    L_VAR,
    L_TRUE,
    L_FALSE,
    L_NULL,
    L_UNDEFINED,
    L_NEW,
    
    L_REF,
    
    L_INT,
    L_FLOAT,
    L_STRING,
    L_ARRAY,
    
    L_PLUS,
    L_MULT,
    
    L_EQUAL,
    L_TYPEEQUAL,
    L_NEQUAL,
    L_NTYPEEQUAL,
    L_LEQUAL,
    L_LSHIFT,
    L_LSHIFTEQUAL,
    L_GEQUAL,
    L_RSHIFT,
    L_RSHIFTUNSIGNED,
    L_RSHIFTEQUAL,
    L_PLUSEQUAL,
    L_MINUSEQUAL,
    L_PLUSPLUS,
    L_MINUSMINUS,
    L_ANDEQUAL,
    L_ANDAND,
    L_OREQUAL,
    L_OROR,
    L_XOREQUAL
};
class jsException {
public:
    std::string text;
    jsException(const std::string &exceptionText);
};
// parsing
class lex{
    public:
        lex(const std::string &input);
        ~lex();
        std::string tokenStr;
        int token;
        char currCh,lastCh;
        void analyses(int expected);
        int getDataPos();
        int tokenStart;
    
        int tokenLastEnd,tokenEnd;
        std::string getSubData(int &pos,int &factor);
        std::string getSubString(int lastPosition);
        void reset();
    private:
        char *data;
        int dataStart,dataEnd,dataPos;
        void getNextChar();
        void getNextToken();
        utils* util;
};

#endif /* lex_hpp */
