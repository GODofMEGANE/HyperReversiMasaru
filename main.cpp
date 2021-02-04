#include <bits/stdc++.h>
#include <unistd.h>
using namespace std;

const int INF = int(999999999);

int first;//1or2(最初の手番のチーム)
int max_depth;//探索の深度

int nextTurn(int now){//ターンを次に進める
    //引数 now:現在のターン 返り値 int:次のターン
    //順番は1->3->2->4->1... または3->1->4->2->3...
    if(first == 1){
        switch(now){
        case 1:
            now = 3;
            break;
        case 2:
            now = 4;
            break;
        case 3:
            now = 2;
            break;
        case 4:
            now = 1;
            break;
        }
    }else{
        switch(now){
        case 1:
            now = 4;
            break;
        case 2:
            now = 3;
            break;
        case 3:
            now = 1;
            break;
        case 4:
            now = 2;
            break;
        }
    }
    return now;
}

struct Field{
    vector<vector<int>> color;
    vector<vector<int>> plus;
    int total_stone;//現在置かれている石合計
    int k;//評価値倍率

    Field(){//コンストラクタ
        color = vector<vector<int>> (10,vector<int>(10,0));
        plus = vector<vector<int>> (10,vector<int>(10,0));
        total_stone = 0;
        k = 1;
        color[3][3] = 1;
        color[4][4] = 1;
        color[5][5] = 1;
        color[6][6] = 1;
        color[3][5] = 2;
        color[4][6] = 2;
        color[5][3] = 2;
        color[6][4] = 2;
        color[3][4] = 3;
        color[4][3] = 3;
        color[5][6] = 3;
        color[6][5] = 3;
        color[3][6] = 4;
        color[4][5] = 4;
        color[5][4] = 4;
        color[6][3] = 4;
    }

    void showField(){//Fieldを整形して出力する
        //引数 なし 返り値 void
        for(int i = 0;i < 10;i++){
            for(int j = 0;j < 10;j++){
                printf("%d ",this->color[i][j]);
            }
            printf("\n");
        }
        printf("\n");
        return;
    }

    pair<int,int> calcPoint(){//評価値を計算する
        //引数 なし 返り値 pair<int,int>:(チーム1の評価値,チーム2の評価値)
        pair<int,int> point = make_pair(0,0);
        vector<bool> none = {true,true,true,true};
        vector<vector<int>> value =
        {{ 15,  0,  0,  0,  0,  0,  0,  0,  0, 15},
         {  0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
         {  0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
         {  0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
         {  0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
         {  0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
         {  0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
         {  0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
         {  0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
         { 15,  0,  0,  0,  0,  0,  0,  0,  0, 15}
        };
        for(int i = 0;i < 10;i++){
            for(int j = 0;j < 10;j++){
                if(this->color[i][j] == 1 || this->color[i][j] == 2){
                    point.first += pow(this->plus[i][j]+1,2)+value[i][j]*k;
                    none[this->color[i][j]-1] = false;
                }
                if(this->color[i][j] == 3 || this->color[i][j] == 4){
                    point.second += pow(this->plus[i][j]+1,2)+value[i][j]*k;
                    none[this->color[i][j]-1] = false;
                }
            }
        }
        if(none[0] || none[1]){
            point.first -= 999*k;
        }
        else if(none[2] || none[3]){
            point.second -= 999*k;
        }
        return point;
    }

    bool checkCanPlace(int turn, int x=-1, int y=-1){//石を置けるかどうかを判定する
        //引数 turn:確認する石の種類 (x,y:置く場所を指定して判定) 返り値 bool:置ける場合true
        bool canPlace = false;
        if(x==-1&&y==-1){
            for(int checkx = 0;checkx < 10;checkx++){
                for(int checky = 0;checky < 10;checky++){
                    if(this->color[checkx][checky] == 0){
                        for(int xi = -1;xi < 2;xi++){
                            for(int yi = -1;yi < 2;yi++){
                                for(int move = 1;checkx+xi*move >= 0 && checkx+xi*move < 10 && checky+yi*move >= 0 && checky+yi*move < 10;move++){
                                    if(this->color[checkx+xi*move][checky+yi*move] == 0 || (this->color[checkx+xi*move][checky+yi*move] == turn && move == 1)){
                                        break;
                                    }
                                    else if(this->color[checkx+xi*move][checky+yi*move] == turn){
                                        canPlace = true;
                                        break;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        else if(this->color[x][y] == 0 && 0<=x && x<10 && 0<=y && y<10){
            for(int xi = -1;xi < 2;xi++){
                for(int yi = -1;yi < 2;yi++){
                    for(int move = 1;x+xi*move >= 0 && x+xi*move < 10 && y+yi*move >= 0 && y+yi*move < 10;move++){
                        if(this->color[x+xi*move][y+yi*move] == 0 || (this->color[x+xi*move][y+yi*move] == turn && move == 1)){
                            break;
                        }
                        else if(this->color[x+xi*move][y+yi*move] == turn){
                            canPlace = true;
                            break;
                        }
                    }
                }
            }
        }
        if(canPlace){
            return true;
        }
        else{
            return false;
        }
    }

    void calcField(int turn, int x, int y){//指定位置に石を置いた場合の次の盤面を計算
        //引数 turn:石の種類 x,y:石を置く位置 返り値 void
        for(int xi = -1;xi < 2;xi++){
            for(int yi = -1;yi < 2;yi++){
                for(int move = 1;x+xi*move >= 0 && x+xi*move < 10 && y+yi*move >= 0 && y+yi*move < 10;move++){
                    if(this->color[x+xi*move][y+yi*move] == 0 || (this->color[x+xi*move][y+yi*move] == turn && move == 1)){
                        break;
                    }
                    else if(this->color[x+xi*move][y+yi*move] == turn){
                        for(int i = move;i > 0;i--){
                            if(turn == 1 || turn == 2){
                                if(this->color[x+xi*i][y+yi*i] == 3 || this->color[x+xi*i][y+yi*i] == 4){
                                    if(this->plus[x+xi*i][y+yi*i] == 0){
                                        this->color[x+xi*i][y+yi*i] = turn;
                                    }
                                    else{
                                        this->plus[x+xi*i][y+yi*i]--;
                                    }
                                }
                                else if(this->color[x+xi*i][y+yi*i] != turn){
                                    this->color[x+xi*i][y+yi*i] = turn;
                                    this->plus[x+xi*i][y+yi*i]++;
                                }
                            }
                            else{
                                if(this->color[x+xi*i][y+yi*i] == 1 || this->color[x+xi*i][y+yi*i] == 2){
                                    if(this->plus[x+xi*i][y+yi*i] == 0){
                                        this->color[x+xi*i][y+yi*i] = turn;
                                    }
                                    else{
                                        this->plus[x+xi*i][y+yi*i]--;
                                    }
                                }
                                else if(this->color[x+xi*i][y+yi*i] != turn){
                                    this->color[x+xi*i][y+yi*i] = turn;
                                    this->plus[x+xi*i][y+yi*i]++;
                                }
                            }
                        }
                        break;
                    }
                }
            }
        }
        this->color[x][y] = turn;
        return;
    }

    pair<int,pair<int,int>> judge(int team, int turn, int depth, int alpha, int beta){//min-max法で次に置くべき位置を決定
        //引数 team:自分のチーム,turn:置く石の色,depth:現在の深さ(再帰用) 返り値:pair<int,pair<int,int>>(現在の手の最低評価値,(置く位置y,置く位置x))
        int thisalpha = INF;
        int thisbeta = -INF;
        if(depth == 0){//最初の探索時だけ石の数を更新
            this->calcTotalStone();
            //探索の深さを石の数に応じて変更
            if(100-total_stone < 10){
                max_depth = 99-total_stone;
                k = 0;
            }
            else if(total_stone < 20 || 100-total_stone < 16){
                max_depth = 5;
                k = 1;
            }
            else{
                max_depth = 4;
                k = 1;
            }
            printf("    Search Depth:%d, Multiplier:%d\n",max_depth+1,k);
        }

        int minmax;
        if(depth % 2 == 1){
            minmax = INF;
        }else{
            minmax = -INF;
        }
        pair<int,int> goodPlace = make_pair(-1,-1);
        bool endSearch = false;
        for(int x = 0;x < 10 && !endSearch;x++){
            for(int y = 0;y < 10 && !endSearch;y++){
                Field dummyField = *this;
                if(dummyField.checkCanPlace(turn,x,y)){
                    dummyField.calcField(turn,x,y);
                    if(depth < max_depth){//まだ探索を継続
                        clock_t minjudge_start = clock();
                        pair<int,pair<int,int>> result = dummyField.judge(team,nextTurn(turn),depth+1,thisalpha,thisbeta);
                        clock_t minjudge_end = clock();
                        if(depth==0){//一番浅い部分のみでログを出力
                            printf("    searching:(%d,%d),%dpts\n",x,y,result.first);
                            double minjudge_time = static_cast<double>(minjudge_end-minjudge_start)/CLOCKS_PER_SEC*1000.0;
                            printf("    SolveTime:%.3lfsec\n",minjudge_time);
                        }
                        if((result.first > minmax && depth%2 == 0) || (result.first < minmax && depth%2 == 1)){//minmaxを更新したら座標も更新
                            minmax = result.first;
                            goodPlace = make_pair(x,y);
                            if(depth%2 == 0){
                                if(alpha < minmax){
                                    endSearch = true;
                                }
                                thisbeta = minmax;
                            }
                            else if(depth%2 == 1){
                                if(beta > minmax){
                                    endSearch = true;
                                }
                                thisalpha = minmax;
                            }
                        }
                    }
                    else{//ここで探索を打ち切る
                        pair<int,int> point_result = dummyField.calcPoint();
                        int point;
                        if(team==1){//チームによって差を出し、敵との評価値の差を求める
                            point = point_result.first-point_result.second;
                        }else{
                            point = point_result.second-point_result.first;
                        }

                        if(depth==0){//max_depth=0の場合にログを出力
                            printf("    searching:(%d,%d),%dpts\n",x,y,point);
                        }
                        if((point > minmax && depth%2 == 0) || (point < minmax && depth%2 == 1)){//minmaxを更新したら座標も更新
                            minmax = point;
                            goodPlace = make_pair(x,y);
                        }
                    }
                }

            }
        }
        return make_pair(minmax,goodPlace);
    }

    void calcTotalStone(){//現在置かれている石の合計を取得し、更新
        //引数 なし 返り値 なし
        int ans = 0;
        for(int i = 0;i < 10;i++){
            for(int j = 0;j < 10;j++){
                if(this->color[i][j] != 0){
                    ans++;
                }
            }
        }
        this->total_stone = ans;
        return;
    }
};







int main(){
    Field field;
    int team;//12どちらかアルゴリズムに任せるチームを選択
    int turn;//1~4(マニュアルモードで開始ターンを指定)
    printf("Select Input Mode (1:Auto 2:Manual)");//Autoにすると初期盤面でスタート
    int select;
    scanf("%d",&select);
    if(select == 1){
        field.showField();
        printf("Input first team and your team\n");//teamに2を指定すると34を自動操作、12を手動操作
        scanf("%d %d",&first,&team);
        if(first == 2){//firstによってturnの初期値を自動設定
            turn = 3;
        }else{
            turn = 1;
        }
    }
    else{
        printf("Input field\n");
        for(int i = 0;i < 10;i++){
            for(int j = 0;j < 10;j++){
                scanf("%d",&field.color[i][j]);//チーム(12vs34,0=null)
            }
        }
        printf("Input plus number\n");
        for(int i = 0;i < 10;i++){
            for(int j = 0;j < 10;j++){
                scanf("%d",&field.plus[i][j]);//+値
            }
        }
        printf("Input first move and your team and turn\n");
        scanf("%d %d %d",&first,&team,&turn);
    }

    field.calcTotalStone();
    printf("Game Start!\n");
    pair<bool,bool> pass = make_pair(false,false);//二人ともPASSだった場合、その時点でゲーム終了
    while((!pass.first || !pass.second) && field.total_stone < 100){
        if((team == 1 && (turn == 1 || turn == 2)) || (team == 2 && (turn == 3 || turn == 4))){//アルゴリズムのターン
            printf("---------Masaru's Turn---------\n");
            printf("    (turn:%d)\n",turn);
            if(field.checkCanPlace(turn)){//PASSしなければいけないか判定
                clock_t judge_start = clock();//探索時間を計測
                pair<int,pair<int,int>> ans = field.judge(team,turn,0,INF,-INF);//最適解を探索
                clock_t judge_end = clock();
                double minjudge_time = static_cast<double>(judge_end-judge_start)/CLOCKS_PER_SEC*1000.0;
                printf("    TotalSolveTime:%.3lfmsec\n",minjudge_time);
                if(100-field.total_stone < 10 && ans.first){
                    printf("*Masaru will win by at least %d point\n",ans.first);
                }
                printf("MEGANE's AI MASARU says (%d,%d)!\n",ans.second.first,ans.second.second);//座標を出力
                field.calcField(turn, ans.second.first, ans.second.second);//同時に盤面も進める
            }
            else{
                printf("PASS!\n");
                pass.first = true;
            }
            field.calcTotalStone();
            turn = nextTurn(turn);
        }
        else{
            printf("---------Your Turn---------\n");//手動のターン
            if(field.checkCanPlace(turn)){//PASSしなければいけないか判定
                field.showField();
                printf("Input your act (turn:%d)\n",turn);
                int x=-1, y=-1;
                do{//置けない場所の場合、エラーを表示
                    if(x!=-1||y!=-1){
                        printf("Can not place here!\n");
                    }
                    printf("y:");
                    scanf("%d",&x);
                    printf("x:");
                    scanf("%d",&y);
                }while(!field.checkCanPlace(turn,x,y));
                field.calcField(turn, x, y);
            }
            else{
                printf("PASS!(Input any number to continue)");
                int dummy;
                scanf("%d",&dummy);//PASS時に流れるのを防止するためダミー入力
                pass.second = true;
            }
            field.calcTotalStone();
            turn = nextTurn(turn);
        }
    }
    printf("\n---------Game End---------\n");
    field.showField();
    field.k = 0;
    pair<int,int> result = field.calcPoint();
    if(result.first == result.second){
        printf("DRAW!\n");
    }
    else if(team == 1 && result.first > result.second){
        printf("MASARU WIN!\n");
    }
    else{
        printf("PLAYER WIN!\n");
    }
    return 0;
}
