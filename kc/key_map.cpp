#include "gui.h"
#include "kc_data.h"
#include "matrix.h"
#include "kbd_layout.h"

extern KC_Main kc;


int8_t Gui::KeysMap()
{
	if (pressKey)  // press key
	{
		int c = 0, ii = -1;
		for (uint i=0; i < ScanKeys; ++i)
		{	if (Matrix_scanArray[i].state != KeyState_Off)  ++c;
			if (Matrix_scanArray[i].state == KeyState_Press)  ii = i;
		}
		if (pressKey == 1)
		{	//  wait until all keys are off
			if (c == 0)
				pressKey = 2;
		}
		else if (pressKey == 2 && ii != -1)
		{	//  pick
			pressKey = 0;  scId = ii;
			//  find draw key for scId
			for (int i=0; i < nDrawKeys; ++i)
				if (drawKeys[i].sc == scId)
				{	drawId = i;  break;  }
		}
		return 1;
	}

	if (moveCur)  // move cursor
	{
		if (kBack)
		{	//  set if key exists in matrix
			uint8_t sc = drawKeys[drawId].sc;
			if (sc < ScanKeys && sc != NO)
				scId = sc;
			moveCur = 0;  return 1;
		}

		//  find closest next in move direction
		int x=2, y=0, ii = -1, B = 2000,
			mxl = -B, mxr = B, myu = B, myd = B, axd = B, axu = B;

		for (int i = 0; i < nDrawKeys; ++i)
		{
			const DrawKey& k = drawKeys[i];

			//  set coords or advance
			if (k.x >=0)  x = k.x;  else  x -= k.x;
			if (k.y > 0)  y = k.y + yPosLay;  else  y -= k.y;

			//  distance to cursor key center
			int dx = x + k.w/2 - drawX,
				dy = y + k.h/2 - drawY, ay = abs(dy), ax = abs(dx);

			if (i != drawId)
			{
				if (kRight > 0 && dx > 0 && abs(dy) < 8)  // >
				{
					if (dx < mxr)
					{	mxr = dx;  ii = i;  }
				}else
				if (kRight < 0 && dx < 0 && abs(dy) < 8)  // <
				{
					if (dx > mxl)
					{	mxl = dx;  ii = i;  }
				}else
				if (kUp < 0 && dy < 0 && ax < 6)  // ^
				{
					if (ay < myu || (ay == myu && ax < axu))
					{	myu = ay;  axu = ax;  ii = i;  }
				}else
				if (kUp > 0 && dy > 0 && ax < 6)  // v
				{
					if (dy <= myd && ax < axd)
					{	myd = dy;  axd = ax;  ii = i;  }
				}
			}
		}
		if (ii != -1)
			drawId = ii;

		if (drawId >= nDrawKeys) drawId -= nDrawKeys;
		if (drawId < 0)			 drawId += nDrawKeys;
		return 1;
	}

	if (pickCode)  // pick code  ----
	{
		if (kBack)
		{	//  apply in kc
			if (scId >= 0 && scId < kc.set.nkeys())
			{
				kc.set.key[nLay][scId] = keyCode;
			}
			pickCode = 0;  return 1;
		}
		if (kFps)
		{	pickCode = 0;  return 1;  }

		if (kRight)  // <- ->
		{	if (grpFilt)
			{	keyGroup += kRight;
				if (keyGroup < 0)		 keyGroup = grpMax-1;
				if (keyGroup >= grpMax)	 keyGroup = 0;
				keyCode = grpStart[keyGroup];
			}else
				keyCode += kRight * 12;
		}else
		if (kUp)	keyCode += kUp;  else
		if (kPgUp)	keyCode += kPgUp * 4;  else

		if (kEnd < 0)  // home  filter
			grpFilt = 1-grpFilt;

		//  grp range
		if (grpFilt)
		{	if (keyCode < grpStart[keyGroup])
				keyCode = grpEnd[keyGroup];
			if (keyCode > grpEnd[keyGroup])
				keyCode = grpStart[keyGroup];
		}
		//  all range
		if (keyCode < 0)			  keyCode += KEYS_ALL_EXT;
		if (keyCode >= KEYS_ALL_EXT)  keyCode -= KEYS_ALL_EXT;
		return 1;
	}
	return 0;
}
