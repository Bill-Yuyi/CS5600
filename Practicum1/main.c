//
// Created by Yuyi Wang on 3/17/24.
//
#include <stdlib.h>
#include <stdio.h>
#include "LRU.h"
#include "randomReplace.h"

#define LRU_CAPACITY 1024
#define RANDOM_MIN 1
#define RANDOM_MAX 1000
#define TEST_TIMES 1000


int main() {
    int i;
    int randNumber;
    int cacheMissForLRU = 0;
    int cacheHitForLRU = 0;
    int cacheMissForRandom = 0;
    int cacheHitForRandom = 0;
    char sender[128];
    char receiver[128];
    char content[752];

    strcpy(sender, "NTiERE?nJDf$;KHJgd.dAvc?Wm0[gaBD+dwdM4RUVC_)Dnm!=fLQUu;xmZcS9t3gUF0%.xLQhrFhb]J@1hH?wDBVGN8&3fNq}u!b}_}&Pq.p4gTLN=}1ix5Ra$0K9)S");
    strcpy(receiver, "NTiERE?nJDf$;KHJgd.dAvc?Wm0[gaBD+dwdM4RUVC_)Dnm!=fLQUu;xmZcS9t3gUF0%.xLQhrFhb]J@1hH?wDBVGN8&3fNq}u!b}_}&Pq.p4gTLN=}1ix5Ra$0K9)S");
    strcpy(content, "/U23ENm7/-@ZyM_*Dtibrc+49%i=P0W#+D)+dD6i(cnuc4#X_6p)xt-H.C(Bz-x[2S.S,YL$j]SP{F8u+@Hty3J3KLgmQ:.N=d]}pDemupY:}+1BPt;+.k5X/?UuWDE:K!4v=[*722?UD7Eh{C;Y*S#({S:*c*BA;ukg{rvTrE4Uhkb9.NtQSwqz;5TA(=SmbDdAff&e%W.8&xkMxZ96CfCVQ4[p+fGxF8=wm!NJ&?S!u]xRPdUH%$N[pt1&cy[{ZKJY,M+1#k&i+18=A;;}?cxT1NyKv@uQ7!&7cZVtvRh3h;7u/#8V2h=VTA_t9SnyH9F1].)Zw@m%!G?MY)3R!8TCGC?;@;%B0-DM%}DbZ!/ik/0&E,,ieg8_aM8,JFp-UuPd,v7t][DRKKmWRz_{YE&,zrq]#(jmkS)jrJqV-5JJv!VdZt$+L/LgFB:?Wvf*CUSP=$qES1+.6W,8x7?/(DZu+JePq,FffgS/Z5Lp*0k@i?FrMa#Yv(}@qV]7pM0D)$)#a)UEanFk3Vxjidt_9y-ixA{{=3&C(KBAPE1UY4ChjbzPrQxy0$LfexiN%_i.MB{CtRuDezbqH$M,*x6P8:)u{Z%*D[c)!g/vi7Cq+en(XANMaC34k.Nh3Kb9[}@G3;wmxzzh&=j#b=7&t/kdXG+&X3j;$F@2kF6WW@rNmz5/G{ht)7D}FGij[_Qnu;V,g[Ft&VRfJhry(]&P*S8]wrj[J=;EFgf8q!2V6m9XWa$Jv}q");

    // create 1000 random messages
    for(i = 0; i < RANDOM_MAX; i++) {
        message_t* new_msg = createMessage(sender, receiver, content);
        if(new_msg == NULL) {
            return 1;
        }
        storeMessage(new_msg);
        destroyMessage(new_msg);
    }

    randomMemory* randMemo = initializeMemory();

    LRUCache* cache = initializeLRU(LRU_CAPACITY);
    for(i = 0; i < TEST_TIMES; i++) {

        randNumber = RANDOM_MIN + rand() % RANDOM_MAX;
        //randNumber = weightedRandomNumber();
        if(findMessage(randMemo, randNumber) == 0) {
            cacheHitForRandom++;
        }else {
            putIntoMemory(randMemo, randNumber);
            cacheMissForRandom++;
        }

        if(LRUCacheFind(cache, randNumber) == 1) {
            cacheMissForLRU++;
        }else{
            cacheHitForLRU++;
        }
        LRUCacheGet(cache, randNumber);
    }

    printf("cache miss: %d, cache hit: %d, ratio: %.2f%% \n", cacheMissForLRU, cacheHitForLRU,
           (double) cacheHitForLRU / TEST_TIMES * 100);
    printf("Random Replacement: cache miss: %d, cache hit: %d, ratio: %.2f%% \n", cacheMissForRandom, cacheHitForRandom,
           (double) cacheHitForRandom / TEST_TIMES * 100);
    destroyLRUCache(cache);
    destroyMemory(randMemo);

    return 0;
}