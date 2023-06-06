#include <stdio.h>

using namespace std;

extern "C" 
{    
    void app_main(void);
}

void app_main() 
{
    printf("\n> Hello World!\n");
}