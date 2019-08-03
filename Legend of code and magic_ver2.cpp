//升級到惹 Bronze .. 
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

/**
 * Auto-generated code below aims at helping you parse
 * the standard input according to the problem statement.
 **/

/*
Wood 3
規則講解 總共兩階段 選卡 and 打牌
選卡: Pick X (選擇第X張) PASS 會選擇第一張

打牌: 第一個玩家手牌4張 第二玩家5張
第一玩家 1 max mana 第二玩家 2 max mana
可以使用到自己的max mana 
第二玩家會收到 +1 max mana bonus 直到他某回合用完
每一回合+1 mana

第一階段 Types 只有 Creatures
Creatures 有一個攻擊點數 和 防禦點數
剛被召喚是不能攻擊的 一回合攻擊一次

有哪些操作
SUMMON id : 召喚id 生物出來
ATTACK id1 id2: 用 id1 生物 攻擊 id2 生物
ATTACK id -1: id 生物直接攻擊對手
PASS : 不做事
可以多次做事，用;分開
有一方HP = 0 就結束遊戲

GameInput 
Draft 階段 mana 都是 0
player 0 是自己 1 是對手

*/

struct Player
{
    int Health = 0,Mana = 0,Deck = 0,Rune = 0, Draw = 0;
    /*
        int playerHealth; // 血量30~ 0
        int playerMana; // max = 12, 2號可以13
        int playerDeck; // 甲板有多少牌
        int playerRune; //現在用不到
        int playerDraw; // 現在用不到
    */
};

struct Card
{
    int cardNumber,instanceId,location,cardType,cost,attack,defense,myHealthChange,opponentHealthChange,cardDraw;
    string abilities;
    /*
            int cardNumber; //卡的號碼 一樣的卡就會一樣 (一種怪獸就是一個號碼)
            int instanceId; //卡的id 在你手上的順序?要使用 每張卡都不一樣  要攻擊也是打這個
            int location;  //0 = 我的手 1 = 我的牌面 -1 敵人牌面
            int cardType; //這裡都0 沒啥用
            int cost; //用一次的cost = mana量
            int attack; //攻擊力
            int defense; //防禦力
            string abilities; //沒用
            int myHealthChange; //沒用
            int opponentHealthChange; //沒用
            int cardDraw; //沒用
    */
    bool operator < (const Card &tmp)const{
        return (double)attack/cost > (double)tmp.attack/tmp.cost;
    }
};
int main()
{
    int loop = 0;
    Player p[2];
    // game loop
    while (1) {
        loop ++;
        for (int i = 0; i < 2; i++) {//第二回合是敵人的
            cin >> p[i].Health >> p[i].Mana >> p[i].Deck >> p[i].Rune >> p[i].Draw; cin.ignore();
        }
        int opponentHand; //敵人手上有多少牌
        int opponentActions; // 敵人上回合使用多少action
        cin >> opponentHand >> opponentActions; cin.ignore();
        for (int i = 0; i < opponentActions; i++) { //可以有對手做了哪些動作的資訊 
            string cardNumberAndAction;
            getline(cin, cardNumberAndAction);
        }
        int cardCount; //在board上和player上的牌 draft時 = 3，有多少牌的資訊
        cin >> cardCount; cin.ignore();
        vector <Card> myboard,enermyboard,myhand;
        for (int i = 0; i < cardCount; i++) { //每張牌有的資訊
            Card tmp;
            cin >> tmp.cardNumber >> tmp.instanceId >> tmp.location >> tmp.cardType >> tmp.cost >> tmp.attack >> tmp.defense >> tmp.abilities >> tmp.myHealthChange >> tmp.opponentHealthChange >> tmp.cardDraw; cin.ignore();
            if(tmp.location == 0)
            {
                myhand.push_back(tmp);
            }
            else if(tmp.location == 1)
            {
                myboard.push_back(tmp);
            }
            else enermyboard.push_back(tmp);
            //cerr << "location = " << tmp.location << endl;
        }

        // Write an action using cout. DON'T FORGET THE "<< endl"
        // To debug: cerr << "Debug messages..." << endl;
        //可以用 ; 分開指令
        string opr;
        if(loop <=30) // 選牌階段
        {
            double maxpower;
            int chooseid = 0;
            for(int i=0;i<myhand.size();i++)
            {
                double power = (double)myhand[i].attack/myhand[i].cost; //以攻擊為重
                //double power = (double)(myhand[i].attack+myhand[i].defense)/myhand[i].cost; //攻防都看
                cerr << i << " power = " << power << endl;
                if(i==0) maxpower = power;
                else
                {
                    if(power > maxpower)
                    {
                        maxpower = power;
                        chooseid = i;
                    }
                }
            }
            cout << "PICK " << chooseid << endl;
        }
        else // 開始比賽
        {
            double maxpower;
            int chooseid = myhand[0].instanceId,choose_i = 0;
            int total_cost = 0;
            /*for(int i=0;i<myhand.size();i++)
            {
                if(p[0].Mana < myhand[i].cost) continue;
                double power = (double)myhand[i].attack/myhand[i].cost;
                //double power = (double)(myhand[i].attack+myhand[i].defense)/myhand[i].cost;
                cerr << i << " power = " << power << endl;
                cerr << "attack = " << myhand[i].attack << endl;
                if(i==0) maxpower = power;
                else
                {
                    if(power > maxpower)
                    {
                        maxpower = power;
                        chooseid = myhand[i].instanceId;
                        choose_i = i;
                        cerr << "instanceId = " << myhand[i].instanceId << endl;
                    }
                }
            }*/
            sort(myhand.begin(),myhand.end());
            int op_num = 0,myboard_num = myboard.size();
            for(int i=0;p[0].Mana && myboard_num<6 && myhand.size();i++)
            {
                cerr << "In SUMMON" << endl;
                if(p[0].Mana < myhand[i].cost) continue;
                cout << "SUMMON " << myhand[i].instanceId << ";";
                myboard_num++;
                p[0].Mana -= myhand[i].cost;
                op_num ++;
            }
            sort(myboard.begin(),myboard.end());
            vector <Card> gcard;
            for(int i=0;i<enermyboard.size();i++)
            {
                cerr << "abi = " << enermyboard[i].abilities << endl;
                if(enermyboard[i].abilities[3] == 'G')
                {
                    gcard.push_back(enermyboard[i]);
                }
            }

            for(int i=0;i<myboard.size();i++)
            {
                if(gcard.size()==0)
                {
                    cout << "ATTACK " << myboard[i].instanceId << " -1;";
                }
                else
                {
                    cout << "ATTACK " << myboard[i].instanceId << " " << gcard[gcard.size()-1].instanceId << ";";
                    int e_health = gcard[gcard.size()-1].defense - myboard[i].attack;
                    if(e_health <= 0)
                    {
                        gcard.pop_back();
                    }
                    else
                    {
                        gcard[gcard.size()-1].defense -= myboard[i].attack;
                    }

                }
                op_num ++;

            }
            if(op_num == 0)
            {
                cout << "PASS" << endl;
            }
            else cout << endl;
        }
        cerr << "loop :" << loop << endl;

    }
}
