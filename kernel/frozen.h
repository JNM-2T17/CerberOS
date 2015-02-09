#define ANNA 0
#define ELSA 1
#define LET_IT_GO_CTR 55
#define SNOWMAN_CTR 11

typedef char songline[55];
songline letItGo[LET_IT_GO_CTR] = {"The snow blows white on the mount tonight\n",
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
songline snowMan[SNOWMAN_CTR] = {"*knock**knock**knock**knock**knock**knock*\n",
				"Do you wanna build a snowman\n",
				"Come on let's go and play\n",
				"I never see you anymore\n",
				"Come out the door\n",
				"It's like you've gone awaaaaaay\n",
				"We used to be best buddies\n",
				"And now we're not\n",
				"I wish you would tell me whyyyyy\n",
				"Do you want to build a snowman\n",
				"I doueshnt have ter ber a snermern\n"};
				
unsigned char isAnnaSinging = 1;
unsigned char isElsaSinging = 1;
unsigned char singer = ANNA;
unsigned int timerCtr = 0;
unsigned int elsaCtr = 0;
unsigned int annaCtr = 0;