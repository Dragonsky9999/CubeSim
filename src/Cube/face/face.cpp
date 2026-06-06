#include "src/Cube/face/face.h"

int getIndex(Face face) {
	switch (face) {
		case Face::U: return 0;
		case Face::D: return 1;
		case Face::F: return 2;
		case Face::B: return 3;
		case Face::R: return 4;
		case Face::L: return 5;
	}
	return -1;
}