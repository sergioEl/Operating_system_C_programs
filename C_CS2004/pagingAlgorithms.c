#include <time.h> 
#include <stdio.h>
#include <stdlib.h>

/*
    COMP2004 assignment8 Q7
        - Paging algorithms : FIFO, LRU, and OPT
    Seokho Han 201761541
    seokhoh@mun.ca
*/


/**************************************
 * The parameters of memory and disk pages 
 *
 * PageFrameList: The dynamically-allocated array representing memory pages
 * FrameNR: the number of page frames in memory
 * elementCout: Point to the next page to be replaced
 *
 * ReferenceString: The sequence of the demanding pages
 * ReferenceLength: The length of the randomized reference string
 * ReferenceSZ: the page number range in the reference string
 *
 */

#define ReferenceLength 10

typedef struct
{
    int *PageFrameList;
    int *Table; // This is for tracking references
    int elementCount;	
}PageFrame;

int ReferenceSZ, FrameNR;

PageFrame memory;

int *ReferenceString;
 

/* Test driver sub functions */

void generateReferenceString();

void initializePageFrame();

void printReferenceString();

void printPageFrame();


/* Algorithm Functions */

int FIFO();

int LRU();

int OPT();

/* The possible algorithm subfunctions */

int FIFOSearch(int PageNumber);

int FIFOInsert(int PageNumber);

int LRUSearch(int PageNumber);

int LRUInsert(int PageNumber);

int OPTSearch(int PageNumber);

int OPTInsert(int PageNumber);




/*******************************
 *
 * The main function is the test driver for FIFO, LRU, & OPT algorithms
 *
 * 1. Initialize the system parameters
 * 2. Initialize the memory pages 
 * 3. Generate the randomized reference string
 * 4. Apply the FIFO algorithm, calculate the number of page faults
 * 5. Apply the LRU algorithm, calculate the number of page faults
 * 6. Apply the OPT algorithm, calculate the number of page faults
 */


int main(int argc, char* argv[])
{

    // if( argc != 3 )
    // {
    //     printf("Command format: Test <reference string size> <number of page frames>");
    // }
    printf("* Range of the page numbers is 0 ~ 9 by default.\n* The length of randomly generated string is 10 by default.\n");
    printf("Enter the number of page frames: ");
    scanf("%d", &FrameNR);

    ReferenceSZ = 10;//atoi(argv[1]);
    //FrameNR = atoi(argv[2]);


   generateReferenceString();


   initializePageFrame();
   printf("page fault of FIFO: %d\n",FIFO());
   free(memory.PageFrameList);
   free(memory.Table);

   printf("\n");
   printf("\n");


   printReferenceString();

   initializePageFrame();
   printf("page fault of LRU: %d\n",LRU());
   free(memory.PageFrameList);
   free(memory.Table);
   
   
   printf("\n");
   printf("\n");


   printReferenceString();

   initializePageFrame();
   printf("page fault of OPT: %d\n",OPT());
   free(memory.PageFrameList);
   free(memory.Table);


   free(ReferenceString);	

   return 0;

}


void generateReferenceString()
{
   int i;
   srand(time(0)); // seed for the random number generator
   ReferenceString = (int *)malloc( sizeof(int) * ReferenceLength );
   printf("The randomized Reference String: ");
   for(i=0; i< ReferenceLength; i++)
   {
	ReferenceString[i] = rand() % ReferenceSZ;
        printf("%d ", ReferenceString[i]);       
   }
   printf("\n");
}


void initializePageFrame()
{
   int i;
   memory.PageFrameList = (int *)malloc( sizeof(int)* FrameNR );
   memory.Table = (int *)malloc( sizeof(int)* FrameNR );
   memory.elementCount =0;    
   for(i=0; i< FrameNR; i++)
   {
	memory.PageFrameList[i] = -1;
    memory.Table[i] = 0;        
   }

}

void printPageFrame()
{
   int i;
   for(i=0; i< FrameNR; i++)
   {
	printf("%2d ",memory.PageFrameList[i]);       
   }
   printf("\n");
}

void printReferenceString()
{
   int i;
   printf("The Same Reference String: ");
   for(i=0; i< ReferenceLength; i++)
   {
        printf("%d ", ReferenceString[i]);       
   }
   printf("\n");

}

int FIFO()
{
    int PagefaultCount=0;
    int i;

   for( i=0; i<ReferenceLength; i++ ) 
   {
       PagefaultCount+=FIFOInsert(ReferenceString[i]);
       printPageFrame();
   }

   return PagefaultCount;
}

int FIFOSearch(int PageNumber)
{
    for (size_t i = 0; i < FrameNR; i++)
    {
        if (memory.PageFrameList[i] == PageNumber)
        {
            return 1;
        }
    }
    return 0;
}

int FIFOInsert(int PageNumber)
{
    int Pagefault=0;
    if( 0==FIFOSearch(PageNumber) )
    {
        //Replace the page HERE
        memory.PageFrameList[memory.elementCount] = PageNumber;
        memory.elementCount = (memory.elementCount + 1) % FrameNR;
        Pagefault = 1;
    }

    return Pagefault;      
}


int LRU()
{
    int PagefaultCount=0;
    int i;

   for( i=0; i<ReferenceLength; i++ ) 
   {
       PagefaultCount+=LRUInsert(i);
       printPageFrame();
   }


   return PagefaultCount;

}

int LRUSearch(int PageNumber)
{
    size_t k;
    // reset the Table
    for (size_t i = 0; i < FrameNR; i++)
    {
        memory.Table[i] = 0;
    }

    for (size_t i = 0; i < FrameNR; i++)
    {
        // When there exists the page in the frame.
        if (memory.PageFrameList[i] == ReferenceString[PageNumber])
        {
            return i;
        }
        // When the frame is not full, insert a new page there.
        else if (memory.PageFrameList[i] == -1) 
        {
            memory.elementCount = i;
            return -1;
        }
        // When the frame is full and doesn't have the new page.
        else
        {
            // Loop backwards and find indices of the least recently used pages.
            for (k = PageNumber-1; k > 0; k--)
            {
                if (memory.PageFrameList[i] == ReferenceString[k])
                {
                    memory.Table[i] = k;
                    break;
                }
            }
        }
    }
    // find the index of the least recently used page in the frame.
    int min = 9999;
    for (size_t i = 0; i < FrameNR; i++)
    {
        // printf("ele : %d...", memory.Table[i]);
        if (memory.Table[i] < min)
        {
            min = memory.Table[i];
            memory.elementCount = i;
        }
    }
    // printf("min: %d.....", memory.elementCount);
    return -1;

}

int LRUInsert(int PageNumber)
{
    int PageFault=0;

    int Findindex = -1;
    Findindex = LRUSearch(PageNumber);

    if ( -1 == Findindex ) 
    { 
      //Replace the page HERE
      memory.PageFrameList[memory.elementCount] = ReferenceString[PageNumber];
      PageFault = 1;
    }
    return PageFault;
}

int OPT()
{
    int PagefaultCount=0;
    int i;

   for( i=0; i<ReferenceLength; i++ ) 
   {
       PagefaultCount+=OPTInsert(i);
       printPageFrame();
   }
   return PagefaultCount;

}

int OPTSearch(int PageNumber)
{
    size_t k, l;
    for (size_t i = 0; i < FrameNR; i++)
    {
        memory.Table[i] = 0;
    }

    for (size_t i = 0; i < FrameNR; i++)
    {
        // When there exists the page in the frame.
        if (memory.PageFrameList[i] == ReferenceString[PageNumber])
        {
            return i;
        }
        // When the frame is not full, insert a new page there.
        else if (memory.PageFrameList[i] == -1) 
        {
            memory.elementCount = i;
            return -1;
        }
        // When the frame is full and doesn't have the new page.
        else
        {
            // Find indices of the pages that will be refernced later.
            for (k = PageNumber + 1; k < ReferenceLength; k++)
            {
                if (memory.PageFrameList[i] == ReferenceString[k])
                {
                    memory.Table[i] = k;
                    break;
                }
            }
        }
    }

    int max = 0;
    for (size_t i = 0; i < FrameNR; i++)
    {
        // printf("ele : %d...", memory.Table[i]);
        // If there exist frame page that won't be referenced, insert a new page there.
        if (memory.Table[i] == 0)
        {
            memory.elementCount = i;
            break;
        }
        // Find the farthest page that will be referenced.
        else if (memory.Table[i] > max)
        {
            max = memory.Table[i];
            memory.elementCount = i;
        }
    }
    // printf("min: %d.....", memory.elementCount);
    return -1;

}

int OPTInsert(int PageNumber)
{
    int PageFault=0;

    int Findindex = -1;
    Findindex = OPTSearch(PageNumber);

    if ( -1 == Findindex ) 
    { 
      //Replace the page HERE
      memory.PageFrameList[memory.elementCount] = ReferenceString[PageNumber];
      PageFault = 1;
    }
    return PageFault;
}