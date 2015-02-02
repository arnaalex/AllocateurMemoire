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

  if(size<=0)
    return NULL;

  int bloc = ceil(log(size)/log(2));
  int alloc_min = ceil(log( sizeof(void*))/log(2)); 
  if (bloc<alloc_min){
    bloc=alloc_min;
  }
  void* res = NULL;
  int i = bloc; 

   
    while((i<BUDDY_MAX_INDEX+1) && TZL[i]==NULL)
      i++;
    
    while(  i<BUDDY_MAX_INDEX+1 && i!=bloc){
      void* un = TZL[i];
      void* deux = (void*)((uintptr_t)TZL[i]+(1<<(i-1)));
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
    return res;  
}

    int 
mem_free(void *ptr, unsigned long size)
{
  
  if(ptr<zone_memoire || (ptr+size)>(zone_memoire+ALLOC_MEM_SIZE))
    return 1;

  int i = ceil(log(size)/log(2));
  int alloc_min = ceil(log( sizeof(void*))/log(2)); 
  if (i<alloc_min){
    i=alloc_min;
  }
  bool found = true;
  void * mon_ad = ptr;
  void* p;
  void* adr_comp;
  void * mon_ad_correct;
  while(i< BUDDY_MAX_INDEX && found){
    mon_ad_correct =(void*)( (uintptr_t)mon_ad -(uintptr_t)zone_memoire); 
    adr_comp =(void*)(((uintptr_t)mon_ad_correct^(1<<(i)))+zone_memoire);

    p = TZL[i];
    found = adr_comp == p ;
    bool first = found;
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
      if(!first)
      *(void**)p = *(void**)adr_comp;
      else
	TZL[i-1]=*(void**)p;
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
    free(zone_memoire);
    zone_memoire = 0;
    return 0;
}

