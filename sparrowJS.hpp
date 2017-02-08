//
//  sparrowJS.hpp
//  sparrowJs
//
//  Created by yang on 17/1/24.
//  Copyright © 2017年 yang. All rights reserved.
//

#ifndef sparrowJS_hpp
#define sparrowJS_hpp

#include <string>
#include <vector>
#include <stack>
#include <sstream>
#include "lex.hpp"

typedef struct variable VARIABLE;
typedef struct funcVar FUNCVAR;
typedef struct factor FACTOR;
typedef struct symbol SYMBOL;

typedef void (*nativeFuncPtr)(VARIABLE &var, void *data ,void *jsContent);

struct variable{
    int refs;
    int token;
    std::string name;
    std::string strData;
    double doubleData;
    signed int intData;
    FUNCVAR* refsFunc;
    
    VARIABLE* prev;
    VARIABLE* next;
    VARIABLE* first;
    VARIABLE* last;
    
    void addChildNoDup(VARIABLE* node){
        VARIABLE* v=this->next;
        if(!v){
            this->next=node;
        }else{
            while(v){
                if(!v->next){
                    v->next=node;
                    break;
                }else{
                    v=this->next;
                }
            }
        }
    };
};

struct funcVar{
    int refs;
    int token;
    std::string name;
    std::string strData;
    
    VARIABLE* arguments;
    
    VARIABLE* prev;
    VARIABLE* next;
    VARIABLE* first;
    VARIABLE* last;
    
    nativeFuncPtr ptr;
};

struct factor{
    int token;
    std::string strData;
    double doubleData;
    int intData;
};
struct symbol{
    int token;
    std::string strData;
};

template <class FactorType>
class FactorStack {
    public:
        FactorStack();
        FactorType top();
        void push(FactorType e);
        FactorType pop();
        bool empty();
    private:
        int count;
        const static int MAXSIZE=100;
        FactorType data[MAXSIZE];
};

class sparrowJS{
    public:
        ~sparrowJS();
        void elva(const std::string &code);
        void addNativeFunc(const std::string &name,nativeFuncPtr ptr,void* data);
    private:
        std::vector<VARIABLE*> stack;// analytic time variable scope
        void stmt(lex* const &l);// execute statement
        VARIABLE* findVariable(const std::string &name);
        VARIABLE* expression(lex* const &l);
        template <typename T>
        int isType(const T &v);
    
        void postfix(char* &pre ,char* &rpn,int &n);// rpn
        double rpnValue(char* &post);// value
    
        std::stringstream* strem=new std::stringstream("");
        void addChild(VARIABLE* node);
        bool environmental=false;
    
        VARIABLE* expressions(lex* const &l);// expressions
        VARIABLE* valueExpression(lex* const &l);// = += -=
        VARIABLE* ternary(lex* const &l);// ?:
        VARIABLE* logic(lex* const &l);// & | ^ && ||
        VARIABLE* condition(lex* const &l);// == != === !=== <= >=
        VARIABLE* shift(lex* const &l);// << >> >>>
        VARIABLE* plusMinus(lex* const &l);// + - ++ --
        VARIABLE* term(lex* const &l);// * / %
        VARIABLE* unary(lex* const &l);// !
        VARIABLE* factor(lex* const &l);// * / %
        VARIABLE* functionCall(lex* const &l,VARIABLE* v,const std::string &name);
        VARIABLE* mathsOp(VARIABLE* fa,VARIABLE* fb,int s);
};

#endif /* sparrowJS_hpp */
