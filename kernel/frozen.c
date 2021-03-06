#include "function.h"

#define ANNA 0 /*singer index for Anna*/
#define ELSA 1 /*singer index for Elsa*/
#define LET_IT_GO_CTR 55 /*lines in Let It Go*/
#define SNOWMAN_CTR 29 /*lines in Do You Want to Build a Snowman*/

typedef char songline[55];
songline letItGo[LET_IT_GO_CTR] = {
				"The snow blows white on the mount tonight\n",
				"Not a footprint to be seen\n",
				"A kingdom of isolation\n",
				"And it looks like I'm the queen\n",
				"The wind is howling like this swirling storm inside\n",
				"Couldn't keep it in\n",
				"Heaven knows I've tried\n",
				"Don't let them in\n",
				"Don't let them see\n",
				"Be the good girl you always have to be\n",
				"Conceal don't feel\n",
				"Don't let them know\n",
				"Well now they knoooooow\n",
				"Let it go\n",
				"Let it go\n",
				"Can't hold it back anymore\n",
				"Let it go\n",
				"Let it go\n",
				"Turn away and the slam the door\n",
				"I don't care what they're going to say\n",
				"Let the storm rage on\n",
				"The cold never bothered me anyway\n",
				"It's funny how some distance\n",
				"Makes everything seem small\n",
				"And the fears that once controlled me\n",
				"Can't get to me at aaaaaaaall\n",
				"It's time to see what I can do\n",
				"To test my limits and break through\n",
				"No right, no wrong\n",
				"No rules for me\n",
				"I'm free\n",
				"Let it go\n",
				"Let it go\n",
				"I am one with the wind and sky\n",
				"Let it go\n",
				"Let it go\n",
				"They'll never see me cry\n",
				"Here I stand\n",
				"And here I'll stay\n",
				"Let the storm rage on\n",
				"My power flurries through the air into the ground\n",
				"My soul is spiraling in frozen fractals all around\n",
				"And one thought crystallizes like an icy blasy\n",
				"I'm never going back\n",
				"The past is in the past\n",
				"Let it go\n",
				"Let it go\n",
				"I will rise the break of dawn\n",
				"Let it go\n",
				"Let it go\n",
				"That perfect girl is gone\n",
				"Here I stand\n",
				"In the light of day\n",
				"Let the storm rage on\n",
				"The cold never bothered me anyway\n"
				};
songline snowMan[SNOWMAN_CTR] = {
				"*knock**knock**knock**knock**knock**knock*\n",
				"Do you want to build a snowman\n",
				"Come on let's go and play\n",
				"I never see you anymore\n",
				"Come out the door\n",
				"It's like you've gone awaaaaaay\n",
				"We used to be best buddies\n",
				"And now we're not\n",
				"I wish you would tell me whyyyyy\n",
				"Do you want to build a snowman\n",
				"I doueshnt have ter ber a snermern\n",
				"Do you want to build a snowman\n",
				"Or ride a bike around the halls\n",
				"I think some company is overdue\n",
				"I've started talking to the pictures on the walls\n",
				"<hang in there, Joan>\n",
				"It gets a little lonely\n",
				"All these empty rooms\n",
				"Just watching the hours tick by\n",
				"tick tock tick tock\n",
				"Elsa, I know you're in there\n",
				"Poeple have been asking where you've been\n",
				"They say stay strong and I'm trying to\n",
				"I'm right out here for you\n",
				"Just let me in\n",
				"We only have each other\n",
				"Just you and me\n",
				"What are we gonna do\n",
				"Do you want to build a snowman\n"
				};
				
unsigned char isAnnaSinging = 0; /*whether Anna is singing*/
unsigned char isElsaSinging = 0; /*whether Elsa is singing*/
unsigned char singer = ANNA; /*initialize singer to Anna*/
unsigned int elsaCtr = 0; /*Elsa's song line*/
unsigned int annaCtr = 0; /*Anna's song line*/

extern unsigned int timerCtr; /*timer ticks*/
extern process *console; /*console process*/

extern void clrscr( process * ); /*clear screen interrupt*/

/***
	tells a singer to go away
	Parameters:
		args - who to shoo away
***/
void goAway( char *args ) {
	
	if( !cmpIgnoreCase( args, "anna" ) ) { /*if argument is anna*/
		if( isAnnaSinging ) { /*if anna is singing*/
			printStr( console, "\n\n\nokay bye.....\n\n\n");
			isAnnaSinging = 0; /*stop anna*/
		} else { /*if anna is being unjustly exiled*/
			printStr( console, "\n\n\nAnna: I'm not even here!\n\n");
		}
	} else if( !cmpIgnoreCase( args, "elsa" ) ) { /*if argument is elsa*/
		if( isElsaSinging ) { /*if elsa is singing*/
			printStr( console, "\n\n\nThe cold never bothered me anyway.....\n\n\n");
			isElsaSinging = 0; /*stop elsa, I mean, the movie is 1 year old
								c'mon*/
		} else { /*if elsa is being unjustly exiled*/
			printStr( console, "\n\n\nElsa: I'm in my mountain, damn it!\n\n");
		}
	}	
}

/***
	tells a singer to singer
	Parameters:
		args - who to call
***/
void callSinger( char *args ) {
	
	/*if argument is anna*/
	if( !cmpIgnoreCase( args, "anna" ) ) {
		if( isAnnaSinging ) { /*if anna is already singing*/
			printStr( console, "\n\n\nAnna: I'm already singing!\n\n" );
		} else { /*if not yet singing*/
			isAnnaSinging = 1; /*set Anna*/
			annaCtr = 0; /*reset counter*/
			clrscr( console ); /*clear screen*/
		}
	/*if argument is elsa*/
	} else if( !cmpIgnoreCase( args, "elsa" ) ) {
		if( isElsaSinging ) { /*if Elsa is already singing*/
			printStr( console, "\n\n\nElsa: I'm already singing!\n\n" );
		} else {
			isElsaSinging = 1; /*set Elsa*/
			elsaCtr = 0; /*reset counter*/
			clrscr( console ); /*clear screen*/
		}
	}
}

/***
	processes a frozen song
***/
void frozenSong() {

	/*every 20 ticks*/
	if( timerCtr % ( 20 / ( isAnnaSinging + isElsaSinging + 1 ) ) == 0 ) {
		if( singer == ANNA && isAnnaSinging) { /*if singer is Anna and Anna is 
												 activated*/
			printStr( console, snowMan[annaCtr % SNOWMAN_CTR]); /*print song line*/
			annaCtr++; /*next line*/
		} else if( singer == ELSA && isElsaSinging ) { /*if singer is Elsa and 
														 Elsa is activated*/
			printStr( console, letItGo[elsaCtr % LET_IT_GO_CTR]); /*print song line*/
			elsaCtr++; /*next line*/
		}
		singer = 1 - singer; /*toggle singer*/
	}
}
