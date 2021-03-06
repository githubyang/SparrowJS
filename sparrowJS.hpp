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
typedef struct array ARRAY;
typedef struct object OBJECT;

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
    ARRAY* array;
    OBJECT* object;
    
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
struct array{
    int refs;
    int token;
    unsigned int key;
    
    ARRAY* prev;
    ARRAY* next;
    ARRAY* first;
    ARRAY* last;
    
    VARIABLE* value;
    void addChildNoDup(ARRAY* node){
        ARRAY* v=this->next;
        if(!v){
            this->next=node;
        }else{
            while(v){
                if(!v->next){
                    v->next=node;
                    break;
                }else{
                    v=v->next;
                }
            }
        }
    };
    VARIABLE* findChild(int &n){
        ARRAY* v=this->next;
        while(v){
            if(v->key==n){
                return v->value;
                break;
            }else{
                v=v->next;
            }
        }
        return new VARIABLE{0};
    };
};

struct object{
    int refs;
    int token;
    std::string key;
    
    VARIABLE* value;
    
    OBJECT* prev;
    OBJECT* next;
    OBJECT* first;
    OBJECT* last;
    
    void addChildNoDup(OBJECT* node){
        OBJECT* v=this->next;
        if(!v){
            this->next=node;
        }else{
            while(v){
                if(!v->next){
                    v->next=node;
                    break;
                }else{
                    v=v->next;
                }
            }
        }
    };
    VARIABLE* findChild(std::string &s){
        OBJECT* v=this->next;
        while(v){
            if(v->key==s){
                return v->value;
                break;
            }else{
                v=v->next;
            }
        }
        return new VARIABLE{0};
    };
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
        VARIABLE* unary(lex* const &l,VARIABLE* v=nullptr);// !
        VARIABLE* factor(lex* const &l,VARIABLE* v=nullptr);// * / %
        VARIABLE* functionCall(lex* const &l,VARIABLE* v,const std::string &name);
        VARIABLE* mathsOp(VARIABLE* fa,VARIABLE* fb,int s);
    
        bool isIf=true;
        VARIABLE* top();
};

#endif /* sparrowJS_hpp */
