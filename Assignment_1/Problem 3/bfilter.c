#include "_BFILTER.h"

uint32_t murmurhash (const char *key, uint32_t len, uint32_t seed) {
    uint32_t c1 = 0xcc9e2d51;
    uint32_t c2 = 0x1b873593;
    uint32_t r1 = 15;
    uint32_t r2 = 13;
    uint32_t m = 5;
    uint32_t n = 0xe6546b64;
    uint32_t h = 0;
    uint32_t k = 0;
    uint8_t *d = (uint8_t *) key; // 32 bit extract from `key'
    const uint32_t *chunks = NULL;
    const uint8_t *tail = NULL; // tail - last 8 bytes
    int i = 0;
    int l = len / 4; // chunk length
    h = seed;
    chunks = (const uint32_t *) (d + l * 4); // body
    tail = (const uint8_t *) (d + l * 4); // last 8 byte chunk of `key'
    // for each 4 byte chunk of `key'
    for (i = -l; i != 0; ++i) {
        // next 4 byte chunk of `key'
        k = chunks[i];
        // encode next 4 byte chunk of `key'
        k *= c1;
        k = (k << r1) | (k >> (32 - r1));
        k *= c2;
        // append to hash
        h ^= k;
        h = (h << r2) | (h >> (32 - r2));
        h = h * m + n;
    }
    
    k = 0;
    // remainder
    switch (len & 3) { // `len % 4'
        case 3: k ^= (tail[2] << 16);
        case 2: k ^= (tail[1] << 8);
        case 1:
            k ^= tail[0];
            k *= c1;
            k = (k << r1) | (k >> (32 - r1));
            k *= c2;
            h ^= k;
    }

    h ^= len;
    h ^= (h >> 16);
    h *= 0x85ebca6b;
    h ^= (h >> 13);
    h *= 0xc2b2ae35;
    h ^= (h >> 16);
    return h;
}

unsigned long* getVector()
{
    return (unsigned long*) calloc( SIZE, sizeof(unsigned long));
}

unsigned long* addB(char *key, unsigned long* vector)
{  int i = 0;
   uint32_t h,d,k,m;
   int a[] = {896,991,566,169,459,251,546,41};
   uint32_t len = (uint32_t) strlen(key);
   for(i = 0; i< NUM_HASHES; i++)
   {
    h = murmurhash(key, (uint32_t) strlen(key), a[i]) % DOMAIN;
    d = h/64;
    m = h%64;
    k = (1<<m);
      if((vector[d] & k) > 0)
          continue;
      else
         vector[d] = ( vector[d] | k );    
   }
   return vector;
}

bool searchB(char *key, unsigned long* vector)
{  int i = 0;
   uint32_t h,d,k,m;
   int a[] = {896,991,566,169,459,251,546,41};
   uint32_t len = (uint32_t) strlen(key);
   for(i = 0; i< NUM_HASHES; i++)
   {
    h = murmurhash(key, (uint32_t) strlen(key), a[i]) % DOMAIN;
    d = h/64;
    m = h%64;
    k = (1<<m);
    if((vector[d] & k) > 0)
       continue;
    else
       return false;    
   }
   return true;
}
/*
int main()
{    int i =0;
    unsigned long* lol = getVector();
    char keys[][8] = {"the", "a", "there", "answer", "any", "by", "bye", "their","the", "a", "thou","thy", "a", };
    char keys2[][8] = {"the", "an", "then", "answer", "why", "sad", "scar", "their","the", "allow", "there","the", "animal", };
    for (i = 0; i < 13; i++){
        lol = add(keys[i], lol);
     }
     for (i = 0; i < 13; i++){
        printf( "%s --- %d \n", keys2[i], (int) search(keys2[i], lol));
     }

}*/
