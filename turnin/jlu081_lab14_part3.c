/*	Author: Jessie Lu
 *  Partner(s) Name: 
 *	Lab Section: 23
 *	Assignment: Lab #14  Exercise #3
 *	Exercise Description: Doesn't include advancement 1 or 2. This advancement deals with the making the AI more difficult to beat. The controls for the user are still the same as advancement 1. By having the AI occupy the middle more make its harder to beat the AI since the user has to try and aim towards the corner. It is also hard to control where the ball goes since the AI uses a random number generator.
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *
 *	Demo link: https://youtu.be/47XN-1JVnWc
 *
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#include "timer.h"
#include "stdlib.h"
#include "time.h"
#endif

// ball position
unsigned char B3, D3;

unsigned char B1, D1;
unsigned char pos1[2] = {};
enum player_States { player_start } player_state;
void playerTick() {
	unsigned char A = ~PINA & 0x03;
	// starting point of player paddles
        static unsigned char pattern1 = 0x80;    // LED pattern - 0: LED off; 1: LED on
        // row of player paddle
        static unsigned char row1 = 0xF8;      // Row(s) displaying pattern.

        switch(player_state) {
                case player_start:
			if (A == 0x01) { // up
				if (row1 != 0xE3) {
					row1 = (row1 << 1) | 0x01;
				}
			}
			else if (A == 0x02) { // down
				if (row1 != 0xF8) {
					row1 = (row1 >> 1) | 0x80;
				}
			}
			else if (A == 0x03 || A == 0x00) {
				pattern1 = pattern1;
				row1 = row1;
			}

			if (row1 == 0xF8) { // first three LEDs
				pos1[0] = 0xFE;
				pos1[1] = 0xFD;
				pos1[2] = 0xFB;
			}
			else if (row1 == 0xF1) { // middle three LEDs
				pos1[0] = 0xFD;
				pos1[1] = 0xFB;
				pos1[2] = 0xF7;
			}
			else if (row1 == 0xE3) { // last three LEDs
				pos1[0] = 0xFB;
				pos1[1] = 0xF7;
				pos1[2] = 0xEF;
			}

			B1 = pattern1;
			D1 = row1;
			player_state = player_start;
                        break;

		default:
			break;
	}
}

unsigned char i;
unsigned char B2, D2;
unsigned char pos2[2];
enum AI_States { AI_start } AI_state;
void AITick() {
	// starting point of AI paddles
        static unsigned char pattern2 = 0x01;    // LED pattern - 0: LED off; 1: LED on
        // row of AI paddle
        static unsigned char row2 = 0xF1;      // Row(s) displaying pattern.
	
	switch(AI_state) {
		case AI_start:
			i = rand();
			if (i%2 == 0 || i%7 == 0) {
				if (row2 != 0xE3) {
					row2 = (row2 << 1) | 0x01;
				}
			}
			else if (i%3 == 0 || i%5 == 0) {
				if (row2 != 0xF8) {
					row2 = (row2 >> 1) | 0x80;
				}
			}
			else {
				pattern2 = pattern2;
				row2 = row2;
			}

			if (row2 == 0xF8) { // first three LEDs
				pos2[0] = 0xFE;
				pos2[1] = 0xFD;
				pos2[2] = 0xFB;
			}
			else if (row2 == 0xF1) { // middle three LEDs
				pos2[0] = 0xFD;
				pos2[1] = 0xFB;
				pos2[2] = 0xF7;
			}
			else if (row2 == 0xE3) { // last three LEDs
				pos2[0] = 0xFB;
				pos2[1] = 0xF7;
				pos2[2] = 0xEF;
			}

			B2 = pattern2;
			D2 = row2;
			AI_state = AI_start;
			break;

		default:
			break;
	}
}

unsigned char LRprev;
enum ball_States { ball_start, ball_checkplayer, ball_checkAI, ball_left, ball_right, ball_lup, ball_rup, ball_ldown, ball_rdown } ball_state;
void ballTick() {
	// starting point of ball
        static unsigned char pattern3 = 0x08;    // LED pattern - 0: LED off; 1: LED on
        // row of ball
        static unsigned char row3 = 0xFB;      // Row(s) displaying pattern.

	switch(ball_state) {
		case ball_start:
			B3 = 0x08;
			D3 = 0xFB;
			pattern3 = B3;
			row3 = D3;
			ball_state = ball_left;
			break;

		case ball_left:
			LRprev = ball_left;
			if (pattern3 != 0x40) { // if not column before paddle
				pattern3 = pattern3 << 1;
				ball_state = ball_left;
			}
			else if (pattern3 == 0x40) { // if column before paddle
				if (row3 == (D1 | row3)) { // if hits player's paddle
					ball_state = ball_checkplayer;
				}
				else { // if doesn't hit paddle
					pattern3 = pattern3 << 1;
					ball_state = ball_start;
				}
			}
			B3 = pattern3;
			D3 = row3;
			break;

		case ball_right:
			LRprev = ball_right;
                        if (pattern3 != 0x02) {
                                pattern3 = pattern3 >> 1;
                                ball_state = ball_right;
                        }
                        else if (pattern3 == 0x02) { // column before paddle
                                if (row3 == (D2 | row3)) { // hits AI's paddle
                                        ball_state = ball_checkAI;
                                }
                                else { // misses paddle
                                        pattern3 = pattern3 >> 1;
                                        ball_state = ball_start;
                                }
                        }
                        B3 = pattern3;
                        D3 = row3;
			break;

		case ball_lup:
			LRprev = ball_lup;
			if (pattern3 == 0x40) { // check if ball is in front of paddle
				if (row3 == (D1 | row3)) { // check if player's hits paddle
					ball_state = ball_checkplayer;
				}
				else { // ball misses paddle
					pattern3 = pattern3 << 1;
					row3 = (row3 >> 1) | 0x80;
					ball_state = ball_start;
				}
			}
			else if (row3 == 0xFE && pattern3 != 0x40) { // if top most row needs to bounce
				ball_state = ball_ldown;
			}
			else if (row3 != 0xFE) { // if not top most row
				row3 = (row3 >> 1) | 0x80;
				pattern3 = pattern3 << 1;
				ball_state = ball_lup;
			}
			B3 = pattern3;
                        D3 = row3;
			break;

		case ball_ldown:
			LRprev = ball_ldown;
			if (pattern3 == 0x40) { // if ball is right before paddle
				if (row3 == (D1 | row3)) { // check if it hits player's paddle
					ball_state = ball_checkplayer;
				}
				else { // ball misses paddle
					pattern3 = pattern3 << 1;
					row3 = (row3 << 1) | 0x01;
				}
			}
			else if (row3 == 0xEF && pattern3 != 0x40) { // if bottom row needs to bounce
				ball_state = ball_lup;
			}
			else if (row3 != 0xEF) { // not bottom row
				row3 = (row3 << 1) | 0x01;
				pattern3 = pattern3 << 1;
				ball_state = ball_ldown;
			}
			B3 = pattern3;
                        D3 = row3;
			break;

		case ball_rup:
			LRprev = ball_rup;
                        if (pattern3 == 0x02) { // check if ball is in front of paddle
                                if (row3 == (D2 | row3)) { // check if AI's hits paddle
                                        ball_state = ball_checkAI;
                                }
                                else { // ball misses paddle
                                        pattern3 = pattern3 >> 1;
                                        row3 = (row3 >> 1) | 0x80;
                                        ball_state = ball_start;
                                }
                        }
                        else if (row3 == 0xFE && pattern3 != 0x02) { // if top most row needs to bounce
                                ball_state = ball_rdown;
                        }
                        else if (row3 != 0xFE && pattern3 != 0x02) { // if not top most row
                                row3 = (row3 >> 1) | 0x80;
                                pattern3 = pattern3 >> 1;
                                ball_state = ball_rup;
                        }
			B3 = pattern3;
                        D3 = row3;
			break;

		case ball_rdown:
			LRprev = ball_rdown;
                        if (pattern3 == 0x02) { // if ball is right before paddle
				if (row3 == (D2 | row3)) { // check if AI's paddle gets hits
                                	ball_state = ball_checkAI;
				}
				else { // ball misses
					pattern3 = pattern3 >> 1;
					row3 = (row3 << 1) | 0x01;
					ball_state = ball_start;
				}
                        }
                        else if (row3 == 0xEF && pattern3 != 0x02) { // if bottom row needs to bounce
                                ball_state = ball_rup;
                        }
                        else if (row3 != 0xEF) { // not bottom row
                                row3 = (row3 << 1) | 0x01;
                                pattern3 = pattern3 >> 1;
                                ball_state = ball_rdown;
                        }
			B3 = pattern3;
                        D3 = row3;
			break;

		case ball_checkplayer:
			if (row3 == pos1[0]) { // hits upper portion of paddle
				if (LRprev == ball_left) {

					ball_state = ball_rdown;
				}
				else if (LRprev == ball_lup) {
					if (row3 == 0xFE) {
						ball_state = ball_rdown;
					}
					else {
						ball_state = ball_rup;
					}
				}
				else if (LRprev == ball_ldown) {
					if (row3 == 0xEF) {
						ball_state = ball_rup;
					}
					else {
						ball_state = ball_rdown;
					}
				}
			}
			else if (row3 == pos1[1]) { // hits middle portion of paddle
				ball_state = ball_right;
			}
			else if (row3 == pos1[2]) { // hits lower portion of paddle
				if (LRprev == ball_left) {
					ball_state = ball_rup;
				}
				else if (LRprev == ball_ldown) {
					if (row3 == 0xEF) {
						ball_state = ball_rup;
					}
					else {
						ball_state = ball_rdown;
					}
				}
				else if (LRprev == ball_lup) {
					if (row3 == 0xFE) {
						ball_state = ball_rdown;
					}
					else {
						ball_state = ball_rup;
					}
				}
			}
			break;
		case ball_checkAI:
			if (row3 == pos2[0]) { // hits upper portion of paddle
				if (LRprev == ball_right) {
					ball_state = ball_ldown;
				}
				else if (LRprev == ball_rup) {
					if (row3 == 0xFE) {
						ball_state = ball_ldown;
					}
					else {
						ball_state = ball_lup;
					}
				}
				else if (LRprev == ball_rdown) {
					if (row3 == 0xFE) {
						ball_state = ball_lup;
					}
					else {
						ball_state = ball_ldown;
					}
				}
			}
			else if (row3 == pos2[1]) { // hits middle portion of paddle
                                ball_state = ball_left;
			}
			else if (row3 == pos2[2]) { // hits lower portion of paddle
				if (LRprev == ball_right) {
					ball_state = ball_lup;
				}
				else if (LRprev == ball_rdown) {
					if (row3 == 0xEF) {
						ball_state = ball_lup;
					}
					else {
						ball_state = ball_ldown;
					}
				}
				else if (LRprev == ball_rup) {
					if (row3 == 0xFE) {
						ball_state = ball_ldown;
					}
					else {
						ball_state = ball_lup;
					}
				}
			}
			break;
		

		default:
			break;
	}	
}

unsigned char dis, B, D;
enum display_States { display_start, display_player, display_AI, display_ball } display_state;
void displayTick() {
	switch(display_state) {
		case display_start:
			B = 0x00;
			D = 0x00;
			display_state = display_player;
			break;

		case display_player:
			B = B1;
			D = D1;
			display_state = display_AI;
			break;

		case display_AI:
			B = B2;
			D = D2;
			display_state = display_ball;
			break;

		case display_ball:
			B = B3;
			D = D3;
			display_state = display_player;
			break;

		default:
			break;
	}
}

int main(void) {
	DDRA = 0x00; PORTA = 0xFF;
	// Used PORTB instead of PORTC because PORTC on my microcontroller does not work. Once I changed my microcontroller I decided to just keep it at PORTB.
	DDRB = 0xFF; PORTB = 0x00;
	DDRD = 0xFF; PORTD = 0x00;
	
	unsigned long player_time = 100; // 100 ms
	unsigned long AI_time = 200;
	unsigned long ball_time = 200; 
	unsigned long display_time = 1; // 1 ms
	const unsigned long timerPeriod = 1; // 1 ms

    	TimerSet(timerPeriod);
	TimerOn();

     	while (1) {
		if (player_time >= 100) {
			playerTick();
			player_time = 0;
		}
		if (AI_time >= 200) {
			AITick();
			AI_time = 0;
		}
		if (ball_time >= 200) {
			ballTick();
			ball_time = 0;
		}
		if (display_time >= 1) {
			displayTick();
			display_time = 0;
		}
                while(!TimerFlag) {}
                TimerFlag = 0;
		player_time += timerPeriod;
		AI_time += timerPeriod;
		ball_time += timerPeriod;
		display_time += timerPeriod;

		PORTB = B;
		PORTD = D;
     	}
     	return 1;
}
