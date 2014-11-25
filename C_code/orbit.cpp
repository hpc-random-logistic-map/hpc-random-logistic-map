#include "orbit.h"
#include <math.h>
int period_check(int size, double* xv)
{
    int maxp = floor((size-100)/2);
    if (maxp < 1) return 0;
    for (int i=size-1; i-maxp-1>100;i--)
    {
        if(xv[i] != xv[i-maxp-1])
            return 0;
    }
    
    return maxp;
    //actual data from 101 to size-1 of xv
}


