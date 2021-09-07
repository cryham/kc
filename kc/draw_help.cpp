#include "gui.h"
#include "Ada4_ST7735.h"
#include "kc_params.h"


//  Help
//....................................................................................
void Gui::DrawHelp()
{
	char a[32];
	d->setClr(21,26,31);
	d->print(strMain[ym]);  d->setFont(0);

	//  page
	d->setClr(22,22,23);
	d->setCursor(W-1 -5*6, 0);
	sprintf(a, "%2d/%d", hpage+1, HAll);
	d->print(a);

	//  titles
	const static char* title[HAll] = {
		"Main", "Demos", },

	*title2[HAll] = {  // 2nd lines
		"", "", };

	const int x = W/3+6;
	d->setClr(25,28,31);  d->setCursor(x, 2);  d->print(title[hpage]);
	d->setClr(21,24,28);  d->setCursor(x,12);  d->print(title2[hpage]);

	//  text
	d->setCursor(0, 32);
	switch (hpage)
	{
	case 0:  //  main
		d->setClr(20,26,31);
		d->println("\x18,\x19        Move Cursor");  // ^,v \x10
		d->moveCursor(0,2);
		d->println("\x1A or Ent        Enter");
		d->moveCursor(0,2);
		d->println("\x1B or Bksp or +  Go Back");

		d->moveCursor(0,8);
		d->setClr(16,22,28);
		d->println("PgUp,PgDn  Prev,Next Page");
		d->moveCursor(0,2);
		d->println("\x1B,\x1A     Dec,Inc Value");  // <,>
		d->setClr(12,18,24);
		d->println("  LCtrl more  LShift less");

		d->moveCursor(0,6);
		d->setClr(16,22,28);
		d->println("F4   Save");
		d->println("F5   Load   Ctrl- Reset");
		break;

	case 1:  //  demos  ------------------------
		d->setClr(26,26,29);
		d->println("\x1B,\x1A   Prev/Next Preset");
		d->println("\x18,\x19   Dec,Inc Speed");
		d->moveCursor(0,4);
		d->println("-     Fps show +mode");
		d->println("*     Params Info");

		d->moveCursor(0,8);
		d->setClr(22,22,25);
		d->println("PgUp,PgDn  Change");
		d->println("Home,End   Params");
		d->moveCursor(0,2);
		d->println("  Ctrl   Others");
		d->println("  Shift  Fine");
		break;
	}
}
