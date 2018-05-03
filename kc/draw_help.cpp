#include "gui.h"
#include "Ada4_ST7735.h"
#include "kc_params.h"


//  Help
//....................................................................................
void Gui::DrawHelp()
{
	char a[32];
	d->setTextColor(RGB(21,26,31));
	d->print(strMain[ym]);  d->setFont(0);

	//  page
	d->setTextColor(RGB(29,29,31));
	d->setCursor(W-1 -5*6, 0);
	sprintf(a, "%2d/%d", hpage+1, HAll);
	d->print(a);

	//  titles
	d->setTextColor(RGB(26,26,31));
	d->setCursor(W/3+6, 4);
	const static char* title[HAll] = {"Main",
		"Mapping", "Pick Key",
		"Sequences", "Edit Seq.",
		"Demos", "Sixtis", "Sixtis cd."};
	d->print(title[hpage]);

	//  text
	d->setCursor(0, 32);
	switch (hpage)
	{
	case 0:  //  main
		d->setTextColor(RGB(20,26,31));
		d->println("\x18,\x19     Move Cursor \x10");  // v,^
		d->moveCursor(0,4);
		d->println("\x1A       Enter");
		d->moveCursor(0,2);
		d->println("\x1B or +  Go Back");

		d->moveCursor(0,10);
		d->setTextColor(RGB(16,22,28));
		d->println("PgUp,PgDn  Prev,Next Page");
		d->moveCursor(0,4);
		d->println("\x1B,\x1A     Dec,Inc Value");  // <,>
		d->setTextColor(RGB(12,18,24));
		d->println("        LCtrl- more");
		break;

	case 1:  //  mapping  ------------------------
		d->setTextColor(RGB(26,29,23));
		d->println("/   Press key for cursor");
		d->moveCursor(0,2);
		d->println("*   Move cursor  * Back");
		d->moveCursor(0,4);
		d->println("Ctrl-Del   Delete");
		d->moveCursor(0,6);
		d->println("PgUp,PgDn  Prev/Next Layer");
		d->moveCursor(0,2);
		d->setTextColor(RGB(17,24,17));
		d->println("  Last is layer use visual");
		d->moveCursor(0,6);
		d->setTextColor(RGB(26,29,23));
		d->println("-   Pick key list ..");
		break;

	case 2:  //  pick key
		d->setTextColor(RGB(23,28,23));
		d->println("\x1B,\x1A,PgUp,PgDn,\x18,\x19  Move");
		d->moveCursor(0,4);
		d->println("+    Set key");
		d->moveCursor(0,2);
		d->println("-    Cancel");
		d->moveCursor(0,6);
		d->println("/    Group filter toggle");
		d->moveCursor(0,2);
		d->println("\x1B,\x1A  Prev,Next Group");
	//	d->println("--------------------------");  // max 26
		break;

	case 3:  //  sequences
		d->setTextColor(RGB(21,29,29));
		//d->println("\x18\x19,PgUp,Dn,
		d->println("Home,End    Move Cursor 3x");
		d->moveCursor(0,6);
		sprintf(a,"L%d-Enter    Edit seq.", par.editLayer);
		d->println(a);

		d->moveCursor(0,6);
		d->println("F4   Save");
		d->println("F5   Load   Ctrl- Reset");

		d->moveCursor(0,6);
		d->println("C,V,X   Copy,Paste,Swap");
		d->moveCursor(0,2);
		d->println("Ctrl-Del    Delete");
		break;

	case 4:  //  edit seq
		d->setTextColor(RGB(6,30,30));
		sprintf(a,"All on Layer %d", par.editLayer);
		d->println(a);
		d->setTextColor(RGB(18,30,30));
		d->moveCursor(0,6);
		d->println("\x18,\x19       Move Cursor");
		d->moveCursor(0,2);
		d->println("Home,End  To Start,End");

		d->moveCursor(0,6);
		d->println("Del,Backspc   Delete");
		d->println("Ins  Insert/Overwrite");
		d->moveCursor(0,6);
		d->println("Enter     Set seq.");
		break;

	/*case 6:  // edit seq cd
		//\xFA\xF9\x07\xFE\xF8 middle dots
		//\xB0\xB1\xB2\xF0
		d->println("F2  \xB1 Wait command");
		d->println("F1  \xB0 Set Delay");
		d->moveCursor(0,2);
		d->println("  Next Digit 1..0");
		d->println("  1 none  4 20ms");
		d->println("  6 100  9 1s");
		break;*/

	case 5:  //  demos  ------------------------
		d->setTextColor(RGB(26,26,29));
		d->println("\x18,\x19   Prev/Next Preset");
		d->println("\x1B,\x1A   Dec,Inc Speed");
		d->moveCursor(0,4);
		d->println("-     Toggle Fps");
		d->println("*     Params Info");
		d->moveCursor(0,8);
		d->setTextColor(RGB(22,22,25));
		d->println("PgUp,PgDn  Change");
		d->println("Home,End   Params");
		d->moveCursor(0,2);
		d->println("  Ctrl  Others");
		d->println("  Shift  Fine");
		break;

	case 6:  //  sixtis  ------------------------
		d->setTextColor(RGB(29,26,23));
		d->println("\x1B,\x1A      Move");
		d->moveCursor(0,6);
		d->println("\x18        Rotate cw");
		d->moveCursor(0,2);
		d->println("5 or /   Rotate ccw");
		d->moveCursor(0,6);
		d->println("\x19        Fall down");
		d->moveCursor(0,2);
		d->println("Ins      Drop");
		break;

	case 7:  //  sixtis cd
		d->setTextColor(RGB(29,25,20));
		d->println("+        Back");
		d->moveCursor(0,2);
		d->println("-        Pause");
		d->moveCursor(0,2);
		d->println("Enter    New Game");
		d->moveCursor(0,6);
		d->println("*        Options toggle");
		d->moveCursor(0,2);
		d->setTextColor(RGB(25,21,16));
		d->println("\x1B,\x1A      Dec,Inc Value");
		d->println("PgUp,PgDn  Page");
		break;
	}
}
