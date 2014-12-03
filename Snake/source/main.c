///////////////////////////////////////
//           SDMC example            //
///////////////////////////////////////

#include <stdbool.h>
#include <3ds.h>
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
	int r,g,b, x,y, zoom;
	b=r=g=x=y=zoom=0;

	//initialize the services we're going to be using
	srvInit(); //needed for everything
	aptInit(); //needed for everything
	hidInit(NULL); //needed for input
	gfxInit(); //makes displaying to screen easier

	u8* top_screen = (unsigned char *)malloc( 288780 );
	u8* bottom_screen = (unsigned char *)malloc( 77080 );
	int i;
	for(i=0;i<288720;i++)
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
		if(keysHeld()&KEY_UP)x++;
		if(keysHeld()&KEY_DOWN)x--;
		if(keysHeld()&KEY_RIGHT)y++;
		if(keysHeld()&KEY_LEFT)y--;

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

		if(!is_top && y > 320)
			y = 0;
		else if(y > 400)
			y=0;
		else if(!is_top && y < 0)
			y=320;
		else if(y < 0)
			y=0;

		if(x > 240){
			x = 0;
			if(!is_top){
				y += 40;
				is_top = true;
			}else if(is_top && y>40 && y<360){
				y -= 40;
				is_top = false;
			}
		}
		else if(x < 0){
			x = 240;
			if(!is_top){
				y += 40;
				is_top = true;
			}else if(is_top && y>40 && y<360){
				y -= 40;
				is_top = false;
			}
		}

		if(is_top){
			MultiPixel(top_screen, x, y, b, g, r, zoom, is_top);
			changePointers(renderEffect(true), top_screen, false);
		}
		else{
			MultiPixel(bottom_screen, x, y, b, g, r, zoom, is_top);
			changePointers(renderEffect(false), bottom_screen, true);
		}

		//wait & swap
		gfxSwapBuffersGpu();
		gspWaitForEvent(GSPEVENT_VBlank0, false);
	}

	exit:
      // free(top_screen); crash
      // free(bottom_screen); crash

	//cleanup and return
	//returning from main() returns to hbmenu when run under ninjhax
	//closing all services even more so
	gfxExit();
	hidExit();
	aptExit();
	srvExit();
	return 0;
}
