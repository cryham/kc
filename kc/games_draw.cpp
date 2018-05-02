#include "games.h"
#include "gui.h"
#include "Ada4_ST7735.h"
using namespace std;


//  Block color
const static int8_t rgb[12][3] = {
	{31,15,0},{31,31,0},{15,31,0},{0,31,0},{0,31,15},{0,31,31},
	{0,15,31},{15,10,31},{20,20,31},{31,10,31},{31,0,15},{31,20,20},
};
uint16_t Games::BlkClr(int8_t b, int8_t d, int8_t dim)
{
	int c = b / 10, e = b % 10, ed = e + d*4;
	return RGB(
		dim * min(31, max(0, rgb[c][0]-5) +ed) /4,
		dim * min(31, max(0, rgb[c][1]-5) +ed) /4,
		dim * min(31, max(0, rgb[c][2]-5) +ed) /4);
}

//  Draw block
// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
void Games::Draw(const Block& b,
	int pos_x, int pos_y, int o_y, int dim)
{
	Ada4_ST7735& d = *g->d;
	for (int y=0; y < o.bsize; ++y)
	for (int x=0; x < o.bsize; ++x)
	{
		int yy = (pos_y + y) % o.size_y * dim_y + ofs_y + o_y;
		int xx = (pos_x + x) % o.size_x * dim_x + ofs_x;
		if (b.b[y][x] &&
			yy >= 0 && yy < H)
		{
			d.fillRect(xx, yy, dim_x, dim_y, BlkClr(b.b[y][x], 0, dim));
			d.drawRect(xx, yy, dim_x, dim_y, BlkClr(b.b[y][x], 2, dim));
			d.drawRect(xx+1, yy+1, dim_x-2, dim_y-2, BlkClr(b.b[y][x], 1, dim));
		}
	}
}
void Games::DrawNext(const Block& b,
	int pos_x, int pos_y)
{
	const int8_t dim = 3;  //~
	Ada4_ST7735& d = *g->d;
	for (int y=0; y < o.bsize; ++y)
	for (int x=0; x < o.bsize; ++x)
	{
		int yy = pos_y + y * dim_y;
		int xx = pos_x + x * dim_x;
		//  grid modes .
		if ( o.dots==0 ||
			(o.dots==1 && (x%2 && y%2)) ||
			(o.dots==2 && (x%3==1 && y%3==1)))
			d.drawPixel(xx + dim_x/2, yy + dim_y/2, BlkClr(b.b[y][x], 0, dim));

		if (b.b[y][x] &&
			xx < W && yy < H)
		{
			d.fillRect(xx, yy, dim_x, dim_y, BlkClr(b.b[y][x], 0, dim));
			d.drawRect(xx, yy, dim_x, dim_y, BlkClr(b.b[y][x], 2, dim));
			d.drawRect(xx+1, yy+1, dim_x-2, dim_y-2, BlkClr(b.b[y][x], 1, dim));
		}
	}
}


//  Draw
// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . 
const static char* sPresets[Games::Presets] = {
	"  Tiny 3+", " Basic 4/2", " Small 4+/4", "Medium 5+/4", "Tetris 4-",
	"Pentis 5/1", "Sixtis 6", "Septis 7+/4", "Septis 7+/1",
	" Octis 8+/1", "  Huge 12"};
	
const static char* sOptPages[Games::O_All] = {
	"Field", "Speed", "Block", "Draw", "Input"};

void Games::Draw()
{
	char a[64];
	int x = 0, y = 0;
	Ada4_ST7735& d = *g->d;
	d.setCursor(x,y);
	d.setTextColor(RGB(30,14,10));

	if (gui==1)  //  game menu
	{
		d.print("Sixtis");  d.setFont(0);

		d.setTextColor(RGB(31,24,12));
		d.setCursor(W/2-6, 4);
		d.println(sPresets[preset]);  // title
		
		int yy = 32;
		for (y=0; y < G_All; ++y)
		{
			int c = abs(y - yg);
			d.setTextColor(RGB(30,20,20));

			if (!c)  d.fillRect(0, yy-1, 2*W/3, 10, RGB(9,7,5));
			d.setCursor(2, yy);
			d.print(!c ? "\x10 ":"  ");  // >
			g->FadeClr(Gui::C_Game, 6, c, 1);

			switch (y)
			{	case G_Resume:   d.println("Resume");  yy+=2;  break;
				case G_NewGame:  d.println("New Game");  break;
				case G_Preset:   d.print("Preset: ");
					sprintf(a,"%d / %d", preset+1, Presets);  d.print(a);
					//d.print("   ");  d.println(ssPresets[preset]);  // title
					break;
				case G_Options: d.println("Options");  break;
				case G_Help:    d.println("Help");  break;
			}
			yy += 10;
		}
		return;
	}
	else if (gui==2)  //  options
	{
		d.print("Sixtis");  d.setFont(0);
		
		d.setTextColor(RGB(28,16,22));
		d.setCursor(W-1 -3*6, 0);
		sprintf(a,"%d/%d", opg+1, O_All);  d.print(a);
		
		d.setTextColor(RGB(28,20,23));
		d.setCursor(W/2-6, 4);
		d.print(sOptPages[opg]);  // title

		x = 12;  y = 32;  int l = 0;
		switch (opg)
		{
		case O_Field:
			OptLine(x,y,l,"Width   %d", o.size_x, 8);
			OptLine(x,y,l,"Height  %d", o.size_y, 8+4);
			OptLine(x,y,l,"Junk lines  %d", o.btm_junk, 8);
			break;
		case O_Speed:
			OptLine(x,y,l,"Fall Speed    %d", o.speed/SpdDet, 8+2);
			OptLine(x,y,l,"Acceleration  %d", o.accel, 8+4);
			break;
		case O_Block:
			OptLine(x,y,l,"Size   %d", o.bsize, 8+2);
			OptLine(x,y,l,"Length min  %d", o.blen_min, 8);
			OptLine(x,y,l,"Length max  %d", o.blen_max, 8+2);
			OptLine(x,y,l,"Bias   %d", o.bbias, 8+2);
			OptLine(x,y,l,"Diagonal  %d", o.bdiag-4, 8);
			break;
		
		case O_Draw:
			OptLine(x,y,l,"Preview blocks  %d", o.nx_cur, 8+2);
			OptLine(x,y,l,"Grid dots   %d", o.dots, 8+2);
			OptLine(x,y,l,"Frame draw  %d", o.frame, 8+2);
			break;
		case O_Input:
			OptLine(x,y,l,"Move in drop  %d", o.move_in_drop, 8+2);
			OptLine(x,y,l,"Fall speed  %d", o.sp_fall, 8);
			OptLine(x,y,l,"Drop speed  %d", o.sp_drop, 8+2);
			break;
		}
		// cursor out
		if (oyg < 0)  oyg = l-1;
		if (oyg >= l)  oyg = 0;
		return;
	}
	d.setFont(0);


	//  Grid  ::
	const int dx = dim_x /2, dy = dim_y /2;
	//const int dx = 0, dy = 0;
	int yy, xx;
	for (y=0; y < o.size_y; ++y)
	{	yy = y * dim_y + ofs_y;
		for (x=0; x < o.size_x; ++x)
		{
			xx = x * dim_x + ofs_x;
			if ( o.dots==0 ||  //  grid modes
				(o.dots==1 && (x%2 == y%2)) ||
				(o.dots==2 && (x%3==1 && y%3==1)))
			d.drawPixel(xx+dx, yy+dy, RGB(16,18,22));  // .
			
			if (grid[y][x])  //  blocks
			{
				d.fillRect(xx, yy, dim_x, dim_y, BlkClr(grid[y][x], 0, 4));
				d.drawRect(xx, yy, dim_x, dim_y, BlkClr(grid[y][x], 2, 4));
				d.drawRect(xx+1, yy+1, dim_x-2, dim_y-2, BlkClr(grid[y][x], 1, 4));
			}
		}
	}
	//  Frame  | |
	if (o.frame > 0)
	{
		yy = ofs_y + o.size_y * dim_y;
		xx = ofs_x + o.size_x * dim_x;
		for (y = ofs_y; y < yy; y += o.frame)
		{
			d.drawPixel(ofs_x-1, y, RGB(18,22,25));
			d.drawPixel(xx, y, RGB(18,22,25));
		}
		//  Frame  _
		xx = ofs_x + o.size_x * dim_x;
		for (x = ofs_x; x < xx; x += o.frame)
			d.drawPixel(x, yy, RGB(20,24,27));
	}
	
	//  Drop preview .
	y = pos_y;  //+1
	while (!Overlaps(blk, pos_x, y))  ++y;
	Draw(blk, pos_x, y-1, 0, 2);  //~

	//  Current block '
	//Draw(d, blk, pos_x, 0);  // grid y
	y = dim_y * time_y / time_spd;
	Draw(blk, pos_x, pos_y-1, y);  // fine y


	//  Next blocks :
	if (!o.nx_cur)  return;
	int by = dim_y * o.bsize;
	int xe, ys = (H-12 - o.nx_cur * by) / (o.nx_cur-1);
	ys = max(0, ys);
	
	for (y=0; y < o.nx_cur; ++y)
	{
		yy = y * (by + ys);

		xx = ofs_x + (o.size_x+3) * dim_x;   // |next to field
		xe = W-1 - dim_x * o.bsize /*- dim_x/2*/;  // screen right|
		if (xx > xe)  xx = xe;
		
		DrawNext(next[y], xx, yy);
		
		//  line ..
		#if 0
		xe = xx + o.bsize * dim_x;
		if (y > 0)
		for (x=xx; x < xe; x+=2)
			d.drawPixel(x, yy-1, RGB(15,20,25));
		#endif
	}

	//  text  --
	#if 0  //  debug
	//x = W-1 -5*6;  y = H-1 - 4*8;
	d.setTextColor(RGB(20,22,25));
	d.setCursor(x,y);  d.print(xo);  d.print(",");  d.print(yo);  y+=8+1;
	d.setCursor(x,y);  d.print(xa);  d.print("-");  d.print(xb);  y+=8;
	d.setCursor(x,y);  d.print(ya);  d.print("|");  d.print(yb);  y+=8;
	d.setCursor(x,y);  d.print("e ");  d.print(errors);  y+=8;
	#else

	//  score  -
	x = 0;  y = H-1-8;
	d.setTextColor(RGB(25,25,10));
	d.setCursor(x,y);  sprintf(a,"Score %d", score);  d.print(a);

	//d.setCursor(x,  y);  d.print("Lines");  y+=8;
	//d.setCursor(x+6,y);  d.print(lines[0]);  y+=8+2;
	d.setTextColor(RGB(10,26,10));
	sprintf(a,"  Speed %ld", speed_y / SpdDet);  d.print(a);
	#endif

	//d.setCursor(x,  y);  d.print("Size");  y+=8;
	//d.setCursor(0,0);  d.print(o.size_x);  d.print(" ");  d.print(o.size_y);  y+=8;

	//d.setCursor(x,y);  d.print("Set ");  d.print(preset);  y+=8;

	//  status  -
	d.setTextColor(RGB(31,18,5));
	d.setCursor(W-1-6*6, H-1-8);
	if (ended)  d.print("Ended"); else
	if (paused) d.print("Pause");
}


//  draw game option line
void Games::OptLine(int& x, int& y, int& l, const char* str, int par, int8_t yy)
{
	char a[32];
	Ada4_ST7735& d = *g->d;

	int c = abs(l - oyg);
	if (!c)
	{	d.fillRect(0, y-1, W-1, 10, RGB(10,4,4));
		d.setCursor(0,y);  d.print("\x10");  // >
	}
	g->FadeClr(Gui::C_GameOpt, 8, c, 2);
	d.setCursor(x,y);
	sprintf(a,str, par);  d.print(a);
	++l;  y += yy+2;
}
