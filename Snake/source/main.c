///////////////////////////////////////
//           SDMC example            //
///////////////////////////////////////

#include <stdbool.h>
#include <3ds.h>
<<<<<<< HEAD

=======
>>>>>>> 9c006023db5caf101bf85fef6c159884e5bad02c
#define MAX_ROWS 240
#define MAX_COLS_UP 400
#define MAX_COLS_DOWN 320

u8* renderEffect(bool isit)
{
	if(isit)
		return gfxGetFramebuffer(GFX_TOP, GFX_LEFT, NULL, NULL);
	else
		return gfxGetFramebuffer(GFX_BOTTOM, GFX_BOTTOM, NULL, NULL);
}
void OnePixel(u8* pic, int x, int y, int b, int g, int r){
	u32 v=(x+y*240)*3;
	pic[v]   =b;
	pic[v+1] =g;
	pic[v+2] =r;
}
void MultiPixel(u8* pic, int x, int y, int b, int g, int r, int zom, bool up){
	int i = 0;
	do{
		OnePixel(pic, x, y+i, b, g, r);
		OnePixel(pic, x, y-i, b, g, r);
		++i;
		if((y+i)>400 && up)break;
		if((y+i)>320 && !up)break;
		if((y-i)<0)break;
	}while(i<zom);
}
void changePointers(u8* gfx, u8* news, bool issmall){
	int big = 400;
	if(issmall)
		big = 320;
	int i, j;
	for(i=1;i<big;i++)
	{
		for(j=1;j<240;j++)
		{
			u32 v=(j+i*240)*3;
			gfx[v]=news[v];
			gfx[v+1]=news[v+1];
			gfx[v+2]=news[v+2];
		}
	}
}

int main(int argc, char** argv)
{
	int r,g,b, row,col, zoom;
	b=r=g=row=col=zoom=0;

	//initialize the services we're going to be using
	srvInit(); //needed for everything
	aptInit(); //needed for everything
	hidInit(NULL); //needed for input
	gfxInit(); //makes displaying to screen easier

	u8* top_screen = (u8 *)linearAlloc( 288780 );
	u8* bottom_screen = (u8 *)linearAlloc( 231120 );

	int i;
	for(i=0;i<288780;i++)
		top_screen[i] = 0;
	for(i=0;i<231120;i++)
		bottom_screen[i] = 0;
	bool is_top = true;
	while(aptMainLoop())
	{
		//exit when user hits B
		hidScanInput();
		if(keysDown()&KEY_START)break;

		//D-PAD to rotate object
		if(keysHeld()&KEY_UP)row++;
		if(keysHeld()&KEY_DOWN)row--;
		if(keysHeld()&KEY_RIGHT)col++;
		if(keysHeld()&KEY_LEFT)col--;

		if(keysHeld()&KEY_X)b++;
		if(keysHeld()&KEY_Y)r++;
		if(keysHeld()&KEY_A)g++;
		if(keysHeld()&KEY_B)b=r=g=0;

		if(keysHeld()&KEY_R)zoom++;
		if(keysHeld()&KEY_L)zoom--;
		if(zoom<0)zoom=0;
		if(zoom>10)zoom=10;
		if(r > 255) r = 0;
		if(g > 255) g = 0;
		if(b > 255) b = 0;

		if(!is_top && col > 320)
			col = 0;
		else if(col > 400)
			col=0;
		else if(!is_top && col < 0)
			col=320;
		else if(col < 0)
			col=0;

		if(row > 240){
			row = 0;
			if(!is_top){
				col += 40;
				is_top = true;
			}else if(is_top && col>40 && col<360){
				col -= 40;
				is_top = false;
			}
		}
		else if(row < 0){
			row = 240;
			if(!is_top){
				col += 40;
				is_top = true;
			}else if(is_top && col>40 && col<360){
				col -= 40;
				is_top = false;
			}
		}

		if(is_top){
			MultiPixel(top_screen, row, col, b, g, r, zoom, is_top);
			changePointers(renderEffect(true), top_screen, false);
		}
		else{
			MultiPixel(bottom_screen, row, col, b, g, r, zoom, is_top);
			changePointers(renderEffect(false), bottom_screen, true);
		}

		//wait & swap
		gfxSwapBuffersGpu();
		gspWaitForEvent(GSPEVENT_VBlank0, false);
	}

	exit:
	linearFree(top_screen);
	linearFree(bottom_screen);
	//cleanup and return
	//returning from main() returns to hbmenu when run under ninjhax
	//closing all handles is super important
	//closing all services even more so
	gfxExit();

	hidExit();
	aptExit();
	srvExit();

	return 0;
}
