#include<stdio.h>
#include <math.h>
#include<string.h>
#include<stdlib.h>

int HitorMiss( int tag, int index );
int cache_size;
int bytes_per_block;
int result;


struct Cache {
  char valid;
  unsigned long tag;
};
int MISS = 0;
int HIT = 0;
int TOTAL = 0;
struct Cache cache[1024];

/* Function to calculate index & offset bits for parameters passed from Result function*/  
int CalculateNumBits(int val) {
	return (int) (log(val) / log(2));	
}

/* Function to extract tag,index offset values by masking for parameters passed from Result function */
int extractvalue(int addr, int start, int end)
{
	int mask = (1 << (end - start)) - 1;
	return (addr >> start) & mask;
}

/* function to count hits,miss,total.
   index is cache line number in direct mapped.*/
int HitorMiss(int tag, int index ) {
	
	int Hit;
	if ( cache[index].valid && cache[index].tag == tag ) {
		Hit = 1;
		HIT++;
	}	
	else {
		Hit = 0;
		MISS++;
		struct Cache line;
		line.valid = 1;
		line.tag = tag;
		cache[index] = line;
	}
	TOTAL++;
	return Hit;
}


int Result(unsigned long myaddr) {
      	cache_size = 32768;
	bytes_per_block = 32;
	int offset_bits = CalculateNumBits(bytes_per_block);
  	int index_bits = CalculateNumBits(cache_size / bytes_per_block);
     	int tag_bits = (32 - index_bits) - offset_bits;
       	int addr = myaddr ;
        int tag = extractvalue(addr, offset_bits + index_bits, 32);
        int index = extractvalue(addr, offset_bits, offset_bits + index_bits);
        int offset = extractvalue(addr, 0, offset_bits);
        result = HitorMiss( tag, index );
        if (result)
		printf("%d: HIT\n " , addr);
        else
                printf("%d: MISS\n " , addr);
        
	return 0;
}


int main(int argc, char *argv[]) {
	
	FILE *ptr;
  	char buff[1025];
  	unsigned long phyaddr;
  	ptr = fopen(argv[1], "r");
  	while (fgets(&buff[0], 2048, ptr)) {
        sscanf(buff, "0x%lx", &phyaddr);
	Result(phyaddr);
  	}
	printf("HIT:%d MISS: %d\n", HIT, MISS);
	printf("%f\n",(float)HIT/TOTAL);
}
