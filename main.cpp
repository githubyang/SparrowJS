/*
 * SparrowJS
 *
 * A single-file Javascript-alike engine
 *
 * Authored By yang <daohangcom@qq.com>
 *
 * Copyright (C) 2016 yang
 *
 */

/*
 * This is a simple program showing how to use SparrowJS
 */


#include <assert.h>
#include <stdio.h>
#include "lex.hpp"
#include "sparrowJS.hpp"





//const char *testCode = "var a=2;var d=a*((-2.3)/3+3*(-22131231))+1*8;print(d);";
//const char *testCode = "var a=1;function f(){var a=4;var b=2+4*(2+3);print(b);};f();print(a);f();";

//const char *testCode = "var a=1;";
//const char *testCode = "function fn(s){return s;};var f=function (p){var s=fn(6);return s;};var b=f(3);print(b);";
//const char *testCode = "var a=2*(3+2+(2*8)/2-5);";
//const char *testCode = "print(9);";
//const char *testCode = "var a=(-2*8)/2-5.3;";
//const char *testCode = "var a=3*(-2);";
//const char *testCode = "if(1>1){print(2);}else if(1>2){print(1345);}else if(3>1){print(333334);}else{print(34);} print(44);";
const char *testCode = "for(var i=0;i<5;i++){print(i);} print(22);";
//const char *testCode = "var a=2;a-=1.1;print(a);";
//const char *testCode = "var s=2;var arr=[s];var b=arr[0];b=6;print(b);";

void js_prints(VARIABLE &v, void *userdata,void *l) {
    VARIABLE* t=v.next;
    if(t->token==L_INT){
        printf("> %d\n", t->intData);
    }
    if(t->token==L_FLOAT){
        printf("> %f\n", t->doubleData);
    }
    if(t->token==L_STRING){
        printf("> %s\n", t->strData.c_str());
    }
}

int main(int argc, char **argv)
{
    //lex *L=new lex(code);
    sparrowJS* sparrow=new sparrowJS();
    sparrow->addNativeFunc("function print(text)", &js_prints, 0);
    sparrow->elva(testCode);
//    while (L->token) {
//        L->analyses(L->token);
//    }
//    delete L;
    
    return 0;
}



