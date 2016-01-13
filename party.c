#include <ncurses.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#define num_of_bottles 10
#define num_of_guests 10
pthread_mutex_t refr;
int visit[num_of_guests][2];
bool ownerdo = 0;
int nbotle_refr = num_of_bottles;
void *guestFunc(void* i)
{
    int arg = * (int*) i;
	while(1)
	{
		visit[arg][1]=1;
        pthread_mutex_lock(&refr);
		visit[arg][1]=2;
		napms(5000);
		if(ownerdo==0)
		pthread_mutex_unlock(&refr);
		visit[arg][1]=3;
		napms(10000);
 		--nbotle_refr;
		++visit[arg][2];
		if(visit[arg][2]==10) {visit[arg][1]=4; napms(120000);}
	}
}

void *ownerFunc()
{
	pthread_mutex_init(&refr, NULL);
	srand(time(NULL));
	while(1)
	{
		if(rand()%50 +1 == 1)
		{
			ownerdo=1;
			pthread_mutex_init(&refr, NULL);
			pthread_mutex_lock(&refr);
			napms(30000);
			pthread_mutex_unlock(&refr);
			nbotle_refr+=3;
			ownerdo=0;
		}
		napms(1000);
	}
}

int h;

int main()
{

    pthread_t owner;
    pthread_t guests[num_of_guests];
	initscr();
	printw("\n");
	pthread_create(&owner, NULL, ownerFunc, NULL);
	int j;
	for (j = 0; j < num_of_guests; ++j)
    {
        pthread_create(&guests[j], NULL, guestFunc, &j);
        visit[j][2] = 0;
    }
	while(1)
	{
		for(h = 0; h < num_of_guests; ++h)
		{
			printw("\n");
			printw("	%d guest   ", h);
			printw("drank %d bottles. Now he ", visit[h][2]);
			if(visit[h][1]==1) printw("stands in line to refrigerator.");
			if(visit[h][1]==2) printw("takes a bottle of beer.");
			if(visit[h][1]==3) printw("drinks beer.");
			if(visit[h][1]==4) printw("sleeps.");
			printw("\n\n\n");
		}
		if(ownerdo==1)   printw("	Owner finds the bottle and puts %d new bottle\n	*Visitor can not use the fridge\n\n", num_of_guests-1);
		if(ownerdo==0)   printw("	Owner is smoking some Nakhla\n\n");
		printw("        Bottles in the fridge = %d\n", nbotle_refr );
		refresh();
		napms(1000);
		clear();
		if (nbotle_refr<=0) { printw("The bear ran out. GAME OVER"); refresh();  napms(60000); endwin();}

	}
	return 0;
}




