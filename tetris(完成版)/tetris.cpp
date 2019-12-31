#include<stdio.h>
#include<windows.h>
#include<conio.h>
#include<time.h>


//�X�e�[�W�ƃt�B�[���h���`

int field[21][12] = { 0 };

int stage[21][12] = { 0 };



//�u���b�N�̒�`

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



//�����u���b�N�̏������W�ݒ�

int x = 4;

int y = 0;



//�Q�[���I�[�o�[�t���O

int gameover = 0;


//�����ɏ��������C����
int lines = 0;

//���v�̃��C����
int totallines = 0;

//�v���g�^�C�v�錾

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

	//�t�B�[���h�̏�����

	fieldformat();


	//�Q�[���I�[�o�[�܂ŏ����p��

	while (!gameover) {
		//���̓`�F�b�N
		if (_kbhit()) {
			controlBlock();
		}

		//�X���[�v
		Sleep(250);

		dropBlock();

	}

	showGamefield();

	return 0;

}



//��ʂ̏�����-------------------------------

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

	//�u���b�N�̐���

	createBlock();

	//��ʂ̕`��

	showGamefield();

}



//�u���b�N�̐���------------------------------------

int createBlock() {

	int i, j;
	int block_type = 0;


	x = 4;
	y = 0;

	//��������
	srand((unsigned)time(NULL));
	block_type = rand() % 7;

	//�u���b�N���X�g����u���b�N�z��֓ǂݍ���
	for(i=0;i<4;i++){
		for(j=0;j<4;j++){
			block[i][j] = 0;
			block[i][j] = block_list[block_type][i][j];
		}
	}


	for (i = 0;i < 4;i++) {

		for (j = 0;j < 4;j++) {

			field[i][j + 4] = stage[i][j + 4] + block[i][j];

			//�����ʒu�ɒu�����u���b�N�����ɌŒ�u���b�N�ɏd�Ȃ��Ă���΃Q�[���I�[�o�[

			if (field[i][j + x] > 1) {

				gameover = 1;

				return 1;

			}

		}

	}
	showGamefield();
	return 0;

}



//��ʕ\��---------------------------

void showGamefield() {

	int i, j;



	//��ʂ��N���A

	system("cls");



	//�t�B�[���h�̕`��

	for (i = 0;i < 21;i++) {

		for (j = 0;j < 12;j++) {

			switch (field[i][j]) {

			case 0:

				printf("�@");

				break;

			case 1:

				printf("��");

				break;

			default:

				printf("��");

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



//�u���b�N�̈ړ��p�֐�

void moveBlock(int x2, int y2) {

	int i, j;

	//�������O�̃u���b�N������

	for (i = 0;i < 4;i++) {

		for (j = 0;j < 4;j++) {

			field[y + i][x + j] -= block[i][j];

		}

	}

	//�u���b�N�̍��W�X�V	

	x = x2;

	y = y2;



	//�V�K���W���t�B�[���h�ɐݒ�

	for (i = 0;i < 4;i++) {

		for (j = 0;j < 4;j++) {

			field[y + i][x + j] += block[i][j];

		}

	}

	showGamefield();

}



//�Փ˔���

int checkOverlap(int x2, int y2) {

	int i, j;

	//�ǂ܂��͌Œ�u���b�N�̏Փ˔���

	for (i = 0;i < 4;i++) {

		for (j = 0;j < 4;j++) {

			//�u���b�N�Ȃ�

			if (block[i][j]) {

				//�ړ�����W��0�łȂ���΁i�󂫂łȂ���΁j

				if (stage[y2 + i][x2 + j] != 0) {

					return 1;

				}

			}

		}

	}

	return 0;

}



//�u���b�N�Œ�-----------------------------------

void lockBlock() {

	int i, j;

	//�u���b�N�����b�N

	for (i = 0;i < 21;i++) {

		for (j = 0;j < 12;j++) {

			stage[i][j] = field[i][j];

		}

	}
	//����񂪑����Ă��邩����
	checkLines();

	//���������C�����̍��v��ۑ�
	totallines += lines;

	for(i=0;i<21;i++){
		for(j=0;j<12;j++){
			field[i][j]=stage[i][j];
		}
	}
}

//�u���b�N�R���g���[���p�֐�
void controlBlock() {
	int key;
	key =_getch();

	//�L�[�ɂ���ĕ���
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

//�u���b�N�̉�]�����p�֐�
int turnBlock() {
	int i, j;
	int tmp[4][4] = { 0 };

	//��]�O�u���b�N���ꎞ�ۑ�
	for (i = 0;i < 4;i++) {
		for (j = 0;j < 4;j++) {
			tmp[i][j] = block[i][j];
		}
	}
	//�u���b�N����]
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


//���C�������p
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
		
		//���C���������Ă��Ȃ���΃��[�v�𔲂���
		if(complete==0){
			break;
		}
		lines++;

		//��̏���
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


//�u���b�N�����p
void dropBlock(){
	if(!checkOverlap(x,y+1)){
		moveBlock(x,y+1);
	}else{
		lockBlock();
		createBlock();
		Sleep(100);
	}
}
