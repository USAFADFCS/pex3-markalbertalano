/** pagequeue.c
 * ===========================================================
 * Name: mark alano, 23 april 2026
 * Section: CS483 / m3
 * Project: PEX3 - Page Replacement Simulator
 * Purpose: Implementation of the PageQueue ADT — a doubly-linked
 *          list for LRU page replacement.
 *          Head = LRU (eviction end), Tail = MRU end.
 * =========================================================== */
#include <stdio.h>
#include <stdlib.h>

#include "pagequeue.h"

/**
 * @brief Create and initialize a page queue with a given capacity
 * create
 */
PageQueue *pqInit(unsigned int maxSize) {
    // TODO: malloc a PageQueue, set head and tail to NULL,
    //       size to 0, maxSize to maxSize, and return the pointer
    PageQueue *pq = (PageQueue*)malloc(sizeof(PageQueue));

    pq->head = NULL;
    pq->tail = NULL;
    pq->size = 0;
    pq->maxSize = maxSize;

    return pq;
}

/**
 * @brief Access a page in the queue (simulates a memory reference)
 */
long pqAccess(PageQueue *pq, unsigned long pageNum) {
    // TODO: Search the queue for pageNum (suggest searching tail->head
    //       so you naturally count depth from the MRU end).
    
    PqNode *step = pq->tail;
    long d = 0;

    //cant be end
    while(step != NULL){

    // HIT path (page found at depth d):
    //   - Remove the node from its current position and re-insert
    //     it at the tail (most recently used).
    //   - Return d.
        if(step->pageNum == pageNum){

            //cant be tail
            if(step != pq->tail){

                //relink prev
                if(step->prev != NULL){

                    step->prev->next = step->next;
                }
                //case: no prev
                else{

                    pq->head = step->next;
                }

                //relink post => NEEDS TO HAVE NO POST CASE

                //case: somethings after
                if(step->next != NULL){

                    step->next->prev = step->prev;
                }

                /*
                else{
                
                    pq->tail = step->prev
                }
                */

                //step to pq prev - step - post => tail - step - 
                //tail0 to step, step to tail0, step to null
                step->prev = pq->tail;
                step->next = NULL;
                pq->tail->next = step;

                //need to udpate pointer to queue tail
                pq->tail = step;
            }

            return d;
        }

        ++d;
        
        //next up
        step = step->prev;
    }

    // MISS path (page not found):
    //   - Allocate a new node for pageNum and insert it at the tail.
    //   - If size now exceeds maxSize, evict the head node (free it).
    //   - Return -1.
    //asked claude for a DLL visual of miss condition

        //allocate and connect
        PqNode *new = (PqNode*)malloc(sizeof(PqNode));

        new->pageNum = pageNum;
        new->next = NULL;
        new->prev = NULL;

        new->prev = pq->tail;

        //cant be mepty
        if(pq->tail != NULL){

            pq->tail->next = new;
        }
        //case: empty
        else{

            pq->head = new;
        }

        pq->tail = new;

        ++pq->size;

        //eviction
        if(pq->size > pq->maxSize){

            PqNode *copy = pq->head;
            pq->head = copy->next;

            //connect to free
            pq->head->prev = NULL;

            //memory leak fix
            free(copy);

            --pq->size;
        }

    return -1;
}

/**
 * @brief Free all nodes in the queue and reset it to empty
 * delete
 */
void pqFree(PageQueue *pq) {
    // TODO: Walk from head to tail, free each node, then free
    //       the PageQueue struct itself.

    PqNode* currentNode = pq->head;

    while(currentNode){

        PqNode *temp = currentNode;
        currentNode = currentNode->next;

        free(temp);
    }

    free(pq);
}

/**
 * @brief Print queue contents to stderr for debugging
 * print
 */
void pqPrint(PageQueue *pq) {
    // TODO (optional): Print each page number from head to tail,
    //                  marking which is head and which is tail.
    //                  Useful for desk-checking small traces.
    PqNode *curr = pq->head;

    while(curr != NULL){

        //https://stackoverflow.com/questions/3209909/how-to-printf-unsigned-long-in-c for
        // long unsigned specifier
        printf("%lu goes to ", curr->pageNum);
        curr = curr->next;
    }
}
