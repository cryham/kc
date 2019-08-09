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
	const static char* title[HAll] = {"Main", "Quick",
		"Mapping", "Mapping",
		"Sequences", "Sequence", "Sequence", "Sequence",
		"Demos", "Sixtis", "Sixtis"},

	*title2[HAll] = {"", "Access",  // 2nd lines
		"", "Pick Key",
		" View", " Edit", "Commands", "Mouse",
		"", " 1/2", " 2/2"};

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

	case 1:  //  quick
		d->setClr(16,22,28);
		d->println("F1   Mapping");
		d->println("F2   Sequences");
		d->moveCursor(0,4);
		d->println("F6   Test Pressed");
		d->println("F7   Test Layout");
		d->println("F8   Setup Mouse");
		d->moveCursor(0,4);
		d->println("F9   Display");
		d->println("F10  Demos Plasma");
		d->moveCursor(0,2);
		d->println("F11  Help");
		d->println("F12  Clock");
		break;

	case 2:  //  mapping  ------------------------
		d->setClr(25,28,23);
		d->println("\x18,\x19 \x1B,\x1A    Move Cursor");
		d->moveCursor(0,2);
		d->println("/    Press key, set cursor");
		d->moveCursor(0,6);
		d->println("PgUp,PgDn  Prev/Next Layer");
		//d->moveCursor(0,2);
		d->setClr(17,24,17);
		d->println("  Last is layer use visual");

		d->moveCursor(0,6);
		d->setClr(25,28,23);
		d->println("- or Ent   Pick key list");
		d->println("Ctrl-Ent   Go to sequence");
		d->moveCursor(0,6);
		d->println("C,V,X      Copy,Paste,Swap");
		//d->moveCursor(0,2);
		d->println("Ctrl-Del   Delete");
		break;

	case 3:  //  pick key
		d->setClr(23,28,23);
		d->println("\x18,\x19 PgUp,PgDn \x1B,\x1A  Move");
		d->moveCursor(0,6);
		d->println("+ or Ent    Set key");
		d->moveCursor(0,2);
		d->println("- or Bksp   Cancel");

		d->moveCursor(0,6);
		d->setClr(21,25,21);
		d->println("/    Group filter toggle");
		d->moveCursor(0,2);
		d->println("\x1B,\x1A  Prev,Next Group");
		break;

	case 4:  //  sequences
	//	d->println("--------------------------");  // max 26
		d->setClr(21,28,28);
		d->println("\x18,\x19 PgUp,PgDn  Move");
		d->moveCursor(0,2);
		d->println("Home,End   Move 3x");
		d->moveCursor(0,6);
		sprintf(a,"L%d-Enter   Edit seq.", par.editLayer);
		d->println(a);
		d->println("Ctrl-Ent   Go to mapping");

		d->moveCursor(0,6);
		d->println("C,V,X      Copy,Paste,Swap");
		//d->moveCursor(0,2);
		d->println("Ctrl-Del   Delete");
		break;

	case 5:  //  edit seq
		d->setClr(6,30,30);
		sprintf(a,"All on Layer %d:", par.editLayer);
		d->println(a);
		d->setClr(18,29,29);
		d->moveCursor(0,6);
		d->println("\x1B,\x1A   Move Cursor  Ctrl 4x");
		d->moveCursor(0,2);
		d->println("Home,End   To Start,End");

		d->moveCursor(0,6);
		d->println("Del,Bksp   Delete");
		d->println("Ins      Insert/Overwrite");
		d->moveCursor(0,6);
		d->println("Enter      Exit Edit");
		break;

	case 6:  //  seq commands ___ help
		d->setClr(25,26,26);
		d->println("Insert command in seq:");
		d->moveCursor(0,6);
		d->println("/  Set Key Delay [1ms]");
		d->moveCursor(0,2);
		d->println("*  Wait [0.1s]");

		d->moveCursor(0,6);
		d->println("C  Comment start/end {}");
		//d->moveCursor(0,2);
		d->println("X  Hide> rest of seq.");
		d->moveCursor(0,2);
		d->println("V  Run other seq.");
		d->moveCursor(0,2);
		d->println("-  Repeat seq, Gui quits");
		d->moveCursor(0,6);
		d->println("\x18,\x19  Adjust parameter");
		break;

	case 7:  //  seq _mouse commands_ help
		d->setClr(24,25,23);
		d->println("Shift- Insert command:");
		d->moveCursor(0,6);
		d->println("\x1B,\x1A \x18,\x19  Move  Ctrl big");
		d->moveCursor(0,4);
		d->println("  Buttons:");
		d->println("Home click  End  double");
		d->println("PgDn press  PgUp release");
		d->moveCursor(0,4);
		d->println("/,*  Wheel move horiz,vert");
		d->moveCursor(0,2);
		d->println("/,*,-  Adjust x,y, Set xy-");
		d->moveCursor(0,6);
		d->println("\x18,\x19  Adjust parameter");
		break;

	case 8:  //  demos  ------------------------
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

	case 9:  //  sixtis  ------------------------
		d->setClr(29,26,23);
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

	case 10:  //  sixtis cd
		d->setClr(29,25,20);
		d->println("+ or Bksp  Back");
		d->moveCursor(0,2);
		d->println("-        Pause");
		d->moveCursor(0,2);
		d->println("Enter    New Game");
		d->moveCursor(0,6);
		d->println("*        Options toggle");
		d->moveCursor(0,2);
		d->setClr(25,21,16);
		d->println("\x1B,\x1A      Dec,Inc Value");
		d->println("PgUp,PgDn  Page");
		break;
	}
}
