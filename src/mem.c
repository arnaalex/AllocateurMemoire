/*****************************************************
 * Copyright Grégory Mounié 2008-2013                *
 * This code is distributed under the GLPv3 licence. *
 * Ce code est distribué sous la licence GPLv3+.     *
 *****************************************************/

#include <stdio.h>
#include <stdlib.h>
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

    *zone_memoire=NULL;
    return 0;
}

    void *
mem_alloc(unsigned long size)
{
    /*  ecrire votre code ici */
  int bloc = ceil(log(size)/log(2));
  void* res = null;
  int i = bloc; 

  // while((i<BUDDY_MAX_INDEX+1) && (res==null)){
  
     
    while((i<BUDDY_MAX_INDEX+1) && TZL[i]==null)
      i++;
    
    while(i>=0 && i<BUDDY_MAX_INDEX+1 && i!=bloc){
      void* un = TZL[i];
      void* deux = TZL[i]+1<<(i-1);
      TZL[i] = *(TZL[i]);
      i=i-1;
      *un = &deux;
      *deux = TZL[i];
      TZ[i] = &un;
      
    }

  }

    return 0;  
}

    int 
mem_free(void *ptr, unsigned long size)
{
    /* ecrire votre code ici */
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

