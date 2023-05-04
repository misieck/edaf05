#include <limits>
#include <cmath>


// [ iR*, iL*, jL*, jR* ]
template <typename VP, typename VD>
void case1(const VP& a, size_t na, const VP& b, size_t nb, const VD& alfas, const VD& betas, size_t iL, size_t jL, size_t j_i_stars[4]) {
    size_t i = 0;
    size_t j = 0;
    while (true){
        double_t gamma = k(a[i], b[j]);
        if ( ( alfas[i] > gamma || std::isinf(alfas[i]) ) && i<iL  ){
            i++;
        } else if ( ( betas[j] > gamma || std::isinf(betas[j]) ) && j<jL  ){
            j++;
        } else {
            break;
        }
    }

    //iR*
    j_i_stars[0] = i;
    //jR*
    j_i_stars[3] = j;
    i=iL;
    j=jL;

    while (true){
        double_t gamma = k(a[i], b[j]);
        if (  betas[j] > gamma  && j!=0  ){
            j = (j+1) % nb;
        } else if ( alfas[i] > gamma  && i != 0  ){
            i = (i+1) % na;
        } else {
            break;
        }
    }

    //iL*
    j_i_stars[1] = i;
    //jL*
    j_i_stars[2] = j;
}


// [ iR*, iL*, jL*, jR* ]
template <typename VP, typename VD>
void case2(const VP& a, size_t na, const VP& b, size_t nb, const VD& alfas, const VD& betas, size_t iL, size_t jL, size_t j_i_stars[4]) {
    size_t i = 0;
    size_t j = 0;
    while (true){
        double_t gamma = k(a[i], b[j]);
        if ( ( alfas[i] > gamma || std::isinf(alfas[i]) ) && i<iL  ){
            i++;
        } else if ( ( betas[j] > gamma || std::isinf(betas[j]) ) && j<jL  ){
            j++;
        } else {
            break;
        }
    }

    //iR*
    j_i_stars[0] = i;
    //jR*
    j_i_stars[3] = j;
    i=iL;
    j=jL;

    while (true){
        double_t gamma = neg_inf( k(a[i], b[j]) );
        size_t ak = (na+i-1) % na;
        size_t bk = (nb+j-1) % nb;
        
        if (  std::isfinite(alfas[ak]) && alfas[ak] < gamma && i != 0  ){
            i = ak;
        } else if ( betas[bk] < gamma  && j != 0  ){
            j = bk;
        } else {
            break;
        }
    }

    //iL*
    j_i_stars[1] = i;
    //jL*
    j_i_stars[2] = j;
}

// [ iR*, iL*, jL*, jR* ]
template <typename VP, typename VD>
void case3(const VP& a, size_t na, const VP& b, size_t nb, const VD& alfas, const VD& betas, size_t iL, size_t jL, size_t j_i_stars[4]) {
    size_t i = 0;
    size_t j = 0;

    while (true){
        double_t gamma = neg_inf( k(a[i], b[j]) );
        size_t ak = (na+i-1) % na;
        size_t bk = (nb+j-1) % nb;
        
        if ( betas[bk] < gamma  && j != jL  ) {
            j = bk;
        } else if (alfas[ak] < gamma  && i != iL  ){
            i = ak;
        } else {
            break;
        }
    }

    //iR*
    j_i_stars[0] = i;
    //jR*
    j_i_stars[3] = j;
    i=iL;
    j=jL;
    
    
    while (true){
        double_t gamma = k(a[i], b[j]);
        if ( betas[j] > gamma && j!=0  ){
            j = (j+1) % nb;
        } else if ( alfas[i] > gamma && i!=0  ){
            i = (i+1) % na;
        } else {
            break;
        }
    }

    //iL*
    j_i_stars[1] = i;
    //jL*
    j_i_stars[2] = j;
}


// [ iR*, iL*, jL*, jR* ]
template <typename VP, typename VD>
void case4(const VP& a, size_t na, const VP& b, size_t nb, const VD& alfas, const VD& betas, size_t iL, size_t jL, size_t j_i_stars[4]) {
    size_t i = 0;
    size_t j = 0;


    while (true){
        double_t gamma = neg_inf ( k(a[i], b[j]) );

        size_t ak = (na+i-1) % na;
        size_t bk = (nb+j-1) % nb;

        if ( betas[bk] < gamma && j!=jL   ){
            j = bk;
        } else if ( alfas[ak] < gamma && i!=iL  ){
            i = ak;
        } else {
            break;
        }
    }

    //iR*
    j_i_stars[0] = i;
    //jR*
    j_i_stars[3] = j;
    i=iL;
    j=jL;

   
    
    while (true){
        double_t gamma = neg_inf( k(a[i], b[j]) );

        size_t ak = (na+i-1) % na;
        size_t bk = (nb+j-1) % nb;
        
        if (  std::isfinite(alfas[ak]) && alfas[ak] < gamma && i != 0  ){
            i = ak;
        } else if ( std::isfinite(betas[bk]) && betas[bk] < gamma && j != 0  ){
            j = bk;
        } else {
            break;
        }
    }

    //iL*
    j_i_stars[1] = i;
    //jL*
    j_i_stars[2] = j;
}

