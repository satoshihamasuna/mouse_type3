/*
 * search.c
 *
 *  Created on: 2021/06/20
 *      Author: sato1
 */

#include "index.h"
#include "mymacro.h"
#include "glob_var.h"

void init_map(int *x, int *y,int goal_size);				//歩数マップの初期化
void init_map_zenme(int *x, int *y,int goal_size);				//歩数マップの初期化
void make_map_zenmen(int *x, int *y,int size,int mask);
void set_wall(int x,int y);				//壁情報の保存
t_bool is_unknown(int x,int y);			//未探索の区間かの判定
t_bool i_am_goal(int x,int y,int *gx,int *gy,int goal_size);
int get_priority(int x, int y, t_direction dir);			//優先度を取得(未探索，前方優先)
int get_nextdir(int *x, int *y,int goal_size ,int mask, t_direction *dir);			//次に行く方向の取得
int get_nextdir_zenmen(int *x, int *y,int goal_size ,int mask, t_direction *dir);			//次に行く方向の取得
//void search_adachi(int gx,int gy,int size);		//足立法
int end_search = 0;

//迷路の初期化
void init_maze(){
	for( int i = 0 ; i < MAZE_SIZE_X ; i++ ){
		for( int j = 0 ; j < MAZE_SIZE_Y ; j++ ){
			wall[i][j].north = wall[i][j].east = wall[i][j].south = wall[i][j].west = UNKNOWN;
		}
	}

	for( int i = 0 ; i < MAZE_SIZE_X ; i++ ){
		wall[i][0].south = WALL;				//南側の壁を追加する
		wall[i][MAZE_SIZE_Y - 1].north = WALL;	//北側の壁を追加する
	}

	for( int j = 0 ; j < MAZE_SIZE_Y ; j++ ){
		wall[0][j].west = WALL;					//西側の壁を追加する
		wall[MAZE_SIZE_X - 1][j].east = WALL;	//東側の壁を追加する
	}

	wall[0][0].east = wall[1][0].west = WALL;				//スタートの東側の壁を追加

}

void init_map(int *x, int *y,int goal_size){
	for( int i = 0; i < MAZE_SIZE_X ; i++ ){
		for( int j = 0 ; j < MAZE_SIZE_Y ; j++ ){
			map[i][j] = 999;
		}
	}

	for(int i = 0;i < goal_size;i++){
		for(int j = 0;j < goal_size;j++){
			map[x[i]][y[j]] = 0;
		}
	}

}

void init_map_zenmen(int *x, int *y,int goal_size){
	if(end_search == 1){
		for( int i = 0; i < MAZE_SIZE_X ; i++ ){
			for( int j = 0 ; j < MAZE_SIZE_Y ; j++ ){
				map[i][j] = 999;
			}
		}
		for(int i = 0;i < goal_size;i++){
			for(int j = 0;j < goal_size;j++){
				map[x[i]][y[j]] = 0;
			}
		}
	}else{
		for( int i = 0; i < MAZE_SIZE_X ; i++ ){
			for( int j = 0 ; j < MAZE_SIZE_Y ; j++ ){
				if( is_unknown(i,j) == true)	map[i][j] = 0;
				else							map[i][j] = 999;
			}
		}
	}

}

t_bool i_am_goal(int x,int y,int *gx,int *gy,int goal_size){
	t_bool flag = false;
	for (int i = 0; i < goal_size;i++){
		for(int j = 0; j < goal_size;j++){
			if(x == gx[i] && y == gy[j]) flag = true;
		}
	}
	return flag;
}

void make_map(int *x, int *y,int size,int mask)	//歩数マップを作成する
{
//座標x,yをゴールとした歩数Mapを作成する。
//maskの値(MASK_SEARCH or MASK_SECOND)によって、
//探索用の歩数Mapを作るか、最短走行の歩数Mapを作るかが切り替わる
	t_bool change_flag;			//Map作成終了を見極めるためのフラグ
    init_map(x,y,size);				//Mapを初期化する

	do
	{
		change_flag = false;				//変更がなかった場合にはループを抜ける
		for(int i = 0; i < MAZE_SIZE_X; i++)			//迷路の大きさ分ループ(x座標)
		{
			for(int j = 0; j < MAZE_SIZE_Y; j++)		//迷路の大きさ分ループ(y座標)
			{
				if(map[i][j] == 999)		//999の場合は次へ
				{
					continue;
				}

				if(j < MAZE_SIZE_Y-1)					//範囲チェック
				{
					if( (wall[i][j].north & mask) == NOWALL)	//壁がなければ(maskの意味はstatic_parametersを参照)
					{
						if(map[i][j+1] == 999)			//まだ値が入っていなければ
						{
							map[i][j+1] = map[i][j] + 1;	//値を代入
							change_flag = true;		//値が更新されたことを示す
						}
					}
				}

				if(i < MAZE_SIZE_X-1)					//範囲チェック
				{
					if( (wall[i][j].east & mask) == NOWALL)		//壁がなければ
					{
						if(map[i+1][j] == 999)			//値が入っていなければ
						{
							map[i+1][j] = map[i][j] + 1;	//値を代入
							change_flag = true;		//値が更新されたことを示す
						}
					}
				}

				if(j > 0)						//範囲チェック
				{
					if( (wall[i][j].south & mask) == NOWALL)	//壁がなければ
					{
						if(map[i][j-1] == 999)			//値が入っていなければ
						{
							map[i][j-1] = map[i][j] + 1;	//値を代入
							change_flag = true;		//値が更新されたことを示す
						}
					}
				}

				if(i > 0)						//範囲チェック
				{
					if( (wall[i][j].west & mask) == NOWALL)		//壁がなければ
					{
						if(map[i-1][j] == 999)			//値が入っていなければ
						{
							map[i-1][j] = map[i][j] + 1;	//値を代入
							change_flag = true;		//値が更新されたことを示す
						}

					}

				}

			}

		}

	}while(change_flag == true);	//全体を作り終わるまで待つ

}

t_bool is_unknown(int x, int y)	//指定された区画が未探索か否かを判断する関数 未探索:true　探索済:false
{
	//座標x,yが未探索区間か否かを調べる

	if((wall[x][y].north == UNKNOWN) || (wall[x][y].east == UNKNOWN) || (wall[x][y].south == UNKNOWN) || (wall[x][y].west == UNKNOWN))
	{			//どこかの壁情報が不明のままであれば
		return true;	//未探索
	}
	else
	{
		return false;	//探索済
	}
}

void set_wall(int x, int y)	//壁情報を記録
{
//引数の座標x,yに壁情報を書き込む
	int n_write,s_write,e_write,w_write;
	n_write = 0;
	s_write = 0;
    e_write = 0;
    w_write = 0;
	//自分の方向に応じて書き込むデータを生成
	//CONV_SEN2WALL()はmacro.hを参照
	switch(mypos.dir){
		case north:	//北を向いている時

			n_write = CONV_SEN2WALL(sen_fr.is_wall || sen_fl.is_wall);	//　前壁の有無を判断
			e_write = CONV_SEN2WALL(sen_r.is_wall);				//右の有無を判断
			w_write = CONV_SEN2WALL(sen_l.is_wall);				//左壁の有無を判断
			s_write = NOWALL;						//後ろは必ず壁がない

			break;

		case east:	//東を向いているとき

			e_write = CONV_SEN2WALL(sen_fr.is_wall || sen_fl.is_wall);	//前壁の有無を判断
			s_write = CONV_SEN2WALL(sen_r.is_wall);				//右壁の有無を判断
			n_write = CONV_SEN2WALL(sen_l.is_wall);				//左壁の有無を判断
			w_write = NOWALL;						//後ろは必ず壁がない

			break;

		case south:	//南を向いているとき

			s_write = CONV_SEN2WALL(sen_fr.is_wall || sen_fl.is_wall);	//前壁の有無を判断
			w_write = CONV_SEN2WALL(sen_r.is_wall);				//右壁の有無を判断
			e_write = CONV_SEN2WALL(sen_l.is_wall);				//左壁の有無を判断
			n_write = NOWALL;						//後ろは必ず壁がない

			break;

		case west:	//西を向いているとき

			w_write = CONV_SEN2WALL(sen_fr.is_wall || sen_fl.is_wall);	//前壁の有無を判断
			n_write = CONV_SEN2WALL(sen_r.is_wall);				//右壁の有無を判断
			s_write = CONV_SEN2WALL(sen_l.is_wall);				//左壁の有無を判断
			e_write = NOWALL;						//後ろは必ず壁がない

			break;


	}

	wall[x][y].north = n_write;	//実際に壁情報を書き込み
	wall[x][y].south = s_write;	//実際に壁情報を書き込み
	wall[x][y].east  = e_write;	//実際に壁情報を書き込み
	wall[x][y].west  = w_write;	//実際に壁情報を書き込み

	if(y < MAZE_SIZE_Y-1)	//範囲チェック
	{
		wall[x][y+1].south = n_write;	//反対側から見た壁を書き込み
	}

	if(x < MAZE_SIZE_X-1)	//範囲チェック
	{
		wall[x+1][y].west = e_write;	//反対側から見た壁を書き込み
	}

	if(y > 0)	//範囲チェック
	{
        wall[x][y-1].north = s_write;	//反対側から見た壁を書き込み
	}

	if(x > 0)	//範囲チェック
	{
		wall[x-1][y].east = w_write;	//反対側から見た壁を書き込み
	}

}

int get_priority(int x, int y, t_direction dir){	//そのマスの情報から、優先度を算出する

	int priority;	//優先度を記録する変数
	priority = 0;

	if(mypos.dir == dir){				//行きたい方向が現在の進行方向と同じ場合
		priority = 2;
	}else if( ((4+mypos.dir-dir)%4) == 2){		//行きたい方向が現在の進行方向と逆の場合
		priority = 0;
	}else{						//それ以外(左右どちらか)の場合
		priority = 1;
	}

	if(is_unknown(x,y) == true){
		priority += 4;				//未探索の場合優先度をさらに付加
	}

	return priority;				//優先度を返す

}

int get_nextdir(int *x, int *y,int goal_size ,int mask, t_direction *dir){
    int little,priority,tmp_priority;

    make_map(x,y,goal_size,mask);

    little = 999;

    priority = 0;

    if((wall[mypos.x][mypos.y].north & mask) == NOWALL){
        tmp_priority = get_priority(mypos.x,mypos.y + 1,north);
        if(map[mypos.x][mypos.y+1] < little){
            little = map[mypos.x][mypos.y+1];
            *dir = north;
            priority = tmp_priority;
        }else if(map[mypos.x][mypos.y+1] == little){
            if(priority < tmp_priority){
                *dir = north;
                priority = tmp_priority;
            }
        }
    }

    if((wall[mypos.x][mypos.y].east & mask) == NOWALL){
        tmp_priority = get_priority(mypos.x + 1,mypos.y,east);
        if(map[mypos.x + 1][mypos.y] < little){
            little = map[mypos.x+1][mypos.y];
            *dir = east;
            priority = tmp_priority;
        }else if(map[mypos.x + 1][mypos.y] == little){
            if(priority < tmp_priority){
                *dir = east;
                priority = tmp_priority;
            }
        }
    }

    if((wall[mypos.x][mypos.y].south & mask) == NOWALL){
        tmp_priority = get_priority(mypos.x,mypos.y-1,south);
        if(map[mypos.x][mypos.y-1] < little){
            little = map[mypos.x][mypos.y-1];
            *dir = south;
            priority = tmp_priority;
        }else if(map[mypos.x][mypos.y-1] == little){
            if(priority < tmp_priority){
                *dir = south;
                priority = tmp_priority;
            }
        }
    }

    if((wall[mypos.x][mypos.y].west & mask) == NOWALL){
        tmp_priority = get_priority(mypos.x-1,mypos.y,west);
        if(map[mypos.x-1][mypos.y] < little){
            little = map[mypos.x-1][mypos.y];
            *dir = west;
            priority = tmp_priority;
        }else if(map[mypos.x-1][mypos.y] == little){
            if(priority < tmp_priority){
                *dir = west;
                priority = tmp_priority;
            }
        }
    }

    return ((int)((4+*dir - mypos.dir)%4));
}

void search_adachi(int *gx,int *gy,int goal_size){
    t_direction glob_nextdir;
    int direction = get_nextdir(gx,gy,goal_size,MASK_SEARCH,&glob_nextdir);

    mypos.dir = glob_nextdir;
    switch(mypos.dir){
        case north:
            mypos.y++;
            break;
        case east:
            mypos.x++;
            break;
        case south:
            mypos.y--;
            break;
        case west:
            mypos.x--;
            break;
    }

    switch(direction){
        case front:
            straight(HALF_SECTION,SEARCH_ACCEL,SEARCH_SPEED,SEARCH_SPEED);
            break;
        case right:
		    turn_R90(SEARCH_TURN_ACCEL, SEARCH_TURN_SPEED);						//右に曲がる
		    straight(HALF_SECTION, SEARCH_ACCEL,SEARCH_SPEED,SEARCH_SPEED);	    //半区画進む
            break;
        case left:
		    turn_L90(SEARCH_TURN_ACCEL, SEARCH_TURN_SPEED);						//左に曲がる
		    straight(HALF_SECTION, SEARCH_ACCEL,SEARCH_SPEED,SEARCH_SPEED);	    //半区画進む
            break;
        case rear:
		    turn_L180(SEARCH_TURN_ACCEL, SEARCH_TURN_SPEED);						//左に曲がる
		    straight(HALF_SECTION, SEARCH_ACCEL,SEARCH_SPEED,SEARCH_SPEED);	    //半区画進む
		    break;
    }


    while(i_am_goal(mypos.x,mypos.y,gx,gy,goal_size) == false){
    	set_wall(mypos.x,mypos.y);
        direction = get_nextdir(gx,gy,goal_size,MASK_SEARCH,&glob_nextdir);

        mypos.dir = glob_nextdir;

        switch(mypos.dir){
            case north:
                mypos.y++;
                break;
            case east:
                mypos.x++;
                break;
            case south:
                mypos.y--;
                break;
            case west:
                mypos.x--;
                break;
        }

        switch(direction){
            case front:
            	 straight(SECTION,SEARCH_ACCEL,SEARCH_SPEED,SEARCH_SPEED);
            	 break;
            case right:
            	sla_R90();
            	break;
            case left:
            	sla_L90();
            	break;
            case rear:
            	if(sen_fr.is_wall == true || sen_fl.is_wall == true){
            		if(sen_l.is_wall == true){
            			STOP_WALL();
            			STOP_WALL2(100);
            			turn_L90(SEARCH_TURN_ACCEL, SEARCH_TURN_SPEED);
            			STOP_WALL2(200);
            			turn_L90(SEARCH_TURN_ACCEL, SEARCH_TURN_SPEED);
            		}else if(sen_r.is_wall == true){
            			STOP_WALL();
            			STOP_WALL2(100);
            			turn_R90(SEARCH_TURN_ACCEL, SEARCH_TURN_SPEED);
            			STOP_WALL2(200);
            			turn_R90(SEARCH_TURN_ACCEL, SEARCH_TURN_SPEED);
            		}else{
            			STOP_WALL();
            			STOP_WALL2(100);
            			turn_L180(SEARCH_TURN_ACCEL, SEARCH_TURN_SPEED);
            		}
            	}else	{
            		straight(HALF_SECTION,SEARCH_ACCEL,SEARCH_SPEED,0.0);				//半区画進む
        		    turn_L180(SEARCH_TURN_ACCEL, SEARCH_TURN_SPEED);						//左に曲がる
            	}
            	straight(HALF_SECTION, SEARCH_ACCEL,SEARCH_SPEED,SEARCH_SPEED);	    //半区画進む
                break;
        }

    }
    set_wall(mypos.x,mypos.y);		//壁をセット
    if(sen_fr.is_wall == true || sen_fl.is_wall == true ) STOP_WALL();
    else	straight(HALF_SECTION,SEARCH_ACCEL,SEARCH_SPEED,0.0);
    run_mode = NON_CON_MODE;
}

void search_adachi_2(int *gx,int *gy,int goal_size,int mask){ //既知区間加速付き
    t_direction glob_nextdir;
    int direction = get_nextdir(gx,gy,goal_size,mask,&glob_nextdir);

    mypos.dir = glob_nextdir;
    switch(mypos.dir){
        case north:
            mypos.y++;
            break;
        case east:
            mypos.x++;
            break;
        case south:
            mypos.y--;
            break;
        case west:
            mypos.x--;
            break;
    }

    switch(direction){
        case front:
            straight(HALF_SECTION,SEARCH_ACCEL,SEARCH_SPEED,SEARCH_SPEED);
            break;
        case right:
		    turn_R90(SEARCH_TURN_ACCEL, SEARCH_TURN_SPEED);						//右に曲がる
		    straight(HALF_SECTION, SEARCH_ACCEL,SEARCH_SPEED,SEARCH_SPEED);	    //半区画進む
            break;
        case left:
		    turn_L90(SEARCH_TURN_ACCEL, SEARCH_TURN_SPEED);						//左に曲がる
		    straight(HALF_SECTION, SEARCH_ACCEL,SEARCH_SPEED,SEARCH_SPEED);	    //半区画進む
            break;
        case rear:
		    turn_L180(SEARCH_TURN_ACCEL, SEARCH_TURN_SPEED);						//左に曲がる
		    straight(HALF_SECTION, SEARCH_ACCEL,SEARCH_SPEED,SEARCH_SPEED);	    //半区画進む
		    break;
    }

    while(i_am_goal(mypos.x,mypos.y,gx,gy,goal_size) == false){
    	set_wall(mypos.x,mypos.y);
        direction = get_nextdir(gx,gy,goal_size,mask,&glob_nextdir);

        mypos.dir = glob_nextdir;

        switch(mypos.dir){
            case north:
                mypos.y++;
                break;
            case east:
                mypos.x++;
                break;
            case south:
                mypos.y--;
                break;
            case west:
                mypos.x--;
                break;
        }

        switch(direction){
            case front:
            	 if(is_unknown(mypos.x, mypos.y) == false)
            		 straight(SECTION,SEARCH_ACCEL,SEARCH_SPEED+0.2,SEARCH_SPEED+0.2);
            	 else
            		 straight(SECTION,SEARCH_ACCEL,SEARCH_SPEED,SEARCH_SPEED);
            	 break;
            case right:
            	sla_R90();
            	break;
            case left:
            	sla_L90();
            	break;
            case rear:
            	if(sen_fr.is_wall == true || sen_fl.is_wall == true ) STOP_WALL();
    		    else	straight(HALF_SECTION,SEARCH_ACCEL,SEARCH_SPEED,0.0);				//半区画進む
    		    turn_L180(SEARCH_TURN_ACCEL, SEARCH_TURN_SPEED);						//左に曲がる
    		    straight(HALF_SECTION, SEARCH_ACCEL,SEARCH_SPEED,SEARCH_SPEED);	    //半区画進む
                break;
        }

    }
    set_wall(mypos.x,mypos.y);		//壁をセット
    if(sen_fr.is_wall == true || sen_fl.is_wall == true ) STOP_WALL();
    else	straight(HALF_SECTION,SEARCH_ACCEL,SEARCH_SPEED,0.0);
    run_mode = NON_CON_MODE;
}

void make_map_zenmen(int *x, int *y,int size,int mask)	//歩数マップを作成する
{
//座標x,yをゴールとした歩数Mapを作成する。
//maskの値(MASK_SEARCH or MASK_SECOND)によって、
//探索用の歩数Mapを作るか、最短走行の歩数Mapを作るかが切り替わる
	t_bool change_flag;			//Map作成終了を見極めるためのフラグ
    init_map_zenmen(x,y,size);				//Mapを初期化する
	int known = 0;	int update = 0;

	do
	{
		change_flag = false;				//変更がなかった場合にはループを抜ける
		for(int i = 0; i < MAZE_SIZE_X; i++)			//迷路の大きさ分ループ(x座標)
		{
			for(int j = 0; j < MAZE_SIZE_Y; j++)		//迷路の大きさ分ループ(y座標)
			{
				if( is_unknown(i,j) != true)
				{
					known++;
				}
				if(map[i][j] == 999)		//999の場合は次へ
				{
					update++;
					continue;
				}

				if(j < MAZE_SIZE_Y-1)					//範囲チェック
				{
					if( (wall[i][j].north & mask) == NOWALL)	//壁がなければ(maskの意味はstatic_parametersを参照)
					{
						if(map[i][j+1] == 999)			//まだ値が入っていなければ
						{
							map[i][j+1] = map[i][j] + 1;	//値を代入
							change_flag = true;		//値が更新されたことを示す
						}
					}
				}

				if(i < MAZE_SIZE_X-1)					//範囲チェック
				{
					if( (wall[i][j].east & mask) == NOWALL)		//壁がなければ
					{
						if(map[i+1][j] == 999)			//値が入っていなければ
						{
							map[i+1][j] = map[i][j] + 1;	//値を代入
							change_flag = true;		//値が更新されたことを示す
						}
					}
				}

				if(j > 0)						//範囲チェック
				{
					if( (wall[i][j].south & mask) == NOWALL)	//壁がなければ
					{
						if(map[i][j-1] == 999)			//値が入っていなければ
						{
							map[i][j-1] = map[i][j] + 1;	//値を代入
							change_flag = true;		//値が更新されたことを示す
						}
					}
				}

				if(i > 0)						//範囲チェック
				{
					if( (wall[i][j].west & mask) == NOWALL)		//壁がなければ
					{
						if(map[i-1][j] == 999)			//値が入っていなければ
						{
							map[i-1][j] = map[i][j] + 1;	//値を代入
							change_flag = true;		//値が更新されたことを示す
						}

					}

				}

			}

		}

	}while(change_flag == true);	//全体を作り終わるまで待つ


	if(update == known &&end_search == 0 ){
		end_search = 1;
	    init_map_zenmen(x,y,size);				//Mapを初期化する
		int known = 0;	int update = 0;

		do
		{
			change_flag = false;				//変更がなかった場合にはループを抜ける
			for(int i = 0; i < MAZE_SIZE_X; i++)			//迷路の大きさ分ループ(x座標)
			{
				for(int j = 0; j < MAZE_SIZE_Y; j++)		//迷路の大きさ分ループ(y座標)
				{
					if( is_unknown(i,j) != true)
					{
						known++;
					}
					if(map[i][j] == 999)		//999の場合は次へ
					{
						update++;
						continue;
					}

					if(j < MAZE_SIZE_Y-1)					//範囲チェック
					{
						if( (wall[i][j].north & mask) == NOWALL)	//壁がなければ(maskの意味はstatic_parametersを参照)
						{
							if(map[i][j+1] == 999)			//まだ値が入っていなければ
							{
								map[i][j+1] = map[i][j] + 1;	//値を代入
								change_flag = true;		//値が更新されたことを示す
							}
						}
					}

					if(i < MAZE_SIZE_X-1)					//範囲チェック
					{
						if( (wall[i][j].east & mask) == NOWALL)		//壁がなければ
						{
							if(map[i+1][j] == 999)			//値が入っていなければ
							{
								map[i+1][j] = map[i][j] + 1;	//値を代入
								change_flag = true;		//値が更新されたことを示す
							}
						}
					}

					if(j > 0)						//範囲チェック
					{
						if( (wall[i][j].south & mask) == NOWALL)	//壁がなければ
						{
							if(map[i][j-1] == 999)			//値が入っていなければ
							{
								map[i][j-1] = map[i][j] + 1;	//値を代入
								change_flag = true;		//値が更新されたことを示す
							}
						}
					}

					if(i > 0)						//範囲チェック
					{
						if( (wall[i][j].west & mask) == NOWALL)		//壁がなければ
						{
							if(map[i-1][j] == 999)			//値が入っていなければ
							{
								map[i-1][j] = map[i][j] + 1;	//値を代入
								change_flag = true;		//値が更新されたことを示す
							}

						}

					}

				}

			}

		}while(change_flag == true);	//全体を作り終わるまで待つ
	}
}

int get_nextdir_zenmen(int *x, int *y,int goal_size ,int mask, t_direction *dir){
    int little,priority,tmp_priority;

    make_map_zenmen(x,y,goal_size,mask);

    little = 999;

    priority = 0;

    if((wall[mypos.x][mypos.y].north & mask) == NOWALL){
        tmp_priority = get_priority(mypos.x,mypos.y + 1,north);
        if(map[mypos.x][mypos.y+1] < little){
            little = map[mypos.x][mypos.y+1];
            *dir = north;
            priority = tmp_priority;
        }else if(map[mypos.x][mypos.y+1] == little){
            if(priority < tmp_priority){
                *dir = north;
                priority = tmp_priority;
            }
        }
    }

    if((wall[mypos.x][mypos.y].east & mask) == NOWALL){
        tmp_priority = get_priority(mypos.x + 1,mypos.y,east);
        if(map[mypos.x + 1][mypos.y] < little){
            little = map[mypos.x+1][mypos.y];
            *dir = east;
            priority = tmp_priority;
        }else if(map[mypos.x + 1][mypos.y] == little){
            if(priority < tmp_priority){
                *dir = east;
                priority = tmp_priority;
            }
        }
    }

    if((wall[mypos.x][mypos.y].south & mask) == NOWALL){
        tmp_priority = get_priority(mypos.x,mypos.y-1,south);
        if(map[mypos.x][mypos.y-1] < little){
            little = map[mypos.x][mypos.y-1];
            *dir = south;
            priority = tmp_priority;
        }else if(map[mypos.x][mypos.y-1] == little){
            if(priority < tmp_priority){
                *dir = south;
                priority = tmp_priority;
            }
        }
    }

    if((wall[mypos.x][mypos.y].west & mask) == NOWALL){
        tmp_priority = get_priority(mypos.x-1,mypos.y,west);
        if(map[mypos.x-1][mypos.y] < little){
            little = map[mypos.x-1][mypos.y];
            *dir = west;
            priority = tmp_priority;
        }else if(map[mypos.x-1][mypos.y] == little){
            if(priority < tmp_priority){
                *dir = west;
                priority = tmp_priority;
            }
        }
    }

    return ((int)((4+*dir - mypos.dir)%4));
}

void search_adachi_zenmen(int *gx,int *gy,int goal_size){
    t_direction glob_nextdir;
    int direction = get_nextdir_zenmen(gx,gy,goal_size,MASK_SEARCH,&glob_nextdir);

    mypos.dir = glob_nextdir;
    switch(mypos.dir){
        case north:
            mypos.y++;
            break;
        case east:
            mypos.x++;
            break;
        case south:
            mypos.y--;
            break;
        case west:
            mypos.x--;
            break;
    }

    switch(direction){
        case front:
            straight(HALF_SECTION,SEARCH_ACCEL,SEARCH_SPEED,SEARCH_SPEED);
            break;
        case right:
		    turn_R90(SEARCH_TURN_ACCEL, SEARCH_TURN_SPEED);						//右に曲がる
		    straight(HALF_SECTION, SEARCH_ACCEL,SEARCH_SPEED,SEARCH_SPEED);	    //半区画進む
            break;
        case left:
		    turn_L90(SEARCH_TURN_ACCEL, SEARCH_TURN_SPEED);						//左に曲がる
		    straight(HALF_SECTION, SEARCH_ACCEL,SEARCH_SPEED,SEARCH_SPEED);	    //半区画進む
            break;
        case rear:
		    turn_L180(SEARCH_TURN_ACCEL, SEARCH_TURN_SPEED);						//左に曲がる
		    straight(HALF_SECTION, SEARCH_ACCEL,SEARCH_SPEED,SEARCH_SPEED);	    //半区画進む
		    break;
    }


    while(i_am_goal(mypos.x,mypos.y,gx,gy,goal_size) == false){
    	set_wall(mypos.x,mypos.y);
        direction = get_nextdir_zenmen(gx,gy,goal_size,MASK_SEARCH,&glob_nextdir);

        mypos.dir = glob_nextdir;

        switch(mypos.dir){
            case north:
                mypos.y++;
                break;
            case east:
                mypos.x++;
                break;
            case south:
                mypos.y--;
                break;
            case west:
                mypos.x--;
                break;
        }

        switch(direction){
            case front:
            	 straight(SECTION,SEARCH_ACCEL,SEARCH_SPEED,SEARCH_SPEED);
            	 break;
            case right:
            	sla_R90();
            	break;
            case left:
            	sla_L90();
            	break;
            case rear:
            	if(sen_fr.is_wall == true || sen_fl.is_wall == true){
            		if(sen_l.is_wall == true){
            			//STOP_WALL();
            			straight(HALF_SECTION,SEARCH_ACCEL,SEARCH_SPEED,0.0);
            			STOP_WALL2(100);
            			turn_L90(SEARCH_TURN_ACCEL, SEARCH_TURN_SPEED);
            			STOP_WALL2(200);
            			turn_L90(SEARCH_TURN_ACCEL, SEARCH_TURN_SPEED);
            		}else if(sen_r.is_wall == true){
            			//STOP_WALL();
            			straight(HALF_SECTION,SEARCH_ACCEL,SEARCH_SPEED,0.0);
            			STOP_WALL2(100);
            			turn_R90(SEARCH_TURN_ACCEL, SEARCH_TURN_SPEED);
            			STOP_WALL2(200);
            			turn_R90(SEARCH_TURN_ACCEL, SEARCH_TURN_SPEED);
            		}else{
            			//STOP_WALL();
            			straight(HALF_SECTION,SEARCH_ACCEL,SEARCH_SPEED,0.0);
            			STOP_WALL2(100);
            			turn_L180(SEARCH_TURN_ACCEL, SEARCH_TURN_SPEED);
            		}
            	}else	{
            		straight(HALF_SECTION,SEARCH_ACCEL,SEARCH_SPEED,0.0);				//半区画進む
        		    turn_L180(SEARCH_TURN_ACCEL, SEARCH_TURN_SPEED);						//左に曲がる
            	}
            	straight(HALF_SECTION-10, SEARCH_ACCEL,SEARCH_SPEED,SEARCH_SPEED);	    //半区画進む
                break;
        }

    }
    set_wall(mypos.x,mypos.y);		//壁をセット
    if(sen_fr.is_wall == true || sen_fl.is_wall == true ) STOP_WALL();
    else	straight(HALF_SECTION,SEARCH_ACCEL,SEARCH_SPEED,0.0);
    run_mode = NON_CON_MODE;
}

void search_adachi_zenmen2(int *gx,int *gy,int goal_size){
    t_direction glob_nextdir;
    int direction = get_nextdir_zenmen(gx,gy,goal_size,MASK_SEARCH,&glob_nextdir);

    switch(direction){
        case front:
            straight(HALF_SECTION,SEARCH_ACCEL,SEARCH_SPEED,SEARCH_SPEED);
            break;
        case right:
		    turn_R90(SEARCH_TURN_ACCEL, SEARCH_TURN_SPEED);						//右に曲がる
		    straight(HALF_SECTION, SEARCH_ACCEL,SEARCH_SPEED,SEARCH_SPEED);	    //半区画進む
            break;
        case left:
		    turn_L90(SEARCH_TURN_ACCEL, SEARCH_TURN_SPEED);						//左に曲がる
		    straight(HALF_SECTION, SEARCH_ACCEL,SEARCH_SPEED,SEARCH_SPEED);	    //半区画進む
            break;
        case rear:
		    turn_L180(SEARCH_TURN_ACCEL, SEARCH_TURN_SPEED);						//左に曲がる
		    straight(HALF_SECTION, SEARCH_ACCEL,SEARCH_SPEED,SEARCH_SPEED);	    //半区画進む
		    break;
    }

    mypos.dir = glob_nextdir;
    switch(mypos.dir){
        case north:
            mypos.y++;
            break;
        case east:
            mypos.x++;
            break;
        case south:
            mypos.y--;
            break;
        case west:
            mypos.x--;
            break;
    }

    set_wall(mypos.x,mypos.y);

    int accel_cnt = 0;

    while(i_am_goal(mypos.x,mypos.y,gx,gy,goal_size) == false){

        direction = get_nextdir_zenmen(gx,gy,goal_size,MASK_SEARCH,&glob_nextdir);

        int next_is_unknown = 0x00;
        switch(glob_nextdir){
            case north:
            	if(is_unknown(mypos.x,mypos.y+1) == false)
            		next_is_unknown = 0x80;
                break;
            case east:
            	if(is_unknown(mypos.x+1,mypos.y) == false)
            		next_is_unknown = 0x80;
                break;
            case south:
            	if(is_unknown(mypos.x,mypos.y-1) == false)
            		next_is_unknown = 0x80;
                break;
            case west:
            	if(is_unknown(mypos.x-1,mypos.y) == false)
            		next_is_unknown = 0x80;
                break;
        }


        switch(direction|next_is_unknown){
            case front|0x80:
            	accel_cnt++;
            	break;
            case front:
        		if(accel_cnt > 0){
        			straight(SECTION*accel_cnt,SEARCH_ACCEL*1.5,0.5,SEARCH_SPEED);
        			accel_cnt = 0;
        		}
        		straight(SECTION,SEARCH_ACCEL,SEARCH_SPEED,SEARCH_SPEED);
            	break;
            case right|0x80:
            case right:
        		if(accel_cnt > 0){
        			straight(SECTION*accel_cnt,SEARCH_ACCEL*1.5,0.5,SEARCH_SPEED);
        			accel_cnt = 0;
        		}
            	sla_R90();
            	break;
            case left|0x80:
            case left:
        		if(accel_cnt > 0){
        			straight(SECTION*accel_cnt,SEARCH_ACCEL*1.5,0.5,SEARCH_SPEED);
        			accel_cnt = 0;
        		}
            	sla_L90();
            	break;
            case rear|0x80:
            case rear:
        		if(accel_cnt > 0){
        			straight(SECTION*accel_cnt,SEARCH_ACCEL*1.5,0.5,SEARCH_SPEED);
        			accel_cnt = 0;
        		}
            	if(sen_fr.is_wall == true || sen_fl.is_wall == true){
            		if(sen_l.is_wall == true){
            			STOP_WALL();
            			STOP_WALL2(100);
            			turn_L90(SEARCH_TURN_ACCEL, SEARCH_TURN_SPEED);
            			STOP_WALL2(200);
            			turn_L90(SEARCH_TURN_ACCEL, SEARCH_TURN_SPEED);
            		}else if(sen_r.is_wall == true){
            			STOP_WALL();
            			STOP_WALL2(100);
            			turn_R90(SEARCH_TURN_ACCEL, SEARCH_TURN_SPEED);
            			STOP_WALL2(200);
            			turn_R90(SEARCH_TURN_ACCEL, SEARCH_TURN_SPEED);
            		}else{
            			STOP_WALL();
            			STOP_WALL2(100);
            			turn_L180(SEARCH_TURN_ACCEL, SEARCH_TURN_SPEED);
            		}
            	}else	{
            		if(sen_l.is_wall == true){
            			straight(HALF_SECTION,SEARCH_ACCEL,SEARCH_SPEED,0.0);
            			turn_L90(SEARCH_TURN_ACCEL, SEARCH_TURN_SPEED);
            			STOP_WALL2(200);
            			turn_L90(SEARCH_TURN_ACCEL, SEARCH_TURN_SPEED);
            		}else if(sen_r.is_wall == true){
            			straight(HALF_SECTION,SEARCH_ACCEL,SEARCH_SPEED,0.0);
            			turn_R90(SEARCH_TURN_ACCEL, SEARCH_TURN_SPEED);
            			STOP_WALL2(200);
            			turn_R90(SEARCH_TURN_ACCEL, SEARCH_TURN_SPEED);
            		}else{
            			straight(HALF_SECTION,SEARCH_ACCEL,SEARCH_SPEED,0.0);
            			turn_L180(SEARCH_TURN_ACCEL, SEARCH_TURN_SPEED);
            		}
            	}
            	straight(HALF_SECTION-10, SEARCH_ACCEL,SEARCH_SPEED,SEARCH_SPEED);	    //半区画進む
                break;
        }

        mypos.dir = glob_nextdir;

        switch(mypos.dir){
            case north:
                mypos.y++;
                break;
            case east:
                mypos.x++;
                break;
            case south:
                mypos.y--;
                break;
            case west:
                mypos.x--;
                break;
        }

        if(is_unknown(mypos.x,mypos.y) == true){
        	set_wall(mypos.x,mypos.y);
        }

    }
	if(accel_cnt > 0){
		straight(SECTION*accel_cnt,SEARCH_ACCEL*1.5,0.5,SEARCH_SPEED);
		accel_cnt = 0;
	}
    if(sen_fr.is_wall == true || sen_fl.is_wall == true ) STOP_WALL();
    else	straight(HALF_SECTION,SEARCH_ACCEL,SEARCH_SPEED,0.0);
    run_mode = NON_CON_MODE;
}

/*
void search_adachi_spin_turn(int gx,int gy){
    t_direction glob_nextdir;
    switch(get_nextdir(gx,gy,MASK_SEARCH,&glob_nextdir)){
        case front:
            straight(HALF_SECTION,SEARCH_ACCEL,SEARCH_SPEED,SEARCH_SPEED);
            break;
        case right:
		    //straight(HALF_SECTION,SEARCH_ACCEL,SEARCH_SPEED,0.0);				//半区画進む
		    turn_R90(SEARCH_TURN_ACCEL, SEARCH_TURN_SPEED);						//右に曲がる
		    straight(HALF_SECTION, SEARCH_ACCEL,SEARCH_SPEED,SEARCH_SPEED);	    //半区画進む
            break;
        case left:
		    //straight(HALF_SECTION,SEARCH_ACCEL,SEARCH_SPEED,0.0);				//半区画進む
		    turn_L90(SEARCH_TURN_ACCEL, SEARCH_TURN_SPEED);						//左に曲がる
		    straight(HALF_SECTION, SEARCH_ACCEL,SEARCH_SPEED,SEARCH_SPEED);	    //半区画進む
            break;
        case rear:
		    //straight(HALF_SECTION,SEARCH_ACCEL,SEARCH_SPEED,0.0);				//半区画進む
		    turn_L180(SEARCH_TURN_ACCEL, SEARCH_TURN_SPEED);						//左に曲がる
		    straight(HALF_SECTION, SEARCH_ACCEL,SEARCH_SPEED,SEARCH_SPEED);	    //半区画進む            break;
    }

    mypos.dir = glob_nextdir;

    switch(mypos.dir){
        case north:
            mypos.y++;
            break;
        case east:
            mypos.x++;
            break;
        case south:
            mypos.y--;
            break;
        case west:
            mypos.x--;
            break;
    }

    while((mypos.x != gx) || (mypos.y != gy)){
        set_wall(mypos.x,mypos.y);
        switch(get_nextdir(gx,gy,MASK_SEARCH,&glob_nextdir)){
            case front:
            	 straight(SECTION,SEARCH_ACCEL,SEARCH_SPEED,SEARCH_SPEED);
                break;
            case right:
    		    straight(HALF_SECTION,SEARCH_ACCEL,SEARCH_SPEED,0.0);				//半区画進む
    		    turn_R90(SEARCH_TURN_ACCEL, SEARCH_TURN_SPEED);						//右に曲がる
    		    straight(HALF_SECTION, SEARCH_ACCEL,SEARCH_SPEED,SEARCH_SPEED);	    //半区画進む
            	break;
            case left:
    		    straight(HALF_SECTION,SEARCH_ACCEL,SEARCH_SPEED,0.0);				//半区画進む
    		    turn_L90(SEARCH_TURN_ACCEL, SEARCH_TURN_SPEED);						//左に曲がる
    		    straight(HALF_SECTION, SEARCH_ACCEL,SEARCH_SPEED,SEARCH_SPEED);	    //半区画進む
            	break;
            case rear:
            	if(sen_fr.is_wall == true || sen_fl.is_wall == true ) STOP_WALL();
    		    else	straight(HALF_SECTION,SEARCH_ACCEL,SEARCH_SPEED,0.0);				//半区画進む
    		    turn_L180(SEARCH_TURN_ACCEL, SEARCH_TURN_SPEED);						//左に曲がる
    		    straight(HALF_SECTION, SEARCH_ACCEL,SEARCH_SPEED,SEARCH_SPEED);	    //半区画進む            break;
                break;
        }

        mypos.dir = glob_nextdir;

        switch(mypos.dir){
            case north:
                mypos.y++;
                break;
            case east:
                mypos.x++;
                break;
            case south:
                mypos.y--;
                break;
            case west:
                mypos.x--;
                break;
        }
    }
    set_wall(mypos.x,mypos.y);		//壁をセット
    if(sen_fr.is_wall == true && sen_fl.is_wall == true ) STOP_WALL();
    else	straight(HALF_SECTION,SEARCH_ACCEL,SEARCH_SPEED,0.0);

    run_mode = NON_CON_MODE;
}
*/

void saitan(int *gx,int *gy,int goal_size){

	mypos.x = mypos.y = 0; mypos.dir = north;

	make_map(gx, gy, goal_size,0x03);
	int run_data[map[0][0]];

	t_direction glob_nextdir;
    int direction = get_nextdir(gx,gy,goal_size,0x03,&glob_nextdir);

    mypos.dir = glob_nextdir;
    switch(mypos.dir){
        case north:
            mypos.y++;
            break;
        case east:
            mypos.x++;
            break;
        case south:
            mypos.y--;
            break;
        case west:
            mypos.x--;
            break;
    }

    run_data[0] = direction;

    int run_cnt = 1;

    while(i_am_goal(mypos.x,mypos.y,gx,gy,goal_size) == false){

        direction = get_nextdir(gx,gy,goal_size,0x03,&glob_nextdir);
        mypos.dir = glob_nextdir;

        switch(mypos.dir){
            case north:
                mypos.y++;
                break;
            case east:
                mypos.x++;
                break;
            case south:
                mypos.y--;
                break;
            case west:
                mypos.x--;
                break;
        }

        run_data[run_cnt] = direction;
        run_cnt++;
        if(run_cnt > 1000) break;
    }

	mypos.x = mypos.y = 0; mypos.dir = north;

    if(run_cnt > 1000){
    	for(int i = 0; i < 4;i++){	MY_LED(0xFF);HAL_Delay(500);MY_LED(0x00);HAL_Delay(500);	}
    }else{
    	for(int i = 0; i < 8;i++){	MY_LED(0xFF);HAL_Delay(100);MY_LED(0x00);HAL_Delay(100);	}
    	MY_LED(0xFF);HAL_Delay(100);
    	while( (sen_fl.value + sen_fr.value + sen_l.value + sen_r.value)/4 < 2000 );
    	for(int i = 0; i < 4;i++){	MY_LED(0xFF);HAL_Delay(300);MY_LED(0x00);HAL_Delay(300);	}
		//gyro_ref = 0.0;gyro_cariblation();

		sen_r.ref = sen_r.c_value+0;
		sen_l.ref = sen_l.c_value+0;

		sen_r.th_control = sen_r.ref - 400;//TH_SEN_R;
		sen_l.th_control = sen_l.ref - 400;//TH_SEN_L;

		len_mouse = degree =	0.0;
		mypos.x = mypos.y = 0;
		mypos.dir = north;

		straight(HALF_SECTION+15,SEARCH_ACCEL,SEARCH_SPEED,SEARCH_SPEED);
    	for(int i = 1 ; i < run_cnt ; i++){
        	float Max_sp = 0.5;	float End_sp = 0.5;
            switch(run_data[i]){
                case front:
                	if(run_data[i+1] == front && i < run_cnt-1 ){Max_sp = 0.7; End_sp = 0.7;}
                	else if(run_data[i-1] == front ){Max_sp = 0.7; End_sp = 0.5;}
                	 straight(SECTION,SEARCH_ACCEL,Max_sp,End_sp);
                    break;
                case right:
                	sla_R90();
                	break;
                case left:
                	sla_L90();
                	break;
                case rear:
                	if(sen_fr.is_wall == true || sen_fl.is_wall == true ) STOP_WALL();
        		    else	straight(HALF_SECTION+7.5,SEARCH_ACCEL,SEARCH_SPEED,0.0);				//半区画進む
        		    turn_L180(SEARCH_TURN_ACCEL, SEARCH_TURN_SPEED);						//左に曲がる
        		    straight(HALF_SECTION, SEARCH_ACCEL,SEARCH_SPEED,SEARCH_SPEED);	    //半区画進む            break;
                    break;
            }
    	}

    	straight(HALF_SECTION,SEARCH_ACCEL,0.3,0.0);
    	run_mode = NON_CON_MODE;

    }

}

void saitan2(int *gx,int *gy,int goal_size){

	mypos.x = mypos.y = 0; mypos.dir = north;

	make_map(gx, gy, goal_size,0x03);
	int run_data[map[0][0]];

	t_direction glob_nextdir;
    int direction = get_nextdir(gx,gy,goal_size,0x03,&glob_nextdir);

    mypos.dir = glob_nextdir;
    switch(mypos.dir){
        case north:
            mypos.y++;
            break;
        case east:
            mypos.x++;
            break;
        case south:
            mypos.y--;
            break;
        case west:
            mypos.x--;
            break;
    }

    run_data[0] = direction;

    int run_cnt = 1;

    while(i_am_goal(mypos.x,mypos.y,gx,gy,goal_size) == false){

        direction = get_nextdir(gx,gy,goal_size,0x03,&glob_nextdir);
        mypos.dir = glob_nextdir;

        switch(mypos.dir){
            case north:
                mypos.y++;
                break;
            case east:
                mypos.x++;
                break;
            case south:
                mypos.y--;
                break;
            case west:
                mypos.x--;
                break;
        }

        run_data[run_cnt] = direction;
        run_cnt++;
        if(run_cnt > 1000) break;
    }

	mypos.x = mypos.y = 0; mypos.dir = north;

    if(run_cnt > 1000){
    	for(int i = 0; i < 4;i++){	MY_LED(0xFF);HAL_Delay(500);MY_LED(0x00);HAL_Delay(500);	}
    }else{
    	for(int i = 0; i < 8;i++){	MY_LED(0xFF);HAL_Delay(100);MY_LED(0x00);HAL_Delay(100);	}
    	MY_LED(0xFF);HAL_Delay(100);
    	while( (sen_fl.value + sen_fr.value + sen_l.value + sen_r.value)/4 < 2000 );
    	for(int i = 0; i < 4;i++){	MY_LED(0xFF);HAL_Delay(300);MY_LED(0x00);HAL_Delay(300);	}
		//gyro_ref = 0.0;gyro_cariblation();

		sen_r.ref = sen_r.c_value+0;
		sen_l.ref = sen_l.c_value+0;

		sen_r.th_control = sen_r.ref - 400;//TH_SEN_R;
		sen_l.th_control = sen_l.ref - 400;//TH_SEN_L;

		len_mouse = degree =	0.0;
		mypos.x = mypos.y = 0;
		mypos.dir = north;

		straight(HALF_SECTION+15,SEARCH_ACCEL,SEARCH_SPEED,SEARCH_SPEED);
    	for(int i = 1 ; i < run_cnt ; i++){
        	float Max_sp = 0.6;	float End_sp = 0.5;float Accel = SEARCH_ACCEL;
            switch(run_data[i]){
                case front:
                	if(run_data[i+1] == front && i < run_cnt-1 ){Max_sp = 0.9; End_sp = 0.9;Accel = 5.0; }
                	else if(run_data[i-1] == front ){Max_sp = 0.9; End_sp = 0.5; Accel = 5.0;}
                	 straight(SECTION, Accel,Max_sp,End_sp);
                    break;
                case right:
                	sla_R90();
                	break;
                case left:
                	sla_L90();
                	break;
                case rear:
                	if(sen_fr.is_wall == true || sen_fl.is_wall == true ) STOP_WALL();
        		    else	straight(HALF_SECTION+7.5,SEARCH_ACCEL,SEARCH_SPEED,0.0);				//半区画進む
        		    turn_L180(SEARCH_TURN_ACCEL, SEARCH_TURN_SPEED);						//左に曲がる
        		    straight(HALF_SECTION, SEARCH_ACCEL,SEARCH_SPEED,SEARCH_SPEED);	    //半区画進む            break;
                    break;
            }
    	}

    	straight(HALF_SECTION,SEARCH_ACCEL,0.3,0.0);
    	run_mode = NON_CON_MODE;


    }

}

void saitan3(int *gx,int *gy,int goal_size){
	int run_pattern[MAZE_SIZE_X*MAZE_SIZE_Y*4];
	int run_cnt = 0;

    t_direction glob_nextdir;
    int direction = get_nextdir(gx,gy,goal_size,0x03,&glob_nextdir);

    mypos.dir = glob_nextdir;
    switch(mypos.dir){
        case north:
            mypos.y++;
            break;
        case east:
            mypos.x++;
            break;
        case south:
            mypos.y--;
            break;
        case west:
            mypos.x--;
            break;
    }

    switch(direction){
        case front:
            run_pattern[run_cnt++] = straight_HSTEP;
        	break;
        case right:
            run_pattern[run_cnt++] = spin_r90;
            run_pattern[run_cnt++] = straight_HSTEP;
   		    break;
        case left:
            run_pattern[run_cnt++] = spin_l90;
            run_pattern[run_cnt++] = straight_HSTEP;

		    break;
        case rear:
            run_pattern[run_cnt++] = turn_l180;
            run_pattern[run_cnt++] = straight_HSTEP;
        	break;
    }


    while(i_am_goal(mypos.x,mypos.y,gx,gy,goal_size) == false){
        direction = get_nextdir(gx,gy,goal_size,0x03,&glob_nextdir);
        mypos.dir = glob_nextdir;

        switch(mypos.dir){
            case north:
                mypos.y++;
                break;
            case east:
                mypos.x++;
                break;
            case south:
                mypos.y--;
                break;
            case west:
                mypos.x--;
                break;
        }

        switch(direction){
            case front:
                run_pattern[run_cnt++] = straight_HSTEP;
                run_pattern[run_cnt++] = straight_HSTEP;
            	break;
            case right:
            	if( run_pattern[run_cnt-1] == turn_l90 || run_pattern[run_cnt-1] == diagonal_l ){
            		run_pattern[run_cnt++] = diagonal_r;
            	}else{
            		run_pattern[run_cnt++] = turn_r90;
            	}
            	break;
            case left:
            	if( run_pattern[run_cnt-1] == turn_r90 || run_pattern[run_cnt-1] == diagonal_r ){
            		run_pattern[run_cnt++] = diagonal_l;
            	}else{
            		run_pattern[run_cnt++] = turn_l90;
            	}
            	break;
            case rear:
            	run_pattern[run_cnt++] = straight_HSTEP;
            	run_pattern[run_cnt++] = turn_l180;
            	run_pattern[run_cnt++] = straight_HSTEP;
                break;
        }

    }

    run_pattern[run_cnt++] = straight_HSTEP;
    run_pattern[run_cnt] = goal;

    if(goal_size >1)	straight(15.0,SEARCH_ACCEL,SEARCH_SPEED,SEARCH_SPEED);
    int cnt = 0;
    while(run_pattern[cnt] != goal){
    	switch(run_pattern[cnt]){
    		case straight_HSTEP:
    			if(run_pattern[cnt+1] == turn_r90 && run_pattern[cnt+2] == diagonal_l){
    				in_R45();	cnt = cnt + 2;
    			}else if(run_pattern[cnt+1] == turn_l90 && run_pattern[cnt+2] == diagonal_r){
    				in_L45();	cnt = cnt + 2;
    			}else if(run_pattern[cnt+1] == turn_r90 && run_pattern[cnt+2] == straight_HSTEP){
    				long_turn_R90();cnt = cnt + 3;
    			}else if(run_pattern[cnt+1] == turn_l90 && run_pattern[cnt+2] == straight_HSTEP){
    				long_turn_L90();cnt = cnt + 3;
    			}else if(run_pattern[cnt+1] == turn_r90 && run_pattern[cnt+2] == turn_r90 && run_pattern[cnt+3] == straight_HSTEP){
    				long_turn_R180();cnt = cnt + 4;
    			}else if(run_pattern[cnt+1] == turn_l90 && run_pattern[cnt+2] == turn_l90 && run_pattern[cnt+3] == straight_HSTEP){
    				long_turn_L180();cnt = cnt + 4;
    			}else if(run_pattern[cnt+1] == turn_r90 && run_pattern[cnt+2] == turn_r90 && run_pattern[cnt+3] == diagonal_l){
    				in_R135(); 	cnt = cnt + 3;
    			}else if(run_pattern[cnt+1] == turn_l90 && run_pattern[cnt+2] == turn_l90 && run_pattern[cnt+3] == diagonal_r){
    				in_L135(); 	cnt = cnt + 3;
    			}else{
    				straight(HALF_SECTION,SEARCH_ACCEL,0.5,0.5);	cnt = cnt + 1;
    			}
    			break;
    		case turn_r90:
    			sla_R90();	cnt = cnt + 1;
    			break;
    		case turn_l90:
    			sla_L90();	cnt = cnt + 1;
    			break;
    		case turn_l180:
    			turn_L180(SEARCH_TURN_ACCEL, SEARCH_TURN_SPEED);	cnt = cnt + 1;
    			break;
    		case diagonal_r:
    			if(run_pattern[cnt+1] == straight_HSTEP ){
    				out_R45();	cnt = cnt + 2;
    			}else if(run_pattern[cnt+1] == turn_r90 && run_pattern[cnt+2] == straight_HSTEP){
    				out_R135();	cnt = cnt + 3;
    			}else if(run_pattern[cnt+1] == turn_r90 && run_pattern[cnt+2] == diagonal_l){
    				v_R90();	cnt = cnt + 2;
    			}else{
    				diagonal(SEARCH_ACCEL,0.5,0.5);	cnt = cnt + 1;
    			}
    			break;
    		case diagonal_l:
    			if(run_pattern[cnt+1] == straight_HSTEP ){
    				out_L45();	cnt = cnt + 2;
    			}else if(run_pattern[cnt+1] == turn_l90 && run_pattern[cnt+2] == straight_HSTEP){
    				out_L135();	cnt = cnt + 3;
    			}else if(run_pattern[cnt+1] == turn_l90 && run_pattern[cnt+2] == diagonal_r){
    				v_L90();	cnt = cnt + 2;
    			}else{
    				diagonal(SEARCH_ACCEL,0.5,0.5);	cnt = cnt + 1;
    			}
    			break;
    		case spin_r90:
    			turn_R90(SEARCH_TURN_ACCEL, SEARCH_TURN_SPEED);	cnt = cnt + 1;
    			break;
    		case spin_l90:
    			turn_L90(SEARCH_TURN_ACCEL, SEARCH_TURN_SPEED);	cnt = cnt + 1;
    			break;
    	}
    }
    if(goal_size > 1){
		straight(SECTION,SEARCH_ACCEL,0.5,0.0);
		switch(mypos.dir){
			case north:
				mypos.y++;
				break;
			case east:
				mypos.x++;
				break;
			case south:
				mypos.y--;
				break;
			case west:
				mypos.x--;
				break;
		}
    }
	accel = 0.0; max_speed = 0.0; tar_speed = 0.0;
	HAL_Delay(300);

    run_mode = NON_CON_MODE;
}

void saitan4(int *gx,int *gy,int goal_size){ 	//only straigh accel
	int run_pattern[MAZE_SIZE_X*MAZE_SIZE_Y*4];
	int run_cnt = 0;

    t_direction glob_nextdir;
    int direction = get_nextdir(gx,gy,goal_size,0x03,&glob_nextdir);

    mypos.dir = glob_nextdir;
    switch(mypos.dir){
        case north:
            mypos.y++;
            break;
        case east:
            mypos.x++;
            break;
        case south:
            mypos.y--;
            break;
        case west:
            mypos.x--;
            break;
    }

    switch(direction){
        case front:
            run_pattern[run_cnt++] = straight_HSTEP;
        	break;
        case right:
            run_pattern[run_cnt++] = spin_r90;
            run_pattern[run_cnt++] = straight_HSTEP;
   		    break;
        case left:
            run_pattern[run_cnt++] = spin_l90;
            run_pattern[run_cnt++] = straight_HSTEP;

		    break;
        case rear:
            run_pattern[run_cnt++] = turn_l180;
            run_pattern[run_cnt++] = straight_HSTEP;
        	break;
    }


    while(i_am_goal(mypos.x,mypos.y,gx,gy,goal_size) == false){
        direction = get_nextdir(gx,gy,goal_size,0x03,&glob_nextdir);
        mypos.dir = glob_nextdir;

        switch(mypos.dir){
            case north:
                mypos.y++;
                break;
            case east:
                mypos.x++;
                break;
            case south:
                mypos.y--;
                break;
            case west:
                mypos.x--;
                break;
        }

        switch(direction){
            case front:
                run_pattern[run_cnt++] = straight_HSTEP;
                run_pattern[run_cnt++] = straight_HSTEP;
            	break;
            case right:
            	run_pattern[run_cnt++] = turn_r90;
            	break;
            case left:
            	run_pattern[run_cnt++] = turn_l90;
            	break;
            case rear:
            	run_pattern[run_cnt++] = straight_HSTEP;
            	run_pattern[run_cnt++] = turn_l180;
            	run_pattern[run_cnt++] = straight_HSTEP;
                break;
        }

    }

    run_pattern[run_cnt++] = straight_HSTEP;
    run_pattern[run_cnt++] = goal;

    int cnt = 0;int accel_cnt = 0;
    if(goal_size >1)	straight(15.0,SEARCH_ACCEL,SEARCH_SPEED,SEARCH_SPEED);
    while(run_pattern[cnt] != goal ){
    	switch(run_pattern[cnt]){
    		case straight_HSTEP:
    			accel_cnt++;
    			if(run_pattern[cnt+1] != straight_HSTEP){
    				if(run_pattern[cnt+1] == goal){
    					if(accel_cnt > 2)	straight(HALF_SECTION*accel_cnt,SEARCH_ACCEL*1.5,1.0,0.0);
    					else				straight(HALF_SECTION*accel_cnt,SEARCH_ACCEL,0.3,0.0);
    				}else{
    					if(accel_cnt > 2)	straight(HALF_SECTION*accel_cnt,SEARCH_ACCEL*1.5,1.0,0.3);
    					else				straight(HALF_SECTION*accel_cnt,SEARCH_ACCEL,0.3,0.3);
    				}
    				accel_cnt = 0;cnt = cnt + 1;
       			}else{
    				cnt = cnt + 1;
    			}
    			//straight(HALF_SECTION,SEARCH_ACCEL,0.3,0.3);cnt = cnt + 1;
    			break;
    		case turn_r90:
    			sla_R90();	cnt = cnt + 1;
    			break;
    		case turn_l90:
    			sla_L90();	cnt = cnt + 1;
    			break;
    		case turn_l180:
    			turn_L180(SEARCH_TURN_ACCEL, SEARCH_TURN_SPEED);	cnt = cnt + 1;
    			break;
    		case spin_r90:
    			turn_R90(SEARCH_TURN_ACCEL, SEARCH_TURN_SPEED);	cnt = cnt + 1;
    			break;
    		case spin_l90:
    			turn_L90(SEARCH_TURN_ACCEL, SEARCH_TURN_SPEED);	cnt = cnt + 1;
    			break;
    	}
    }
    //straight(HALF_SECTION,SEARCH_ACCEL,0.3,0.0);
    run_mode = NON_CON_MODE;
}

void saitan5(int *gx,int *gy,int goal_size){
	int run_pattern[MAZE_SIZE_X*MAZE_SIZE_Y*4];
	int run_cnt = 0;

    t_direction glob_nextdir;
    int direction = get_nextdir(gx,gy,goal_size,0x03,&glob_nextdir);

    mypos.dir = glob_nextdir;
    switch(mypos.dir){
        case north:
            mypos.y++;
            break;
        case east:
            mypos.x++;
            break;
        case south:
            mypos.y--;
            break;
        case west:
            mypos.x--;
            break;
    }

    switch(direction){
        case front:
            run_pattern[run_cnt++] = straight_HSTEP;
        	break;
        case right:
            run_pattern[run_cnt++] = spin_r90;
            run_pattern[run_cnt++] = straight_HSTEP;
   		    break;
        case left:
            run_pattern[run_cnt++] = spin_l90;
            run_pattern[run_cnt++] = straight_HSTEP;

		    break;
        case rear:
            run_pattern[run_cnt++] = turn_l180;
            run_pattern[run_cnt++] = straight_HSTEP;
        	break;
    }


    while(i_am_goal(mypos.x,mypos.y,gx,gy,goal_size) == false){
        direction = get_nextdir(gx,gy,goal_size,0x03,&glob_nextdir);
        mypos.dir = glob_nextdir;

        switch(mypos.dir){
            case north:
                mypos.y++;
                break;
            case east:
                mypos.x++;
                break;
            case south:
                mypos.y--;
                break;
            case west:
                mypos.x--;
                break;
        }

        switch(direction){
            case front:
                run_pattern[run_cnt++] = straight_HSTEP;
                run_pattern[run_cnt++] = straight_HSTEP;
            	break;
            case right:
            	if( run_pattern[run_cnt-1] == turn_l90 || run_pattern[run_cnt-1] == diagonal_l ){
            		run_pattern[run_cnt++] = diagonal_r;
            	}else{
            		run_pattern[run_cnt++] = turn_r90;
            	}
            	break;
            case left:
            	if( run_pattern[run_cnt-1] == turn_r90 || run_pattern[run_cnt-1] == diagonal_r ){
            		run_pattern[run_cnt++] = diagonal_l;
            	}else{
            		run_pattern[run_cnt++] = turn_l90;
            	}
            	break;
            case rear:
            	run_pattern[run_cnt++] = straight_HSTEP;
            	run_pattern[run_cnt++] = turn_l180;
            	run_pattern[run_cnt++] = straight_HSTEP;
                break;
        }

    }

    run_pattern[run_cnt++] = straight_HSTEP;
    run_pattern[run_cnt] = goal;

    if(goal_size >1)	straight(15.0,SEARCH_ACCEL,SEARCH_SPEED,SEARCH_SPEED);
    int cnt = 0;int accel_cnt = 0;
    while(run_pattern[cnt] != goal){
    	switch(run_pattern[cnt]){
    		case straight_HSTEP:
    			if(run_pattern[cnt+1] == turn_r90 && run_pattern[cnt+2] == diagonal_l){
					if(accel_cnt > 2)	straight(HALF_SECTION*accel_cnt,SEARCH_ACCEL*1.5,1.0,0.5);
					else				straight(HALF_SECTION*accel_cnt,SEARCH_ACCEL,0.5,0.5);
    				in_R45();	cnt = cnt + 2;	accel_cnt = 0;
    			}else if(run_pattern[cnt+1] == turn_l90 && run_pattern[cnt+2] == diagonal_r){
					if(accel_cnt > 2)	straight(HALF_SECTION*accel_cnt,SEARCH_ACCEL*1.5,1.0,0.5);
					else				straight(HALF_SECTION*accel_cnt,SEARCH_ACCEL,0.5,0.5);
    				in_L45();	cnt = cnt + 2;	accel_cnt = 0;
    			}else if(run_pattern[cnt+1] == turn_r90 && run_pattern[cnt+2] == straight_HSTEP){
					if(accel_cnt > 2)	straight(HALF_SECTION*accel_cnt,SEARCH_ACCEL*1.5,1.0,0.5);
					else				straight(HALF_SECTION*accel_cnt,SEARCH_ACCEL,0.5,0.5);
    				long_turn_R90();cnt = cnt + 3;	accel_cnt = 0;
    			}else if(run_pattern[cnt+1] == turn_l90 && run_pattern[cnt+2] == straight_HSTEP){
					if(accel_cnt > 2)	straight(HALF_SECTION*accel_cnt,SEARCH_ACCEL*1.5,1.0,0.5);
					else				straight(HALF_SECTION*accel_cnt,SEARCH_ACCEL,0.5,0.5);
    				long_turn_L90();cnt = cnt + 3;	accel_cnt = 0;
    			}else if(run_pattern[cnt+1] == turn_r90 && run_pattern[cnt+2] == turn_r90 && run_pattern[cnt+3] == straight_HSTEP){
					if(accel_cnt > 2)	straight(HALF_SECTION*accel_cnt,SEARCH_ACCEL*1.5,1.0,0.5);
					else				straight(HALF_SECTION*accel_cnt,SEARCH_ACCEL,0.5,0.5);
    				long_turn_R180();cnt = cnt + 4;	accel_cnt = 0;
    			}else if(run_pattern[cnt+1] == turn_l90 && run_pattern[cnt+2] == turn_l90 && run_pattern[cnt+3] == straight_HSTEP){
					if(accel_cnt > 2)	straight(HALF_SECTION*accel_cnt,SEARCH_ACCEL*1.5,1.0,0.5);
					else				straight(HALF_SECTION*accel_cnt,SEARCH_ACCEL,0.5,0.5);
    				long_turn_L180();cnt = cnt + 4;	accel_cnt = 0;
    			}else if(run_pattern[cnt+1] == turn_r90 && run_pattern[cnt+2] == turn_r90 && run_pattern[cnt+3] == diagonal_l){
					if(accel_cnt > 2)	straight(HALF_SECTION*accel_cnt,SEARCH_ACCEL*1.5,1.0,0.5);
					else				straight(HALF_SECTION*accel_cnt,SEARCH_ACCEL,0.5,0.5);
    				in_R135(); 	cnt = cnt + 3;	accel_cnt = 0;
    			}else if(run_pattern[cnt+1] == turn_l90 && run_pattern[cnt+2] == turn_l90 && run_pattern[cnt+3] == diagonal_r){
					if(accel_cnt > 2)	straight(HALF_SECTION*accel_cnt,SEARCH_ACCEL*1.5,1.0,0.5);
					else				straight(HALF_SECTION*accel_cnt,SEARCH_ACCEL,0.5,0.5);
    				in_L135(); 	cnt = cnt + 3;	accel_cnt = 0;
    			}else{
    				accel_cnt = accel_cnt + 1;	cnt = cnt + 1;
    			}
    			break;
    		case turn_r90:
				if(accel_cnt > 2)	straight(HALF_SECTION*accel_cnt,SEARCH_ACCEL*1.5,1.0,0.5);
				else				straight(HALF_SECTION*accel_cnt,SEARCH_ACCEL,0.5,0.5);
    			sla_R90();	cnt = cnt + 1;	accel_cnt = 0;
    			break;
    		case turn_l90:
				if(accel_cnt > 2)	straight(HALF_SECTION*accel_cnt,SEARCH_ACCEL*1.5,1.0,0.5);
				else				straight(HALF_SECTION*accel_cnt,SEARCH_ACCEL,0.5,0.5);
    			sla_L90();	cnt = cnt + 1;	accel_cnt = 0;
    			break;
    		case turn_l180:
				if(accel_cnt > 2)	straight(HALF_SECTION*accel_cnt,SEARCH_ACCEL*1.5,1.0,0.5);
				else				straight(HALF_SECTION*accel_cnt,SEARCH_ACCEL,0.5,0.5);
    			turn_L180(SEARCH_TURN_ACCEL, SEARCH_TURN_SPEED);	cnt = cnt + 1;	accel_cnt = 0;
    			break;
    		case diagonal_r:
    			if(run_pattern[cnt+1] == straight_HSTEP ){
    				out_R45();	cnt = cnt + 2;
    			}else if(run_pattern[cnt+1] == turn_r90 && run_pattern[cnt+2] == straight_HSTEP){
    				out_R135();	cnt = cnt + 3;
    			}else if(run_pattern[cnt+1] == turn_r90 && run_pattern[cnt+2] == diagonal_l){
    				v_R90();	cnt = cnt + 2;
    			}else{
    				diagonal(SEARCH_ACCEL,0.5,0.5);	cnt = cnt + 1;
    			}
    			break;
    		case diagonal_l:
    			if(run_pattern[cnt+1] == straight_HSTEP ){
    				out_L45();	cnt = cnt + 2;
    			}else if(run_pattern[cnt+1] == turn_l90 && run_pattern[cnt+2] == straight_HSTEP){
    				out_L135();	cnt = cnt + 3;
    			}else if(run_pattern[cnt+1] == turn_l90 && run_pattern[cnt+2] == diagonal_r){
    				v_L90();	cnt = cnt + 2;
    			}else{
    				diagonal(SEARCH_ACCEL,0.5,0.5);	cnt = cnt + 1;
    			}
    			break;
    		case spin_r90:
				if(accel_cnt > 2)	straight(HALF_SECTION*accel_cnt,SEARCH_ACCEL*1.5,1.0,0.5);
				else				straight(HALF_SECTION*accel_cnt,SEARCH_ACCEL,0.5,0.5);

    			turn_R90(SEARCH_TURN_ACCEL, SEARCH_TURN_SPEED);	cnt = cnt + 1;	accel_cnt = 0;
    			break;
    		case spin_l90:
				if(accel_cnt > 2)	straight(HALF_SECTION*accel_cnt,SEARCH_ACCEL*1.5,1.0,0.5);
				else				straight(HALF_SECTION*accel_cnt,SEARCH_ACCEL,0.5,0.5);

    			turn_L90(SEARCH_TURN_ACCEL, SEARCH_TURN_SPEED);	cnt = cnt + 1;	accel_cnt = 0;
    			break;
    	}
    }

    if(accel_cnt > 2)	straight(HALF_SECTION*accel_cnt,SEARCH_ACCEL*1.5,1.0,0.5);
	else				straight(HALF_SECTION*accel_cnt,SEARCH_ACCEL,0.5,0.5);
	accel_cnt = 0;

    if(goal_size > 1){
		straight(SECTION+7.4,SEARCH_ACCEL,0.5,0.0);
		switch(mypos.dir){
			case north:
				mypos.y++;
				break;
			case east:
				mypos.x++;
				break;
			case south:
				mypos.y--;
				break;
			case west:
				mypos.x--;
				break;
		}
    }
	accel = 0.0; max_speed = 0.0; tar_speed = 0.0;
	HAL_Delay(300);

    run_mode = NON_CON_MODE;
}
