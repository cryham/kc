#include "gui.h"
#include "Ada4_ST7735.h"


//  Help
//....................................................................................
void Gui::DrawHelp()
{
	char a[16];
	d->setTextColor(RGB(23,27,31));
	d->print(strMain[ym]);  d->setFont(0);

	//  page
	d->setTextColor(RGB(28,28,31));
	d->setCursor(W-1 -5*6, 0);
	sprintf(a, "%d/%d", hpage+1, HAll);
	d->print(a);

	//  titles
	d->setTextColor(RGB(26,26,31));
	d->setCursor(W/3+6, 4);
	const static char* title[HAll] = {"Menu",
		"Mapping", "Sequences", "Edit Sequence", //"Edit Seq cd",
		"Demos", "Demos cd", "Sixtis", "Sixtis cd"
		"","","",""};
	d->print(title[hpage]);

	//  text
	d->setTextColor(RGB(22,26,31));
	switch (hpage)
	{
	case 0:  // main  ------------------------
		d->setCursor(0, 32);
		d->println("\x18,\x19   Move Cursor \x10");
		d->moveCursor(0,2);
		d->println("PgUp,PgDn   Prev/Next Page");
		d->moveCursor(0,4);
		d->println("\x1B or +Add   Back");
		d->moveCursor(0,2);
		d->println("\x1A  Enter");
		break;

	case 1:  // sequences  ------------------------
		d->setCursor(0, 32);
		d->println("\x18\x19  Cursor");
		d->moveCursor(0,2);
		d->println("PgUp,PgDn  Page");

		d->moveCursor(0,4);
		d->println("Num Enter   Edit");

		d->moveCursor(0,4);
		d->println("F4   Save");
		d->println("F5   Load");

		d->moveCursor(0,4);
		d->println("C   Copy");
		d->println("V   Paste");
		d->println("X   Swap");
		d->println("Ctrl-Del   Delete");
		break;

	case 5:  // edit seq
		d->setCursor(0, 32);
		d->println("All on Layer 2");  //par?
		//d->moveCursor(0,2);
		d->println("\x18\x19  Cursor  (Layer2)");
		d->println("PgUp,PgDn  Start/End");
		d->moveCursor(0,2);
		d->println("Ins +  Insert/overwr");
		d->println("Del  Delete");
		d->println("Enter  Clear");
		break;

	/*case 6:  // edit seq cd
		d->setCursor(0, 32+2);
		//\xFA\xF9\x07\xFE\xF8 middle dots
		//\xB0\xB1\xB2\xF0
		d->println("F2  \xB1 Wait command");
		d->println("F1  \xB0 Set Delay");
		d->moveCursor(0,2);
		d->println("  Next Digit 1..0");
		d->println("  1 none  4 20ms");
		d->println("  6 100  9 1s");
		break;*/

	case 7:  // demos  ------------------------
		d->setCursor(0, 32+2);
		d->println("\x18\x19  Prev/Next");
		d->moveCursor(0,2);
		d->println("+ F  Toggle Fps");
		d->moveCursor(0,4);
		d->println("R  Reset all");
		d->println("- Esc  Params Info");
		d->println("  Ctrl  Mode,Off");
		break;

	case 8:  // demos cd
		d->setCursor(0, 32+8);
		d->println("PgUp,PgDn  Change");
		d->moveCursor(0,2);
		d->println("Home,End   Params");
		d->moveCursor(0,2);
		d->println("  Ctrl  Others");
		d->println("  Shift  Fine");
		break;

	case 9:  // sixtis  ------------------------
		d->setCursor(0, 32);
		d->println("\x1B\x1A   Move");
		d->moveCursor(0,2);
		d->println("\x18    Rotate cw");
		d->println("5 /   Rotate ccw");
		d->moveCursor(0,2);
		d->println("\x19    Fall down");
		d->println("Ins    Drop");
		break;

	case 10:  // sixtis cd
		d->setCursor(0, 32+4);
		d->println("BckSpc Esc  Exit");
		d->println("+ Spc  Pause");
		d->println("Enter  New Game");
		d->moveCursor(0,4);
		d->println("- *  Options");
		d->println("\x1B\x1A  Dec,Inc Param");
		d->println("PgUp,PgDn  Page");
		break;
	}
}
