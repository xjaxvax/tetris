#include<stdio.h>
#include<unistd.h>

//ステージとフィールドを定義
int field[21][12]={0};
int stage[21][12]={0};

//ブロックの定義
int block[4][4]={{0,1,0,0},
		 {0,1,0,0},
		 {0,1,0,0},
		 {0,1,0,0}};

//落下ブロックの初期座標設定
int x=4;
int y=0;

//ゲームオーバーフラグ
int gameover=0;

//プロトタイプ宣言
void fieldformat();
int createBlock();
void showGamefield();
void moveBlock(int,int);
int checkOverlap(int,int);
void lockBlock();

int main(){
	//フィールドの初期化
	fieldformat();

	//ゲームオーバーまで処理継続
	while(!gameover){
	int second=1;
	sleep(second);
	if(!checkOverlap(x,y+1)){
		moveBlock(x,y+1);	
	}else{
		lockBlock();
		createBlock();
	}
	}

	return 0;	
}

//画面の初期化-------------------------------
void fieldformat(){
	int i,j;

	for(i=0;i<21;i++){
		for(j=0;j<12;j++){
			if(j==0||j==11||i==20){
				stage[i][j]=field[i][j]=9;
			}else{
				stage[i][j]=field[i][j]=0;

			}
		}
	}
	//ブロックの生成
	createBlock();
	//画面の描画
	showGamefield();
}

//ブロックの生成------------------------------------
int createBlock(){
	int i,j;
	for(i=0;i<4;i++){
		for(j=0;j<4;j++){
			field[i][j+4]=stage[i][j+4]+block[i][j];
//初期位置に置いたブロックが既に固定ブロックに重なっていればゲームオーバー
			if(field[i][j+x]>1){
				gameover=1;
				return 1;
			}
		}
	}
	return 0;
}

//画面表示---------------------------
void showGamefield(){
	int i,j;

	//画面をクリア
	printf("\x1b[2J");
	
	//フィールドの描画
	for(i=0;i<21;i++){
		for(j=0;j<12;j++){
			switch(field[i][j]){
				case 0:
					printf("　");
					break;
				case 1:
					printf(" □");
					break;	
				default:
					printf("⬛");
					break;
			}
		}
		printf("\n");
	}
	if(gameover){
		printf("\x1b[2J");
		printf("GAME OVER");
	}
}

//ブロックの移動用関数
void moveBlock(int x2,int y2){
	int i,j;
	//落下直前のブロックを消去
	for(i=0;i<4;i++){
		for(j=0;j<4;j++){
			field[y+i][x+j]-=block[i][j];
		}
	}
	//ブロックの座標更新	
	x=x2;
	y=y2;

	//新規座標をフィールドに設定
	for(i=0;i<4;i++){
		for(j=0;j<4;j++){
			field[y+i][x+j]+=block[i][j];
		}
	}
		showGamefield();
}	

//衝突判定
int checkOverlap(int x2,int y2){
	int i,j;
	//壁または固定ブロックの衝突判定
	for(i=0;i<4;i++){
		for(j=0;j<4;j++){
			//ブロックなら
			if(block[i][j]){
				//移動先座標が0でなければ（空きでなければ）
				if(stage[y2+i][x2+j]!=0){
					return 1;
				}
			}
		}
	}
	return 0;
}

//ブロック固定-----------------------------------
void lockBlock(){
	int i,j;
	//ブロックをロック
	for(i=0;i<21;i++){
		for(j=0;j<12;j++){
			stage[i][j]=field[i][j];
		}
	}
}
