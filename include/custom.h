#ifndef CUSTOM_H
#define CUSTOM_H

#define B 255
#define A 32

// 4x4 charset
unsigned char cc0[] = { // Custom Character 0
	0b00000, 0b00000, 0b00000, 0b00111, 0b01111, 0b11111, 0b11111, 0b11111};

unsigned char cc1[] = { // Custom Character 1*/
	0b11100, 0b11110, 0b11111, 0b11111, 0b11111, 0b11111, 0b11111, 0b11111};

unsigned char cc2[] = { // Custom Character 2*/
	0b11111, 0b11111, 0b11111, 0b11111, 0b11111, 0b00000, 0b00000, 0b00000};

unsigned char cc3[] = { // Custom Character 3*/
	0b00000, 0b00000, 0b00000, 0b11111, 0b11111, 0b11111, 0b11111, 0b11111};

unsigned char cc4[] = { // Custom Character 4*/
	0b11111, 0b11111, 0b11111, 0b11111, 0b11111, 0b11111, 0b01111, 0b00111};

unsigned char cc5[] = { // Custom Character 5*/
	0b00111, 0b01111, 0b11111, 0b11111, 0b11111, 0b11111, 0b11111, 0b11111};

unsigned char cc6[] = { // Custom Character 6*/
	0b00000, 0b00000, 0b00000, 0b11100, 0b11110, 0b11111, 0b11111, 0b11111};

unsigned char cc7[] = { // Custom Character 7*/
	0b11111, 0b11111, 0b11111, 0b11111, 0b11111, 0b11111, 0b11110, 0b11100};

//			   			0            1            2            3            4            5            6            7            8            9
unsigned char bn1[] = {5, 2, 2, 1, 0, 5, B, A, 5, 2, 2, 1, 2, 2, 2, 1, 5, A, A, B, B, 2, 2, 2, 5, 2, 2, 2, 2, 2, 2, B, 5, 2, 2, 1, 5, 2, 2, 1};
unsigned char bn2[] = {B, A, A, B, A, A, B, A, A, A, A, B, A, 3, 3, B, B, 3, 3, B, B, 3, 3, 6, B, 3, 3, 6, A, A, 0, 7, B, 3, 3, B, 4, 3, 3, B};
unsigned char bn3[] = {B, A, A, B, A, A, B, A, 5, 2, 2, 2, A, A, A, B, A, A, A, B, A, A, A, B, B, A, A, B, A, A, B, A, B, A, A, B, A, A, A, B};
unsigned char bn4[] = {4, 3, 3, 7, 3, 3, B, 3, B, 3, 3, 3, 4, 3, 3, 7, A, A, A, B, 4, 3, 3, 7, 4, 3, 3, 7, A, A, B, A, 4, 3, 3, 7, 4, 3, 3, 7};

#endif
