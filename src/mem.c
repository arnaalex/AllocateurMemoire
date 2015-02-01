/*****************************************************
 * Copyright Grégory Mounié 2008-2013                *
 * This code is distributed under the GLPv3 licence. *
 * Ce code est distribué sous la licence GPLv3+.     *
 *****************************************************/

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include <math.h>
#include "mem.h"

/** squelette du TP allocateur memoire */

void *zone_memoire = 0;

/* ecrire votre code ici */
void *TZL[BUDDY_MAX_INDEX+1];

    int 
mem_init()
{
    if (! zone_memoire)
        zone_memoire = (void *) malloc(ALLOC_MEM_SIZE);
    if (zone_memoire == 0)
    {
        perror("mem_init:");
        return -1;
    }

    /* ecrire votre code ici */
    //Initialisation de TZL 
    for(int i=0; i<BUDDY_MAX_INDEX; ++i){
        TZL[i]=NULL;
    }
    TZL[BUDDY_MAX_INDEX]=zone_memoire;

    *(void**)zone_memoire=NULL;
    return 0;
}

    void *
mem_alloc(unsigned long size)
{
    /*  ecrire votre code ici */
  if(size<=0)
    return NULL;

  int bloc = ceil(log(size)/log(2));
  void* res = NULL;
  int i = bloc; 
     
    while((i<BUDDY_MAX_INDEX+1) && TZL[i]==NULL)
      i++;
    
    while(i>=0 && i<BUDDY_MAX_INDEX+1 && i!=bloc){
      void* un = TZL[i];
      int k = 1<<(i-1); 
      //printf("%d\n",k);
      void* deux = TZL[i]+k;
      TZL[i] = *((void**)TZL[i]);
      i=i-1;
      *(void**)un = deux;
      *(void**)deux = TZL[i];
      TZL[i] = un; 
    }
    
    if(i==bloc){
      res =  TZL[i];
      TZL[i] = *((void **)TZL[i]);
    }

    return (void**)res;  
}

    int 
mem_free(void *ptr, unsigned long size)
{
    /* ecrire votre code ici */
  int i = ceil(log(size)/log(2));
  bool found = true;
  void * mon_ad = ptr;
  void* p;
  void* adr_comp;
  while(i< BUDDY_MAX_INDEX && found){
    adr_comp =(void*)(((uintptr_t)mon_ad)^i);
    p = TZL[i];
    found = false;
    while(p!=NULL && !found){
      if(adr_comp == *(void**)p){
	found = true;
      }
      else{
	p = *(void**)p;
      }
    }
    if(found){
      i++;
      *(void**)p = *(void**)adr_comp;
      mon_ad = (mon_ad<adr_comp)?mon_ad:adr_comp;
    }
  }

    *(void**)mon_ad = TZL[i];
    TZL[i] = mon_ad;

    return 0;
}


    int
mem_destroy()
{
    /* ecrire votre code ici */
    free(zone_memoire);
    zone_memoire = 0;
    return 0;
}

