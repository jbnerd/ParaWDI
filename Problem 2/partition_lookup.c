#include <stdio.h>
#include <stdlib.h>

int* divide(int num_of_proc)
{   int count = 0;
    float table[] = {11.682 ,16.116 ,21.354 ,24.528 ,27.327 ,31.354 ,32.996 ,37.196 ,44.49 ,45.001 ,45.457 ,47.872 ,51.698 ,53.982 ,
    61.613 ,65.932 ,66.154 ,68.98 ,75.666 ,91.644 ,92.827 ,93.651 ,99.148 ,99.193 ,99.956 ,100.00};
    int* index = (int*)calloc(num_of_proc, sizeof(int));
    for(int i = 0; i <26 ; i++)
    {   index[count] += 1;
        if(table[i] > ((count+1) * (100.00/num_of_proc)))
        {   //
            count+=1;
         }
         ///printf("alphabet: %2d --- process: %2d  --C.freq of Alphabet %4f \n",i , count , table[i]);
    }
    //printf("\n\n\n\n");
    return index;
}

int main(){

divide(2);
divide(4);
divide(8);
divide(16);

}
