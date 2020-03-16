/* paging.c */
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

/* Write a value to memory */
void wr_mem(int Location, int Value);

/* Read a value from memory. */
int rd_mem(int Location);

/* These declarations are global so your procedures can use them */

/* File descriptor for the page file */
int pageFile;

/* Main memory, also called RAM */
int memory[5];

/* The page table, we have only one for this assignment.*/
int pagetable[8];

/*
Put any globals you need to share between the two procdeures here
You need the various arrays to remember if a page is in memory,
and to remember if it was reference and to track what is in a frame.
You also need to remember where you are in your memory scan.
You put them here so you can initialize them in the main program
(once) and use them in both your procedures.
*/
int in_mem[8];
int ref[5];
int counter;
int count;

int main()
{
    int loopcount;
    /* set up the page file */
    pageFile = open("pagefile.bin",O_RDWR|O_CREAT|O_TRUNC,S_IRUSR | S_IWUSR);
    /* 8 bytes of 0 */
    for (loopcount = 0 ; loopcount < 8 ; loopcount++)
    {
        write(pageFile,"\0",1); /* cheap way to have an array with a 0 */
    }

    /* YOU need to initialize your globals here. */
    for(int i = 0; i < 8; i++)
    {
        if(i < 5)
        {
            memory[i] = -1;
            ref[i] = -1;
        }
        in_mem[i] = -1;
    }
    counter = 0;
    /*
    YOU DO NOT CHANGE the rest of the main program, this is the part
    that tests to see if you've implemented paging correctly.
    */

    /* initialize all pages to contain a value */
    for (loopcount = 0 ; loopcount < 8 ; loopcount++)
    {
        /* mem[loopcount] = loopcount; */
        wr_mem(loopcount, loopcount);
    };

    /* cause some more paging */
    wr_mem(4, rd_mem(2) + rd_mem(4) ); /* mem[4] = mem[2] + mem[4] */
    wr_mem(6, rd_mem(2) + rd_mem(7) ); /* mem[6] = mem[2] + mem[7] */
    wr_mem(1, rd_mem(1) + rd_mem(3) ); /* mem[1] = mem[1] + mem[3] */
    wr_mem(5, rd_mem(3) + rd_mem(7) ); /* mem[5] = mem[3] + mem[7] */

    /* we are done the test */
    close(pageFile);
    return 0;
};

/*
This procedure illustrates the use of the lseek command.
It reads the fourth byte (offset 3) of the page file.
*/
int DummyProcedure()
{
    char ch;
    lseek(pageFile,3,SEEK_SET);
    read(pageFile,&ch,1);
    return ch;
};

/* YOU need to implement wr_mem here. */
void wr_mem(int Location, int Value)
{
    int pos = counter % 5;
    if(memory[pos] == -1)
    {
        memory[pos] = Location;
        ref[pos] = 0;
        in_mem[Location] = pos;
        pagetable[Location] = Value;
        printf("Page %d is loaded into frame %d.\n", Location, counter);
        count++;
    }
    else if(count < 8)
    {
        lseek(pageFile, Location, SEEK_SET);
        write(pageFile, &Location, 1);
        counter--;
        count++;
        printf("Page %d is loaded into disk.\n", Location);
    }
    else
    {
        if(in_mem[Location] == -1)
        {
            while(1)
            {
                int LRE = counter % 5;
                if(ref[LRE] == 0)
                {
                    int pageout = memory[LRE];
                    lseek(pageFile, pageout, SEEK_SET);
                    write(pageFile, &pageout, 1);
                    lseek(pageFile, Location, SEEK_SET);
                    int page;
                    read(pageFile, &page, 1);
                    write(pageFile, "\0", 1);
                    memory[LRE] = page;
                    in_mem[pageout] = -1;
                    in_mem[page] = LRE;
                    pagetable[page] = Value;
                    printf("Page %d is paged out from frame %d ", pageout, LRE);
                    printf("frame %d is empty, ", LRE);
                    printf("page %d is loaded into frame %d.\n", page, LRE);
                    break;
                }
                else
                {
                    ref[LRE] = 0;
                    counter++;
                }
            }
        }
        else
        {
            pagetable[Location] = Value;
            ref[in_mem[Location]] = 1;
            printf("Page %d is in frame %d.\n", Location, in_mem[Location]);
            counter--;
        }
    }
    counter++;
}
/* YOU need to implement rd_mem here. */
int rd_mem(int Location)
{
    int val;
    if(in_mem[Location] == -1)
    {
        while(1)
        {
            int LRE = counter % 5;
            if(ref[LRE] == 0)
            {
                int pageout = memory[LRE];
                lseek(pageFile, pageout, SEEK_SET);
                write(pageFile, &pageout, 1);
                lseek(pageFile, Location, SEEK_SET);
                int page;
                read(pageFile, &page, 1);
                write(pageFile, "\0", 1);
                memory[LRE] = page;
                in_mem[pageout] = -1;
                in_mem[page] = LRE;
                val = pagetable[page];
                printf("Page %d is paged out from frame %d, ", pageout, LRE);
                printf("frame %d is empty, ", LRE);
                printf("page %d is loaded into frame %d.\n", page, LRE);
                break;
            }
            else
            {
                ref[LRE] = 0;
                counter++;
            }
        }
    }
    else
    {
        val = pagetable[Location];
        ref[in_mem[Location]] = 1;
        printf("Page %d is in frame %d.\n", Location, in_mem[Location]);
        counter--;
    }
    counter++;
    return val;
}
