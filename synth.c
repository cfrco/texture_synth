/*
 * Texture Synthesis
 * Author : catcfrco (z82206.cat[at]gmail.com)
 */
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<math.h>

//include<string.h>
void* memset(void *ptr,int value,size_t num);

#define INDEX(_r,_c,_len) ( (_r) * (_len) + (_c))

void winput(double *src,int sr,int sc,int src_col,int hwsize,
            double *dst,int dr,int dc,int dst_col)
{
    int r,c,i,j;

    i = dr-hwsize;
    for(r=sr-hwsize;r<=sr+hwsize;++r,++i){
        j = dc-hwsize;
        for(c=sc-hwsize;c<=sc+hwsize;++c,++j)
            dst[INDEX(i,j,dst_col)] = src[INDEX(r,c,src_col)];
    }
}

void winset_char(char *valid,char v,int row,int col,int col_len,int hwsize)
{
    int r,c;
    for(r=row-hwsize;r<=row+hwsize;++r){
        for(c=col-hwsize;c<=col+hwsize;++c)
            valid[INDEX(r,c,col_len)] = v;
    }
}

void winget(double *arr,double *dst,int r,int c,int row_len,int col_len,int hwsize){
    int i,j;
    int re = r+hwsize, ce = c+hwsize;

    if(re >= row_len) re = row_len-1;
    if(ce >= col_len) ce = col_len-1;

    for(i=r-hwsize;i<0;++i){
        for(j=c-hwsize;j<=c+hwsize;++j)
            *dst++ = 0;
    }
    for(;i<=re;++i){
        for(j=c-hwsize;j<0;++j)
            *dst++ = 0;
        for(;j<=ce;++j)
            *dst++ = arr[INDEX(i,j,col_len)];
        for(;j<=c+hwsize;++j)
            *dst++ = 0;
    }
    for(;i<=r+hwsize;++i){
        for(j=c-hwsize;j<=c+hwsize;++j)
            *dst++ = 0;
    }
}

void winget_char(char *valid,char *dst,int r,int c,int row_len,int col_len,int hwsize){
    int i,j;
    int re = r+hwsize, ce = c+hwsize;

    if(re >= row_len) re = row_len-1;
    if(ce >= col_len) ce = col_len-1;

    for(i=r-hwsize;i<0;++i){
        for(j=c-hwsize;j<=c+hwsize;++j)
            *dst++ = 0;
    }
    for(;i<=re;++i){
        for(j=c-hwsize;j<0;++j)
            *dst++ = 0;
        for(;j<=ce;++j)
            *dst++ = valid[INDEX(i,j,col_len)];
        for(;j<=c+hwsize;++j)
            *dst++ = 0;
    }
    for(;i<=r+hwsize;++i){
        for(j=c-hwsize;j<=c+hwsize;++j)
            *dst++ = 0;
    }
}

void gauss_kern(double *out,int size,double sigma)
{
    int hsize = (size-1)/2,r,c;
    double q,s = 2.0 * sigma * sigma;
    double sum = 0;

    if(size%2!=1) --size;

    for(r=-hsize;r<=hsize;++r){
        for(c=-hsize;c<=hsize;++c){
            q = sqrt(r*r+c*c);
            q = (exp(-(q*q)/s))/(M_PI * s);
            out[INDEX(r+hsize,c+hsize,size)] = q;
            sum += q;
        }
    }
    
    for(r=0;r<size*size;++r)
        out[r] /= sum;
}

int find_unfilled(char *valid,int row_len,int col_len,
                   int *out,int outlen,int *now){
    if(*now >= 0){
        *now -= 1;
        return out[(*now)+1];
    }else{
        int r,c;
        for(r=0;r<row_len;++r){
            for(c=0;c<col_len;++c){
                if(valid[INDEX(r,c,col_len)]) continue;
                if(r < row_len-1 && valid[INDEX(r+1,c,col_len)]){
                    if(*now+1 < outlen) out[++(*now)] = r*col_len+c;
                    else break;
                }
                else if(r > 0 && valid[INDEX(r-1,c,col_len)]){
                    if(*now+1 < outlen) out[++(*now)] = r*col_len+c;
                    else break;
                }
                else if(c < col_len-1 && valid[INDEX(r,c+1,col_len)]){
                    if(*now+1 < outlen) out[++(*now)] = r*col_len+c;
                    else break;
                }
                else if(c > 0 && valid[INDEX(r,c-1,col_len)]){
                    if(*now+1 < outlen) out[++(*now)] = r*col_len+c;
                    else break;
                }
            } 
        }

        if(*now >= 0){
            *now -= 1;
            return out[(*now)+1];
        }else return -1;
    }
}

int find_min(double *arr,int len){
    int ind = 0,i;
    double min = arr[0];

    for(i=1;i<len;++i){
        if(arr[i] < min){
            min = arr[i];
            ind = i;
        }
    }
    return ind;
}

int list_lessthan(double *arr,double value,int len,int *indlist)
{
    int i,j=-1,t;
    for(i=0;i<len;++i){
        if(arr[i] <= value){
            if(i!=++j){
                t = indlist[i];
                indlist[i] = indlist[j];
                indlist[j] = t;
            }
        }
    }
    return j+1;
}

int list_lessthan2(double *arr,double value,int len,int *indlist,int *ssdlist)
{
    int i,j=-1,t;
    double dt;
    for(i=0;i<len;++i){
        if(arr[i] <= value){
            if(i!=++j){
                t = indlist[i];
                indlist[i] = indlist[j];
                indlist[j] = t;

                dt = ssdlist[i];
                ssdlist[i] = ssdlist[j];
                ssdlist[j] = dt;
            }
        }
    }
    return j+1;
}

int maxcount(int *indlist,double *ssdlist,int len,int *countlist)
{
    int i,j,max=-1,ind=-1;
    double d;

    for(i=0;i<len;++i)
        countlist[i] = 0;

    for(i=0;i<len;++i){
        if(countlist[i]==-1) continue;

        for(j=i+1;j<len;++j){
            d = countlist[i] - countlist[j]; 
            d *= d;

            if(d < 100){
                countlist[i]++; 
                countlist[j] = -1;
            }
        } 

        if(countlist[i] > max){
            max = countlist[i];
            ind = i;
        }
    }

    return ind;
}

// mod == 0 => best match
// mod == 1 => random choice
// mod == 2 => counting
int find_best_match(double *raw_red,double *raw_green,double *raw_blue,
                    int raw_row,int raw_col,
                    double *tmp_red,double *tmp_green,double *tmp_blue,
                    int wsize,double *mask,double err,
                    int *index_list,double *ssd_list,int *count_list,int mod)
{
    int r,c,hwsize=(wsize-1)/2,min_ind=-1,p=-1;
    double min_ssd;

    // foreach all window    
    for(r=hwsize;r<raw_row-hwsize;++r){
        for(c=hwsize;c<raw_col-hwsize;++c){
            //calucate SSD
            int wr,wc,i=0,ind;
            double ssd = 0,t;
            for(wr=r-hwsize;wr<=r+hwsize;++wr){
                for(wc=c-hwsize;wc<=c+hwsize;++wc,++i){
                    ind = INDEX(wr,wc,raw_col);
                    t = raw_red[ind] - tmp_red[i];
                    ssd += t*t*mask[i];
                    
                    t = raw_green[ind]-tmp_green[i];
                    ssd += t*t*mask[i];
                    
                    t = raw_blue[ind]-tmp_blue[i];
                    ssd += t*t*mask[i];
                } 
            }
            index_list[p+1] = r*raw_col+c;
            ssd_list[++p] = ssd;
        }
    }

    min_ind = find_min(ssd_list,p+1);

    if(mod == 0) return index_list[min_ind];
    
    r = list_lessthan(ssd_list,ssd_list[min_ind]*(1+err),p+1,index_list);
    if(mod == 1) return index_list[rand()%r];
    
    // mod == 2
    r = maxcount(index_list,ssd_list,r,count_list);
    return index_list[r];
}

void synth(double *raw_red,double *raw_green,double *raw_blue,
           int raw_row,int raw_col,
           double *new_red,double *new_green,double *new_blue,
           int new_row,int new_col,int wsize,int psize,double err,int mod)
{
    int i,j;
    int hwsize = (wsize-1)/2,npixels = new_row*new_col;

    //init
    srand(time(NULL));
    memset(new_red,0,npixels*sizeof(double));
    memset(new_green,0,npixels*sizeof(double));
    memset(new_blue,0,npixels*sizeof(double));

    //allocate memory
    char *valid = (char*)malloc(npixels*sizeof(char));
    memset(valid,0,npixels);

    double *tmp_red   = (double*)malloc(wsize*wsize*sizeof(double));
    double *tmp_green = (double*)malloc(wsize*wsize*sizeof(double));
    double *tmp_blue  = (double*)malloc(wsize*wsize*sizeof(double));
    char *validmask   = (char*)malloc(wsize*wsize*sizeof(char));
    double *mask      = (double*)malloc(wsize*wsize*sizeof(double));
    double *ssd_list  = (double*)malloc(raw_row*raw_col*sizeof(double));
    int *index_list   = (int*)malloc(raw_row*raw_col*sizeof(int));
    int *count_list   = (int*)malloc(raw_row*raw_col*sizeof(int));
    int *valid_list   = (int*)malloc(100*sizeof(int)); // for getunfilled2
    double *gauss     = (double*)malloc(wsize*wsize*sizeof(double));
    gauss_kern(gauss,wsize,1);

    //put patch
    int r = rand()%(raw_row-psize)+psize/2,c = rand()%(raw_col-psize)+psize/2;
    winput(raw_red  ,r,c,raw_col,psize/2,new_red  ,new_row/2,new_col/2,new_col);
    winput(raw_green,r,c,raw_col,psize/2,new_green,new_row/2,new_col/2,new_col);
    winput(raw_blue ,r,c,raw_col,psize/2,new_blue ,new_row/2,new_col/2,new_col);
    winset_char(valid,1,new_col/2,new_row/2,new_col,psize/2);
    
    //synth
    int now = -1;
    int target = find_unfilled(valid,new_row,new_col,valid_list,100,&now);
    while(target != -1){
        r = target/new_col;
        c = target%new_col;
        
        winget(new_red  ,tmp_red  ,r,c,new_row,new_col,hwsize);
        winget(new_green,tmp_green,r,c,new_row,new_col,hwsize);
        winget(new_blue ,tmp_blue ,r,c,new_row,new_col,hwsize);
        winget_char(valid,validmask,r,c,new_row,new_col,hwsize);

        for(i=0;i<wsize*wsize;++i)
            mask[i] = gauss[i]*validmask[i];

        int best = find_best_match(raw_red,raw_green,raw_blue,raw_row,raw_col,
                                   tmp_red,tmp_green,tmp_blue,wsize,mask,err,
                                   index_list,ssd_list,count_list,mod);
        //set filled
        valid[r*new_col+c] = 1;
        
        //put pixel
        new_red[target] = raw_red[best];
        new_green[target] = raw_green[best];
        new_blue[target] = raw_blue[best];

        target = find_unfilled(valid,new_row,new_col,valid_list,100,&now);
    }

    //free memory
    free(tmp_red);
    free(tmp_green);
    free(tmp_blue);
    free(validmask);
    free(mask);
    free(ssd_list);
    free(index_list);
    free(count_list);
    free(valid_list);
    free(gauss);
}

void _pyramid(double *nr,double *ng,double *nb,int row,int col,int wsize,
              double *or,double *og,double *ob)
{
    int orow = row/2, ocol = col/2,r,c,i;
    int hwsize = (wsize-1)/2;

    double *tmp_red   = (double*)malloc(wsize*wsize*sizeof(double));
    double *tmp_green = (double*)malloc(wsize*wsize*sizeof(double));
    double *tmp_blue  = (double*)malloc(wsize*wsize*sizeof(double));
    double *mask      = (double*)malloc(wsize*wsize*sizeof(double));
    gauss_kern(mask,wsize,1);

    double sr,sg,sb;
    
    for(r=0;r<orow;++r){
        for(c=0;c<ocol;++c){
            winget(nr,tmp_red  ,r*2,c*2,row,col,hwsize);
            winget(ng,tmp_green,r*2,c*2,row,col,hwsize);
            winget(nb,tmp_blue ,r*2,c*2,row,col,hwsize);

            sr = sg = sb = 0;

            for(i=0;i<wsize*wsize;++i){
                sr += tmp_red[i]  *mask[i];
                sg += tmp_green[i]*mask[i];
                sb += tmp_blue[i] *mask[i];
            }

            or[INDEX(r,c,ocol)] = sr;
            og[INDEX(r,c,ocol)] = sg;
            ob[INDEX(r,c,ocol)] = sb;
        }
    }
    
    free(tmp_red);
    free(tmp_green);
    free(tmp_blue);
}

void _pyramidvalid(char *in,int row,int col,char *out)
{
    int ocol = col/2,r,c;
    memset(out,0,(row/2)*(col/2));

    for(r=0;r<row;++r){
        for(c=0;c<col;++c){
            if(in[INDEX(r,c,col)] == 1)
                out[INDEX(r/2,c/2,ocol)] = 1;
        }
    }
}

int pyramid_find_best_match(double *(*rawp)[3],int raw_row,int raw_col,int level,
                            double *tmp_red,double *tmp_green,double *tmp_blue,
                            int wsize,double *mask,double err,
                            int *index_list,double *ssd_list,int *count_list,int mod)
{
    int r,c,hwsize=(wsize-1)/2,min_ind=-1,p=-1;
    double min_ssd;

    // foreach all window    
    for(r=hwsize;r<raw_row-hwsize;++r){
        for(c=hwsize;c<raw_col-hwsize;++c){
            //calucate SSD
            int wr,wc,i=0,ind;
            double ssd = 0,t;
            for(wr=r-hwsize;wr<=r+hwsize;++wr){
                for(wc=c-hwsize;wc<=c+hwsize;++wc,++i){
                    ind = INDEX(wr,wc,raw_col);
                    t = rawp[level][0][ind] - tmp_red[i];
                    ssd += t*t*mask[i];
                    
                    t = rawp[level][1][ind] - tmp_green[i];
                    ssd += t*t*mask[i];
                    
                    t = rawp[level][2][ind] - tmp_blue[i];
                    ssd += t*t*mask[i];
                } 
            }
            for(wr=r/2-hwsize;wr<=r/2+hwsize;++wr){
                for(wc=c/2-hwsize;wc<=c/2+hwsize;++wc,++i){
                    ind = INDEX(wr,wc,raw_col/2);
                    t = rawp[level+1][0][ind] - tmp_red[i];
                    ssd += t*t;
                    
                    t = rawp[level+1][1][ind] - tmp_green[i];
                    ssd += t*t;
                    
                    t = rawp[level+1][2][ind] - tmp_blue[i];
                    ssd += t*t;
                } 
            }
            index_list[p+1] = r*raw_col+c;
            ssd_list[++p] = ssd;
        }
    }

    min_ind = find_min(ssd_list,p+1);

    if(mod == 0) return index_list[min_ind];
    
    r = list_lessthan(ssd_list,ssd_list[min_ind]*(1+err),p+1,index_list);
    if(mod == 1) return index_list[rand()%r];
    // mod == 2
    r = maxcount(index_list,ssd_list,r,count_list);
    return index_list[r];
}

void synth_pyramid(double *raw_red,double *raw_green,double *raw_blue,
                   int raw_row,int raw_col,
                   double *new_red,double *new_green,double *new_blue,
                   int new_row,int new_col,int wsize,int psize,double err,int level)
{
    int i,j;
    int hwsize = (wsize-1)/2,npixels = new_row*new_col;

    //init
    srand(time(NULL));
    memset(new_red,0,npixels*sizeof(double));
    memset(new_green,0,npixels*sizeof(double));
    memset(new_blue,0,npixels*sizeof(double));

    //allocate memory
    char *valid = (char*)malloc(npixels*sizeof(char));
    memset(valid,0,npixels);

    double *tmp_red   = (double*)malloc(wsize*wsize*sizeof(double)*2);
    double *tmp_green = (double*)malloc(wsize*wsize*sizeof(double)*2);
    double *tmp_blue  = (double*)malloc(wsize*wsize*sizeof(double)*2);
    char *validmask   = (char*)malloc(wsize*wsize*sizeof(char));
    double *mask      = (double*)malloc(wsize*wsize*sizeof(double)*2);
    double *ssd_list  = (double*)malloc(raw_row*raw_col*sizeof(double));
    int *index_list   = (int*)malloc(raw_row*raw_col*sizeof(int));
    int *count_list   = (int*)malloc(raw_row*raw_col*sizeof(int));
    int *valid_list   = (int*)malloc(100*sizeof(int)); // for getunfilled2
    double *gauss     = (double*)malloc(wsize*wsize*sizeof(double));
    gauss_kern(gauss,wsize,1);

    double *(*rawp)[3] = (double *(*)[3]) malloc(sizeof(double*)*(level+1)*3);
    double *(*outp)[3] = (double *(*)[3]) malloc(sizeof(double*)*level*3);
    char *(*validp)    = (char *(*)) malloc(sizeof(char*)*level);

    //put patch
    int r = rand()%(raw_row-psize)+psize/2,c = rand()%(raw_col-psize)+psize/2;
    winput(raw_red  ,r,c,raw_col,psize/2,new_red  ,new_row/2,new_col/2,new_col);
    winput(raw_green,r,c,raw_col,psize/2,new_green,new_row/2,new_col/2,new_col);
    winput(raw_blue ,r,c,raw_col,psize/2,new_blue ,new_row/2,new_col/2,new_col);
    winset_char(valid,1,new_col/2,new_row/2,new_col,psize/2);
    
    //generate pyramid
    rawp[0][0] = raw_red;
    rawp[0][1] = raw_green;
    rawp[0][2] = raw_blue;
    outp[0][0] = new_red;
    outp[0][1] = new_green;
    outp[0][2] = new_blue;
    validp[0]  = valid;

    int prow = raw_row,pcol = raw_col;
    int pnrow = new_row,pncol = new_col;
    for(i=1;i<level;++i){
        //alocate memory
        rawp[i][0] = (double*)malloc(sizeof(double)*(prow/2)*(pcol/2));
        rawp[i][1] = (double*)malloc(sizeof(double)*(prow/2)*(pcol/2));
        rawp[i][2] = (double*)malloc(sizeof(double)*(prow/2)*(pcol/2));
        outp[i][0] = (double*)malloc(sizeof(double)*(pnrow/2)*(pncol/2));
        outp[i][1] = (double*)malloc(sizeof(double)*(pnrow/2)*(pncol/2));
        outp[i][2] = (double*)malloc(sizeof(double)*(pnrow/2)*(pncol/2));
        validp[i]  = (char*)malloc(sizeof(char)*(pnrow/2)*(pncol/2));

        //generate
        _pyramid(rawp[i-1][0],rawp[i-1][1],rawp[i-1][2],prow,pcol,5,
                 rawp[i][0]  ,rawp[i][1]  ,rawp[i][2]);
        _pyramid(outp[i-1][0],outp[i-1][1],outp[i-1][2],pnrow,pncol,5,
                 outp[i][0]  ,outp[i][1]  ,outp[i][2]);
        _pyramidvalid(validp[i-1],pnrow,pncol,validp[i]);

        prow /= 2;
        pcol /= 2;
        pnrow /= 2;
        pncol /= 2;
    }
    rawp[level][0] = (double*)malloc(sizeof(double)*prow*pcol);
    rawp[level][1] = (double*)malloc(sizeof(double)*prow*pcol);
    rawp[level][2] = (double*)malloc(sizeof(double)*prow*pcol);
    memset(rawp[level][0],0,sizeof(double)*prow*pcol);
    memset(rawp[level][1],0,sizeof(double)*prow*pcol);
    memset(rawp[level][2],0,sizeof(double)*prow*pcol);

    //synth for each level
    int target,now;
    for(i=level-1;i>=0;--i){
        int trow = raw_row/(1<<i),tcol = raw_col/(1<<i);
        int tnrow = new_row/(1<<i),tncol = new_col/(1<<i);

        if(i==level-1){
            for(j=0;j<wsize*wsize;++j){
                mask[j+wsize*wsize] = 0;
                tmp_red[j+wsize*wsize] = 0;
                tmp_green[j+wsize*wsize] = 0;
                tmp_blue[j+wsize*wsize] = 0;
            }
        }else{
            for(j=0;j<wsize*wsize;++j)
                mask[j+wsize*wsize] = gauss[j];
        }
        
        now = -1; 
        target = find_unfilled(validp[i],tnrow,tncol,valid_list,100,&now);
        while(target!=-1){
            r = target/tncol;
            c = target%tncol;
            
            winget(outp[i][0],tmp_red  ,r,c,tnrow,tncol,hwsize);
            winget(outp[i][1],tmp_green,r,c,tnrow,tncol,hwsize);
            winget(outp[i][2],tmp_blue ,r,c,tnrow,tncol,hwsize);
            winget_char(validp[i],validmask,r,c,tnrow,tncol,hwsize);
            if(i!=level-1){
                winget(outp[i+1][0],tmp_red+(wsize*wsize)  ,r/2,c/2,tnrow/2,tncol/2,hwsize);
                winget(outp[i+1][1],tmp_green+(wsize*wsize),r/2,c/2,tnrow/2,tncol/2,hwsize);
                winget(outp[i+1][2],tmp_blue+(wsize*wsize) ,r/2,c/2,tnrow/2,tncol/2,hwsize);
            }

            for(j=0;j<wsize*wsize;++j)
                mask[j] = gauss[j]*validmask[j];

            int best = pyramid_find_best_match(rawp,trow,tcol,i,
                                               tmp_red,tmp_green,tmp_blue,
                                               wsize,mask,err,
                                               index_list,ssd_list,count_list,0);
            //set filled
            validp[i][r*tncol+c] = 1;
            
            //put pixel
            outp[i][0][target] = rawp[i][0][best];
            outp[i][1][target] = rawp[i][1][best];
            outp[i][2][target] = rawp[i][2][best];

            target = find_unfilled(validp[i],tnrow,tncol,valid_list,100,&now);
        }
    }
    
    //free memory
    free(tmp_red);
    free(tmp_green);
    free(tmp_blue);
    free(validmask);
    free(mask);
    free(ssd_list);
    free(index_list);
    free(count_list);
    free(gauss);

    for(i=1;i<level;++i){
        free(rawp[i][0]);
        free(rawp[i][1]);
        free(rawp[i][2]);

        free(outp[i][0]);
        free(outp[i][1]);
        free(outp[i][2]);

        free(validp[i]);
    }
    free(rawp[level][0]);
    free(rawp[level][1]);
    free(rawp[level][2]);
    free(rawp);
    free(outp);
    free(validp);
}
