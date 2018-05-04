#include "games.h"
#include "gui.h"


//  options check  . . .
void Games::Checks()
{
	if (o.blen_max < o.blen_min)  o.blen_max = o.blen_min;  // min <= max
	if (o.blen_min > o.blen_max)  o.blen_min = o.blen_max;
	if (o.bsize > o.size_x)  o.bsize = o.size_x;  // block <= field
	if (o.bsize > o.size_y)  o.bsize = o.size_y;
	int bb = o.bsize*o.bsize;
	if (o.blen_min > bb)  o.blen_min = bb;  // block_min < bsize^2
	if (o.blen_max > bb)  o.blen_max = bb;

	if (g->kPgUp)
		opg = (opg + g->kPgUp + O_All) % O_All;
}

//  Keys
// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . 
int Games::KeyPress(int8_t& mlevel)
{
	//  global
	if (g->kAdd || g->kBckSp)
	{
		if (gui==2)  gui=0;  // off options
		else
		if (gui==1)  mlevel = 0;  // <back to menu
		else  gui = 1;  // gui on
	}

	if (g->kMul)
	{
		if (gui==2)  gui = 0;  // toggle options
		else  gui = 2;
	}
	
	
	if (gui==1)  // - menu -
	{
		if (g->kUp < 0)  yg = (yg - 1 + G_All) % G_All;
		if (g->kUp > 0)  yg = (yg + 1) % G_All;
		
		if (yg == G_Preset)
		{
			if (g->kRight < 0)
			{	preset = (preset - 1 + Presets) % Presets;	NewSet();  }
			if (g->kRight > 0)
			{	preset = (preset + 1 + Presets) % Presets;  NewSet();  }
			return 0;
		}
		if (g->kRight > 0 || g->kEnt2)  // enter>
		{
			switch (yg)
			{
			case G_Resume:  gui=0;  break;
			case G_NewGame:  NewGame();  gui=0;  break;
			case G_Options:  gui = 2;  break;
			case G_Help:  return 1;  // set screen after
			}
		}
		if (g->kRight < 0 || g->kBckSp)
			mlevel = 0;  // <back to menu
		return 0;
	}

	if (gui==2)  // - options -
	{
		if (g->kUp)  oyg += g->kUp;
		
		int s = g->kCtrl ? 4 : 1, k = g->kRight * s;
		
		if (k)  // change params  ----
		switch (opg)
		{
		case O_Field:  switch (oyg)  {
			case 0:  o.size_x = g->RangeAdd(o.size_x, k, 2, smax_x);  NewGrid();  break;
			case 1:  o.size_y = g->RangeAdd(o.size_y, k, 2, smax_y);  NewGrid();  break;  //8 21
			case 2:  o.btm_junk = g->RangeAdd(o.btm_junk, k, 0, smax_y);  break;
			}	break;
		case O_Speed:  switch (oyg)  {
			case 0:  o.speed += k*SpdDet*4;  o.speed = max(0, min(200*SpdDet, o.speed));  break;
			case 1:  o.accel = g->RangeAdd(o.accel, k, 0, 80);  break;
			}	break;
		case O_Block:  switch (oyg)  {
			case 0:  o.bsize = g->RangeAdd(o.bsize, k, 2, bmax);  break;

			case 1:  o.blen_min = g->RangeAdd(o.blen_min, k, -16, 32);  break;
			case 2:  o.blen_max = g->RangeAdd(o.blen_max, k,   1, 32);  break;

			case 3:  o.bbias = g->RangeAdd(o.bbias, k, -16, 16);  break;
			case 4:  o.bdiag = g->RangeAdd(o.bdiag, k,   2, 8);  break;
			}	break;
		
		case O_Draw:  switch (oyg)  {
			case 0:  o.nx_cur = g->RangeAdd(o.nx_cur, k, 0, 4);  break;
			case 1:  o.dots   = g->RangeAdd(o.dots  , k, 0, 3);  break;
			case 2:  o.frame  = g->RangeAdd(o.frame , k, 0, 4);  break;
			}	break;
		case O_Input:  switch (oyg)  {
			case 0:  o.move_in_drop = 1-o.move_in_drop;  break;
			
			case 1:  o.sp_fall = g->RangeAdd(o.sp_fall, k, 1, 40);  break;
			case 2:  o.sp_drop = g->RangeAdd(o.sp_drop, k, 1, 10);  break;
			}	break;
		}
		Checks();
		return 0;
	}


	//  ---  game  ---
	
	if (g->kEnt || g->kEnt2)  // new
		NewGame();

	if (ended)  return 0;

	if (g->kSub && !gui)  // || key(SPACE))  // pause
		paused = 1 - paused;

	if (paused)  return 0;


	//  --  move  --
	if (!drop || o.move_in_drop)
	{
		//  rotate  control
		if (g->kUp < 0)  // rot cw
		{
			Rotate(cpy, blk, 1);  //  check possible
			if (!Overlaps(cpy, pos_x, pos_y))
				Copy(blk, cpy);
		}
		if (g->kDel || g->kDiv)  // rot ccw
		{
			Rotate(cpy, blk, 0);  //  check possible
			if (!Overlaps(cpy, pos_x, pos_y))
				Copy(blk, cpy);
		}

		//  move
		if (g->kRight < 0)
		{
			int old_x = pos_x;
			--pos_x;  if (pos_x < 0)  pos_x = o.size_x-1;
			if (Overlaps(blk, pos_x, pos_y))
				pos_x = old_x;  //  if not possible restore
		}
		if (g->kRight > 0)
		{
			int old_x = pos_x;
			++pos_x;  if (pos_x > o.size_x-1)  pos_x = 0;
			if (Overlaps(blk, pos_x, pos_y))
				pos_x = old_x;  //  if not possible restore
		}
	}

	fall = g->kDnH;  // fall faster hold

	if (g->kIns)  // drop
		drop = 1;


	Update();

	return 0;
}
