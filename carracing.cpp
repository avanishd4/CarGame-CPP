//       OOP Project
//       Console Based Car Racing Game
//       Team members- Ankur Rai  19/11/EC/051
//                  - Avanish Dubey 19/11/EC/055
//                  - Yashdeep Patwa 19/11/EC/056


#include "windows.h"
#include <iostream>
#include<conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string>
#include <fstream>
#include <ios>
#include <cstdlib>
#define KEY_LEFT 75
#define KEY_RIGHT 77
#define FOREGROUND_WHITE (FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE)
using namespace std;

//Defining and Inizilizing Var for Track
int Xlngt = 12;                              		    //    road width
int left_track_x = 44;                        	      	//    road left x coord
int right_track_x = 57;                      	        //    road right x coord

//Dificulty changing Variables
int Difi = 1;                                      	    //    number of oncoming cars
int DifiLevl;                                    
char Dificlty;
char ChrPlyr[]="Player1";

char chrGetKey;

//Declaring Array for Road track metrix 
int ArryDraw[12][26][2];      							// 0 for enemy , 1 for player

int Incremt = 0;                       					// move enemy car forward
int bol_game_status = 1;             	 				// game status 1 or 0
int XcordDrw = 0;                      					// player car
int XcordComptrCar=0;                   				// computer car
int XcordComptrCar1=0;                 					// computer car1
int XcordComptrCar2=0;                 					// computer car2
int score = 1;		
int level = 1;
int delay = 85;

//Declaring Functions
DWORD WINAPI ThreadFn(LPVOID vpParam);

void gotoxy(int x, int y);
void Intro();
int Exit();
void LogHighScore(char name[], int score);
void readScore();


// Class for changing Colors
class TextAttr {
	friend ostream& operator<<(ostream& out, TextAttr attr)
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), attr.value);
		return out;
	}
public:
	explicit TextAttr(WORD attributes) : value(attributes) {}
private:
	WORD value;
};


//printing Intro Menu
void Intro() {

	system("cls");

	gotoxy(40, 3);
	cout << TextAttr(FOREGROUND_GREEN) << "Console Based Car Racing Game";
	gotoxy(40, 4);
	cout << TextAttr(FOREGROUND_WHITE);
	cout << "Enter Player's name : ";
	cin >> ChrPlyr;

	gotoxy(40, 6);
	cout << "____________________________________________ ";
    gotoxy(40, 7);
	cout << TextAttr(FOREGROUND_BLUE)<<ChrPlyr<< TextAttr(FOREGROUND_RED) << "   Select Difficulty Level ";
	cout << TextAttr(FOREGROUND_WHITE);

    gotoxy(40, 8);
    cout << "_____________________________________________ " ;
	gotoxy(40, 9);
	cout << TextAttr(FOREGROUND_BLUE);
    cout << "       Easy        (1) ";
	gotoxy(40, 10);
	cout << TextAttr(FOREGROUND_WHITE);
    cout << "       Medium      (2)";
	gotoxy(40,11);
	cout << TextAttr(FOREGROUND_RED);
    cout<< "       Hard        (3) ";
	gotoxy(40, 12);
	cout << TextAttr(FOREGROUND_WHITE);
	cout << "___________________________________________ ";
	gotoxy(40, 14);

    int intInput;
	cout << TextAttr(FOREGROUND_WHITE)<<"Enter your choice :";
	cin >> intInput;
	DifiLevl = intInput;

	if (intInput==1) {
		delay = 85;
		Dificlty = 'e';
	}
	else if (intInput == 2) {
		delay = 68;
		Dificlty = 'm';

	}
	else if (intInput == 3) {
		delay = 37;
		Dificlty = 'h';
	}
	else  {
		gotoxy(40, 15);
		cout << " Invalid Input: Set to default dificulty (Easy) .....";
		system("cls");
		Dificlty = 'e';
	}

}

void gotoxy(int x, int y)
{
	COORD coord;
	coord.X = x;
	coord.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}


class GameTemplate{
	public: 
	// rest everything by nulling the Array
	void resetb() 
	{
		for (int j = 0; j<26; j++) {
			for (int i = 0; i<Xlngt; i++) {

				ArryDraw[i][j][0] = 0;
				ArryDraw[i][j][1] = 0;
			}
		}
	}
	// display score section 
	void score_section()
	{
    	gotoxy(65, 4);
		cout << TextAttr(FOREGROUND_WHITE) << "SCORE:" << score;
		gotoxy(65, 5);
		cout << TextAttr(FOREGROUND_WHITE)  << "LEVEL:" << level;
	}
	void drawp(int x, int y, int Cr) {  

		if (x >= 0 && x<Xlngt && y >= 0 && y<26) {
			ArryDraw[x][y][Cr] = 1;
		}
	}
	void drawCar(int x, int y, int Cr)  //
	{
		drawp(1 + x, 0 + y, Cr);
		drawp(0 + x, 1 + y, Cr);
		drawp(1 + x, 1 + y, Cr);
		drawp(2 + x, 1 + y, Cr);
		drawp(1 + x, 2 + y, Cr);
		drawp(0 + x, 3 + y, Cr);
		drawp(1 + x, 3 + y, Cr);
		drawp(2 + x, 3 + y, Cr);

	}	
	 // intialise cars
	void init_cars()
	{
    	drawCar(XcordComptrCar, Incremt, 0);            // Prints the first single car as default on the console.

		if (Difi==1)
        {
			drawCar(XcordComptrCar1, Incremt, 0);
		}
		else if (Difi == 2)                      // Prints 2 or 3 enemy car on the console.
        {
			drawCar(XcordComptrCar1, Incremt, 0);
			drawCar(XcordComptrCar2, Incremt, 0);
		}
		
		drawCar(XcordDrw, 22, 1);
	}
	// Left and right track line
	void track_left_right(int j)
	{
    	//Printing Left track line
    	for(int i=40; i<=left_track_x;i++)
   		 {
       		gotoxy(i, j + 1);
        	cout << TextAttr(FOREGROUND_GREEN) << "±";
    	}

    	//Drawing the Right track line
    	for(int i=right_track_x; i<=(right_track_x+4);i++)
    	{
        	gotoxy(i, j + 1);
        	cout << TextAttr(FOREGROUND_GREEN) << "±";
    	}
	}
	// drawing cars and tracks
	void draw_game()
	{
   		for (int j = 0; j<26; j++) {

            track_left_right(j);                                                //Drawing Left and right track line
			cout << TextAttr(FOREGROUND_WHITE);

			//drawing the Cars
			gotoxy(45, j + 1);
			for (int i = 0; i<Xlngt; i++)
            {
            	// enemy car
				if (ArryDraw[i][j][0] == 1) {
					cout << TextAttr(FOREGROUND_BLUE) << "*";
					cout << TextAttr(FOREGROUND_WHITE);
				}
				// players car
				else if (ArryDraw[i][j][1] == 1) {
					cout << TextAttr(FOREGROUND_RED) << "+";
					cout << TextAttr(FOREGROUND_WHITE);
				}
				// car track
				else {
					cout << " ";
				}
			}
			cout << TextAttr(FOREGROUND_WHITE);

			//Checking whether  Players Car is Crashed with oncoming Computer Controlled Cars
			if (Incremt >= 22 && (XcordDrw == XcordComptrCar || XcordDrw == XcordComptrCar1))
            {
				bol_game_status = 0;
			}
		}
	}
	
};

class Game{
	public: 
	void instructions(){
	gotoxy(44, 2);

	cout << TextAttr(FOREGROUND_GREEN) << "Console Based Car Racing Game";
	gotoxy(44, 4);


	cout << TextAttr(FOREGROUND_WHITE) << "CONTROLS";
	gotoxy(44, 5);
	cout << "========";
	gotoxy(44, 6);
	cout << "[A]=LEFT  or ARROW LEFT  [<-]";
	gotoxy(44, 7);
	cout << "[D]=RIGHT or ARROW RIGHT [->]";


	for (int i = 2,j=0; i >= 0; i--,j++) {  // Timer before game started

		gotoxy(44, 8);
		cout << i;
		gotoxy(45+j, 8);
		cout << "*";
		gotoxy(51, 15);
		Sleep(1000);

	}

	system("cls"); // clean background
	}
	// Level Changing
	void changeLevel(){
	if (score % 5 == 0 && (Incremt == 25 || Incremt == -4)) {
		level++;

		//changing the speeeds of PC controlled Cars
		if (DifiLevl==1) {
			delay = delay - 10;
		}
		else if (DifiLevl == 2) {
			delay = delay - 5;
		}
		else if (DifiLevl == 3) {
			delay = delay - 5;
		}
		
		}
	}
	int getCar() //   this return random position of enemies car
	{
		int arr[4] = {0,3,6,9};
		return arr[rand() % 4];
	}

	void game_over(){
		if (delay>0)
    	{
		gotoxy(59, 4);
		cout << TextAttr(FOREGROUND_RED) << "GAME OVER" << endl;
		gotoxy(59, 5);
		cout << TextAttr(FOREGROUND_WHITE);
		cout << "TOP SCORE:" << TextAttr(FOREGROUND_RED) << score;
		gotoxy(59, 6);
		cout << TextAttr(FOREGROUND_WHITE) << "LEVEL:" << level;

		Exit();
		
		}
	}
	
	void winning_game(){
	if (delay<0)
        {
			gotoxy(59, 4);
			cout << TextAttr(FOREGROUND_BLUE) << "YOU WIN!" << endl;
			gotoxy(59, 5);
			cout << TextAttr(FOREGROUND_WHITE);
			cout << "SCORE:" << TextAttr(FOREGROUND_RED) << score;
			gotoxy(59, 6);
			cout << TextAttr(FOREGROUND_WHITE) << "LEVEL:" << "12";

			bol_game_status = 0;

			Exit();
		}
	}
	void track_decision(){
		if (Incremt == 25) {
			Incremt = -4;
			XcordComptrCar = getCar();

			int RndCord = XcordComptrCar;

			while (XcordComptrCar == RndCord) {
				//srand(0);

				RndCord = rand() % 4;
				RndCord = RndCord * 3;
			}
			XcordComptrCar1 = RndCord;

			int RndCord1 = XcordComptrCar1;

			while(XcordComptrCar1 == RndCord1) {
				//srand(0);

				RndCord1 = rand() % 4;
				RndCord1 = RndCord1 * 3;
			}
			XcordComptrCar2 = RndCord1;

			score++;
		}
	}
	
};


int main()
{
	Game game;
	GameTemplate game_template;
	
	Intro();                                                            // Introduction
	system("cls");    
	game.instructions();                
	
	//Calling Tread
	DWORD qThreadID;
	HANDLE hThread = CreateThread(0, 0, ThreadFn, 0, 0, &qThreadID);    // Thread

																		
	while (bol_game_status)                                             //Game started
    {       
		game_template.score_section();
		game_template.resetb();
		game_template.init_cars();
		game_template.draw_game();                                      

 		Incremt++;                                                      // moving enemies car

		game.track_decision();                                               // Scoring and designating tracks for computer controlled Cars

		game.changeLevel();

		game.winning_game();                                                	// Winning event and displaying winners  scores

		Sleep(delay);     	                                            //  Delay of While Loop

		if (score == 5)                                                 //designating no of oncoming cars
        {  
			Difi = 2;
		}

	}	//end while

	LogHighScore(ChrPlyr,score);						                //Saving scores into text file

	game.game_over();                                                       // Game Over Event
	
	CloseHandle(hThread);                                            //Closing the thread
	_getch();

	return 0;
}



DWORD WINAPI ThreadFn(LPVOID vpParam)  // Thread used to move player car on the terminal
{
	/// Moving the player's Car
		int rigtCr = 3;
		int leftCr = 0;
		
		while(1){
			char chr = _getch();
			
			if(Xlngt == 12){
				
				if(chr == KEY_LEFT){
					if( XcordDrw == 3){
						XcordDrw = 0;
					}else if( XcordDrw == 6){
						XcordDrw = 3;
					}else if( XcordDrw == 9){
						XcordDrw = 6;
					}
				}else if(chr == KEY_RIGHT){
					if( XcordDrw == 0){
						XcordDrw = 3;
					}else if( XcordDrw == 3){
						XcordDrw = 6;
					}else if(XcordDrw == 6){
						XcordDrw = 9;
					}
				}else if(chr == 'a' || chr == 'A'){
					if( XcordDrw == 3){
						XcordDrw = 0;
					}else if( XcordDrw == 6){
						XcordDrw = 3;
					}else if( XcordDrw == 9){
						XcordDrw = 6;
					}
				}else if( chr == 'd' || chr == 'D'){
					if( XcordDrw == 0){
						XcordDrw = 3;
					}else if( XcordDrw == 3){
						XcordDrw = 6;
					}else if(XcordDrw == 6){
						XcordDrw = 9;
					}
				}
			}
			
		}
	return 0;
}


//Saving High Scores to TXT file
void LogHighScore(char name[],int scoret) {

	try {
		std::ofstream log("Scorelogfile.txt", std::ios_base::app | std::ios_base::out);
		if (Dificlty == 'h')
        {
			log << "Player " << name << "  Top Score :" << scoret << "   status :  | Hard |\n";
		}
		else if(Dificlty == 'e')
        {
            log << "Player " << name << "  Top Score :" << scoret << "   status :  | Easy |\n";
		}
		else if (Dificlty == 'm')
        {
            log << "Player " << name << "  Top Score :" << scoret << "   status :  | Medium |\n";
		}

	}
	catch (exception& e)
	{
		cout << e.what() << '\n';
	}
}

//Reading and displaying data in TXT FILE of HIGH SCORES
void readScore() {

	gotoxy(44, 3);
	cout << TextAttr(FOREGROUND_RED)<< "------------------ScoreBoard--------------------";
	gotoxy(44, 5);
	cout << TextAttr(FOREGROUND_WHITE) << "_______________________________________________";
	string STRING;
	ifstream infile;
	int lineY=6;
	infile.open("Scorelogfile.txt");
	while (!(infile.eof())) 
	{
		getline(infile, STRING);

		gotoxy(44, lineY);
		cout << STRING<<endl; 

		lineY++;
	}
	infile.close();
	gotoxy(44 , lineY + 1);
	for (int i = 0; i <8; i++) {  

		cout << "##";
		Sleep(500);
		gotoxy(44 + i, lineY + 1);
	}
	system("cls");
}

// Displaying High Scores and Exit Sequence
int Exit() {
	system("cls");
	gotoxy(44, 3);
	readScore();

	exit(0);
}
