
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define W 5
#define S 4
#define LEN(arr) (sizeof(arr) / sizeof(arr[0]))
#define MAXEDGES 8000
#define INSIZE 6
#define INFMT "%5s"

typedef struct counter{
    char chars[W+1];
    int counts[W];
} counter;

typedef struct node{
    char word[W+1];
    counter cl;
    counter ca;
    struct node * edges[MAXEDGES];
    size_t nedges;
    int visited;
    int depth;
} node;

typedef struct queue{
    node** data;
    size_t head;
    size_t tail;
} queue;


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

void  queueInit(size_t size,queue* queue){
    queue->data = calloc(size,sizeof(node*));
    queue->head = 0;
    queue->tail = 0;
}

void  queueFree(queue* queue){
    free(queue->data);
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
    const char *c, *p = A->cl.chars;

    for (int i = 0; i<LEN(A->cl.chars); ++i){

        char cA = A->cl.chars[i];
        if (!cA){ //compared all characters 
            break; 
        }
        
        int cntA =  A->cl.counts[i];
        int cntB = findInCounter(&B->ca, cA);
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
    char* current = nodes[currentIndex].word;
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
        if(!strcmp( nodes[i].word, word)){
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
                    queueFree(&Q);
                    return w->depth;
                }
            }
        }
    }
    queueFree(&Q);
    return -1;
}

int main()
{
    char in[INSIZE]; // declare a char array to hold the word
    node* nodes;
    int N;
    int Q;
    scanf("%d", &N);
    scanf("%d", &Q);
    nodes = malloc(sizeof(node) *N);
    int wordN = 0;


    for (int i = 0; i < N; i++) {
        scanf(INFMT, in); // read in each word separated by spaces
        strncpy( nodes[wordN].word, in, LEN(nodes[0].word) );
        nodes[wordN].ca = countChars(in);
        nodes[wordN].cl = countChars(&in[1]);
        nodes[wordN].nedges = 0;
        addEdges(nodes, wordN);
        wordN++;
    }
    
    for (int i = 0; i < Q; i++) {
        
        char request1[INSIZE];
        char request2[INSIZE];
        scanf(INFMT, request1); // read in each word separated by spaces
        scanf(INFMT, request2); // read in each word separated by spaces
        node* node1 = findNode(nodes,request1,N);
        const node* node2 = findNode(nodes,request2,N);
        for (int i = 0; i < N; ++i){
            nodes[i].visited = 0;
        }
        int nbrSteps = bFS(nodes,N,node1,node2); //Breadth first search
        nbrSteps < 0 ? printf("Impossible\n") : printf("%d\n", nbrSteps);
        //printf("Steps from %s ro %s: %d", request1, request2, nbrSteps);
    }
       
        
    free(nodes);
    return 0;
}
    


