
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
    char word[W+1];
    counter cl;
    counter ca;
} word;


void countChar(counter *counter, char c);

counter countChars(const char in[W+1]){
    counter count;
    memset(&count, 0, sizeof(count) );
    const char *p, *c = in;
     
    while( *(p = c++) ){
        countChar(&count, *p);
    }
    return count;
}

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
    
    ret.ca = countChars(in);
    ret.cl = countChars(&in[1]);
    return ret;
}





typedef struct node{
    word  w;
    struct node * edges[10000];
    size_t nedges;
    int visited;
    int depth;
} node;

typedef struct queue{
    node** data;
    size_t head;
    size_t tail;
} queue;

void  queueInit(size_t size,queue* queue){
    queue->data = calloc(size,sizeof(node*));
    queue->head = 0;
    queue->tail = 0;
}

void queuePush(queue *queue, node *node) { queue->data[queue->tail++] = node; }

node *queuePop(queue *queue) { return queue->data[queue->head++]; }

int findInCounter(const counter* counter, char c){
    for (int i = 0; i<LEN(counter->chars); ++i){
        if (counter->chars[i] == c){
            return counter->counts[i];            
        }   
    }
    return 0;
}




int hasEdgeFromAToB(const node* A, const node* B){
    const char *c, *p = A->w.cl.chars;

    for (int i = 0; i<LEN(A->w.cl.chars); ++i){

        char cA = A->w.cl.chars[i];
        if (!cA){ //compared all characters 
            break; 
        }
        
        int cntA =  A->w.cl.counts[i];
        int cntB = findInCounter(&B->w.ca, cA);
        if (cntB < cntA){
            
            return 0;
        }
    }
    
    return 1;
}

void addEdge(node *from,  node *to) {
    from->edges[from->nedges++] = to;
}

void addEdges(node* nodes, size_t currentIndex){
    word current = nodes[currentIndex].w;
    for (int i = 0; i<currentIndex; ++i){
        //        nodes[i].edges[]
        if(hasEdgeFromAToB(&nodes[currentIndex], &nodes[i])){
            addEdge(&nodes[currentIndex],&nodes[i]);
        }
        if(hasEdgeFromAToB(&nodes[i], &nodes[currentIndex])){
            addEdge(&nodes[i], &nodes[currentIndex]);
        }

    }
}

node* findNode(node* nodes, const char* word, int size){
    for(int i = 0; i < size;i++){
        if(!strcmp( nodes[i].w.word, word)){
            return &nodes[i];
        }
    }
    return 0;
}

int bFS(const node* nodes,int size,  node* sNode, const node* tNode){
    if(sNode == tNode){return 0;}
    queue Q;
    queueInit(size, &Q);
    queuePush(&Q, sNode);

    sNode->visited = 1;
    sNode->depth = 0;

    //while queue is not empty
    while(Q.head != Q.tail){
        node* v = queuePop(&Q);
        
        for(int i = 0; i < v->nedges;i++){
            node * w = v->edges[i];

            if(!w->visited){

                w->visited = 1;
                w->depth = v->depth + 1;

                queuePush(&Q,w);

                if(w == tNode){
                    return w->depth;
                }
            }
        }
    }
    return -1;
}

int main()
{
    char in[6]; // declare a char array to hold the word
    node* nodes;
    int N;
    int Q;
    scanf("%d", &N);
    scanf("%d", &Q);
    nodes = malloc(sizeof(node) *N);
    int wordN = 0;


    for (int i = 0; i < N; i++) {
        scanf("%5s", in); // read in each word separated by spaces
        nodes[wordN].w =createWord(in);
        nodes[wordN].nedges = 0;
        addEdges(nodes, wordN);
        wordN++;
    }
    
    for (int i = 0; i < Q; i++) {
        
        char request1[6];
        char request2[6];
        scanf("%5s", request1); // read in each word separated by spaces
        scanf("%5s", request2); // read in each word separated by spaces
        node* node1 = findNode(nodes,request1,N);
        const node* node2 = findNode(nodes,request2,N);
        for (int i = 0; i < N; ++i){
            nodes[i].visited = 0;
        }
        int nbrSteps = bFS(nodes,N,node1,node2); //Breadth first search
        nbrSteps < 0 ? printf("Impossible\n") : printf("%d\n", nbrSteps);
        //printf("Steps from %s ro %s: %d", request1, request2, nbrSteps);
    }
       
        
 
    return 0;
}
    


