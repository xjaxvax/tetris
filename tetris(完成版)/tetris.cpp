#include<stdio.h>
#include<windows.h>
#include<conio.h>
#include<time.h>


//ステージとフィールドを定義

int field[21][12] = { 0 };

int stage[21][12] = { 0 };



//ブロックの定義

int block[4][4]= {0};
int block_list[7][4][4] = {
			{ {0,1,0,0},

			 {0,1,0,0},

			 {0,1,0,0},

			 {0,1,0,0} },


			{ {0,1,0,0},

			 {0,1,1,0},

			 {0,0,1,0},

			 {0,0,0,0} },



			{ {0,0,1,0},

			 {0,1,1,0},

			 {0,1,0,0},

			 {0,0,0,0} },


			{ {0,1,0,0},

			 {0,1,0,0},

			 {0,1,1,0},

			 {0,0,0,0} },


			{ {0,0,1,0},

			 {0,0,1,0},

			 {0,1,1,0},

			 {0,0,0,0} },

			
			{ {0,1,0,0},

			 {1,1,1,0},

			 {0,0,0,0},

			 {0,0,0,0} },


			{ {0,1,1,0},

			 {0,1,1,0},

			 {0,0,0,0},

			 {0,0,0,0} },

};



//落下ブロックの初期座標設定

int x = 4;

int y = 0;



//ゲームオーバーフラグ

int gameover = 0;


//同時に消したライン数
int lines = 0;

//合計のライン数
int totallines = 0;

//プロトタイプ宣言

void fieldformat();

int createBlock();

void showGamefield();

void moveBlock(int, int);

int checkOverlap(int, int);

void lockBlock();

void controlBlock();

int turnBlock();

void checkLines();

void dropBlock();

int main() {

	//フィールドの初期化

	fieldformat();


	//ゲームオーバーまで処理継続

	while (!gameover) {
		//入力チェック
		if (_kbhit()) {
			controlBlock();
		}

		//スリープ
		Sleep(250);

		dropBlock();

	}

	showGamefield();

	return 0;

}



//画面の初期化-------------------------------

void fieldformat() {

	int i, j;



	for (i = 0;i < 21;i++) {

		for (j = 0;j < 12;j++) {

			if (j == 0 || j == 11 || i == 20) {

				stage[i][j] = field[i][j] = 9;

			}
			else {

				stage[i][j] = field[i][j] = 0;



			}

		}

	}

	//ブロックの生成

	createBlock();

	//画面の描画

	showGamefield();

}



//ブロックの生成------------------------------------

int createBlock() {

	int i, j;
	int block_type = 0;


	x = 4;
	y = 0;

	//乱数生成
	srand((unsigned)time(NULL));
	block_type = rand() % 7;

	//ブロックリストからブロック配列へ読み込み
	for(i=0;i<4;i++){
		for(j=0;j<4;j++){
			block[i][j] = 0;
			block[i][j] = block_list[block_type][i][j];
		}
	}


	for (i = 0;i < 4;i++) {

		for (j = 0;j < 4;j++) {

			field[i][j + 4] = stage[i][j + 4] + block[i][j];

			//初期位置に置いたブロックが既に固定ブロックに重なっていればゲームオーバー

			if (field[i][j + x] > 1) {

				gameover = 1;

				return 1;

			}

		}

	}
	showGamefield();
	return 0;

}



//画面表示---------------------------

void showGamefield() {

	int i, j;



	//画面をクリア

	system("cls");



	//フィールドの描画

	for (i = 0;i < 21;i++) {

		for (j = 0;j < 12;j++) {

			switch (field[i][j]) {

			case 0:

				printf("　");

				break;

			case 1:

				printf("□");

				break;

			default:

				printf("■");

				break;

			}

		}

		printf("\n");

	}
	printf("Total Lines:%d",totallines);

	if (gameover) {

		system("cls");

		printf("\n\nGAME OVER!\n\n");

	}

}



//ブロックの移動用関数

void moveBlock(int x2, int y2) {

	int i, j;

	//落下直前のブロックを消去

	for (i = 0;i < 4;i++) {

		for (j = 0;j < 4;j++) {

			field[y + i][x + j] -= block[i][j];

		}

	}

	//ブロックの座標更新	

	x = x2;

	y = y2;



	//新規座標をフィールドに設定

	for (i = 0;i < 4;i++) {

		for (j = 0;j < 4;j++) {

			field[y + i][x + j] += block[i][j];

		}

	}

	showGamefield();

}



//衝突判定

int checkOverlap(int x2, int y2) {

	int i, j;

	//壁または固定ブロックの衝突判定

	for (i = 0;i < 4;i++) {

		for (j = 0;j < 4;j++) {

			//ブロックなら

			if (block[i][j]) {

				//移動先座標が0でなければ（空きでなければ）

				if (stage[y2 + i][x2 + j] != 0) {

					return 1;

				}

			}

		}

	}

	return 0;

}



//ブロック固定-----------------------------------

void lockBlock() {

	int i, j;

	//ブロックをロック

	for (i = 0;i < 21;i++) {

		for (j = 0;j < 12;j++) {

			stage[i][j] = field[i][j];

		}

	}
	//横一列が揃っているか判定
	checkLines();

	//消したライン数の合計を保存
	totallines += lines;

	for(i=0;i<21;i++){
		for(j=0;j<12;j++){
			field[i][j]=stage[i][j];
		}
	}
}

//ブロックコントロール用関数
void controlBlock() {
	int key;
	key =_getch();

	//キーによって分岐
	switch (key) {
	case 0x4b:
		if (!checkOverlap(x - 1, y)) {
			moveBlock(x - 1, y);
		}
		break;
	case 0x4d:
		if (!checkOverlap(x + 1, y)) {
			moveBlock(x + 1, y);
		}
		break;
	case 0x50:
		if (!checkOverlap(x, y + 1)) {
			moveBlock(x, y + 1);
		}
		break;
	case 0x20:
	case 0x0d:
		turnBlock();
		break;
	}
}

//ブロックの回転処理用関数
int turnBlock() {
	int i, j;
	int tmp[4][4] = { 0 };

	//回転前ブロックを一時保存
	for (i = 0;i < 4;i++) {
		for (j = 0;j < 4;j++) {
			tmp[i][j] = block[i][j];
		}
	}
	//ブロックを回転
	for (i = 0;i < 4;i++) {
		for (j = 0;j < 4;j++) {
			block[i][j] = tmp[3 - j][i];
		}
	}

	if (checkOverlap(x, y)) {
		for (i = 0;i < 4;i++) {
			for (j = 0;j < 4;j++) {
				block[i][j] = tmp[i][j];
			}
		}
		return 1;
	}

	for (i = 0;i < 4;i++) {
		for (j = 0;j < 4;j++) {
			field[y + i][x + j] -= tmp[i][j];
			field[y + i][x + j] += block[i][j];
		}
	}
	showGamefield();
	return 0;
}


//ライン消去用
void checkLines(){
	int i,j,k;
	int complete;
	lines = 0;

	while(1){
		for(i=0;i<20;i++){
			complete = 1;
			for(j=1;j<11;j++){
				if(stage[i][j]==0){
					complete = 0;
				}
			}
			if(complete==1){
				break;
			}
		}
		
		//ラインが揃っていなければループを抜ける
		if(complete==0){
			break;
		}
		lines++;

		//列の消去
		for(j=1;j<11;j++){
			stage[i][j]=0;
		}
		for(k=i;k>0;k--){
			for(j=1;j<11;j++){
				stage[k][j]=stage[k-1][j];
			}
		}
	}
}


//ブロック落下用
void dropBlock(){
	if(!checkOverlap(x,y+1)){
		moveBlock(x,y+1);
	}else{
		lockBlock();
		createBlock();
		Sleep(100);
	}
}
