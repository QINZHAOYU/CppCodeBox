/** *****************************************************************************
*    @File      :  test_hello.cpp
*    @Brief     :  src/model/hello 模块测试算例。
*
** ******************************************************************************/
#include <stdio.h>
#include "model/hello/HelloDemo.h"

using hello::HelloDemo;


int main(int argc, const char* argv[])
{
    printf("---*--- Test of Hello Model: ---*---\n");

    HelloDemo demo;
    int state = demo.run(argc, argv);
    for(auto &iter: demo.appInfo())
        printf("%f: %f\n", iter->first, iter->second);
    
    char msg = (state == 0)?"done":"failed";
    printf("---*--- %s ---*---\n", msg); 
    return state;
}