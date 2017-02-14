//
//  sparrowJS.cpp
//  sparrowJs
//
//  Created by yang on 17/1/24.
//  Copyright © 2017年 yang. All rights reserved.
//

#include "sparrowJS.hpp"
#include "lex.hpp"

template <class T> FactorStack<T>::FactorStack(){
    this->count=0;
}

template <class T> T FactorStack<T>::top(){
    if(this->count==0){
        throw new jsException("Out of operation of the stack is empty");
    }
    return this->data[(this->count-1)];
}

template <class T> void FactorStack<T>::push(T e){
    if(this->count>MAXSIZE){
        throw new jsException("Press in stack");
    }
    this->data[(this->count)]=e;
    this->count+=1;
}

template <class T> T FactorStack<T>::pop(){
    if(this->count==0){
        throw new jsException("Press the operation and launch of the stack is empty");
    }
    T e=this->data[(this->count-1)];
    this->count-=1;
    return e;
}

template <class T> bool FactorStack<T>::empty(){
    return this->count==0?true:false;
}




template <typename T>
int sparrowJS::isType(const T &v){
    int r=0;
    if(v.token==L_INT){
        r=L_INT;
    }else if(v.token==L_FLOAT){
        r=L_FLOAT;
    }
    return r;
}

sparrowJS::~sparrowJS(){
    delete strem;
}

void sparrowJS::postfix(char *&pre, char *&rpn, int &n){
    FactorStack<char> symbolStack;
    int i=0,j=0;
    utils* util=new utils();
    symbolStack.push('#');
    bool minusSign=false;
    while (util->priority(pre[i])!=-1) {
        if( (i==0&&pre[i]=='-')||(i!=0&&pre[i]=='-'&&pre[i-1]=='(')||(util->isNumber(pre[i])||pre[i]=='.') ){
            rpn[j++]=pre[i];
            n+=1;
        }
        else if(pre[i]=='('&&pre[i+1]!='-'){
            symbolStack.push(pre[i]);
        }
        else if(pre[i]=='('&&pre[i+1]=='-'){
            minusSign=true;
        }
        else if(pre[i]==')'&&!minusSign){
            while (symbolStack.top()!='(') {
                rpn[j++]=symbolStack.pop();
                n+=1;
            }
            symbolStack.pop();
        }
        else if(pre[i]==')'&&minusSign){
            minusSign=false;
        }
        else if(util->isOperator(pre[i])){
            rpn[j++]=' ';
            n+=1;
            char t=symbolStack.top();
            if( (t=='='&&pre[i]=='=')||(t=='>'&&pre[i]=='=')||(t=='='&&pre[i]=='<')||(t=='|'&&pre[i]=='|')){
                symbolStack.push(pre[i]);
            }
            else{
                while ( util->priority(pre[i]) <= util->priority(t) ) {
                    rpn[j++]=symbolStack.pop();
                    t=symbolStack.top();
                    n+=1;
                }
                symbolStack.push(pre[i]);
            }
        }
        i+=1;
    }
    
    while (!symbolStack.empty()) {
        rpn[j++]=symbolStack.pop();
        n+=1;
    }
    delete util;
}

double sparrowJS::rpnValue(char* &rpn){
    FactorStack<double> factorStack;
    int i=0;
    utils* util=new utils();
    double a,b;
    while (rpn[i]!='#') {
        if( (rpn[i]=='-'&&i==0)||(rpn[i]=='-'&&i!=0&&rpn[i-1]==' ')||(util->isNumber(rpn[i])||rpn[i]=='.')){
            factorStack.push(util->translatedNumber(rpn, i));
        }
        else if(rpn[i]==' '){
            i+=1;
        }
        else if(rpn[i]=='*'){
            a=factorStack.pop();
            b=factorStack.pop();
            factorStack.push(a*b);
            i+=1;
        }
        else if (rpn[i]=='/'){
            a=factorStack.pop();
            b=factorStack.pop();
            factorStack.push(b/a);
            i+=1;
        }
        else if (rpn[i]=='+'){
            a=factorStack.pop();
            b=factorStack.pop();
            factorStack.push(a+b);
            i+=1;
        }
        else if (rpn[i]=='-'){
            a=factorStack.pop();
            b=factorStack.pop();
            factorStack.push(b-a);
            i+=1;
        }
        else if (rpn[i]=='='&&rpn[i+1]=='='){
            a=factorStack.pop();
            b=factorStack.pop();
            factorStack.push(b==a);
            i+=2;
        }
        else if (rpn[i]=='='&&rpn[i+1]=='<'){
            a=factorStack.pop();
            b=factorStack.pop();
            factorStack.push(b<=a);
            i+=2;
        }
        else if (rpn[i]=='='&&rpn[i+1]=='>'){
            a=factorStack.pop();
            b=factorStack.pop();
            factorStack.push(b>=a);
            i+=2;
        }
        else if (rpn[i]=='|'&&rpn[i+1]=='|'){
            a=factorStack.pop();
            b=factorStack.pop();
            factorStack.push(b||a);
            i+=2;
        }
        else if (rpn[i]=='<'){
            a=factorStack.pop();
            b=factorStack.pop();
            factorStack.push(b<a);
            i+=1;
        }
        else if (rpn[i]=='>'){
            a=factorStack.pop();
            b=factorStack.pop();
            factorStack.push(b>a);
            i+=1;
        }
    }
    return factorStack.pop();
}

VARIABLE* sparrowJS::expression(lex *const &l){
    VARIABLE* res=new VARIABLE{0};
    char *exp =new char[100];
    memset(exp, 0, 100);
    while (l->token!=59&&l->token!=0&&l->token!=9&&l->token!=10&&l->token!=13) {
        if(l->token!=L_ID&&l->token!=L_INT&&l->token!=L_FLOAT){
            switch (l->token) {
                case 40:
                    strcat(exp,"(");
                    break;
                case 41:
                    strcat(exp,")");
                    break;
                case 42:
                    strcat(exp,"*");
                    break;
                case 47:
                    strcat(exp,"/");
                    break;
                case 43:
                    strcat(exp,"+");
                    break;
                case 45:
                    strcat(exp,"-");
                    break;
                    
                case L_EQUAL:
                    strcat(exp,"==");
                    break;
                case L_LEQUAL:
                    strcat(exp,"<=");
                    break;
                case L_GEQUAL:
                    strcat(exp,">=");
                    break;
                case L_ANDAND:
                    strcat(exp,"&&");
                    break;
                case L_OROR:
                    strcat(exp,"||");
                    break;
                    
                case 60:
                    strcat(exp,"<");
                    break;
                case 62:
                    strcat(exp,">");
                    break;
                    
                default:
                    break;
            }
        }else if(l->token==L_ID){
            VARIABLE* t=this->findVariable(l->tokenStr);
            int type=this->isType(*t);
            char* result=new char[100];
            if(type==L_INT){
                (*this->strem).str("");
                (*this->strem).clear();
                (*this->strem)<<t->intData;
                (*this->strem)>>result;
                strcat(exp,result);
            }
            if(type==L_FLOAT){
                (*this->strem).str("");
                (*this->strem).clear();
                (*this->strem)<<t->doubleData;
                (*this->strem)>>result;
                strcat(exp,result);
            }
            delete []result;
        }else if(l->token==L_FLOAT){
            strcat(exp,l->tokenStr.c_str());
        }else if(l->token==L_STRING){
            strcat(exp,l->tokenStr.c_str());
        }else if(l->token==L_INT){
            strcat(exp,l->tokenStr.c_str());
        }
        l->analyses(l->token);
    }
    strcat(exp,"#");
    
    char *rpn=new char[100];
    memset(rpn, 0, 100);
    int n=0;
    this->postfix(exp, rpn, n);
    double v=this->rpnValue(rpn);
    res->token=L_FLOAT;
    res->doubleData=v;
    return res;
}

void sparrowJS::addChild(VARIABLE *node){
    if(this->stack.size()==0){
        this->stack.push_back(node);
    }else{
        VARIABLE* v=this->stack.back();
        VARIABLE* t=this->stack.back();
        if(!v->next){
            v->next=node;
        }else{
            while (v) {
                if(!v->next){
                    if(v->name==node->name){
                        node->next=v->next;
                        node->prev=v->prev;
                        *v=*node;
                    }else{
                        v->next=node;
                        node->prev=v;
                    }
                    break;
                }
                v=v->next;
            }
            t->last=node;
        }
    }
    
}


VARIABLE* sparrowJS::expressions(lex *const &l){
    VARIABLE* res=this->valueExpression(l);
    return res;
}

VARIABLE* sparrowJS::valueExpression(lex *const &l){
    VARIABLE* res=this->ternary(l);
    if(l->token==L_PLUSEQUAL||l->token==L_MINUSEQUAL||l->token=='='){
        int s=l->token;
        l->analyses(l->token);
        VARIABLE* f=this->ternary(l);
        res=this->mathsOp(res, f, s);
    }
    return res;
}

VARIABLE* sparrowJS::ternary(lex *const &l){
    VARIABLE* res=this->logic(l);
    if(l->token=='?'){
        l->analyses('?');
        int t=this->isType(*res);
        int b=0;
        if(t==L_INT){
            b=res->intData;
        }else if(t==L_FLOAT){
            b=(int)res->doubleData;
        }
        if(b>0){
            res=this->expressions(l);
            l->analyses(':');
            this->expressions(l);
        }else{
            this->expressions(l);
            l->analyses(':');
            res=this->expressions(l);
        }
    }
    return res;
}
VARIABLE* sparrowJS::unary(lex *const &l,VARIABLE* v){
    VARIABLE* res=nullptr;
    if(l->token=='!'){
        int s=l->token;
        l->analyses(l->token);
        VARIABLE* f=this->factor(l);
        res=this->mathsOp(res, f, s);
    }else{
        res=this->factor(l,v);
    }
    
    return res;
}
VARIABLE* sparrowJS::logic(lex *const &l){
    VARIABLE* res=this->condition(l);
    while (l->token==L_ANDAND||l->token==L_OROR||l->token==L_NTYPEEQUAL) {
        int s=l->token;
        l->analyses(l->token);
        VARIABLE* f=this->condition(l);
        res=this->mathsOp(res, f, s);
        
    }
    return res;
}
VARIABLE* sparrowJS::condition(lex *const &l){
    VARIABLE* res=this->shift(l);
    while (l->token==L_TYPEEQUAL||l->token==L_LEQUAL||l->token==L_GEQUAL||l->token=='<'||l->token=='>') {
        int s=l->token;
        l->analyses(l->token);
        VARIABLE* f=this->shift(l);
        res=this->mathsOp(res, f, s);
    }
    return res;
}
VARIABLE* sparrowJS::shift(lex *const &l){
    VARIABLE* res=this->plusMinus(l);
    return res;
}
VARIABLE* sparrowJS::plusMinus(lex *const &l){
    bool negate = false;
    if (l->token=='-') {
        l->analyses('-');
        negate = true;
    }
    VARIABLE* res=this->term(l);
    if(negate){
        VARIABLE* zero=new VARIABLE{0};
        zero->token=L_INT;
        zero->intData=0;
        res=this->mathsOp(zero,res,'-');
    }
    while (l->token=='+'||l->token=='-') {
        int s=l->token;
        l->analyses(s);
        VARIABLE* f=this->term(l);
        res=this->mathsOp(res,f,s);
    }
    if(l->token==L_PLUSPLUS){
        int s=l->token;
        l->analyses(s);
        res=this->mathsOp(res,res,s);
    }
    return res;
}
VARIABLE* sparrowJS::term(lex *const &l){
    VARIABLE* res=this->unary(l);
    while(l->token=='*'||l->token=='/'||l->token=='%'){
        int s=l->token;
        l->analyses(s);
        VARIABLE* f=this->unary(l);
        res=this->mathsOp(res,f,s);
    }
    return res;
}
VARIABLE* sparrowJS::factor(lex *const &l,VARIABLE* v){
    VARIABLE* res=nullptr;
    if(l->token=='('){
        l->analyses(l->token);
        res=this->expressions(l);
        l->analyses(')');
        return res;
    }
    if(l->token=='{'){
        res=new VARIABLE{0};
        OBJECT* obj=new OBJECT{0};
        l->analyses('{');
        while (l->token!='}') {
            if(l->token==L_ID||l->token==L_STRING){
                std::string name=l->tokenStr;
                VARIABLE* node=new VARIABLE{0};
                OBJECT* object=new OBJECT{0};
                l->analyses(l->token);
                l->analyses(':');
                node=this->expressions(l);
                object->key=name;
                object->value=node;
                obj->addChildNoDup(object);
            }else{
            }
            if(l->token==','){
                l->analyses(',');
            }else if(l->token==';'){
                break;
            }
        }
        res->object=obj;
        res->token=L_OBJECT;
        res->refs=1;
        return res;
    }
    
    if(l->token==L_ID){
        if(v){
            res=v;
        }else{
            res=this->findVariable(l->tokenStr);
        }
        if(!res){
            res=new VARIABLE{0};
        }
        
        std::string idName=l->tokenStr;
        l->analyses(L_ID);
        
        if (l->token=='('||l->token=='['||l->token=='='||l->token=='.') {
            VARIABLE* temp=nullptr;
            if(v){
                temp=v;
            }else{
                temp=this->findVariable(idName);
            }
            if(!temp){
                throw new jsException("Variable or function not defined");
            }
            if(l->token=='='){
                l->analyses(l->token);
                VARIABLE* v=this->expressions(l);
                res=this->mathsOp(temp, v, '=');
                return res;
            }
            if(l->token=='.'){
                l->analyses('.');
                std::string name=l->tokenStr;
                if(temp->token==L_OBJECT){
                    res=temp->object->findChild(name);
                }
                res=this->factor(l,res);
                return res;
            }
            
            l->analyses(l->token);
            if(temp->token==L_FUNCTION){
                if(strcmp(temp->refsFunc->strData.c_str(),"")){
                    res=this->functionCall(l, temp, idName);
                }else{
                    VARIABLE* param=new VARIABLE{0};
                    while (l->token!=')') {
                        VARIABLE* o=nullptr;
                        if(l->token==L_INT){
                            o=new VARIABLE{0};
                            o->token=L_INT;
                            o->intData=std::stoi(l->tokenStr.c_str());
                        }
                        if(l->token==L_FLOAT){
                            double result;
                            (*this->strem).str("");
                            (*this->strem).clear();
                            (*this->strem)<<l->tokenStr;
                            (*this->strem)>>result;
                            o=new VARIABLE{0};
                            o->token=L_FLOAT;
                            o->doubleData=result;
                        }
                        if(l->token==L_ID){
                            o=this->findVariable(l->tokenStr);
                            if(!o){
                                o=new VARIABLE{0};
                            }
                            
                        }
                        if(!param->next){
                            param->next=o;
                        }else{
                            VARIABLE* v=param->next;
                            VARIABLE* t=nullptr;
                            while (v) {
                                if(!v->next){
                                    t=v;
                                }
                                v=v->next;
                            }
                            t->next=o;
                        }
                        l->analyses(l->token);
                    }
                    temp->refsFunc->ptr(*param,0,0);
                    l->analyses(')');
                    delete param;
                }
            }
            else if (temp->token==L_ARRAY){
                VARIABLE* r=this->expressions(l);
                l->analyses(']');
                VARIABLE* res=temp->array->findChild(r->intData);
                return res;
            }
        }
        return res;
    }
    if(l->token==L_INT){
        VARIABLE* res=new VARIABLE{0};
        res->token=l->token;
        (*this->strem).str("");
        (*this->strem).clear();
        (*this->strem)<<l->tokenStr;
        (*this->strem)>>(res->intData);
        l->analyses(l->token);
        return res;
    }
    if(l->token==L_FLOAT){
        VARIABLE* res=new VARIABLE{0};
        res->token=l->token;
        (*this->strem).str("");
        (*this->strem).clear();
        (*this->strem)<<l->tokenStr;
        (*this->strem)>>(res->doubleData);
        l->analyses(l->token);
        return res;
    }
    if(l->token==L_FUNCTION){
        l->analyses(L_FUNCTION);
        VARIABLE* res=new VARIABLE{0};
        l->analyses(l->token);
        FUNCVAR* fn=new FUNCVAR{0};
        fn->arguments=new VARIABLE{0};
        
        res->refsFunc=fn;
        
        while (l->token!=')') {
            VARIABLE* o=this->findVariable(l->tokenStr);
            if(!o){
                VARIABLE* p=new VARIABLE{0};
                p->name=l->tokenStr;
                fn->arguments->addChildNoDup(p);
            }else{
                fn->arguments->addChildNoDup(o);
            }
            l->analyses(l->token);
            if(l->token!=')'){
                l->analyses(',');
            }
        }
        l->analyses(')');
        int a=l->tokenStart;
        if(l->token=='{'){
            l->analyses(l->token);
            int brackets=1;
            while (l->token && brackets) {
                if (l->token == '{') brackets++;
                if (l->token == '}') brackets--;
                l->analyses(l->token);
            }
            std::string str=l->getSubString(a);
            res->refsFunc->strData=str;
            res->refsFunc->refs=1;
        }
        res->token=L_FUNCTION;
        return res;
    }
    if(l->token==L_STRING){
        VARIABLE* res=new VARIABLE{0};
        res->token=l->token;
        (*this->strem).str("");
        (*this->strem).clear();
        (*this->strem)<<l->tokenStr;
        (*this->strem)>>(res->strData);
        l->analyses(l->token);
        return res;
    }
    if(l->token=='['){
        VARIABLE* res=new VARIABLE{0};
        l->analyses('[');
        unsigned int n=0;
        ARRAY* array=new ARRAY{0};
        while (l->token!=']') {
            ARRAY* arr=new ARRAY{0};
            VARIABLE* v=this->expressions(l);
            arr->key=n;
            arr->value=v;
            array->addChildNoDup(arr);
            n+=1;
            if (l->token != ']') l->analyses(',');
        }
        l->analyses(']');
        res->token=L_ARRAY;
        res->array=array;
        return res;
    }
    return 0;
}

// 语句分析
void sparrowJS::stmt(lex* const &l){
    if(l->token==L_ID){
        this->isIf=true;
        this->expressions(l);
        l->analyses(l->token);
    }
    if (l->token==L_VAR){
        this->isIf=true;
        VARIABLE* node=nullptr;
        l->analyses(L_VAR);
        std::string variableName=l->tokenStr;
        while (l->token != ';') {
            l->analyses(l->token);
            if(l->token=='='){
                l->analyses(l->token);
                node=this->expressions(l);
                node->name=variableName;
                this->addChild(node);
            }
            if (l->token != ';'){
                l->analyses(l->token);
                variableName=l->tokenStr;
            }
        }
        l->analyses(';');
    }
    if(l->token==L_FUNCTION){
        this->isIf=true;
        l->analyses(L_FUNCTION);
        std::string funcName=l->tokenStr;
        VARIABLE* res=new VARIABLE{0};
                res->name=funcName;
        l->analyses(l->token);
        FUNCVAR* fn=new FUNCVAR{0};
        fn->arguments=new VARIABLE{0};
        res->refsFunc=fn;

        l->analyses('(');
        
        while (l->token!=')') {
            VARIABLE* o=this->findVariable(l->tokenStr);
            if(!o){
                VARIABLE* p=new VARIABLE{0};
                p->name=l->tokenStr;
                fn->arguments->addChildNoDup(p);
            }else{
                fn->arguments->addChildNoDup(o);
            }
            l->analyses(l->token);
            if(l->token!=')'){
                l->analyses(',');
            }
        }
        l->analyses(')');
        int a=l->tokenStart;
        
        
        if(l->token=='{'){
            l->analyses(l->token);
            int brackets=1;
            while (l->token && brackets) {
                if (l->token == '{') brackets++;
                if (l->token == '}') brackets--;
                l->analyses(l->token);
            }
            std::string str=l->getSubString(a);
            res->refsFunc->strData=str;
            res->refsFunc->refs=1;
        }
        this->addChild(res);
    }
    if (l->token==L_IF||l->token==L_ELSEIF){
        l->analyses(l->token);
        l->analyses('(');
        VARIABLE* res=this->expressions(l);
        l->analyses(')');
        if(res->intData>0){
            l->analyses('{');
            if(this->isIf){
                this->expressions(l);
                l->analyses(';');
                l->analyses('}');
                this->isIf=false;
            }else{
                int brackets=1;
                while (l->token && brackets) {
                    if (l->token == '{') brackets++;
                    if (l->token == '}') brackets--;
                    l->analyses(l->token);
                }
            }
            
            if(l->token){}
            //l->analyses('}');
        }else{
            l->analyses(l->token);
            int brackets=1;
            while (l->token && brackets) {
                if (l->token == '{') brackets++;
                if (l->token == '}') brackets--;
                l->analyses(l->token);
            }
            if(this->isIf&&l->token==L_ELSE){
                l->analyses(L_ELSE);
                l->analyses(l->token);
                this->expressions(l);
            }else if(!this->isIf&&l->token==L_ELSE){
                int brackets=1;
                while (l->token && brackets) {
                    if (l->token == '{') brackets++;
                    if (l->token == '}') brackets--;
                    l->analyses(l->token);
                }
            }
            
        }
    }
    if (l->token==L_ELSE){
        l->analyses(l->token);
        if(this->isIf){
            l->analyses('{');
            this->expressions(l);
            l->analyses(';');
            l->analyses('}');
        }else if(!this->isIf){
            l->analyses('{');
            int brackets=1;
            while (l->token && brackets) {
                if (l->token == '{') brackets++;
                if (l->token == '}') brackets--;
                l->analyses(l->token);
            }
        }
        this->isIf=true;
    }
    if (l->token==L_RETURN){
        this->isIf=true;
        l->analyses(L_RETURN);
        VARIABLE* res=new VARIABLE{0};
        if(l->token!=';'){
            res=this->expressions(l);
            res->last=res;
            this->stack.pop_back();
            this->stack.push_back(res);
        }
    }
    if(l->token==L_FOR){
        this->isIf=true;
        l->analyses(L_FOR);
        l->analyses('(');
        this->stmt(l);
        int start=l->tokenStart;
        
        while (l->token!=';') {
            l->analyses(l->token);
        }
        l->analyses(';');
        std::string str=l->getSubString(start);
        
        lex* newLex=new lex(str);
        VARIABLE* isLoop=this->expressions(newLex);
        bool one=isLoop->intData>0;
        if(one&&isLoop->intData>0){
            VARIABLE* t=this->expressions(l);
            l->analyses(')');
            int num=l->tokenStart;
            l->analyses('{');
            int brackets=1;
            while (l->token && brackets) {
                if (l->token == '{') brackets++;
                if (l->token == '}') brackets--;
                l->analyses(l->token);
            }
            std::string blockStr=l->getSubString(num);
            lex* blockLex=new lex(blockStr);
            
            while (isLoop->intData>0) {
                blockLex->reset();
                blockLex->analyses('{');
                this->stmt(blockLex);
                newLex->reset();
                isLoop=this->expressions(newLex);
                t->intData+=1;
            }
            delete blockLex;
            delete newLex;
        }else if(!one){
            this->expressions(l);
            l->analyses(')');
            l->analyses('{');
            int brackets=1;
            while (l->token && brackets) {
                if (l->token == '{') brackets++;
                if (l->token == '}') brackets--;
                l->analyses(l->token);
            }
        }
    }
    else{
        //l->analyses(l->token);
    }
    
}

void sparrowJS::elva(const std::string &code){
    lex *l=new lex(code);
    while (l->token) {
        try {
            this->stmt(l);
        } catch (jsException* e) {
            printf("\n %s \n",e->text.c_str());
            l->analyses(l->token);
        }
    }
    delete l;
}

VARIABLE* sparrowJS::top(){
    VARIABLE* res=nullptr;
    if(this->stack.size()==0){
        return NULL;
    }else{
        VARIABLE* v=this->stack[int(this->stack.size())-1];
        while (v) {
            if(!v->next){
                res=v;
                break;
            }
            v=v->next;
        }
    }
    return res;
}

VARIABLE* sparrowJS::findVariable(const std::string &name){
    VARIABLE* res=nullptr;
    if(this->stack.size()==0){
        return NULL;
    }else{
        VARIABLE* v=this->stack[int(this->stack.size())-1];
        while (v) {
            if(v->name==name){
                res=v;
                break;
            }
            v=v->next;
        }
        if(!res){
            v=this->stack[0];
            while (v) {
                if(v->name==name){
                    res=v;
                }
                v=v->next;
            }
        }
    }
    return res;
}

void sparrowJS::addNativeFunc(const std::string &name, nativeFuncPtr ptr, void *data){
    lex* l=new lex(name);
    l->analyses(L_FUNCTION);
    std::string funcName=l->tokenStr;
    VARIABLE* temp=this->findVariable(funcName);
    if(!temp){
        temp=new VARIABLE{0};
    }
    temp->token=L_FUNCTION;
    temp->refs=1;
    temp->name=funcName;
    FUNCVAR* fn=new FUNCVAR{0};
    fn->token=L_REF;
    fn->refs=1;
    fn->ptr=ptr;
    temp->refsFunc=fn;
    
    this->addChild(temp);
    //this->stack.push_back(temp);
    delete l;
}
VARIABLE *sparrowJS::functionCall(lex *const &l, VARIABLE *var,const std::string &name){
    lex* lexBlock=new lex(var->refsFunc->strData);
    
    VARIABLE* r=new VARIABLE{0};
    this->environmental=true;
    this->stack.push_back(r);
    
    VARIABLE* param=this->stack.back();
    while (l->token!=')') {
        VARIABLE* o=nullptr;
        o=this->expressions(l);
        if(!param->next){
            o->name=var->refsFunc->arguments->next->name;
            param->next=o;
        }else{
            VARIABLE* v=param->next;
            VARIABLE* p=var->refsFunc->arguments->next;
            while (v) {
                if(!v->next){
                    o->name=p->name;
                    v=o;
                    break;
                }
                v=v->next;
                p=p->next;
            }
            param->last=o;
        }
        //l->analyses(l->token);
    }
    l->analyses(')');
    lexBlock->analyses('{');
    this->stmt(lexBlock);
    //l->analyses(';');
    
    this->environmental=false;
    VARIABLE* res=(this->stack.back())->last;
    this->stack.pop_back();
    delete r;
    delete lexBlock;
    return res;
}
VARIABLE* sparrowJS::mathsOp(VARIABLE *fa, VARIABLE *fb, int s){
    int at=0,bt=0;
    VARIABLE* res=new VARIABLE{0};
    if(s!='!'){
        at=this->isType(*fa);
        bt=this->isType(*fb);
    }else{
        bt=this->isType(*fb);
        if(bt==L_INT){
            int c=!fb->intData;
            res->token=L_INT;
            res->intData=c;
        }else if(bt==L_FLOAT){
            int c=!fb->doubleData;
            res->token=L_INT;
            res->intData=c;
        }
        //delete fb;
        return res;
    }
    
    if(s=='*'){
        if(at==L_INT&&bt==L_INT){
            int c=fa->intData*fb->intData;
            res->token=L_INT;
            res->intData=c;
        }else if(at==L_FLOAT&&bt==L_FLOAT){
            double c=fa->doubleData*fb->doubleData;
            res->token=L_FLOAT;
            res->doubleData=c;
        }else if(at==L_FLOAT&&bt==L_INT){
            double c=fa->doubleData*fb->intData;
            res->token=L_FLOAT;
            res->doubleData=c;
        }else if(at==L_INT&&bt==L_FLOAT){
            double c=fa->doubleData*fb->intData;
            res->token=L_FLOAT;
            res->doubleData=c;
        }
    }
    else if(s=='/'){
        if(at==L_INT&&bt==L_INT){
            int c=fa->intData/fb->intData;
            res->token=L_INT;
            res->intData=c;
        }else if(at==L_FLOAT&&bt==L_FLOAT){
            double c=fa->doubleData/fb->doubleData;
            res->token=L_FLOAT;
            res->doubleData=c;
        }else if(at==L_FLOAT&&bt==L_INT){
            double c=fa->doubleData/fb->intData;
            res->token=L_FLOAT;
            res->doubleData=c;
        }else if(at==L_INT&&bt==L_FLOAT){
            double c=fa->doubleData/fb->intData;
            res->token=L_FLOAT;
            res->doubleData=c;
        }
    }
    else if (s=='+'){
        if(at==L_INT&&bt==L_INT){
            int c=fa->intData+fb->intData;
            res->token=L_INT;
            res->intData=c;
        }else if(at==L_FLOAT&&bt==L_FLOAT){
            double c=fa->doubleData+fb->doubleData;
            res->token=L_FLOAT;
            res->doubleData=c;
        }else if(at==L_FLOAT&&bt==L_INT){
            double c=fa->doubleData+fb->intData;
            res->token=L_FLOAT;
            res->doubleData=c;
        }else if(at==L_INT&&bt==L_FLOAT){
            double c=fa->doubleData+fb->intData;
            res->token=L_FLOAT;
            res->doubleData=c;
        }
    }
    else if (s=='-'){
        if(at==L_INT&&bt==L_INT){
            int c=fa->intData-fb->intData;
            res->token=L_INT;
            res->intData=c;
        }else if(at==L_FLOAT&&bt==L_FLOAT){
            double c=fa->doubleData-fb->doubleData;
            res->token=L_FLOAT;
            res->doubleData=c;
        }else if(at==L_FLOAT&&bt==L_INT){
            double c=fa->doubleData-fb->intData;
            res->token=L_FLOAT;
            res->doubleData=c;
        }else if(at==L_INT&&bt==L_FLOAT){
            double c=fa->intData-fb->doubleData;
            res->token=L_FLOAT;
            res->doubleData=c;
        }
    }
    else if (s==L_TYPEEQUAL){
        if(at==L_INT&&bt==L_INT){
            int c=fa->intData==fb->intData;
            res->token=L_INT;
            res->intData=c;
        }else if(at==L_FLOAT&&bt==L_FLOAT){
            int c=fa->doubleData==fb->doubleData;
            res->token=L_INT;
            res->intData=c;
        }else if(at==L_FLOAT&&bt==L_INT){
            int c=fa->doubleData==fb->intData;
            res->token=L_INT;
            res->intData=c;
        }else if(at==L_INT&&bt==L_FLOAT){
            double c=fa->intData==fb->doubleData;
            res->token=L_INT;
            res->intData=c;
        }
    }
    else if(s==L_LEQUAL){
        if(at==L_INT&&bt==L_INT){
            int c=fa->intData<=fb->intData;
            res->token=L_INT;
            res->intData=c;
        }else if(at==L_FLOAT&&bt==L_FLOAT){
            int c=fa->doubleData<=fb->doubleData;
            res->token=L_INT;
            res->intData=c;
        }else if(at==L_FLOAT&&bt==L_INT){
            int c=fa->doubleData<=fb->intData;
            res->token=L_INT;
            res->intData=c;
        }else if(at==L_INT&&bt==L_FLOAT){
            double c=fa->intData<=fb->doubleData;
            res->token=L_INT;
            res->intData=c;
        }
    }
    else if(s==L_GEQUAL){
        if(at==L_INT&&bt==L_INT){
            int c=fa->intData>=fb->intData;
            res->token=L_INT;
            res->intData=c;
        }else if(at==L_FLOAT&&bt==L_FLOAT){
            int c=fa->doubleData>=fb->doubleData;
            res->token=L_INT;
            res->intData=c;
        }else if(at==L_FLOAT&&bt==L_INT){
            int c=fa->doubleData>=fb->intData;
            res->token=L_INT;
            res->intData=c;
        }else if(at==L_INT&&bt==L_FLOAT){
            double c=fa->intData>=fb->doubleData;
            res->token=L_INT;
            res->intData=c;
        }
    }
    else if(s=='<'){
        if(at==L_INT&&bt==L_INT){
            int c=fa->intData<fb->intData;
            res->token=L_INT;
            res->intData=c;
        }else if(at==L_FLOAT&&bt==L_FLOAT){
            int c=fa->doubleData<fb->doubleData;
            res->token=L_INT;
            res->intData=c;
        }else if(at==L_FLOAT&&bt==L_INT){
            int c=fa->doubleData<fb->intData;
            res->token=L_INT;
            res->intData=c;
        }else if(at==L_INT&&bt==L_FLOAT){
            double c=fa->intData<fb->doubleData;
            res->token=L_INT;
            res->intData=c;
        }
    }
    else if(s=='>'){
        if(at==L_INT&&bt==L_INT){
            int c=fa->intData>fb->intData;
            res->token=L_INT;
            res->intData=c;
        }else if(at==L_FLOAT&&bt==L_FLOAT){
            int c=fa->doubleData>fb->doubleData;
            res->token=L_INT;
            res->intData=c;
        }else if(at==L_FLOAT&&bt==L_INT){
            int c=fa->doubleData>fb->intData;
            res->token=L_INT;
            res->intData=c;
        }else if(at==L_INT&&bt==L_FLOAT){
            double c=fa->intData>fb->doubleData;
            res->token=L_INT;
            res->intData=c;
        }
    }
    else if (s==L_ANDAND){
        if(at==L_INT&&bt==L_INT){
            int c=fa->intData&&fb->intData;
            res->token=L_INT;
            res->intData=c;
        }else if(at==L_FLOAT&&bt==L_FLOAT){
            int c=fa->doubleData&&fb->doubleData;
            res->token=L_INT;
            res->intData=c;
        }else if(at==L_FLOAT&&bt==L_INT){
            int c=fa->doubleData&&fb->intData;
            res->token=L_INT;
            res->intData=c;
        }else if(at==L_INT&&bt==L_FLOAT){
            double c=fa->intData&&fb->doubleData;
            res->token=L_INT;
            res->intData=c;
        }
    }
    else if (s==L_OROR){
        if(at==L_INT&&bt==L_INT){
            int c=fa->intData||fb->intData;
            res->token=L_INT;
            res->intData=c;
        }else if(at==L_FLOAT&&bt==L_FLOAT){
            int c=fa->doubleData||fb->doubleData;
            res->token=L_INT;
            res->intData=c;
        }else if(at==L_FLOAT&&bt==L_INT){
            int c=fa->doubleData||fb->intData;
            res->token=L_INT;
            res->intData=c;
        }else if(at==L_INT&&bt==L_FLOAT){
            double c=fa->intData||fb->doubleData;
            res->token=L_INT;
            res->intData=c;
        }
    }
    else if (s==L_NTYPEEQUAL){
        if(at==L_INT&&bt==L_INT){
            int c=fa->intData!=fb->intData;
            res->token=L_INT;
            res->intData=c;
        }else if(at==L_FLOAT&&bt==L_FLOAT){
            int c=fa->doubleData!=fb->doubleData;
            res->token=L_INT;
            res->intData=c;
        }else if(at==L_FLOAT&&bt==L_INT){
            int c=fa->doubleData!=fb->intData;
            res->token=L_INT;
            res->intData=c;
        }else if(at==L_INT&&bt==L_FLOAT){
            double c=fa->intData!=fb->doubleData;
            res->token=L_INT;
            res->intData=c;
        }
    }
    else if(s==L_PLUSEQUAL){
        if(at==L_INT&&bt==L_INT){
            fa->intData+=fb->intData;
        }else if(at==L_FLOAT&&bt==L_FLOAT){
            fa->doubleData+=fb->doubleData;
        }else if(at==L_FLOAT&&bt==L_INT){
            fa->doubleData+=fb->intData;
        }else if(at==L_INT&&bt==L_FLOAT){
            fa->doubleData=fa->intData+fb->doubleData;
            fa->token=L_FLOAT;
            fa->intData=0;
        }
        //delete fb;
        delete res;
        return fa;
    }
    else if(s==L_MINUSEQUAL){
        if(at==L_INT&&bt==L_INT){
            fa->intData-=fb->intData;
        }else if(at==L_FLOAT&&bt==L_FLOAT){
            fa->doubleData-=fb->doubleData;
        }else if(at==L_FLOAT&&bt==L_INT){
            fa->doubleData-=fb->intData;
        }else if(at==L_INT&&bt==L_FLOAT){
            fa->doubleData=fa->intData-fb->doubleData;
            fa->token=L_FLOAT;
            fa->intData=0;
        }
        //delete fb;
        delete res;
        return fa;
    }
    else if(s==L_PLUSPLUS){
        bt=this->isType(*fb);
        if(bt==L_INT){
            fb->intData+=1;
            fb->token=L_INT;
        }else if(bt==L_FLOAT){
            fb->doubleData+=1;
            res->token=L_FLOAT;
        }
        delete res;
        return fb;
    }
    else if(s=='='){
        if(at==L_INT&&bt==L_INT){
            fa->intData=fb->intData;
            fa->token=L_INT;
        }else if(at==L_FLOAT&&bt==L_FLOAT){
            fa->doubleData=fb->doubleData;
            fa->token=L_FLOAT;
        }else if(at==L_FLOAT&&bt==L_INT){
            fa->intData=fb->intData;
            fa->token=L_INT;
        }else if(at==L_INT&&bt==L_FLOAT){
            fa->doubleData=fb->doubleData;
            res->token=L_FLOAT;
        }
    }

    //delete fa;
    //delete fb;
    return res;
}
