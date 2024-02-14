// about: Dear Emily Snow Program
// author: Cesar Carrillo

// imports
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <ncurses.h>

// return a random number from range
int randr(int min, int max) { return (rand() % (max - min + 1)) + min; }

// get terminal width
int scrGetWidth() 
{
	int height 	= 0;
	int width 	= 0;
 
    getmaxyx(stdscr, height, width);

	return width;
}

// get terminal height
int scrGetHeight() 
{ 
	int height 	= 0;
	int width 	= 0;
	
	getmaxyx(stdscr, height, width);

	return height;
}

// Snow flake structure
typedef struct SnowFlake 
{
    float x; // position
    float y;
    
    bool dead; // alive status

    struct SnowFlake* next; // next in list
} SnowFlake;

// Create a snow flake
SnowFlake* createSnowFlake(int x, int y)
{
    SnowFlake* flake = (SnowFlake*) malloc(sizeof(SnowFlake));
    
    if(flake == NULL) { return NULL; }

    flake->x = x;
    flake->y = y;

    flake->dead = false;
    flake->next = NULL;

    return flake;
}

// Create and add a new snow flake to the node list
void generateSnowFlake(SnowFlake** head, int x, int y)
{
	// if list is empty
    if((*head) == NULL) { (*head) = createSnowFlake(x, y); return; } 

	// otherwise find the last node and append the snowflake
    SnowFlake* node = (*head);

    while(node != NULL)
    {
        if(node->next == NULL)
        {
            node->next = createSnowFlake(x, y); return;
        }
         
        node = node->next;
    }
}

// update a snow flake
void updateSnowFlake(SnowFlake* flake)
{
	if(flake == NULL) { return; }

	flake->y += .2f; // move it down

	if(flake->y >= scrGetHeight()) { flake->dead = true; }
}

// draw a snowflake
void drawSnowFlake(SnowFlake* flake)
{
	if(flake == NULL) { return; }

	mvprintw(flake->y, flake->x, "*");
}

// update snow flake list
void updateSnowFlakeList(SnowFlake** head)
{
	SnowFlake* node 	= (*head);
	SnowFlake* nodeLast = NULL;
        
	while(node != NULL)
    {
		updateSnowFlake(node);
		drawSnowFlake(node);

		// delete flake if dead
		if(node->dead)
		{
			SnowFlake* deadFlake = node;

			if(nodeLast == NULL) // head of list
			{
				(*head) = (*head)->next;

				updateSnowFlake(node);
				drawSnowFlake(node);
			}
			else { nodeLast->next = node->next; }	
			
			free(deadFlake);
		}
		
		nodeLast	= node;
		node		= node->next;
	}
}

// free generated snow flakes
void freeSnowFlakes(SnowFlake** head)
{
	SnowFlake* node = (*head);

	while(node != NULL)
	{
		SnowFlake* deadFlake = node;

		node = node->next;

		free(deadFlake);
	}
}

// entry point
int main(int argc, char** args)
{
  	srand(time(NULL)); // random seed

	// start and configure ncurses
    initscr();

    nodelay(stdscr, 1);
    curs_set(0);

	// snow flake list 
    SnowFlake* flakeHead = NULL;
    
    int c = 0;	// input character

	// program loop
	while((c = getch()) != 'q')
    {
        clear();
      
		// 40% chance of spawning a new snowflake  
        if(randr(0, 100) <= 40) { generateSnowFlake(&flakeHead, randr(0, scrGetWidth()), 0); }
       
		// update snow flake list
		updateSnowFlakeList(&flakeHead);

		refresh();
        usleep(70 * 1000);
    }
	
	// free snow flakes and exit ncurses
	freeSnowFlakes(&flakeHead);

    curs_set(1);
    endwin();
    
    return 0;
}
