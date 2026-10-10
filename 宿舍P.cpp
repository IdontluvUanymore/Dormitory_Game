#include<bits/stdc++.h>
#include<conio.h> 
using namespace std;

const string Team_members="Team_members:\n[0] ideas & the_second_programmer: (...) A舍\n[1] character_designer: Gao Jianling\n[2] ideas & the_first_programmer: A_t\n\n",
		   Special_thanks="Special_thanks:\n[0] lousy_ideas: Change & Egg\n[1] game_test: Huang Baodong\n[2] supporter: Gwe Yi\n\n",
   the_information_of_dog="The invincible in the universe,\nthe most handsome,\nthe most Yushulinfeng,\nand the most natural and unrestrained.\n\t\t\t\t--itself\n\nThe public pet dog of Class 2014.\n\t\t\t\t--its_classmates\n\nIt was the Bed.E of Room.513 before it left.\n\t\t\t\t\t--its_roomates";

void wait(int time){
	clock_t t0=clock();
	while(clock()-t0!=time*1000){
	}
}

struct hurt;

class per {
	
	public:

		int label;				//玩家编号
		string job;				//职业
		float HP;				//生命上限
		float hp;				//当前生命值
		int bit;				//当前豆数
		int add_bit;			//本回合增加的豆数
		int cost_bit;			//本回合消耗的豆数 
		bool defense;			//当前是否处于防御状态
		char use;				//当前使用的技能的符号
		int dizzy;				//剩余晕眩回合
		bool turn_hurt;			//当前是否处于反转状态 
		
		bool b;

		virtual void show() = 0;
		virtual bool choose() = 0;
		void Add_Bit() {
			add_bit = 1;
		}
		void Bit() {
			bit += add_bit;
			add_bit = 0;
		}
		void costBit(){
			bit-=cost_bit;
			cost_bit=0; 
		}
		virtual void showSkill() = 0;
		virtual void Hit(int p) = 0;
		virtual void Defense() = 0;
		virtual void injure(hurt h) = 0;
		virtual bool alive() {
			if (hp > 0) {
				return true;
			} else {
				return false;
			}
		}

};

struct hurt {
	string kind;		//攻击的类型 
	int cost;			//攻击消耗的豆数 
	per* from;			//发起攻击的玩家
	int target;			//被攻击目标的编号
	float num;			//攻击的伤害值
	bool indefensible;	//攻击是否破防
	int dizzy_num;		//攻击眩晕回合数 
	bool hiden;			//攻击是否隐藏 
};

vector<per*>p;			//各个玩家的指针（之所以用指针是因为这样可以优化内存分配）
int sum;				//总人数
queue<hurt> hts;		//攻击队列

class knight :public per {

	friend class hypnotist;

	public:
		
		struct shield {
			float HP;
			float hp;
			bool is_being_used;
		}s;

		struct cam {
			bool is_being_used;
		}c;

		void Shield() {
			b=true;
			if (c.is_being_used) {
				c.is_being_used = false;
			}
			s.is_being_used = true;
		}

		void Shield_Repair() {
			if (!s.is_being_used) {
				s.hp += 0.25;
			}
			if (s.hp > s.HP) {
				s.hp = s.HP;
			}
		}

		void Cam() {
			b=true;
			if (s.is_being_used) {
				s.is_being_used = false;
			}
			c.is_being_used = true;
		}

		void Rush(int n) {
			cost_bit+=4; 
			s.HP=0;
			s.hp=0;
			s.is_being_used=false; 
			Cam();
			dizzy += 1;
			hurt rush = { "眩晕物理伤害",4,this,n,3,false,1,false };
			hts.push(rush);
		}

		void Assault(int n) {
			cost_bit+= 3;
			Shield();
			hurt assault = { "普通物理伤害",3,this,n,4,false };
			hts.push(assault);
		}

		void Hit(int p) {
			cost_bit+=1;
			if (c.is_being_used) {
				hurt hit = { "普通物理伤害",1,this,p,1.5,false };
				hts.push(hit);
			} else {
				hurt hit = { "普通物理伤害",1,this,p,1,false };
				hts.push(hit);
			}
		}

		void Defense() {
			defense = true;
		}

		void injure(hurt h) {
			if(h.kind=="吸血"){
				hp-=h.num;
			}else if ((!defense) || (defense && !(h.indefensible))) {
				if (s.is_being_used) {
					if (s.hp >= h.num) {
						s.hp -= h.num;
					} else {
						s.hp = 0;
						hp -= h.num - s.hp;
					}
				} else {
					hp -= h.num;
				}
			}
		}

		void show() {
			cout << label << "号玩家(" << job << "):" << endl;
			cout << "\thp: " << hp << "/" << HP << "\tbit: " << bit << endl;
			cout << "\t盾: " << s.hp << "/" << s.HP << endl;
			if (s.is_being_used&&b || c.is_being_used&&!b) {
				cout << "\t正在持剑" << endl;
			}
			if (c.is_being_used&&b || s.is_being_used&&!b) {
				cout << "\t正在持盾" << endl;
			}
		}

		void showSkill() {
			cout << "*****************************************************" << endl
			     << "***攒豆--B                                          *" << endl
			     << "***普攻--H（持剑时伤害更高，耗豆：1）               *" << endl
			     << "***防御--D                                          *" << endl
			     << "***持剑--C                                          *" << endl
			     << "***持盾--S                                          *" << endl
			     << "***持盾重击--R（使用后双方眩晕，盾损坏，耗豆：4）   *" << endl
			     << "***持剑重击--A（耗豆：3）                           *" << endl
			     << "*****************************************************" << endl;
		}

		bool choose() {
			Shield_Repair();
			if (dizzy) {
				cout << "晕眩回合: " << dizzy << "->" << dizzy-1 << "\t无法行动";
				dizzy-=1;
				use = '*';
				system("pause");
				return true;
			}
			use = getch();
			if (use == 'b') {
				Add_Bit();
			} else if (use == 'h') {
				if (bit >= 1) {
					 
					int n = (int)getch() - 48;
					if (n == label) {
						cout << "Retry, please." << endl;
						return false;
					} else if (n < sum && n >= 0 && p[n]->alive()) {
						Hit(n);
					} else {
						cout << "Retry, please." << endl;
						return false;
					}
				} else {
					cout << "Retry, please." << endl;
					return false;
				}
			} else if (use == 'd') {
				Defense();
			} else if (use == 's') {
				Shield();
			} else if (use == 'c') {
				Cam();
			}else if(use == 'r'){
				if(bit>=4&&s.is_being_used){					 
					int n=(int)getch()-48;
					if (n!=label&&n<sum&&n>=0 && p[n]->alive()){
						Rush(n);
					}else{
						cout << "Retry, please." << endl;
						return false;
					}
				}else{
					cout << "Retry, please." << endl;
					return false;
				}
			}else if(use == 'a'){
				if(bit >= 3 && c.is_being_used) {
					int n = (int)getch() - 48;
					if(n!=label&&n<sum&&n>=0 && p[n]->alive()){
						Assault(n);
					}else{
						cout << "Retry, please." << endl;
						return false;
					}
				} else {
					cout << "Retry, please." << endl;
					return false;
				}
			} else if (use == '.') {
				showSkill();
				system("pause");
				return false;
			} else {
				cout << "What the FUCK ?! 请重试。" << endl;
				return false;
			}
			return true;
		}



		knight() {
			job = "骑士";
			HP = hp = 10;
			bit = 0;
			add_bit=0;
			cost_bit=0;
			defense = false;
			s.HP = s.hp = 4;
			Shield();
			dizzy = 0;
			turn_hurt=false;
			b=false;
		}

};

class drunker :public per {

	friend class hypnotist;

	public:

		float d;

		void Wine() {
			d += 1;
		}

		bool alive() {
			if (d < 8) {
				return true;
			} else {
				return false;
			}
		}

		void Hit(int p) {
			if (d < 4) {
				cost_bit+=1;
				hurt hit = { "普通物理伤害",1,this,p,(float)1+d/2,false };
				hts.push(hit);
			} else {
				cost_bit+=1;
				hurt hit = { "穿刺物理伤害",1,this,p,(float)1+d/2,true };
				hts.push(hit);
			}
		}

		void Defense() {
			defense = true;
		}

		void injure(hurt h) {
			if ((!defense) || (defense && h.indefensible)) {
				d += h.num;
			}
		}

		void show() {
			cout << label << "号玩家(" << job << "):" << endl;
			cout << "\thp: " << hp << "/" << HP << "\tbit: " << bit << endl;
			cout << "\t醉值: " << d << endl;
		}

		void showSkill() {
			cout << "*************" << endl
			     << "*  攒豆--B  *" << endl
			     << "*  普攻--H  *" << endl
			     << "*  防御--D  *" << endl
			     << "*  喝酒--W  *" << endl
			     << "*************" << endl;
		}

		bool choose() {
			if (!alive()) {
				cout << "醉值超过限制,醉死" << endl;
				use = '*';
				system("pause");
				return true;
			}
			if (dizzy) {
				cout << "晕眩回合: " << dizzy << "->" << dizzy-1 << "\t无法行动";
				dizzy-=1;
				use = '*';
				system("pause");
				return true;
			}
			use = getch();
			if(use == 'b'){
				Add_Bit();
			}else if(use == 'h'){
				if(bit>=1){
					int n = (int)getch() - 48;
					if(n!=label&&n<sum&&n>=0 && p[n]->alive()){
						Hit(n);
					}else{
						cout << "Retry, please." << endl;
						return false;
					}
				}else{
					cout << "Retry, please." << endl;
					return false;
				}
			} else if (use == 'd') {
				Defense();
			} else if (use == 'w') {
				Wine();
			} else if (use == '.') {
				showSkill();
				system("pause");
				return false;
			} else {
				cout << "What the FUCK ?! 请重试。" << endl;
				return false;
			}
			return true;
		}



		drunker() {
			job = "酒鬼";
			HP = hp = 10;
			bit = 0;
			add_bit=0;
			cost_bit=0;
			d = 0;
			dizzy = 0;
			turn_hurt=false;
		}

};

class pharmacist :public per {

	friend class hypnotist;

	public:

		//调制毒药
		struct poison {
			bool is_being_used;
			int sigh;
		} po;

		//调制毒药
		void Poison() {
			cost_bit+=2;
			po.is_being_used = true;
		}

		//毒药印记
		void Sigh() {
			if (po.is_being_used) {
				po.sigh += 1;
			}
		}

		//毒伤（破防）
		void Posion_Take_Effect() {
			for (int i = 0; i < sum; i++) {
				if (p[i] != this) {
					cost_bit+=1;
					hurt poe = { "穿刺毒药伤害",1,this,i,(float)po.sigh/2,true };
					hts.push(poe);
				}
			}
			po.is_being_used = false;
			po.sigh = 0;
		}

		void Hit(int p) {
			cost_bit+=1;
			hurt hit = { "普通物理伤害",1,this,p,1,false };
			hts.push(hit);
		}

		void Defense() {
			defense = true;
		}

		void injure(hurt h) {
			if ((!defense) || (defense && h.indefensible)) {
				hp -= h.num;
			}
		}

		void showSkill() {
			cout << "*************" << endl
			     << "*  攒豆--B  *" << endl
			     << "*  普攻--H  *" << endl
			     << "*  防御--D  *" << endl
			     << "*  叠毒--P  *" << endl
			     << "*  毒爆--E  *" << endl
			     << "*************" << endl;
		}

		void show() {
			cout << label << "号玩家(" << job << "):" << endl;
			cout << "\thp: " << hp << "/" << HP << "\tbit: " << bit << endl;
			if (po.is_being_used) {
				cout << "\t毒药正在配置中";
				cout << "\t毒药印记: " << po.sigh << endl;
			}
		}

		bool choose() {
			if (dizzy) {
				cout << "晕眩回合: " << dizzy << "->" << dizzy-1 << "\t无法行动";
				dizzy-=1;
				use = '*';
				Sigh();
				system("pause");
				return true;
			}
			use = getch();
			if (use == 'b') {
				Add_Bit();
			} else if (use == 'h') {
				if (bit >= 1) {
					int n = (int)getch() - 48;
					if (n == label) {
						cout << "Retry, please." << endl;
						return false;
					} else if (n < sum && n >= 0 && p[n]->alive()) {
						Hit(n);
					} else {
						cout << "Retry, please." << endl;
						return false;
					}
				} else {
					cout << "Retry, please." << endl;
					return false;
				}
			} else if (use == 'd') {
				Defense();
			} else if (use == 'p') {
				if (bit >= 2) {
					Poison();
				} else {
					cout << "Retry, please." << endl;
					return false;
				}
			} else if (use == 'e') {
				if (po.is_being_used) {
					Posion_Take_Effect();
				}
			} else if (use == '.') {
				showSkill();
				system("pause");
				return false; 
			} else {
				cout << "What the FUCK ?! 请重试。" << endl;
				return false;
			}
			Sigh();
			return true;
		}

		pharmacist() {
			job = "药师";
			HP = hp = 10;
			bit = 0;
			add_bit=0;
			cost_bit=0;
			po.is_being_used = false;
			po.sigh = 0;
			dizzy = 0;
			turn_hurt=false;
		}

};

class priest:public per{
	
	private:
		
		struct ghoul{
			int num;
			int blood_owner;
		}g;
		
	public:
		void Hit(int p) {
			cost_bit+=1;
			hurt hit = { "普通物理伤害",1,this,p,1,false };
			hts.push(hit);
		}

		void Defense() {
			defense = true;
		}

		void Ghoul(){
			hurt costhp={"召唤",0,this,label,0.5,true,0,false};
			hts.push(costhp); 
			if(g.num==0){
				g.blood_owner=-1;
			}
			g.num+=1;
		}
		
		void Suck(){
			g.blood_owner=hts.front().target;
			hts.push(hts.front());
			hts.pop();
			for(int i=1;i<hts.size();i++){
				if(p[hts.front().target]->hp>p[g.blood_owner]->hp || p[hts.front().target]->hp==p[g.blood_owner]->hp&&p[hts.front().target]->bit>=p[g.blood_owner]->bit){
					g.blood_owner=hts.front().target;
				}
				hts.pop();
				hts.push(hts.front());
			}
			if(g.blood_owner!=label){
				hurt suck={"\b\bs(吸血",0,this,g.blood_owner,(float)g.num/2,true,0,false};
				hts.push(suck);
			}
		}
		
		void Callback(){
			g.num-=1;
			hurt addhp={"",0,this,label,-0.5,true,0,true};
			hts.push(addhp); 
		}
		
		void Sacrifice(){
			bit-=1;
			hurt addhp={"",0,this,label,(float)g.num/-2,true,0,true};
			hts.push(addhp); 
			g.num=0;
			g.blood_owner=-1; 
		}
		
		void injure(hurt h) {
			if(h.from==this){
				hp-=h.num;
				return;
			} 
			if(g.num){
				g.num=0;
				if(h.from==p[g.blood_owner]){
					g.blood_owner=-1;
					hurt regive={"返还血量",0,this,h.from->label,(float)g.num/-2,true,0,false};
					hts.push(regive);
				}
			}else if((!defense) || (defense && h.indefensible)){
				hp-=h.num;
			}
		}

		void showSkill() {
			cout << "***************" << endl
			     << "*  攒豆--B    *" << endl
			     << "*  普攻--H    *" << endl
			     << "*  防御--D    *" << endl
			     << "*  食尸鬼--G  *" << endl
			     << "*  召回--C    *" << endl
			     << "*  祭祀--S    *" << endl
			     << "***************" << endl;
		}

		void show() {
			cout << label << "号玩家(" << job << "):" << endl;
			cout << "\thp: " << hp << "/" << HP << "\tbit: " << bit << endl;
			if(g.num){
				cout<<"\t食尸鬼数量:"<<g.num<<"\t'血'的故主: ";
				if(g.blood_owner==-1){
					cout<<"NULL  }"<<endl;
				}else{
					cout<<g.blood_owner<<"号玩家("<<p[g.blood_owner]->job<<")  }"<<endl;
				}
			}
		}

		bool choose() {
			if (dizzy) {
				cout << "晕眩回合: " << dizzy << "->" << dizzy-1 << "\t无法行动";
				dizzy-=1;
				use = '*';
				Suck();
				system("pause");
				return true;
			}
			use = getch();
			if (use == 'b') {
				Add_Bit();
			} else if (use == 'h') {
				if (bit >= 1) {					 
					int n = (int)getch() - 48;
					if (n == label) {
						cout << "Retry, please." << endl;
						return false;
					} else if (n < sum && n >= 0 && p[n]->alive()) {
						Hit(n);
					} else {
						cout << "Retry, please." << endl;
						return false;
					}
				} else {
					cout << "Retry, please." << endl;
					return false;
				}
			} else if (use == 'd') {
				Defense();
			}else if(use=='g'){
				if(hp>=0.5){
					Ghoul();
				}
			}else if(use=='c'){
				if(g.num){
					Callback();
				}else{
					cout<<"Retry, please."<<endl;
					return false;
				}
			}else if(use=='s'){
				if(g.num&&bit>=1){
					Sacrifice();
				}else{
					cout<<"Retry, please."<<endl;
					return false;
				}
			}else if (use == '.') {
				showSkill();
				system("pause");
				return false; 
			} else {
				cout << "What the FUCK ?! 请重试。" << endl;
				return false;
			}
			if(g.num){
				Suck();
			}
			return true;
		}
		
		priest(){
			job="祭司";
			HP=hp=10;
			defense=false;
			bit=0;
			cost_bit=0;
			add_bit=0;
			g={0,-1};
			dizzy=0;
			turn_hurt=false;
		}
};
//class hypnotist : public per {
//	
//	public:
//
//		struct pocket_watch { //怀表
//			bool is_being_used;
//		} pw;
//
//		void Sleep() { //催眠
//			if (pw.is_being_used == false) {
//				cost_bit+=5;
//				pw.is_being_used = true;
//			} else {
//				cout << "怀表已使用" << endl;
//
//			}
//		}
//
//		void Close_Sleep() {
//			if (pw.is_being_used == true) {
//				pw.is_being_used = false;
//			} else {
//				cout << "怀表未使用" << endl;
//				system("pause");
//			}
//		}
//
//		void injure(hurt h) {
//			if (pw.is_being_used == false) {
//				if ((!defense) || (defense && h.indefensible)) {
//					hp -= h.num;
//				}
//			} else {
//				h.from->hp -= h.num;//直接让对手扣血，相当于穿刺伤害，如果觉得不合适的话就改成 int t = h.from->label;  h.from->label = h.target;  h.target = t;  就是把攻击者和被攻击者的编号互换，这样应该可以保留原本攻击的各项属性，比如破防之类的
//				hp = HP;
//			}
//		}
//
//		void Hit(int p) {
//			cost_bit+=1;
//			hurt hit { "普通魔法伤害",this,p,1,false };
//			hts.push(hit);
//		}
//
//		void Defense() {
//			defense = true;
//		}
//
//		void showSkill() {
//			cout << "*****************************************************" << endl
//			     << "***攒豆--B*******************************************" << endl
//			     << "***普攻--H*******************************************" << endl
//			     << "***防御--D*******************************************" << endl
//			     << "***催眠--S*******************************************" << endl
//			     << "***取消催眠--C***************************************" << endl
//			     << "*****************************************************" << endl;
//		}
//
//		void show() {
//			cout << label << "号玩家(" << job << "):" << endl;
//			cout << "\thp: " << hp << "/" << HP << "\tbit: " << bit << endl;
//
//		}
//
//		bool choose() {
//			if (dizzy) {
//				dizzy -= 1;
//				cout << "晕眩回合: " << dizzy << "\t无法行动";
//				use = '*';
//				system("pause");
//				return true;
//			}
//			use = getch();
//			if (use == 'b') {
//				Add_Bit();
//			} else if (use == 'h') {
//				if (bit >= 1) {
//					 
//					int n = (int)getch() - 48;
//					if (n == label) {
//						cout << "不能自残" << endl;
//						return false;
//					} else if (n < sum && n >= 0 && p[n]->alive()) {
//						Hit(n);
//					} else {
//						cout << "此玩家不存在场上，请重试" << endl;
//					}
//				} else {
//					cout << "你没有bit，请重试" << endl;
//					return false;
//				}
//			} else if (use == 'd') {
//				Defense();
//			} else if (use == 's') {
//				int num = 0;
//				if (bit >= 5) {
//					Sleep();
//					num++;
//					if (num >= 5) { //最多使用五次催眠，不然可能有点强，参考月圆之夜里面有个叫幽灵船长还是什么的,反伤贼恶心
//						pw.is_being_used == false;
//						cout << "怀表已损坏" << endl;
//						return false;
//					}
//				} else {
//					cout << "你没有bit，请重试。" << endl;
//					return false;
//				}
//
//			} else if (use == 'c') {
//				Close_Sleep();
//			} else if (use == '.') {
//				showSkill();
//				return false;
//			} else {
//				cout << "What the Fuck?! 请重试。" << endl;
//				return false;
//			}
//			return true;
//		}
//
//
//		hypnotist() {
//			job = "催眠师";
//			HP = hp = 10;
//			bit = 0;
//			dizzy = 0;
//			pw.is_being_used = false;
//		}
//
//};

class dog:public per{
	
	friend class hypnotist;
	
	private:
		
		int sharp_teeth;
		int clothesline_poles;
		
	public:
		
		void Hit(int p) {
			cost_bit+=1;
			hurt hit={"咬",1,this,p,1,false};
			if(clothesline_poles==0){
				clothesline_poles=7;
				hit.hiden=true;
			}
			hts.push(hit);
		}

		void Defense() {
			defense = true;
		}

		void Turn(){
			sharp_teeth-=1;
			turn_hurt=true;
		}
		
		void injure(hurt h) {
			if(turn_hurt){
				h.cost=0;
				h.target=h.from->label;
				h.from=this;
				hts.push(h);
			}else if((!defense) || (defense && h.indefensible)) {
				hp -= h.num;
			}
		}

		void showSkill() {
			cout << "*************" << endl
			     << "*  攒豆--B  *" << endl
			     << "*  普攻--H  *" << endl
			     << "*  防御--D  *" << endl
			     << "*  反转--T  *" << endl
			     << "*************" << endl;
		}

		void show() {
			cout << label << "号玩家(" << job << "):" << endl;
			cout << "\thp: " << hp << "/" << HP << "\tbit: " << bit << endl;
			cout<<"\t利齿: "<<sharp_teeth<<endl;
			cout<<"\t撑衣杆: "<<clothesline_poles<<endl;
		}

		bool choose() {
			if (dizzy) {
				dizzy -= 1;
				cout << "晕眩回合: " << dizzy << "\t无法行动";
				use = '*';
				system("pause");
				return true;
			}
			use = getch();
			if (use == 'b') {
				Add_Bit();
			}else if(use == 'h'){
				if(bit >= 1){
					int n = (int)getch() - 48;
					if(n!=label&&n<sum&&n>=0 && p[n]->alive()) {
						Hit(n);
					}else{
						cout << "Retry, please." << endl;
						return false;
					}
				}else{
					cout << "Retry, please." << endl;
					return false;
				}
			} else if (use == 'd') {
				Defense();
			} else if (use=='t'){
				Turn();
			} else if (use == '.') {
				showSkill();
				system("pause");
			} else {
				cout << "What the FUCK ?! 请重试。" << endl;
				return false;
			}
			if(clothesline_poles){
				clothesline_poles-=1;
			}
			return true;
		}

		dog(){
			job="A舍"; 
			HP=hp=10;
			bit=0;
			add_bit=0;
			cost_bit=0;
			defense=false;
			dizzy=0;
			sharp_teeth=5;
			clothesline_poles=7;
			turn_hurt=false;
		}
		
};

int main() {

	//主界面
	cout<<Team_members<<Special_thanks;
	printf("键入 空格 开始宿舍P;\n\n");
	printf("键入 / 查看Team_member[0]的更多信息;\n\n");
	printf("如果在P时不知道当前角色技能,在自己回合键入 . ;\n\n");
	printf("无论何时,键入 ^C 或 Alt + F4 退出宿舍P;\n\n");
	while(true){
		char gc=getch();
		if(gc==32){
			system("cls");
			break;
		}else if(gc=='/'){
			system("cls");
			cout<<the_information_of_dog<<endl;
			system("pause"); 
			system("cls");
			cout<<Team_members<<Special_thanks;
			printf("键入 空格 开始宿舍P;\n\n");
			printf("键入 / 查看Team_member[0]的更多信息;\n\n");
			printf("如果在P时不知道当前角色技能,在自己回合键入 . ;\n\n");
			printf("无论何时,键入 ^C 或 Alt + F4 退出宿舍P;\n\n");
		}else{
			cout<<"Retry, please."<<endl;
		}
	}

	//选角阶段
	cout << "输入玩家数: " << endl;
	cin >> sum;
	system("cls");
	for (int i = 0; i < sum; i++) {
		int Job;
		cout << "请" << i << "号玩家输入职业" << endl;
		cout << "+******************+" << endl;
		cout << "*  0.祭司      √  *" << endl;
		cout << "*  1.骑士      √  *" << endl;
		cout << "*  2.酒鬼      √  *" << endl;
		cout << "*  3.药师      √  *" << endl;
		cout << "*  4.催眠师        *" << endl;
		cout << "*  5.剑客          *" << endl;
		cout << "*  6.矿工          *" << endl;
		cout << "*  ...             *" << endl;
		cout << "*  X.A舍(狗狗) √  *" << endl;
		cout << "+******************+" << endl;
		g:
		cin >> Job;
		switch (Job) {
			case 0:
				p.push_back(new(priest));
				break;
			case 1:
				p.push_back(new(knight));
				break;
			case 2:
				p.push_back(new(drunker));
				break;
			case 3:
				p.push_back(new(pharmacist));
				break;
//			case 4:
//				p.push_back(new(hypnotist));
//				break;
//			case 5:
//				p.push_back(new(swordman));
//				break;
//			case 6:
//				p.push_back(new(miner));
//				break;
			case 20145135:
				p.push_back(new(dog));
				break;
			default:
				cout << "What the FUCK ?! 请重试。" << endl;
				goto g;
		}

		system("cls");
	}

	for (int i = 0; i < p.size(); i++) {
		p[i]->label = i;
	}

	int survivor = 0, winner;
	while (true) {

		//出招阶段��
		for(int i=0;i<sum;i++){
			if(p[i]->job=="骑士"){
				p[i]->b=false;
			}
		}
		for (int i = 0; i < sum; i++) {
			if (p[i]->alive()) {
				for (int j = 0; j < sum; j++) {
					p[j]->show();
					cout << "\n";
				}
				cout << i << "号玩家(" << p[i]->job << ")出招中..." << endl;
				while (true) {
					if (p[i]->choose()) {
						break;
					}
				}
				system("cls");
			}
		}

		//攻击结算
		while(!hts.empty()){
			hurt now = hts.front();
			p[now.target]->injure(now);
			if(!now.hiden){
				cout<<now.from->label<<"号玩家("<<now.from->job<< ")使用"<<now.cost<<"点能量使用"<<now.from->use<<"对"<<now.target<<"号玩家("<<p[now.target]->job<<")发起"<<now.num<<"点"<<now.kind<<endl;
				if(p[now.target]->turn_hurt){
					cout<<"但被反转了"<<endl;
				}
				if(p[now.target]->defense&&!now.indefensible){
					cout<<"但被防住了"<<endl; 
				}
			}
			if(now.kind=="眩晕物理伤害"){
				p[now.target]->dizzy+=1;
			} 
			hts.pop();
		}

		//效果系数结算
		for (int i = 0; i < sum; i++) {
			p[i]->Bit();
			p[i]->defense = false;
			p[i]->costBit();
			p[i]->turn_hurt=false;
		}
		system("pause");
		system("cls");

		//胜负判定
		survivor = 0;
		for (int i = 0; i < sum; i++) {
			if (p[i]->alive()) {
				survivor += 1;
			}
		}

		//结束阶段
		if (survivor == 0) {
			cout << "游戏结束" << endl;
			cout << "无人生还，同归于尽" << endl;
			return 0;
		} else if (survivor == 1) {
			cout << "游戏结束" << endl;
			for (int i = 0; i < sum; i++) {
				if (p[i]->alive()) {
					cout << i << "号玩家(" << p[i]->job << ")获得胜利" << endl;
					return 0;
				}
			}
		}

	}

}
