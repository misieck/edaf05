#include <stddef.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <limits.h>
#include <stdlib.h>
#include <tgmath.h>

#define CHARS_LEN 50
#define WHITES " \t\n"

#define EMPTY ULONG_MAX
#define MAXCHARS 26
#define MAXLENGTH 3500

#define STAR_WEIGHT -4

#define STR_IMPL_(x) #x      //stringify argument
#define STR(x) STR_IMPL_(x)  //indirection to expand argument macros


typedef enum direction{
    RIGHT = 1,
    UP,
    DIAG,

} direction;


//a - RIGHT, b - UP, c - DIAG
int max(int a, int b, int c, direction* best_idx){
    int max = b>=a ? (*best_idx=UP,b) : (*best_idx=RIGHT,a);
    return max>=c ? max : (*best_idx=DIAG,c);
}

typedef struct cache{
    int * gains;
    direction* directions;
    size_t len_x;
    size_t len_y;
} cache;

typedef struct in_data{
    size_t count_chars;
    //const char* chars;
    size_t indexes[MAXCHARS];
    size_t count_cases;
    union {
        int flat[MAXCHARS*MAXCHARS];
    } weights;
} in_data;

typedef struct str_data{
    size_t len_x;
    size_t len_y;
    char str_x[MAXLENGTH+1];
    char str_y[MAXLENGTH+1];

} str_data;

int read ( FILE* file,  in_data* input) //   size_t indexes[MAXCHARS], size_t* count_chars, int ** weights, size_t* count_strings)
{
  
    if (file != NULL) {
        char line [1000];

        fgets(line,sizeof line,file);  /* read a line from a file */
        char *pc;
        char chars[CHARS_LEN];

        pc = strtok (line, WHITES);
        input->count_chars = 0;
        while(pc){
            assert(input->count_chars < 50);
            input->indexes[*pc-'A'] = input->count_chars;
            chars[ (input->count_chars)++ ] = *pc;
            pc = strtok(NULL, WHITES);
        }

        chars[input->count_chars] = 0;
        size_t matrix_count = (input->count_chars) * (input->count_chars);

        for (int i = 0; i < matrix_count ; ++i){
            
            int v;
            scanf("%d", &v);
            input->weights.flat[i] = v;
            
        }

        scanf("%lu", &input->count_cases);
        //printf ("\n");
        //while(fgets(line,sizeof line,file)!= NULL) /* read a line from a file */ {
            
            
        //  fprintf(stdout,"%s",line); //print the file contents on stdout.
        //}

        //printf("%s\n", chars);
        
    }
    return 0;
}

int test_coordinate(size_t len_x, size_t len_y, size_t x, size_t y){
    return x <= len_x && y <= len_y;
}



int get_weight(size_t x, size_t y, str_data* str_in, in_data* input){
    assert( x-1 < str_in->len_x );
    assert( y-1 < str_in->len_y );
    char X = str_in->str_x[x-1];
    char Y = str_in->str_y[y-1];
    return input->weights.flat[ input->indexes[X - 'A'] + input->count_chars *  input->indexes[Y - 'A'] ];
}


int calc_gain_candidate(size_t x, size_t y, str_data* str_in, in_data* input, cache* gains, direction weight_dir){
    //  int gains[str_in->len_x + 1][str_in->len_y + 1]
    if(!test_coordinate(str_in->len_x, str_in->len_y, x, y)) return INT_MIN;
    int weight = STAR_WEIGHT;
    if (weight_dir == DIAG){
        weight = get_weight(x,y,str_in,input);
    }
    return gains->gains[x + gains->len_x*y ] + weight;
}



int calc_gain_at(size_t gian_x, size_t gain_y,  str_data* str_in, in_data* input, cache *cache)//[ str_in->len_x + 1 ][ str_in->len_y + 1] ) 

{
    //direction DIAG
    int gain3 = calc_gain_candidate( gian_x + 1, gain_y + 1, str_in, input, cache, DIAG);
   
    //direction RIGHT
    int gain1 = calc_gain_candidate( gian_x + 1, gain_y,     str_in, input, cache, RIGHT);

    //direction UP
    int gain2 = calc_gain_candidate( gian_x    , gain_y + 1, str_in, input, cache, UP);

    direction direction;
    cache->gains[gian_x + cache->len_x*gain_y] = max(gain1,gain2,gain3, &direction);
    cache->directions[gian_x + cache->len_x*gain_y] = direction;
    //printf("(%ld, %ld): %d, %d\n", gian_x, gain_y, gains[gian_x][gain_y], direction);
    return 0;
}

//len_x, len_y are respective string lengths, the graph dims are one larger
void calc_gains( str_data * str_in, in_data* input, cache* gains
                 //int gains[str_in->len_x + 1][str_in->len_y + 1]
                ) {

    const size_t len_x = str_in->len_x;
    const size_t len_y = str_in->len_y;
    
    const size_t min_dim = len_x < len_y ? len_x : len_y;
    const size_t max_dim = len_y < len_x ? len_x:len_y;
    const size_t nbr_equal_diags = max_dim - min_dim + 1;
    
    gains->gains[ gains->len_x * gains->len_y - 1] = 0;
    
    int x_base = len_x ;
    //Calculates the cost of every point diagonally from the top
    for(int i = 1; i < min_dim ; i++){
        x_base -= 1;
        for(int j = 0; j < i + 1; j++){

            calc_gain_at( x_base + j, len_y - j, str_in, input, gains);
            
        }
    }

    for(int i = 0; i < nbr_equal_diags; i++){
        x_base --;
        for(int j = 0 ; j < min_dim + 1; j++){

            calc_gain_at( x_base + j, len_y - j, str_in, input, gains);
            
        }
    }

    for(int i = 0; i <= min_dim-1; i++){
        x_base --;
        for(int j = -x_base; j < min_dim - i - x_base; j++){
            calc_gain_at( x_base + j, len_y - j, str_in, input, gains);
            
        }
    }
    
}


void print_word(const char* str, size_t str_len, const int *stars, size_t stars_len){
    int stars_idx = 0;
    for (int i = 0; i < str_len + 1  ; i++){
        assert(stars_len > stars_idx);
        while( stars[stars_idx] == i){
            printf("*");
            stars_idx += 1;
        }
        if (i<str_len) printf("%c", str[i]);
    }
    printf(" ");
}

int main(){

    in_data input;
    str_data str_in;
    memset(input.indexes, 0xff, MAXCHARS*sizeof(size_t));
    read(stdin, &input);

    for (int k = 0; k < input.count_cases; ++k) {
        char str_x[MAXLENGTH + 1];
        char str_y[MAXLENGTH + 1];
        size_t len_x, len_y;
        scanf("%" STR(MAXLENGTH) "s %" STR(MAXLENGTH) "s", str_x, str_y);
        len_x = strlen(str_x);
        len_y = strlen(str_y);
        
        // the algorithm "assumes" x is larger than y, so switch them otherwise
        const char *src_x = str_x;
        const char *src_y = str_y;
        str_in.len_x = len_x;
        str_in.len_y = len_y;
        int swapped = 0;
        if (len_y > len_x) {
            swapped = 1;
            src_x = str_y;
            src_y = str_x;
            str_in.len_x = len_y;
            str_in.len_y = len_x;
        }
        strncpy(str_in.str_x, src_x, str_in.len_x);
        strncpy(str_in.str_y, src_y, str_in.len_y);

        cache cache;
        cache.len_x = str_in.len_x + 1;
        cache.len_y = str_in.len_y + 1;
        cache.gains = malloc( sizeof(int) * (cache.len_x)*(cache.len_y) );
        cache.directions = malloc( sizeof(direction) *(cache.len_x)*(cache.len_y) );

        calc_gains(&str_in, &input, &cache);
      


        size_t len_stars_x = str_in.len_y;
        size_t len_stars_y = str_in.len_x;
        
        int stars_x[ len_stars_x ];
        int stars_y[ len_stars_y ];
        memset(stars_x, 0xff, sizeof(int) * len_stars_x);
        memset(stars_y, 0xff, sizeof(int) * len_stars_y);
        
        size_t stars_x_idx = 0;
        size_t stars_y_idx = 0;


        int x = 0;
        int y = 0;
        while (x != str_in.len_x || y != str_in.len_y) {
            //printf("%d ", cache.directions[x + cache.len_x*y] );
            switch (cache.directions[x + cache.len_x*y ]) {
                // going right, i.e. eating x char, and putting star in y
            case RIGHT:
                assert(stars_y_idx < len_stars_y);
                stars_y[stars_y_idx++] = y;
                x++;
                break;
            case UP:
                assert(stars_x_idx < len_stars_x);
                stars_x[stars_x_idx++] = x;
                y++;
                break;
            case DIAG:
                x++;
                y++;
                break;
            }
        }
        
        // printf("\n");
        if (!swapped){
            print_word(str_in.str_x, str_in.len_x, &stars_x[0], len_stars_x);
            print_word(str_in.str_y, str_in.len_y, &stars_y[0], len_stars_y);
        } else {
            print_word(str_in.str_y, str_in.len_y, &stars_y[0], len_stars_y);
            print_word(str_in.str_x, str_in.len_x, &stars_x[0], len_stars_x);
                        
        }
        printf("\n");
        free(cache.gains);
        free(cache.directions);
    }
}
