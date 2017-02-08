# SparrowJS
Minimal JavaScript engine

the SparrowJS separated from the JSNative about the project,the project is not yet perfect，many grammars are not supported
,but supports basic arithmetic and function calls,and supports JS call c++.

## how to use?

```c++
#include <stdio.h>
#include "lex.hpp"
#include "sparrowJS.hpp"

int main(int argc, char **argv)
{
    sparrowJS* sparrow=new sparrowJS();
    sparrow->elva("javascript code");
    delete sparrow;
    return 0;
}

```
