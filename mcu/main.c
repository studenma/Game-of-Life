/*
*	Jmeno: Martin Studeny
*	Login: xstude23
*	VHDL pouzito od Ing. Vasicka
*	(90%) - ve VHDL nedoslo k zadne zmene, C - pouzity knihovny display.h, keyboard.h a fitkitlib.h
*	Posledni zmena: 2.1.2018
*/
/*******************************************************************************
   main.c: LCD + keyboard demo
   Copyright (C) 2009 Brno University of Technology,
                      Faculty of Information Technology
   Author(s): Zdenek Vasicek <vasicek AT stud.fit.vutbr.cz>

   LICENSE TERMS

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions
   are met:
   1. Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
   2. Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in
      the documentation and/or other materials provided with the
      distribution.
   3. All advertising materials mentioning features or use of this software
      or firmware must display the following acknowledgement:

        This product includes software developed by the University of
        Technology, Faculty of Information Technology, Brno and its
        contributors.

   4. Neither the name of the Company nor the names of its contributors
      may be used to endorse or promote products derived from this
      software without specific prior written permission.

   This software or firmware is provided ``as is'', and any express or implied
   warranties, including, but not limited to, the implied warranties of
   merchantability and fitness for a particular purpose are disclaimed.
   In no event shall the company or contributors be liable for any
   direct, indirect, incidental, special, exemplary, or consequential
   damages (including, but not limited to, procurement of substitute
   goods or services; loss of use, data, or profits; or business
   interruption) however caused and on any theory of liability, whether
   in contract, strict liability, or tort (including negligence or
   otherwise) arising in any way out of the use of this software, even
   if advised of the possibility of such damage.

   $Id$


*******************************************************************************/

#include <fitkitlib.h>
#include <keyboard/keyboard.h>
#include <lcd/display.h>

char last_ch;

#define rychlost_mala 35
#define rychlost_velka 10

int rychlost = rychlost_mala;

// TISK NAPOVEDY
void print_user_help(void) {
    term_send_str("PROJEKT DO PREDMETU IMP\n# Ovladani hry:\n
    	Po spusteni staci kliknout na tlacitka 1-9, cimz se vybere pocatecni populace\n
    	Po chvili se hra spusti (aby bylo mozne videt pocatecni nastaveni)\n
    	Zruseni hry se provede tlacitkem 0\n
    	Je mozne vybrat mezi 2 rychlostmi:\n
    	\tZvyseni rychlosti tlacitkem D\n
    	\tSnizeni rychlosti tlacitkem #\n
    	Pozastavecni hry se provede tlacitkem A\n
    	\tPote je mozne bud krokovat stisknutim tlacitka B, nebo hru opet spustit tlacitkem C\n");
    term_send_crlf();
}

// DETEKCE KLAVES
void keyboard() {
	char tmp;
	tmp = key_decode(read_word_keyboard_4x4());
	if (tmp != last_ch) {
		last_ch = tmp;
		if (tmp != 0) {
			term_send_crlf();
			term_send_str("Klavesa ");
			term_send_char(tmp);
			term_send_crlf();  
		}
	}
	else 
		last_ch = '-';
}

unsigned char decode_user_cmd(char *cmd_ucase, char *cmd) {
  return CMD_UNKNOWN;
}

void fpga_initialized() {
  LCD_init();
  LCD_clear();
 // LCD_append_string("STISKNI 1-9");
}

//{STAV, POCET SOUSEDU}
//1 ziva , 0 mrtva
int display[8][8][2] = { {
		{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0}},
		{{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0}},
		{{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0}},
		{{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0}},
		{{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0}},
		{{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0}},
		{{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0}},
		{{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0}}
	};

// FUNKCE PRO POCITANI SOUSEDU
void countNeighbours() {
  int i, j;
	for(i = 0; i < 8; i++) {
		for(j = 0; j < 8; j++) {
			int tmp = 0;
			// LEVY HORNI ROH
			if(i == 0 && j == 7) {
				if(display[i][j-1][0] == 1)
					tmp++;
				if(display[i+1][j][0] == 1)
					tmp++;
				if(display[i+1][j-1][0] == 1)
					tmp++;
				if(display[i][j-7][0] == 1)
					tmp++;
				if(display[i+1][j-7][0] == 1)
					tmp++;
				if(display[i+7][j][0] == 1)
					tmp++;
				if(display[i+7][j-1][0] == 1)
					tmp++;
				if(display[i+7][j-7][0] == 1)
					tmp++;
				display[i][j][1] = tmp;
			}
			// PRAVY HORNI ROH
			else if(i == 0 && j == 0) {
				if(display[i][j+1][0] == 1)
					tmp++;
				if(display[i+1][j][0] == 1)
					tmp++;
				if(display[i+1][j+1][0] == 1)
					tmp++;
				if(display[i][j+7][0] == 1)
					tmp++;
				if(display[i+1][j+7][0] == 1)
					tmp++;
				if(display[i+7][j+7][0] == 1)
					tmp++;
				if(display[i+7][j][0] == 1)
					tmp++;
				if(display[i+7][j+1][0] == 1)
					tmp++;
				display[i][j][1] = tmp;
			}
			// LEVY DOLNI ROH
			else if(i == 7 && j == 7) {
				if(display[i][j-1][0] == 1)
					tmp++;
				if(display[i-1][j][0] == 1)
					tmp++;
				if(display[i-1][j-1][0] == 1)
					tmp++;
				if(display[i-1][j-7][0] == 1)
					tmp++;
				if(display[i][j-7][0] == 1)
					tmp++;
				if(display[i-7][j-7][0] == 1)
					tmp++;
				if(display[i-7][j][0] == 1)
					tmp++;
				if(display[i-7][j-1][0] == 1)
					tmp++;
				display[i][j][1] = tmp;
			}
			// PRAVY DOLNI ROH
			else if(i == 7 && j == 0) {
				if(display[i][j+1][0] == 1)
					tmp++;
				if(display[i-1][j][0] == 1)
					tmp++;
				if(display[i-1][j+1][0] == 1)
					tmp++;
				if(display[i-1][j+7][0] == 1)
					tmp++;
				if(display[i][j+7][0] == 1)
					tmp++;
				if(display[i-7][j+7][0] == 1)
					tmp++;
				if(display[i-7][j][0] == 1)
					tmp++;
				if(display[i-7][j+1][0] == 1)
					tmp++;
				display[i][j][1] = tmp;
			}
			// HORNI RADA BEZ ROHU
			else if(i == 0 && j != 7 && j != 0) {
				if(display[i][j-1][0] == 1)
					tmp++;
				if(display[i][j+1][0] == 1)
					tmp++;
				if(display[i+1][j][0] == 1)
					tmp++;
				if(display[i+1][j-1][0] == 1)
					tmp++;
				if(display[i+1][j+1][0] == 1)
					tmp++;
				if(display[i+7][j][0] == 1)
					tmp++;
				if(display[i+7][j+1][0] == 1)
					tmp++;
				if(display[i+7][j-1][0] == 1)
					tmp++;
				display[i][j][1] = tmp;
			}
			// DOLNI RADA BEZ ROHU
			else if(i == 7 && j != 7 && j != 0) {
				if(display[i][j-1][0] == 1)
					tmp++;
				if(display[i][j+1][0] == 1)
					tmp++;
				if(display[i-1][j][0] == 1)
					tmp++;
				if(display[i-1][j-1][0] == 1)
					tmp++;
				if(display[i-1][j+1][0] == 1)
					tmp++;
				if(display[i-7][j][0] == 1)
					tmp++;
				if(display[i-7][j+1][0] == 1)
					tmp++;
				if(display[i-7][j-1][0] == 1)
					tmp++;
				display[i][j][1] = tmp;
			}
			// LEVY SOUPEC BEZ ROHU
			else if(j == 7 && i != 0 && i != 7) {
				if(display[i-1][j][0] == 1)
					tmp++;
				if(display[i+1][j][0] == 1)
					tmp++;
				if(display[i][j-1][0] == 1)
					tmp++;
				if(display[i+1][j-1][0] == 1)
					tmp++;
				if(display[i-1][j-1][0] == 1)
					tmp++;
				if(display[i][j-7][0] == 1)
					tmp++;
				if(display[i+1][j-7][0] == 1)
					tmp++;
				if(display[i-1][j-7][0] == 1)
					tmp++;
				display[i][j][1] = tmp;
			}
			// PRAVY SOUPEC BEZ ROHU
			else if(j == 0 && i != 0 && i != 7) {
				if(display[i-1][j][0] == 1)
					tmp++;
				if(display[i+1][j][0] == 1)
					tmp++;
				if(display[i][j+1][0] == 1)
					tmp++;
				if(display[i+1][j+1][0] == 1)
					tmp++;
				if(display[i-1][j+1][0] == 1)
					tmp++;
				if(display[i][j+7][0] == 1)
					tmp++;
				if(display[i+1][j+7][0] == 1)
					tmp++;
				if(display[i-1][j+7][0] == 1)
					tmp++;
				display[i][j][1] = tmp;
			}
			// MID
			else {
				if(display[i][j+1][0] == 1)
					tmp++;
				if(display[i][j-1][0] == 1)
					tmp++;
				if(display[i-1][j-1][0] == 1)
					tmp++;
				if(display[i-1][j][0] == 1)
					tmp++;
				if(display[i-1][j+1][0] == 1)
					tmp++;
				if(display[i+1][j-1][0] == 1)
					tmp++;
				if(display[i+1][j][0] == 1)
					tmp++;
				if(display[i+1][j+1][0] == 1)
					tmp++;
				display[i][j][1] = tmp;
			}
		}
	}
}

/*
*	POCATECNI NASTAVENI
*/
// kridlo
void set() {
	display[0][0][0] = 0;
	display[0][1][0] = 0;
	display[0][2][0] = 0;
	display[0][3][0] = 0;
	display[0][4][0] = 0;
	display[0][5][0] = 1;
	display[0][6][0] = 0;
	display[0][7][0] = 1;

	display[1][0][0] = 0;
	display[1][1][0] = 0;
	display[1][2][0] = 0;
	display[1][3][0] = 0;
	display[1][4][0] = 0;
	display[1][5][0] = 1;
	display[1][6][0] = 1;
	display[1][7][0] = 0;

	display[2][0][0] = 0;
	display[2][1][0] = 0;
	display[2][2][0] = 0;
	display[2][3][0] = 0;
	display[2][4][0] = 0;
	display[2][5][0] = 0;
	display[2][6][0] = 1;
	display[2][7][0] = 0;

	display[3][0][0] = 0;
	display[3][1][0] = 0;
	display[3][2][0] = 0;
	display[3][3][0] = 0;
	display[3][4][0] = 0;
	display[3][5][0] = 0;
	display[3][6][0] = 0;
	display[3][7][0] = 0;

	display[4][0][0] = 0;
	display[4][1][0] = 0;
	display[4][2][0] = 0;
	display[4][3][0] = 0;
	display[4][4][0] = 0;
	display[4][5][0] = 0;
	display[4][6][0] = 0;
	display[4][7][0] = 0;

	display[5][0][0] = 0;
	display[5][1][0] = 0;
	display[5][2][0] = 0;
	display[5][3][0] = 0;
	display[5][4][0] = 0;
	display[5][5][0] = 0;
	display[5][6][0] = 0;
	display[5][7][0] = 0;

	display[6][0][0] = 0;
	display[6][1][0] = 0;
	display[6][2][0] = 0;
	display[6][3][0] = 0;
	display[6][4][0] = 0;
	display[6][5][0] = 0;
	display[6][6][0] = 0;
	display[6][7][0] = 0;

	display[7][0][0] = 0;
	display[7][1][0] = 0;
	display[7][2][0] = 0;
	display[7][3][0] = 0;
	display[7][4][0] = 0;
	display[7][5][0] = 0;
	display[7][6][0] = 0;
	display[7][7][0] = 0;

	countNeighbours();
}

// kridlo 2
void set2() {
	display[0][0][0] = 0;
	display[0][1][0] = 1;
	display[0][2][0] = 0;
	display[0][3][0] = 1;
	display[0][4][0] = 0;
	display[0][5][0] = 0;
	display[0][6][0] = 0;
	display[0][7][0] = 0;

	display[1][0][0] = 0;
	display[1][1][0] = 1;
	display[1][2][0] = 1;
	display[1][3][0] = 0;
	display[1][4][0] = 0;
	display[1][5][0] = 0;
	display[1][6][0] = 0;
	display[1][7][0] = 0;

	display[2][0][0] = 0;
	display[2][1][0] = 0;
	display[2][2][0] = 1;
	display[2][3][0] = 0;
	display[2][4][0] = 0;
	display[2][5][0] = 0;
	display[2][6][0] = 0;
	display[2][7][0] = 0;

	display[3][0][0] = 0;
	display[3][1][0] = 0;
	display[3][2][0] = 0;
	display[3][3][0] = 0;
	display[3][4][0] = 0;
	display[3][5][0] = 0;
	display[3][6][0] = 0;
	display[3][7][0] = 0;

	display[4][0][0] = 0;
	display[4][1][0] = 0;
	display[4][2][0] = 0;
	display[4][3][0] = 0;
	display[4][4][0] = 0;
	display[4][5][0] = 0;
	display[4][6][0] = 0;
	display[4][7][0] = 0;

	display[5][0][0] = 0;
	display[5][1][0] = 0;
	display[5][2][0] = 0;
	display[5][3][0] = 0;
	display[5][4][0] = 0;
	display[5][5][0] = 0;
	display[5][6][0] = 0;
	display[5][7][0] = 0;

	display[6][0][0] = 0;
	display[6][1][0] = 0;
	display[6][2][0] = 0;
	display[6][3][0] = 0;
	display[6][4][0] = 0;
	display[6][5][0] = 0;
	display[6][6][0] = 0;
	display[6][7][0] = 0;

	display[7][0][0] = 0;
	display[7][1][0] = 0;
	display[7][2][0] = 0;
	display[7][3][0] = 0;
	display[7][4][0] = 0;
	display[7][5][0] = 0;
	display[7][6][0] = 0;
	display[7][7][0] = 0;

	countNeighbours();
}

// cara - opakovani
void set3() {
	display[0][0][0] = 0;
	display[0][1][0] = 0;
	display[0][2][0] = 0;
	display[0][3][0] = 0;
	display[0][4][0] = 0;
	display[0][5][0] = 0;
	display[0][6][0] = 0;
	display[0][7][0] = 0;

	display[1][0][0] = 0;
	display[1][1][0] = 0;
	display[1][2][0] = 0;
	display[1][3][0] = 0;
	display[1][4][0] = 0;
	display[1][5][0] = 0;
	display[1][6][0] = 0;
	display[1][7][0] = 0;

	display[2][0][0] = 0;
	display[2][1][0] = 0;
	display[2][2][0] = 0;
	display[2][3][0] = 0;
	display[2][4][0] = 0;
	display[2][5][0] = 0;
	display[2][6][0] = 0;
	display[2][7][0] = 0;

	display[3][0][0] = 0;
	display[3][1][0] = 0;
	display[3][2][0] = 0;
	display[3][3][0] = 0;
	display[3][4][0] = 0;
	display[3][5][0] = 0;
	display[3][6][0] = 0;
	display[3][7][0] = 0;

	display[4][0][0] = 0;
	display[4][1][0] = 1;
	display[4][2][0] = 1;
	display[4][3][0] = 1;
	display[4][4][0] = 1;
	display[4][5][0] = 1;
	display[4][6][0] = 1;
	display[4][7][0] = 0;

	display[5][0][0] = 0;
	display[5][1][0] = 0;
	display[5][2][0] = 0;
	display[5][3][0] = 0;
	display[5][4][0] = 0;
	display[5][5][0] = 0;
	display[5][6][0] = 0;
	display[5][7][0] = 0;

	display[6][0][0] = 0;
	display[6][1][0] = 0;
	display[6][2][0] = 0;
	display[6][3][0] = 0;
	display[6][4][0] = 0;
	display[6][5][0] = 0;
	display[6][6][0] = 0;
	display[6][7][0] = 0;

	display[7][0][0] = 0;
	display[7][1][0] = 0;
	display[7][2][0] = 0;
	display[7][3][0] = 0;
	display[7][4][0] = 0;
	display[7][5][0] = 0;
	display[7][6][0] = 0;
	display[7][7][0] = 0;

	countNeighbours();
}

// KNOT
void set4() {
	display[0][0][0] = 0;
	display[0][1][0] = 0;
	display[0][2][0] = 0;
	display[0][3][0] = 0;
	display[0][4][0] = 0;
	display[0][5][0] = 0;
	display[0][6][0] = 0;
	display[0][7][0] = 0;

	display[1][0][0] = 0;
	display[1][1][0] = 0;
	display[1][2][0] = 0;
	display[1][3][0] = 0;
	display[1][4][0] = 0;
	display[1][5][0] = 0;
	display[1][6][0] = 0;
	display[1][7][0] = 0;

	display[2][0][0] = 0;
	display[2][1][0] = 0;
	display[2][2][0] = 0;
	display[2][3][0] = 1;
	display[2][4][0] = 1;
	display[2][5][0] = 0;
	display[2][6][0] = 0;
	display[2][7][0] = 0;

	display[3][0][0] = 0;
	display[3][1][0] = 0;
	display[3][2][0] = 0;
	display[3][3][0] = 0;
	display[3][4][0] = 1;
	display[3][5][0] = 1;
	display[3][6][0] = 0;
	display[3][7][0] = 0;

	display[4][0][0] = 0;
	display[4][1][0] = 0;
	display[4][2][0] = 0;
	display[4][3][0] = 0;
	display[4][4][0] = 1;
	display[4][5][0] = 0;
	display[4][6][0] = 0;
	display[4][7][0] = 0;

	display[5][0][0] = 0;
	display[5][1][0] = 0;
	display[5][2][0] = 0;
	display[5][3][0] = 0;
	display[5][4][0] = 0;
	display[5][5][0] = 0;
	display[5][6][0] = 0;
	display[5][7][0] = 0;

	display[6][0][0] = 0;
	display[6][1][0] = 0;
	display[6][2][0] = 0;
	display[6][3][0] = 0;
	display[6][4][0] = 0;
	display[6][5][0] = 0;
	display[6][6][0] = 0;
	display[6][7][0] = 0;

	display[7][0][0] = 0;
	display[7][1][0] = 0;
	display[7][2][0] = 0;
	display[7][3][0] = 0;
	display[7][4][0] = 0;
	display[7][5][0] = 0;
	display[7][6][0] = 0;
	display[7][7][0] = 0;

	countNeighbours();
}

// VSE
void set5() {
	display[0][0][0] = 1;
	display[0][1][0] = 1;
	display[0][2][0] = 1;
	display[0][3][0] = 1;
	display[0][4][0] = 1;
	display[0][5][0] = 1;
	display[0][6][0] = 1;
	display[0][7][0] = 1;

	display[1][0][0] = 1;
	display[1][1][0] = 1;
	display[1][2][0] = 1;
	display[1][3][0] = 1;
	display[1][4][0] = 1;
	display[1][5][0] = 1;
	display[1][6][0] = 1;
	display[1][7][0] = 1;

	display[2][0][0] = 1;
	display[2][1][0] = 1;
	display[2][2][0] = 1;
	display[2][3][0] = 1;
	display[2][4][0] = 1;
	display[2][5][0] = 1;
	display[2][6][0] = 1;
	display[2][7][0] = 1;

	display[3][0][0] = 1;
	display[3][1][0] = 1;
	display[3][2][0] = 1;
	display[3][3][0] = 1;
	display[3][4][0] = 1;
	display[3][5][0] = 1;
	display[3][6][0] = 1;
	display[3][7][0] = 1;

	display[4][0][0] = 1;
	display[4][1][0] = 1;
	display[4][2][0] = 1;
	display[4][3][0] = 1;
	display[4][4][0] = 1;
	display[4][5][0] = 1;
	display[4][6][0] = 1;
	display[4][7][0] = 1;

	display[5][0][0] = 1;
	display[5][1][0] = 1;
	display[5][2][0] = 1;
	display[5][3][0] = 1;
	display[5][4][0] = 1;
	display[5][5][0] = 1;
	display[5][6][0] = 1;
	display[5][7][0] = 1;

	display[6][0][0] = 1;
	display[6][1][0] = 1;
	display[6][2][0] = 1;
	display[6][3][0] = 1;
	display[6][4][0] = 1;
	display[6][5][0] = 1;
	display[6][6][0] = 1;
	display[6][7][0] = 1;

	display[7][0][0] = 1;
	display[7][1][0] = 1;
	display[7][2][0] = 1;
	display[7][3][0] = 1;
	display[7][4][0] = 1;
	display[7][5][0] = 1;
	display[7][6][0] = 1;
	display[7][7][0] = 1;

	countNeighbours();
}

// sachovnice
void set6() {
	display[0][0][0] = 1;
	display[0][1][0] = 0;
	display[0][2][0] = 1;
	display[0][3][0] = 0;
	display[0][4][0] = 1;
	display[0][5][0] = 0;
	display[0][6][0] = 1;
	display[0][7][0] = 0;

	display[1][0][0] = 0;
	display[1][1][0] = 1;
	display[1][2][0] = 0;
	display[1][3][0] = 1;
	display[1][4][0] = 0;
	display[1][5][0] = 1;
	display[1][6][0] = 0;
	display[1][7][0] = 1;

	display[2][0][0] = 1;
	display[2][1][0] = 0;
	display[2][2][0] = 1;
	display[2][3][0] = 0;
	display[2][4][0] = 1;
	display[2][5][0] = 0;
	display[2][6][0] = 1;
	display[2][7][0] = 0;

	display[3][0][0] = 0;
	display[3][1][0] = 1;
	display[3][2][0] = 0;
	display[3][3][0] = 1;
	display[3][4][0] = 0;
	display[3][5][0] = 1;
	display[3][6][0] = 0;
	display[3][7][0] = 1;

	display[4][0][0] = 1;
	display[4][1][0] = 0;
	display[4][2][0] = 1;
	display[4][3][0] = 0;
	display[4][4][0] = 1;
	display[4][5][0] = 0;
	display[4][6][0] = 1;
	display[4][7][0] = 0;

	display[5][0][0] = 0;
	display[5][1][0] = 1;
	display[5][2][0] = 0;
	display[5][3][0] = 1;
	display[5][4][0] = 0;
	display[5][5][0] = 1;
	display[5][6][0] = 0;
	display[5][7][0] = 1;

	display[6][0][0] = 1;
	display[6][1][0] = 0;
	display[6][2][0] = 1;
	display[6][3][0] = 0;
	display[6][4][0] = 1;
	display[6][5][0] = 0;
	display[6][6][0] = 1;
	display[6][7][0] = 0;

	display[7][0][0] = 0;
	display[7][1][0] = 1;
	display[7][2][0] = 0;
	display[7][3][0] = 1;
	display[7][4][0] = 0;
	display[7][5][0] = 1;
	display[7][6][0] = 0;
	display[7][7][0] = 1;

	countNeighbours();
}

// radky
void set7() {
	display[0][0][0] = 0;
	display[0][1][0] = 1;
	display[0][2][0] = 0;
	display[0][3][0] = 1;
	display[0][4][0] = 0;
	display[0][5][0] = 1;
	display[0][6][0] = 0;
	display[0][7][0] = 1;

	display[1][0][0] = 0;
	display[1][1][0] = 1;
	display[1][2][0] = 0;
	display[1][3][0] = 1;
	display[1][4][0] = 0;
	display[1][5][0] = 1;
	display[1][6][0] = 0;
	display[1][7][0] = 1;

	display[2][0][0] = 0;
	display[2][1][0] = 1;
	display[2][2][0] = 0;
	display[2][3][0] = 1;
	display[2][4][0] = 0;
	display[2][5][0] = 1;
	display[2][6][0] = 0;
	display[2][7][0] = 1;

	display[3][0][0] = 0;
	display[3][1][0] = 1;
	display[3][2][0] = 0;
	display[3][3][0] = 1;
	display[3][4][0] = 0;
	display[3][5][0] = 1;
	display[3][6][0] = 0;
	display[3][7][0] = 1;

	display[4][0][0] = 0;
	display[4][1][0] = 1;
	display[4][2][0] = 0;
	display[4][3][0] = 1;
	display[4][4][0] = 0;
	display[4][5][0] = 1;
	display[4][6][0] = 0;
	display[4][7][0] = 1;

	display[5][0][0] = 0;
	display[5][1][0] = 1;
	display[5][2][0] = 0;
	display[5][3][0] = 1;
	display[5][4][0] = 0;
	display[5][5][0] = 1;
	display[5][6][0] = 0;
	display[5][7][0] = 1;

	display[6][0][0] = 0;
	display[6][1][0] = 1;
	display[6][2][0] = 0;
	display[6][3][0] = 1;
	display[6][4][0] = 0;
	display[6][5][0] = 1;
	display[6][6][0] = 0;
	display[6][7][0] = 1;

	display[7][0][0] = 0;
	display[7][1][0] = 1;
	display[7][2][0] = 0;
	display[7][3][0] = 1;
	display[7][4][0] = 0;
	display[7][5][0] = 1;
	display[7][6][0] = 0;
	display[7][7][0] = 1;

	countNeighbours();
}

// ctverec
void set8() {
	display[0][0][0] = 0;
	display[0][1][0] = 0;
	display[0][2][0] = 0;
	display[0][3][0] = 0;
	display[0][4][0] = 0;
	display[0][5][0] = 0;
	display[0][6][0] = 0;
	display[0][7][0] = 0;

	display[1][0][0] = 0;
	display[1][1][0] = 0;
	display[1][2][0] = 0;
	display[1][3][0] = 0;
	display[1][4][0] = 0;
	display[1][5][0] = 0;
	display[1][6][0] = 0;
	display[1][7][0] = 0;

	display[2][0][0] = 0;
	display[2][1][0] = 0;
	display[2][2][0] = 0;
	display[2][3][0] = 0;
	display[2][4][0] = 0;
	display[2][5][0] = 0;
	display[2][6][0] = 0;
	display[2][7][0] = 0;

	display[3][0][0] = 0;
	display[3][1][0] = 0;
	display[3][2][0] = 0;
	display[3][3][0] = 1;
	display[3][4][0] = 1;
	display[3][5][0] = 0;
	display[3][6][0] = 0;
	display[3][7][0] = 0;

	display[4][0][0] = 0;
	display[4][1][0] = 0;
	display[4][2][0] = 0;
	display[4][3][0] = 1;
	display[4][4][0] = 1;
	display[4][5][0] = 0;
	display[4][6][0] = 0;
	display[4][7][0] = 0;

	display[5][0][0] = 0;
	display[5][1][0] = 0;
	display[5][2][0] = 0;
	display[5][3][0] = 0;
	display[5][4][0] = 0;
	display[5][5][0] = 0;
	display[5][6][0] = 0;
	display[5][7][0] = 0;

	display[6][0][0] = 0;
	display[6][1][0] = 0;
	display[6][2][0] = 0;
	display[6][3][0] = 0;
	display[6][4][0] = 0;
	display[6][5][0] = 0;
	display[6][6][0] = 0;
	display[6][7][0] = 0;

	display[7][0][0] = 0;
	display[7][1][0] = 0;
	display[7][2][0] = 0;
	display[7][3][0] = 0;
	display[7][4][0] = 0;
	display[7][5][0] = 0;
	display[7][6][0] = 0;
	display[7][7][0] = 0;

	countNeighbours();
}

// vse bez okraju
void set9() {
	display[0][0][0] = 0;
	display[0][1][0] = 0;
	display[0][2][0] = 0;
	display[0][3][0] = 0;
	display[0][4][0] = 0;
	display[0][5][0] = 0;
	display[0][6][0] = 0;
	display[0][7][0] = 0;

	display[1][0][0] = 0;
	display[1][1][0] = 1;
	display[1][2][0] = 1;
	display[1][3][0] = 1;
	display[1][4][0] = 1;
	display[1][5][0] = 1;
	display[1][6][0] = 1;
	display[1][7][0] = 0;

	display[2][0][0] = 0;
	display[2][1][0] = 1;
	display[2][2][0] = 1;
	display[2][3][0] = 1;
	display[2][4][0] = 1;
	display[2][5][0] = 1;
	display[2][6][0] = 1;
	display[2][7][0] = 0;

	display[3][0][0] = 0;
	display[3][1][0] = 1;
	display[3][2][0] = 1;
	display[3][3][0] = 1;
	display[3][4][0] = 1;
	display[3][5][0] = 1;
	display[3][6][0] = 1;
	display[3][7][0] = 0;

	display[4][0][0] = 0;
	display[4][1][0] = 1;
	display[4][2][0] = 1;
	display[4][3][0] = 1;
	display[4][4][0] = 1;
	display[4][5][0] = 1;
	display[4][6][0] = 1;
	display[4][7][0] = 0;

	display[5][0][0] = 0;
	display[5][1][0] = 1;
	display[5][2][0] = 1;
	display[5][3][0] = 1;
	display[5][4][0] = 1;
	display[5][5][0] = 1;
	display[5][6][0] = 1;
	display[5][7][0] = 0;

	display[6][0][0] = 0;
	display[6][1][0] = 1;
	display[6][2][0] = 1;
	display[6][3][0] = 1;
	display[6][4][0] = 1;
	display[6][5][0] = 1;
	display[6][6][0] = 1;
	display[6][7][0] = 0;

	display[7][0][0] = 0;
	display[7][1][0] = 0;
	display[7][2][0] = 0;
	display[7][3][0] = 0;
	display[7][4][0] = 0;
	display[7][5][0] = 0;
	display[7][6][0] = 0;
	display[7][7][0] = 0;

	countNeighbours();
}

// KROK VYVOJE
void behave() {
  int i,j;
	for(i = 0; i < 8; i++) {
		for(j = 0; j < 8; j++) {
			// kazda ziva bunka s mene nez dvema zivymi sousedy zemre
			if(display[i][j][0] == 1 && display[i][j][1] < 2)
				display[i][j][0] = 0;
			// kazda ziva bunka se dvema nebo tremi zivymi sousedy zustava zit
			else if(display[i][j][0] == 1 && (display[i][j][1] == 2 || display[i][j][1] == 3))
				display[i][j][0] = 1;
			// kazda ziva bunka s vice nez tremi zivymi sousedy zemre
			else if(display[i][j][0] == 1 && display[i][j][1] > 3)
				display[i][j][0] = 0;
			// kazda mrtva buka s prave tremi zivymi sousedy ozivne
			else if(display[i][j][0] == 0 && display[i][j][1] == 3)
				display[i][j][0] = 1;
		}
	}
	countNeighbours();
}

void init() {
  // sloupce
  P4DIR |= BIT4; // PORT 3    ->   SLOUPEC 0
  P4DIR |= BIT5; // PORT 4    ->   SLOUPEC 1
  P4DIR |= BIT2; // PORT 5    ->   SLOUPEC 2
  P4DIR |= BIT3; // PORT 6    ->   SLOUPEC 3
  P4DIR |= BIT0; // PORT 7    ->   SLOUPEC 4
  P4DIR |= BIT1; // PORT 8    ->   SLOUPEC 5
  P3DIR |= BIT6; // PORT 9    ->   SLOUPEC 6
  P3DIR |= BIT7; // PORT 10   ->   SLOUPEC 7
  
  // radky
  P1DIR |= BIT6; // PORT 25    ->   RADEK 0
  P2DIR |= BIT7; // PORT 18    ->   RADEK 1
  P2DIR |= BIT6; // PORT 17    ->   RADEK 2
  P2DIR |= BIT5; // PORT 20    ->   RADEK 3
  P2DIR |= BIT4; // PORT 19    ->   RADEK 4
  P2DIR |= BIT3; // PORT 22    ->   RADEK 5
  P2DIR |= BIT0; // PORT 23    ->   RADEK 6 
  P2DIR |= BIT1; // PORT 24    ->   RADEK 7
}

// ZAPNE VSECHNY DIODY
void print() {
  /*
  *  SLOUPCE 1
  */
  P4OUT |= BIT4; // SLOUPEC 0 do stavu 1
  P4OUT |= BIT5; // SLOUPEC 1 do stavu 1
  P4OUT |= BIT2; // SLOUPEC 2 do stavu 1
  P4OUT |= BIT3; // SLOUPEC 3 do stavu 1
  P4OUT |= BIT0; // SLOUPEC 4 do stavu 1
  P4OUT |= BIT1; // SLOUPEC 5 do stavu 1
  P3OUT |= BIT6; // SLOUPEC 6 do stavu 1
  P3OUT |= BIT7; // SLOUPEC 7 do stavu 1


  /*
  *  SLOUPCE 0
  */ 
  //P4OUT &= ~BIT4; // SLOUPEC 0 do stavu 0
  // P4OUT &= ~BIT5; // SLOUPEC 1 do stavu 0
  // P4OUT &= ~BIT2; // SLOUPEC 2 do stavu 0
  //P4OUT &= ~BIT3; // SLOUPEC 3 do stavu 0
  //P4OUT &= ~BIT0; // SLOUPEC 4 do stavu 0
  // P4OUT &= ~BIT1; // SLOUPEC 5 do stavu 0
  // P3OUT &= ~BIT6; // SLOUPEC 6 do stavu 0
  // P3OUT &= ~BIT7; // SLOUPEC 7 do stavu 0    
 
  /*
  *  RADKY 0
  */ 
  P1OUT &= ~BIT6; // RADEK 0 do stavu 0
  P2OUT &= ~BIT7; // RADEK 1 do stavu 0
  P2OUT &= ~BIT6; // RADEK 2 do stavu 0
  P2OUT &= ~BIT5; // RADEK 3 do stavu 0
  P2OUT &= ~BIT4; // RADEK 4 do stavu 0
  P2OUT &= ~BIT3; // RADEK 5 do stavu 0
  P2OUT &= ~BIT0; // RADEK 6 do stavu 0
  P2OUT &= ~BIT1; // RADEK 7 do stavu 0  

  /*
  *  RADKY 1
  */
   //P1OUT |= BIT6; // RADEK 0 do stavu 1
  // P2OUT |= BIT7; // RADEK 1 do stavu 1
   //P2OUT |= BIT6; // RADEK 2 do stavu 1
  // P2OUT |= BIT5; // RADEK 3 do stavu 1
  // P2OUT |= BIT4; // RADEK 4 do stavu 1
   //P2OUT |= BIT3; // RADEK 5 do stavu 1
  // P2OUT |= BIT0; // RADEK 6 do stavu 1
  // P2OUT |= BIT1; // RADEK 7 do stavu 1  
}

// ZOBRAZENI AKTUALNE ZIVYCH BUNEK
void printDisplay() {
  int row, column;
  for (row=0; row<8; row++) {
      /*
      *  VYPNUTI VSECH RADKU
      */
      P1OUT |= BIT6; // RADEK 0 do stavu 1
      P2OUT |= BIT7; // RADEK 1 do stavu 1
      P2OUT |= BIT6; // RADEK 2 do stavu 1
      P2OUT |= BIT5; // RADEK 3 do stavu 1
      P2OUT |= BIT4; // RADEK 4 do stavu 1
      P2OUT |= BIT3; // RADEK 5 do stavu 1
      P2OUT |= BIT0; // RADEK 6 do stavu 1
      P2OUT |= BIT1; // RADEK 7 do stavu 1  

      // VYPNUTI VSECH SLOUPCU
      P4OUT &= ~BIT4; // SLOUPEC 0 do stavu 0
      P4OUT &= ~BIT5; // SLOUPEC 1 do stavu 0
      P4OUT &= ~BIT2; // SLOUPEC 2 do stavu 0
      P4OUT &= ~BIT3; // SLOUPEC 3 do stavu 0
      P4OUT &= ~BIT0; // SLOUPEC 4 do stavu 0 
      P4OUT &= ~BIT1; // SLOUPEC 5 do stavu 0
      P3OUT &= ~BIT6; // SLOUPEC 6 do stavu 0
      P3OUT &= ~BIT7; // SLOUPEC 7 do stavu 0           

      for(column=0; column<8; column++) {
            if(display[row][column][0] == 1) {
              // ZAPNUTI AKTUALNIHO SLOUPCE
              if(column == 0)
                P4OUT |= BIT4; // SLOUPEC 0 do stavu 1
              else if(column == 1)
                P4OUT |= BIT5; // SLOUPEC 1 do stavu 1
              else if(column == 2)
                P4OUT |= BIT2; // SLOUPEC 2 do stavu 1
              else if(column == 3)
                P4OUT |= BIT3; // SLOUPEC 3 do stavu 1
              else if(column == 4)
                P4OUT |= BIT0; // SLOUPEC 4 do stavu 1
              else if(column == 5)
                P4OUT |= BIT1; // SLOUPEC 5 do stavu 1
              else if(column == 6)
                P3OUT |= BIT6; // SLOUPEC 6 do stavu 1
              else if(column == 7)
                P3OUT |= BIT7; // SLOUPEC 7 do stavu 1
            }
          }
            // Zapnuti aktuaniho radku   
            switch(row){
              case 0:
                  P1OUT &= ~BIT6; // RADEK 0 do stavu 0
                  break;
              case 1:
                  P2OUT &= ~BIT7; // RADEK 1 do stavu 0
                  break;
              case 2:
                  P2OUT &= ~BIT6; // RADEK 2 do stasvu 0
                  break;
              case 3:
                  P2OUT &= ~BIT5; // RADEK 3 do stavu 0
                  break; 
              case 4:
                  P2OUT &= ~BIT4; // RADEK 4 do stavu 0
                  break;  
              case 5:
                  P2OUT &= ~BIT3; // RADEK 5 do stavu 0
                  break; 
              case 6:
                  P2OUT &= ~BIT0; // RADEK 6 do stavu 0
                  break;  
              case 7:
                  P2OUT &= ~BIT1; // RADEK 7 do stavu 0 
                  break;       
              default:
                  break;
              }
              delay_ms(1);  
   }

      /*
      *  VYPNUTI VSECH RADKU
      */
      P1OUT |= BIT6; // RADEK 0 do stavu 1
      P2OUT |= BIT7; // RADEK 1 do stavu 1
      P2OUT |= BIT6; // RADEK 2 do stavu 1
      P2OUT |= BIT5; // RADEK 3 do stavu 1
      P2OUT |= BIT4; // RADEK 4 do stavu 1
      P2OUT |= BIT3; // RADEK 5 do stavu 1
      P2OUT |= BIT0; // RADEK 6 do stavu 1
      P2OUT |= BIT1; // RADEK 7 do stavu 1  

      // VYPNUTI VSECH SLOUPCU
      P4OUT &= ~BIT4; // SLOUPEC 0 do stavu 0
      P4OUT &= ~BIT5; // SLOUPEC 1 do stavu 0
      P4OUT &= ~BIT2; // SLOUPEC 2 do stavu 0
      P4OUT &= ~BIT3; // SLOUPEC 3 do stavu 0
      P4OUT &= ~BIT0; // SLOUPEC 4 do stavu 0 
      P4OUT &= ~BIT1; // SLOUPEC 5 do stavu 0
      P3OUT &= ~BIT6; // SLOUPEC 6 do stavu 0
      P3OUT &= ~BIT7; // SLOUPEC 7 do stavu 0 
}

// ZOBRAZENI ZACATKU A PO CHVILI SPUSTENI VYVOJE
void potvrzeni() {
	LCD_clear();
	LCD_append_string("CEKEJ");
	last_ch = '-';
	int i;
	for(i = 0; i < 150; i++) {
		printDisplay();
		keyboard();
		if(last_ch == '0')
			reset();
	}
	game();
}

// POCATECNI STAV
void reset() {
	last_ch = '-';
	LCD_clear();
	LCD_append_string("VYBER 1-9");
	keyboard();
	while(1) {
		printDisplay();
	  	keyboard();
		if (last_ch == '1') {
			set();
			potvrzeni();
		}
		else if (last_ch == '2') {
			set2();
			potvrzeni();
		}
		else if (last_ch == '3') {
			set3();
			potvrzeni();
		}
		else if (last_ch == '4') {
			set4();
			potvrzeni();
		}
		else if (last_ch == '5') {
			set5();
			potvrzeni();
		}
		else if (last_ch == '6') {
			set6();
			potvrzeni();
		}
		else if (last_ch == '7') {
			set7();
			potvrzeni();
		}
		else if (last_ch == '8') {
			set8();
			potvrzeni();
		}
		else if (last_ch == '9') {
			set9();	  
			potvrzeni();	
		}
	}	
}

/*
* ZMENA RYCHLOSTI
*/
void rychlost_plus() {
	term_send_str("Zvysuji rychlost\n");
	int i;
	if(rychlost == rychlost_mala) {
		rychlost = rychlost_velka;
	}
}

void rychlost_minus() {
	term_send_str("Snizuji rychlost\n");
	int i;
	if(rychlost == rychlost_velka) {
		rychlost = rychlost_mala;
	}
}

// REAKCE NA TLACITKA PRI PROBIHAJICIM VYVOJI
void game() {
	LCD_clear();
    LCD_append_string("HRA");
    keyboard();
    int i;
    while(1) {
    	keyboard();
    	if(last_ch == 'A') {
    		pause();
    	}
    	else if(last_ch == '0') {
    		reset();
    	} 
    	else if(last_ch == 'D') {
    		rychlost_plus();
    	}  
    	else if(last_ch == '#') {
    		rychlost_minus();
    	}
    	for(i = 0; i < rychlost; i++) {
	    	printDisplay(); // ZOBRAZENI
	    	keyboard();
	    	if(last_ch == 'A') {
	    		pause();
	    	} 
	    	else if(last_ch == '0') {
	    		reset();
	    	}  
	    	else if(last_ch == 'D') {
	    		rychlost_plus();
	    	} 
	    	else if(last_ch == '#') {
    			rychlost_minus();
    		}
   		}    
	    behave();
	    for(i = 0; i < rychlost; i++) {
	    	printDisplay(); // ZOBRAZENI
	      	keyboard();
	     	 if(last_ch == 'A') {
	    		pause();
	    	} 
	    	else if(last_ch == '0') {
	    		reset();
	    	} 
	    	else if(last_ch == 'D') {
	    		rychlost_plus();
	    	}  
	    	else if(last_ch == '#') {
    			rychlost_minus();
    		}
	    }  
	    keyboard();
	    if(last_ch == '0') {
	    	reset();
	    }  
	    if(last_ch == 'A') {
	    	pause();
	    }  
	    else if(last_ch == 'D') {
	    	rychlost_plus();
	    } 
	    else if(last_ch == '#') {
    		rychlost_minus();
    	}
	}	
}

// PAUZA
void pause() {
	term_send_str("pauza\n");
	int i;
	while(1) {
		printDisplay();
		keyboard();
		if(last_ch == 'B') {
			behave();
			for(i = 0; i < 30; i++) {
				printDisplay();
			}
		}
		else if(last_ch == 'C') {
			game();
		}
		else if(last_ch == '0') {
			reset();
    	}  
    	else if(last_ch == 'D') {
    		rychlost_plus();
    	}
    	else if(last_ch == '#') {
    		rychlost_minus();
    	}  

	}
}

int main() {
	last_ch = '-';
	int i;

	init();
	initialize_hardware();
	keyboard_init();   

	term_send_str("Start\n");

	// test - jestli funguji diody
	for(i = 0; i< 20000; i++) {
		print(); // vsechny diody
	}
	
	reset();
	game();

	return 0;
}
