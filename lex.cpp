//
//  lex.cpp
//  sparrowJs
//
//  Created by yang on 17/1/19.
//  Copyright © 2017年 yang. All rights reserved.
//

#include "lex.hpp"

jsException::jsException(const std::string &exceptionText) {
    text = exceptionText;
}
lex::~lex(){
    delete util;
};
lex::lex(const std::string &input){
    data=strdup(input.c_str());
    dataStart=0;
    dataEnd=(int)strlen(data);
    tokenStr="";
    token=0;
    dataPos=0;
    tokenEnd=0;
    tokenLastEnd=0;
    tokenStart=0;
    util=new utils();
    this->getNextChar();
    this->getNextChar();
    this->getNextToken();
};
void lex::reset(){
    dataStart=0;
    tokenStr="";
    token=0;
    dataPos=0;
    tokenEnd=0;
    tokenLastEnd=0;
    tokenStart=0;
    this->getNextChar();
    this->getNextChar();
    this->getNextToken();
}
void lex::getNextChar(){
    this->currCh=this->lastCh;
    if(this->dataPos<=this->dataEnd){
        this->lastCh=this->data[this->dataPos];
    }else{
        this->lastCh=0;
        //this->token=L_EOF;
    }
    this->dataPos++;
};

void lex::getNextToken(){
    this->token=L_EOF;
    this->tokenStr.clear();
    this->tokenStr="";
    while(this->currCh&&util->isSpace(this->currCh)){
        this->getNextChar();
    }
    if(this->currCh=='/'&&this->lastCh=='/'){// determine whether to comment
        while (this->currCh&&this->currCh!='\n') {
            this->getNextChar();
        }
        this->getNextChar();
        this->getNextToken();
    }
    if(currCh=='/'&&lastCh=='*'){// determine whether a block comment
        while ((this->currCh&&this->lastCh)&&(this->currCh!='*'&&this->currCh!='/')) {
            this->getNextChar();
        }
        this->getNextChar();
        this->getNextToken();
    }
    tokenStart = dataPos-2;
    // if it is ID
    if(util->isVariable(this->currCh)){
        this->token=L_ID;// get variable token
        while (util->isVariable(this->currCh)) {
            this->tokenStr+=this->currCh;
            this->getNextChar();
        }
        if(this->tokenStr=="if"){
            this->token=L_IF;
        }else if (this->tokenStr=="else"){
            this->token=L_ELSE;
            if(this->currCh==' '&&this->lastCh=='i'){
                this->getNextChar();
                while (util->isVariable(this->currCh)) {
                    this->tokenStr+=this->currCh;
                    this->getNextChar();
                }
                if(this->tokenStr=="elseif"){
                    this->token=L_ELSEIF;
                }
            }
        }else if(this->tokenStr=="do"){
            this->token=L_DO;
        }else if(this->tokenStr=="while"){
            this->token=L_WHILE;
        }else if(this->tokenStr=="for"){
            this->token=L_FOR;
        }else if(this->tokenStr=="break"){
            this->token=L_BREAK;
        }else if(this->tokenStr=="continue"){
            this->token=L_CONTINUE;
        }else if(this->tokenStr=="function"){
            this->token=L_FUNCTION;
        }else if(this->tokenStr=="return"){
            this->token=L_RETURN;
        }else if(this->tokenStr=="var"){
            this->token=L_VAR;
        }else if(this->tokenStr=="true"){
            this->token=L_TRUE;
        }else if(this->tokenStr=="false"){
            this->token=L_FALSE;
        }else if(this->tokenStr=="null"){
            this->token=L_NULL;
        }else if(this->tokenStr=="undefined"){
            this->token=L_UNDEFINED;
        }else if(this->tokenStr=="new"){
            this->token=L_NEW;
        }
    }else if (util->isNumber(currCh)){
        this->token=L_INT;
        while (util->isNumber(currCh)) {
            this->tokenStr+=currCh;
            this->getNextChar();
        }
//        if(currCh!=';'&&currCh!='\r'&&currCh!='\n'&&currCh!='.'&&currCh!=','&&currCh!='+'&&currCh!='-'){
//            throw new jsException("ERROR: Syntax error");
//        }
        if(currCh=='.'){
            this->token=L_FLOAT;
            if(!strcmp(this->tokenStr.c_str(),"")){
                this->tokenStr='0';
            }
            this->tokenStr+='.';
            this->getNextChar();
            while (util->isNumber(currCh)) {
                this->tokenStr+=currCh;
                this->getNextChar();
            }
//            if(currCh!=';'&&currCh!='\r'&&currCh!='\n'&&currCh!=','){
//                throw new jsException("ERROR: Syntax error");
//            }
        }
        
    }else if (currCh=='"'||currCh=='\''){
        this->token=L_STRING;
        this->getNextChar();
        while (currCh&&currCh!='"'&&currCh!='\'') {
            if(currCh=='\\'){
                this->getNextChar();
                if(currCh=='"'||currCh=='\''){
                    this->tokenStr+=currCh;
                }
            }else{
                this->tokenStr+=currCh;
            }
            this->getNextChar();
        }
        this->getNextChar();
    }else{// symbol
        this->token=currCh;
        printf("分析符号:%c \n",currCh);
        if(currCh)this->getNextChar();
        if(this->token=='='&&this->currCh=='='){
            this->token=L_TYPEEQUAL;
            this->getNextChar();
        }else if(this->token=='!'&&this->currCh=='='){
            this->token=L_NTYPEEQUAL;
            this->getNextChar();
        }else if(this->token=='<'&&this->currCh=='='){
            this->token=L_LEQUAL;
            this->getNextChar();
        }else if(this->token=='>'&&this->currCh=='='){
            this->token=L_GEQUAL;
            this->getNextChar();
        }else if(this->token=='&'&&this->currCh=='&'){
            this->token=L_ANDAND;
            this->getNextChar();
        }else if(this->token=='|'&&this->currCh=='|'){
            this->token=L_OROR;
            this->getNextChar();
        }
        else if(this->token=='+'&&this->currCh=='='){
            this->token=L_PLUSEQUAL;
            this->getNextChar();
        }
        else if(this->token=='+'&&this->currCh=='+'){
            this->token=L_PLUSPLUS;
            this->getNextChar();
        }
        else if(this->token=='-'&&this->currCh=='='){
            this->token=L_MINUSEQUAL;
            this->getNextChar();
        }
        tokenLastEnd = tokenEnd;
        tokenEnd = dataPos-3;
    }
    printf("Analysis result:%s \n",tokenStr.c_str());
};
std::string lex::getSubString(int lastPosition) {
    int lastCharIdx = tokenEnd+1;
    if (lastCharIdx < dataEnd) {
        /* save a memory alloc by using our data array to create the
         substring */
        char old = data[lastCharIdx];
        data[lastCharIdx] = 0;
        std::string value = &data[lastPosition];
        data[lastCharIdx] = old;
        return value;
    } else {
        return std::string(&data[lastPosition]);
    }
}

void lex::analyses(int expected){
    if (this->token!=expected) {
        throw new jsException("ERROR: Syntax error");
    }
    this->getNextToken();
}
int lex::getDataPos(){
    return this->dataPos;
}
std::string lex::getSubData(int &pos,int &factor){
    int position=(this->dataPos)-factor;
    char old=this->data[position];
    data[position]=0;
    std::string value = &data[pos];
    data[position]=old;
    return value;
}
