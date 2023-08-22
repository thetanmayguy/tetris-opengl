// piece.cpp

#include "Piece.h"

Piece::Piece() {
	Piece(0);
}

/*
Creating a new piece, setting its type, color and base rotation
*/
Piece::Piece(int numType) {
	type = numType;
	switch (type) {
		// r,b,g values :-
	case 0: color(1.0, 0.0, 1.0); break; // square    => yellow
	case 1: color(1.0, 0.0, 0.0); break; // line      => red
	case 2: color(0.0, 1.0, 1.0); break; // T shape   => cyan
	case 3: color(0.0, 1.0, 0.0); break; // L shape   => blue
	case 4: color(0.0, 0.0, 1.0); break; // reverse L => green
	case 5: color(0.5, 1.0, 0.0); break; // Z shape   => violet
	case 6: color(1.0, 1.0, 0.0); break; // reverse Z => magenta
	}
	rotation = 0;
}

/*
Getting an array with information about a specific piece based on its rotation
*/
const int* Piece::rotations() {
	return gamePieces[type][rotation];
}

/*
Setting the color of a piece
*/
void Piece::color(float r, float g, float b) {
	redVal = r;
	greenVal = g;
	blueVal = b;
}

/*
Increase or decrease the rotation index of game pieces
*/
void Piece::rotatePiece(int dir) {
	if(dir > 0) {
		if (rotation == 3)
			rotation = 0;
		else
			rotation += dir;
	}
	else {
		if (rotation == 0)
			rotation = 3;
		else
			rotation += dir;
	}
}
