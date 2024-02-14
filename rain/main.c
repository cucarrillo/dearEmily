// about: Dear Emily Rain Program
// author: Cesar Carrillo

// imports
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <ncurses.h>

int SCR_HEIGHT;
int SCR_WIDTH;

// return a random number from range
int randr(int min, int max) { return (rand() % (max - min + 1)) + min; }

// rain drop structure
typedef struct RainDrop
{
    int x;
    int y;

    bool dead;

    struct RainDrop* next;
} RainDrop;

RainDrop* createRainDrop(int x, int y)
{
    RainDrop* drop = (RainDrop*) malloc(sizeof(RainDrop));

    if(drop == NULL) { return NULL; }

    drop->x = x;
    drop->y = y;

    drop->dead = false;    
    drop->next = NULL;

    return drop;
}

void updateRainDrop(RainDrop* drop)
{
    if(drop == NULL)    { return; }
    if(drop->dead)      { return; }

    drop->y++;

    if(drop->y >= SCR_HEIGHT) { drop->dead = true; }
}

void drawRainDrop(RainDrop* drop)
{
    if(drop == NULL)    { return; }
    if(drop->dead)      { return; }

    mvprintw(drop->y, drop->x, "|");
}

const char* SPLASH_FRAMES[] = {     " . ",
                                    " o ",
                                    "-^-",
                                    "/-\\",
                                    "-.-",
                                    "___",
                                    " _ "   };


typedef struct RainSplash
{
    int x;
    int y;
    int frame;
    bool dead;
    struct RainSpash* next;
} RainSplash;

RainSplash* createRainSplash(int x, int y)
{
    RainSplash* splash = (RainSplash*) malloc(sizeof(RainSplash));

    if(splash == NULL) { return NULL; }    
    
    splash->x = x;
    splash->y = y;

    splash->frame = 0;

    splash->dead = false;
    splash->next = NULL;

    return splash;
}

void drawRainSplash(RainSplash* splash)
{
    if(splash == NULL)  { return; }
    if(splash->dead)    { return; }

    mvprintw(splash->y, splash->x - 1, SPLASH_FRAMES[splash->frame]);
}

void updateRainSplash(RainSplash* splash)
{
    if(splash == NULL)  { return; }
    if(splash->dead)    { return; } 
    
    splash->frame++;

    if(splash->frame > 6) { splash->dead = true; }
}

void generateRainSplash(RainSplash** head, int x, int y)
{
	if((*head) == NULL) { (*head) = createRainSplash(x, y); return; }

    RainSplash* node = (*head);

    while(node != NULL)
    {
        if(node->next == NULL)
        {
            node->next = createRainSplash(x, y); return;
        }
        
        node = node->next;
    }
}


void generateRainDrop(RainDrop** head)
{
	if((*head) == NULL) 
	{ (*head) = createRainDrop(randr(0, SCR_WIDTH), 0); return; }

    RainDrop* node = (*head);

    while(node != NULL)
    {
        if(node->next == NULL)
        {
            node->next = createRainDrop(randr(0, SCR_WIDTH), 0); return;
        }
        
        node = node->next;
    }
}

int main(int argc, char** args)
{
    srand(time(NULL));

    initscr();

    getmaxyx(stdscr, SCR_HEIGHT, SCR_WIDTH);
    nodelay(stdscr, 1);
    curs_set(0);
    
    RainDrop*   dropHead    = NULL;
    RainSplash* splashHead  = NULL;
    
    int c = 0;	// input character

	while((c=getch())!='q')
    {
        clear();

        getmaxyx(stdscr, SCR_HEIGHT, SCR_WIDTH);
        
        RainDrop* dropNode = dropHead;
		RainDrop* dropNodeLast = NULL;
    
        if(randr(0, 100) >= 30) { generateRainDrop(&dropHead); }
        
        while(dropNode != NULL)
        {
            if(!dropNode->dead)
            {
                updateRainDrop(dropNode);
                drawRainDrop(dropNode);
                
                if(dropNode->dead)
                {
                    generateRainSplash(&splashHead, dropNode->x, dropNode->y - 1);

					RainDrop* deadDrop = dropNode;

					if(dropNodeLast == NULL)
					{
						dropHead = dropNode->next;

						updateRainDrop(dropHead);
						drawRainDrop(dropHead);
					}
					else { dropNodeLast->next = dropNode->next; }
					
					free(deadDrop);
                }

            }

			dropNodeLast = dropNode;
            dropNode = dropNode->next;
        }

        RainSplash* splashNode = splashHead;
		RainSplash* splashNodeLast = NULL;

        while(splashNode != NULL)
        {
            if(!splashNode->dead)
            {
                updateRainSplash(splashNode);
                drawRainSplash(splashNode);

				if(splashNode->dead)
				{
				RainSplash* deadSplash = splashNode;

				if(splashNodeLast == NULL)
				{
					splashHead = splashNode->next;

					updateRainSplash(splashNode);
					drawRainSplash(splashNode);
				}
				else
				{
					splashNodeLast->next = splashNode->next;
				}

				free(deadSplash);
				}
            }

			splashNodeLast = splashNode;
            splashNode = splashNode->next;
        }   

        refresh();
        usleep(70 * 1000);
    }

	while(splashHead != NULL)
	{
		RainSplash* deadSplash = splashHead;
		
		splashHead = splashHead->next;

		free(deadSplash);
	}

	while(dropHead != NULL)
	{
		RainDrop* deadDrop = dropHead;
	
		dropHead = dropHead->next;
		free(deadDrop);
	}

    // todo: free splash and rain nodes

    curs_set(1);
    endwin();
    
    return 0;
}
