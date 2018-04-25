#include <games.h>
#define PROGMEM
#include "FreeSans9pt7b.h"


//  Draw block
// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . 
void Games::Draw(Ada4_ST7735& d, const Block& b,
	int pos_x, int pos_y, int o_y, int dim)
{
	if (!dim)
	for (int y=0; y < o.bsize; ++y)
	for (int x=0; x < o.bsize; ++x)
	{
		int yy = (pos_y + y) % o.size_y * dim_y + ofs_y + o_y;
		int xx = (pos_x + x) % o.size_x * dim_x + ofs_x;
		if (b.b[y][x] &&
			yy >= 0 && yy < H)
			d.fillRect(xx, yy, dim_x, dim_y, WHITE);
	}
	else  //  dim, prv
	for (int y=0; y < o.bsize; ++y)
	for (int x=0; x < o.bsize; ++x)
	{
		int yy = (pos_y + y) % o.size_y * dim_y + ofs_y + o_y;
		int xx = (pos_x + x) % o.size_x * dim_x + ofs_x;
		if (b.b[y][x] &&
			yy >= 0 && yy < H)
			for (int j=0; j < dim_y; ++j)
			for (int i=0; i < dim_x; ++i)
				if ((xx+i) % 2 == (yy+j) % 2)
					d.drawPixel(xx + i, yy + j, WHITE);
	}
}
void Games::DrawNext(Ada4_ST7735& d, const Block& b,
	int pos_x, int pos_y)
{
	for (int y=0; y < o.bsize; ++y)
	for (int x=0; x < o.bsize; ++x)
	{
		int yy = pos_y + y * dim_y;
		int xx = pos_x + x * dim_x;
		//  grid modes .
		if (o.dots==0 || 
			o.dots==1 && (x%2 && y%2) ||
			o.dots==2 && (x%3==1 && y%3==1))
			d.drawPixel(xx + dim_x/2, yy + dim_y/2, WHITE);

		if (b.b[y][x] &&
			xx < W && yy < H)
			d.fillRect(xx, yy, dim_x, dim_y, WHITE);
	}
}


//  Draw
// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . 
const static char* sPresets[Games::Presets] = {
	" Tiny 3+", " Basic 4/2", " Small 4+/4", "Medium 5+/4", " Tetris 4",
	" Pentis 5/1", " Sixtis 6", "Septis 7+/1", "Octis 8+/1", " Huge 12"};
	
const static char* sOptPages[Games::O_All] = {
	"Field", "Speed", "Block", "Draw", "Input"};

void Games::Draw(Ada4_ST7735& d)
{
	char a[64];
	int x = 0, y = 0;
	d.setCursor(x,y);

	if (gui==1)
	{
		d.setFont(&FreeSans9pt7b);
		d.print("Sixtis");  d.setFont(0);

		d.setCursor(W/2-6, 4);
		d.println(sPresets[preset]);  // title
		
		d.setCursor(0, 16);
		for (y=0; y < G_All; ++y)
		{
			d.print(y==yg ? "\x10":" ");
			switch (y)
			{	case G_Resume:   d.println("Resume");  break;
				case G_NewGame:  d.println("New Game");  break;
				case G_Preset:   d.print("Preset: ");
					d.print(preset+1);  d.print("/");  d.println(Presets);
					//d.print("   ");  d.println(ssPresets[preset]);  // title
					break;
				case G_Options: d.println("Options");  break;
				case G_Help:    d.println("Help");  break;
			}
		}
		return;
	}
	if (gui==2)
	{
		d.setFont(&FreeSans9pt7b);
		d.print("Sixtis");  d.setFont(0);
		
		d.setCursor(W-1 -3*6, 0);
		d.print(opg+1);  d.print("/");  d.print(O_All);
		
		d.setCursor(W/2-6, 4);
		d.print(sOptPages[opg]);  // title

		x = 8;  y = 16+8;  int l = 0;
		#define  line(str, par, yy)  \
			if (l == oyg) {  d.setCursor(0,y);  d.print("\x10");  }  d.setCursor(x,y);  \
			d.print(str);  d.print(par);  ++l;  y+=yy;  d.setCursor(x,y)

		switch (opg)
		{
		case O_Field:
			line("Width   ", o.size_x, 8);
			line("Height  ", o.size_y, 8+4);
			line("Junk lines  ", o.btm_junk, 8);
			break;
		case O_Speed:
			line("Fall Speed    ", o.speed/SpdDet, 8+2);
			line("Acceleration  ", o.accel, 8+4);
			break;
		case O_Block:  y-=6;
			line("Size   ", o.bsize, 8+2);
			line("Length min  ", o.blen_min, 8);
			line("Length max  ", o.blen_max, 8+2);
			line("Bias   ", o.bbias, 8+2);
			line("Diagonal  ", o.bdiag-4, 8);
			break;
		
		case O_Draw:
			line("Preview blocks  ", o.nx_cur, 8+2);
			line("Grid dots   ", o.dots, 8+2);
			line("Frame draw  ", o.frame, 8+2);
			break;
		case O_Input:
			line("Key repeat  ", o.key_rpt, 8+2);
			line("Move in drop  ", o.move_in_drop, 8+2);
			line("Fall speed  ", o.sp_fall, 8);
			line("Drop speed  ", o.sp_drop, 8+2);
			break;
		}
		// cursor out
		if (oyg < 0)  oyg = l-1;
		if (oyg >= l)  oyg = 0;
		return;
	}
	d.setFont(0);

	#if 0  //  debug --
	//x = W-1 -5*6;  y = H-1 - 4*8;
	d.setCursor(x,y);  d.print(xo);  d.print(",");  d.print(yo);  y+=8+1;
	d.setCursor(x,y);  d.print(xa);  d.print("-");  d.print(xb);  y+=8;
	d.setCursor(x,y);  d.print(ya);  d.print("|");  d.print(yb);  y+=8;
	d.setCursor(x,y);  d.print("e ");  d.print(errors);  y+=8;
	#else
	//  score
	d.setCursor(x,  y);  d.print("Score");  y+=8;
	d.setCursor(x+6,y);  d.print(score);  y+=8+1;

	//d.setCursor(x,  y);  d.print("Lines");  y+=8;
	//d.setCursor(x+6,y);  d.print(lines[0]);  y+=8+2;

	d.setCursor(x,  y);  d.print("Speed");  y+=8;
	d.setCursor(x+6,y);  d.print(speed_y / SpdDet);  y+=8+1;
	#endif

	//d.setCursor(x,  y);  d.print("Size");  y+=8;
	//d.setCursor(0,0);  d.print(o.size_x);  d.print(" ");  d.print(o.size_y);  y+=8;

	//d.setCursor(x,y);  d.print("Set ");  d.print(preset);  y+=8;

	//  status
	d.setCursor(0,H-1-8);
	if (ended)  d.print("Ended");
	if (paused) d.print("Pause");
	
	
	//  Grid  ::
	const int dx = dim_x /2, dy = dim_y /2;
	//const int dx = 0, dy = 0;
	int yy, xx;
	for (y=0; y < o.size_y; ++y)
	{	yy = y * dim_y + ofs_y;
		for (x=0; x < o.size_x; ++x)
		{
			xx = x * dim_x + ofs_x;
			//  grid modes
			if (o.dots==0 || 
				o.dots==1 && (x%2 == y%2) ||
				o.dots==2 && (x%3==1 && y%3==1))
			d.drawPixel(xx+dx, yy+dy, WHITE);  // .
			
			if (grid[y][x])  //  blocks
				d.fillRect(xx, yy, dim_x, dim_y, WHITE);
		}
	}
	//  Frame  | |
	if (o.frame > 0)
	{
		xx = ofs_x + o.size_x * dim_x;
		for (y = ofs_y; y < H; y += o.frame)
		{
			d.drawPixel(ofs_x-1, y, WHITE);
			d.drawPixel(xx, y, WHITE);
		}
	}
	
	//  Current block '
	//Draw(d, blk, pos_x, 0);  // grid y
	y = dim_y * time_y / time_spd;
	Draw(d, blk, pos_x, pos_y-1, y);  // fine y

	//  Drop preview .
	y = pos_y;  //+1
	while (!Overlaps(blk, pos_x, y))  ++y;
	Draw(d, blk, pos_x, y-1, 0, 1);

	
	//  Next blocks :
	if (!o.nx_cur)  return;
	int by = dim_y * o.bsize;
	int xe, ys = (H - o.nx_cur * by) / (o.nx_cur-1);
	ys = max(0, ys);
	
	for (y=0; y < o.nx_cur; ++y)
	{
		yy = y * (by + ys);

		xx = ofs_x + (o.size_x+3) * dim_x;   // |next to field
		xe = W-1 - dim_x * o.bsize /*- dim_x/2*/;  // screen right|
		if (xx > xe)  xx = xe;
		
		DrawNext(d, next[y], xx, yy);
		
		//  line ..
		xe = xx + o.bsize * dim_x;
		if (y > 0)
		for (x=xx; x < xe; x+=2)
			d.drawPixel(x, yy-1, WHITE);
	}
}
