
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define W 5
#define S 4
#define LEN(arr) (sizeof(arr) / sizeof(arr[0]))

typedef struct counter{
    char chars[W+1];
    int counts[W];
} counter;


typedef struct word {
    char word[W];
    counter cl;
    counter ca;
} word;


void countChar(counter* counter, char c)
{
    int N = sizeof (counter->chars);
    for (int i = 0; i < N; i++){
        char cInI = counter->chars[i];
        if (cInI == c){
            counter->counts[i]++;
            break;
        } else if (cInI == 0){
            counter->chars[i] = c;
            counter->counts[i] = 1;
            break;
        }
    }
}

word createWord(char in[100]){
    word ret;
    memset(&ret, 0, sizeof(ret) );
    strncpy( ret.word, in, LEN(ret.word) );

    for (int i = 0; i<W; ++i){
        countChar(&ret.ca, in[i]);       
    }

    for (int i = S-1; i>=0; --i){
        countChar(&ret.cl, in[W-1-i]);       
    }
    
    printf("%s \n", ret.word);
    return ret;
}





typedef struct node{
    word  w;
    struct node * edges;
    size_t nedges;
} node;


int findInCounter(const counter* counter, char c){
    for (int i = 0; i<LEN(counter->chars); ++i){
        if (counter->chars[i] == c){
            return counter->counts[i];            
        }   
    }
    return 0;
}

bool hasEdgeFromAToB(const node* A, const node* B){
    const char *c, *p = A->w.cl.chars;
    
    while ( !(p = c++) ) {
        int n = findInCounter(&B->w.ca, *p);
        

    };


    
}

void addEdges(node* nodes, size_t currentIndex){
    word current = nodes[currentIndex].w;
    for (int i = 0; i<currentIndex; ++i){
        //        nodes[i].edges[]

    }
}

int main()
{
    char in[100]; // declare a char array to hold the word
    node* nodes;
    int N;
    int Q;
    scanf("%d", &N);
    scanf("%d", &Q);
    nodes = malloc(sizeof(node) *N);
    int wordN = 0;

    while(scanf("%99s", in) == 1) { // read in each word separated by spaces
        nodes[wordN++].w =createWord(in);
        addEdges(nodes, wordN);
    }
    return 0;
}
    


