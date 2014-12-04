/*
 * FontStruct.h
 *
 *  Created on: 4/12/2014
 *      Author: saq
 */

#ifndef FONTSTRUCT_H_
#define FONTSTRUCT_H_
#define MAX_HIGHT 6
#define MAX_WIDTH 5
#define MAX_COLS  320
#define MAX_ROWS  240
//int letters_size = {4,4,4,4,4,4,4,4,1,3,4,1,5,4,4,4,4,4,4,4,4,4,4,4,4};

struct FontStruct{
	u8* bottom_screen;
	int col, row;
};
void FontStruct_constructor(struct FontStruct *pon){
	pon->bottom_screen = (u8 *)linearAlloc( 77080 );
	int i;
	for(i=0;i<77080;i++)
		pon->bottom_screen[i] = 255;
	pon->col =1; pon->row = MAX_ROWS-1;
}
void FontStruct_add_letter(struct FontStruct *pon, char letter){
	if((pon->col + MAX_WIDTH+2) > MAX_COLS) return;
	if((pon->row - MAX_HIGHT-2) < MAX_ROWS) return;
	u32 v=(x+y*240)*3;
	pic[v]   =b;
	pic[v+1] =g;
	pic[v+2] =r;
	switch(letter){
		case 'a':
			pon->bottom_screen
			break;
		case 'b':

			break;
		case 'c':

			break;
		case 'd':

			break;
		case 'e':

			break;
		case 'f':

			break;
		case 'g':

			break;
		case 'h':

			break;
		case 'i':

			break;
		case 'j':

			break;
		case 'k':

			break;
		case 'l':
			break;
		case 'm':

			break;
		case 'n':

			break;
		case 'o':

			break;
		case 'p':

			break;
		case 'q':

			break;
		case 'r':

			break;
		case 's':

			break;
		case 't':
			break;
		case 'u':

			break;
		case 'v':

			break;
		case 'w':

			break;
		case 'x':

			break;
		case 'y':

			break;
		case 'z':

			break;
	}
}
void FontStruct_deconstructor(struct FontStruct *pon){
	linearFree(pon->bottom_screen);
}
#endif /* FONTSTRUCT_H_ */
