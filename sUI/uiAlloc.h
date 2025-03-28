#ifndef __UIALLOC_H
#define __UIALLOC_H
#include "mType.h"
#include "winManage.h"


void uiHeapInit(uint8* addr,uint32 size);
winHandle uiAlloc(uint32 size);
void uiFree(winHandle handle);
void uiHeapInfor(uint32 n);
void heapAllocTest(void);


#define uiHandleToPtr(n)    (n)
#define uiPtrToHandle(n)    (n)





#endif
