
#include <stdio.h>
#include <ctype.h>

#define START_X 8
#define START_Y 4

#define SIZE_X 17
#define SIZE_Y 9

#define STRING_LEN 32

unsigned char field[SIZE_X][SIZE_Y] = {0};

unsigned char values[] = {
	' ', '.', 'o', '+', '=', '*', 'B', '0', 'X', '@', '%', '&', '#', '/', '^', 'S', 'E'
};

unsigned char hex(unsigned char c){
	if(c >= '0' && c <= '9'){
		return c-'0';
	}else if(c >= 'a' && c <= 'f'){
		return c - 'a' + 10;
	}else if(c >= 'A' && c <= 'F'){
		return c - 'A' + 10;
	}
	return -1;
}

int main(){
	int x = START_X;
	int y = START_Y;
	
	for(int i = 0; i < STRING_LEN/2; i++){
		unsigned char word[2] = {0};
		for(int j = 0; j < 2; j++){
			unsigned char c = getchar();
			if(!isxdigit(c)){
				j--;
				continue;
			}
			word[j] = hex(c);
		}
		unsigned char move[4] = {0};
		move[0] = word[1] & 0x03;
		move[1] = word[1] >> 2;
		move[2] = word[0] & 0x03;
		move[3] = word[0] >> 2;
		
		for(int j = 0; j < 4; j++){
			if(move[j] & 0b10){
				y++;
			}else{
				y--;
			}
			if(move[j] & 0b01){
				x++;
			}else{
				x--;
			}
		
			if(x > SIZE_X-1){
				x = SIZE_X-1;
			}else if(x < 0){
				x = 0;
			}
			if(y > SIZE_Y-1){
				y = SIZE_Y-1;
			}else if(y < 0){
				y = 0;
			}
			field[x][y]++;
		}
	}
		
	field[START_X][START_Y] = 15;
	field[x][y] = 16;
	
	printf("+-----------------+\n");
	for(y = 0; y < SIZE_Y; y++){
		putchar('|');
		for(x = 0; x < SIZE_X; x++){
			unsigned char count = field[x][y];
			if(count > 16){
				count = 14;
			}
			putchar(values[count]);
		}
		printf("|\n");
	}
	printf("+-----------------+\n");
}

