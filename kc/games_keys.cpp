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

	if (g->kPgUp > 0)  opg = (opg - 1 + O_All) % O_All;
	if (g->kPgUp < 0)  opg = (opg + 1) % O_All;
}

//  Keys
// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . 
int Games::KeyPress(int8_t& mlevel)
{
	//  global
	if (g->kBack) // || g->kEsc)
	{
		if (gui==2)  gui=0;  // off options
		else
		if (gui==1)  mlevel = 0;  // <back to menu
		else  gui = 1;  // gui on
	}

	if (g->kInf)  // keyp(ASTERISK) || keyp(MINUS))
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
		if (g->kRight > 0)  // enter>
		{
			switch (yg)
			{
			case G_Resume:  gui=0;  break;
			case G_NewGame:  NewGame();  gui=0;  break;
			case G_Options:  gui = 2;  break;
			case G_Help:  return 1;
			}
		}
		if (g->kRight < 0)
			mlevel = 0;  // <back to menu
		return 0;
	}

	if (gui==2)  // - options -
	{
		oyg = (oyg + g->kUp + 6) % 6;
		
		int s = g->kCtrl ? 4 : 1, k = g->kRight * s;
		
		if (k)  // change params  ----
		switch (opg)
		{
		case O_Field:  switch (oyg)  {
			case 0:  o.size_x   += k;  o.size_x   = max(2, min(smax_x, o.size_x));  NewGrid();  break;
			case 1:  o.size_y   += k;  o.size_y   = max(2, min(smax_y, o.size_y));  NewGrid();  break;  //8 21
			case 2:  o.btm_junk += k;  o.btm_junk = max(0, min(smax_y, o.btm_junk));  break;
			}	break;
		case O_Speed:  switch (oyg)  {
			case 0:  o.speed += k*SpdDet*4;  o.speed = max(0, min(200*SpdDet, o.speed));  break;
			case 1:  o.accel += k;  o.accel = max(0, min( 80, o.accel));  break;
			}	break;
		case O_Block:  switch (oyg)  {
			case 0:  o.bsize += k;  o.bsize = max(2, min(bmax, o.bsize));  break;

			case 1:  o.blen_min += k;  o.blen_min = max(-16, min(32, o.blen_min));  break;
			case 2:  o.blen_max += k;  o.blen_max = max(1, min(32, o.blen_max));  break;

			case 3:  o.bbias += k;  o.bbias = max(-16, min(16, o.bbias));  break;
			case 4:  o.bdiag += k;  o.bdiag = max(2, min(8, o.bdiag));  break;
			}	break;
		
		case O_Draw:  switch (oyg)  {
			case 0:  o.nx_cur += k;  o.nx_cur = max(0, min(4, o.nx_cur));  break;
			case 1:  o.dots   += k;  o.dots   = max(0, min(3, o.dots));  break;
			case 2:  o.frame  += k;  o.frame  = max(0, min(4, o.frame));  break;
			}	break;
		case O_Input:  switch (oyg)  {
			case 0:  o.key_rpt += k;  o.key_rpt = max(0, min(60, o.key_rpt));  break;
			case 1:  o.move_in_drop = 1-o.move_in_drop;  break;
			
			case 2:  o.sp_fall += k;  o.sp_fall = max(1, min(40, o.sp_fall));  break;
			case 3:  o.sp_drop += k;  o.sp_drop = max(1, min(10, o.sp_drop));  break;
			}	break;
		}
		Checks();
		return 0;
	}


	//  ---  game  ---
	
	if (g->kEnt)  // new
		NewGame();

	if (ended)  return 0;

	if (g->kInf && !gui)  // || key(SPACE))  // pause
		paused = 1 - paused;

	if (paused)  return 0;


	//  --  move  --
	if (!drop || o.move_in_drop)
	{
		//  rotate  control
		if ((demo && random(500)==0) ||
			g->kUp < 0)  // rot cw
		{
			Rotate(cpy, blk, 1);  //  check possible
			if (!Overlaps(cpy, pos_x, pos_y))
				Copy(blk, cpy);
		}
		if (g->kDel > 0)  //|| keyp(5))  // rot ccw
		{
			Rotate(cpy, blk, 0);  //  check possible
			if (!Overlaps(cpy, pos_x, pos_y))
				Copy(blk, cpy);
		}

		//  move
		if ((demo && random(350)==0) || g->kRight < 0)  // move
		{
			int old_x = pos_x;
			--pos_x;  if (pos_x < 0)  pos_x = o.size_x-1;
			if (Overlaps(blk, pos_x, pos_y))
				pos_x = old_x;  //  if not possible restore
		}

		if ((demo && random(350)==0) || g->kRight > 0)  // mvoe
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


	#if 0
	//  - other -
	if (key(M))  // demo-
		demo = 1 - demo;

	if (key(1))
	{	if (speed_y > 5*SpdDet)  speed_y -= 5*SpdDet;
		else  speed_y = 0;  UpdSpeed();  }
	if (key(2))
	{	speed_y += 5*SpdDet;  UpdSpeed();  }
	#endif


	Update();

	return 0;
}
