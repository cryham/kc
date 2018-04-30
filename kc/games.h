#pragma once
#include <stdint.h>
#include "def.h"
struct Gui;


struct Games
{
	//  main
	Games();
	void Init(Gui* pGui);
	Gui* g = 0;
	
	void Draw();
	void Update();
	int KeyPress(int8_t& mlevel);
	
	uint32_t old_ti, dt_sum;
	
	struct Options
	{
		//  field
		int8_t size_x, size_y;  // field size
		int8_t btm_junk;  // bottom lines filled at start
		//  draw
		int8_t nx_cur;   // preview, number of next blocks
		int8_t dots;    // grid dots draw type> is less
		int8_t frame;   // frame | | draw type > is less

		//  block generate
		int8_t blen_min, blen_max,  // legth min..max
			bsize,  // max size x=y
			bdiag,  // directions: 4 norm, 8 full diagonal
			bbias;  // bias 16 more (max) or 0 less (min) blocks

		//  fall speed at start, accel
		int32_t speed, accel;
		
		//  input, fine
		int8_t sp_drop, sp_fall;
		int8_t key_rpt, move_in_drop;
	} o;


	//  dim auto
	int8_t dim_x, dim_y,  //  grid dim, box size pixels
		ofs_x, ofs_y;  // field offset on screen
		
	//  move
	int8_t pos_x, pos_y;
	int8_t fall, drop;  // bool

	
	//  speed
	#define SpdDet (16384*3)
	int16_t time_y;
	int32_t speed_y, time_spd;

	inline void UpdSpeed()
	{	time_spd = 10000 * SpdDet / speed_y;  }

	
	//  grid data  ::
	const static int8_t
		smax_x = 32, smax_y = 32;
	int8_t grid[smax_y][smax_x];

	//  blocks  .
	const static int8_t
		nx_max = 4, bmax = 8;
	struct Block
	{	int8_t b[bmax][bmax];  };
	
	Block blk, cpy, next[nx_max], prv;

	
	//  block generator
	void GenBlock(Block& b), NewBlock(), NewGrid();
	void Checks(), NewSet();

	//  utility
	uint16_t BlkClr(int8_t b, int8_t d, int8_t dim);
	void Draw(    const Block& b,  int pos_x, int pos_y, int o_y=0, int dim=4);
	void DrawNext(const Block& b,  int pos_x, int pos_y);
	
	int Overlaps(const Block& b,  int pos_x, int pos_y);	// test block overlap in grid

	void Clear(Block& b);
	void Copy(Block& to, const Block& from);
	
	int8_t xo,yo, xa,xb, ya,yb;  // range
	void GetRange(const Block& b);
	void Rotate(Block& to, const Block& from, int cw);

	
	//  global state
	int8_t paused, demo, ended;
	
	const static int8_t Presets = 11;
	int8_t preset;  // game type
	void NewGame();

	//  stats
	uint16_t score, lines, errors;


	//  gui menu  ---
	enum EGui {
		G_NewGame=0, G_Preset, G_Resume, G_Options, G_Help, G_All };
	enum EOpt {
		O_Field=0, O_Speed, O_Block, O_Draw, O_Input, O_All };
	
	int8_t gui, yg,  // level, cursor
		oyg, opg;  // opt
	void OptLine(int& x, int& y, int& l, const char* str, int par, int8_t yy);
};
