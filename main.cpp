#include <SFML/Graphics.hpp>
#include<iostream>
#include<stdio.h>
#include<vector>
#include <cstdlib>
#include <ctime>
#include<map>
#include<stack>
#include <iomanip>
#include <fstream>
using namespace std;
using namespace sf;
const int leng = 69, X = 552, Y = 552, oo = 1000000000; //X và Y là kích thước bàn cờ, leng là kích thước ô vuông cờ
int luotdi = 1; //luotdi=1: người đi trước, luotdi=0: máy đi trước, luotdi=3: không ai được đi, game đã end
int B[8][8]; //lưu vị trí các con cờ trên mảng 2 chiều, âm là quân đen, dương là quân trắng
Vector2i A[32];// lấy nhanh tọa độ quan cờ dựa trên bảng B
bool playAsBlack = false;
bool hoa = false;
bool thang = false;
bool thua = false;
bool TestAI=false; // kiểm tra độ thông mình
clock_t start;

map<string, int>di;

float thoigian = 90 * 60;
float AITime = 90 * 60;
int demNuoc = 0;
//trạng thái, dùng để kiếm tra trạng thái đó đã lặp bao nhiêu lần, nếu đã 3 lần thì không đi nước đó nữa
map< string, int>map_board;

//lưu sẵn hình dạng cho tiện
Sprite hau_den, hau_trang, chot_den, chot_trang;

//n là chỉ số của quân đang được click chuột
int n = 0;

//lưu thông tin các bước đã đi
vector <int> save;

//định nghĩa quân cờ
struct quan
{
	int mau; // 1: màu trắng, 0: màu đen
	int loai;// 1: xe, 2: mã, 3: tượng, 4: hậu, 5: tướng, 6: chốt
	Sprite s;// hình dạng
} f[32];
//lưu bàn cờ nè
int board[8][8] =
{ -1, -2, -3, -4, -5, -3, -2, -1,
-6, -6, -6, -6, -6, -6, -6, -6,
0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0,
6, 6, 6, 6, 6, 6, 6, 6,
1, 2, 3, 4, 5, 3, 2, 1 };
// số âm lưu vị trí quân đen, dương là quân trắng

// tham khảo từ chess-programming-wiki, có bổ sung, chỉnh sửa lại
int value[7] = { 0, 500, 300, 400, 900, 1000000, 100 };
int vua[8][8] =
{

	-6, -7, -7, -7, -7, -7, -7, -6,
	-5, -6, -6, -6, -6, -6, -6, -5,
	-4, -5, -5, -5, -5, -5, -5, -4,
	-3, -4, -4, -4, -4, -4, -4, -3,
	-2, -3, -3, -3, -3, -3, -3, -2,
	-1, -2, -2, -2, -2, -2, -2, -1,
	0, -1, -1, -1, -1, -1, -1, 0,
	0, 0, 0, 0, 0, 0, 0, 0
};
int hau[8][8] =
{
	-2,-1,-1, -1, -1,-1,-1,-2,
	-1, 1, 1, 1, 1, 1, 1,-1,
	-1, 1, 2, 2, 2, 2, 1,-1,
	-1, 1, 2, 3, 3, 2, 1,-1,
	-1, 1, 2, 3, 3, 2, 1,-1,
	-1, 1, 2, 2, 2, 2, 1,-1,
	-1, 1, 1, 1, 1, 1, 1,-1,
	-2,-1,-1,-1,-1,-1,-1,-2
};
int xe[8][8] =
{
	-1,-1,-1,-1,-1,-1,-1,-1,
	-1, 0, 0, 0, 0, 0, 0,-1,
	-1, 0, 1, 1, 1, 1, 0,-1,
	-1, 0, 1, 2, 2, 1, 0,-1,
	-1, 0, 1, 2, 2, 1, 0,-1,
	-1, 0, 1, 1, 1, 1, 0,-1,
	-1, 0, 0, 0, 0, 0, 0,-1,
	-1,-1,-1,-1,-1,-1,-1,-1
};
int tuong[8][8] =
{
	-2,-1,-1,-1,-1,-1,-1,-2,
	-1, 0, 0, 0, 0, 0, 0,-1,
	-1, 0, 1, 1, 1, 1, 0,-1,
	-1, 1, 1, 1, 1, 1, 1,-1,
	-1, 0, 1, 1, 1, 1, 0,-1,
	-1, 1, 1, 1, 1, 1, 1,-1,
	-1, 1, 0, 0, 0, 0, 1,-1,
	-2,-1,-1,-1,-1,-1,-1,-2
};
int ma[8][8] =
{
	-2,-1,-1,-1,-1,-1,-1,-2,
	-1, 0, 0, 1, 1, 0, 0,-1,
	-1, 0, 2, 3, 3, 2, 0,-1,
	-1, 1, 3, 4, 4, 3, 1,-1,
	-1, 0, 3, 4, 4, 3, 0,-1,
	-1, 1, 2, 3, 3, 2, 1,-1,
	-1, 0, 0, 1, 1, 0, 0,-1,
	-2,-1,-1,-1,-1,-1,-1,-2
};
int chot[8][8] =
{
	800, 800, 800, 800, 800, 800, 800, 800,
	4,4,4,4,4,4,4,4,
	3,3,3,3,3,3,3,3,
	2,2,2,2,2,2,2,2,
	1,1,1,1,1,1,1,1,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0
};
//chuyển tọa độ trên màn hình thành tọa độ trên bàn cờ
int getBoardPosition(Vector2f p)
{
	return (int(p.y) / leng) * 10 + int(p.x) / leng;
}

//chuyển tọa độ bàn cờ thành tọa độ màn hình
Vector2f getPosition(int s)
{
	return Vector2f((s % 10)*leng + leng / 2, (s / 10)*leng + leng / 2);
}

//lấy giá trị quân cờ
int getValue(int i)
{
	if (A[i].x > 7)return -1; //con này nằm ngoài bàn cờ rồi, nghĩa là nó đã bị ăn
	int v;
	int x = A[i].x;
	int y = A[i].y;
	if (f[i].loai == 1)if (f[i].mau == 1)
		v = value[f[i].loai] + xe[x][y]; else v = value[f[i].loai] + xe[7 - x][y];

	if (f[i].loai == 2)if (f[i].mau == 1)
		v = value[f[i].loai] + ma[x][y]; else v = value[f[i].loai] + ma[7 - x][y];

	if (f[i].loai == 3)if (f[i].mau == 1)
		v = value[f[i].loai] + tuong[x][y]; else v = value[f[i].loai] + tuong[7 - x][y];

	if (f[i].loai == 4)if (f[i].mau == 1)
		v = value[f[i].loai] + hau[x][y]; else v = value[f[i].loai] + hau[7 - x][y];

	if (f[i].loai == 5)if (f[i].mau == 1)
		v = value[f[i].loai] + vua[x][y]; else v = value[f[i].loai] + vua[7 - x][y];

	if (f[i].loai == 6)if (f[i].mau == 1)
		v = value[f[i].loai] + chot[x][y]; else v = value[f[i].loai] + chot[7 - x][y];

	if (f[i].mau == 0)return v; //màu đen là máy nên lấy số dương, trắng là người chơi nên lấy số âm
	return -v;
}

//lấy giá trị trạng thái bàn cờ
//nếu hàm này trả giá trị dương: đen đang có lợi thế, âm thì người chơi đang có lợi thế
int getValueBoard()
{
	int sum = 0;
	for (int i = 0; i < 32; i++)
	{
		if (f[i].loai == 5 && A[i].x > 7) {
			return f[i].mau == 0 ? -oo : oo;
		}
		sum += getValue(i);
	}
	return sum;
}

//kiểm tra chốt đã đến được cuối con đường hay chưa
//nếu tới rồi thì phong hậu
int checkQueen(int n)
{
	if (f[n].loai == 6)if (A[n].x == 0 || A[n].x == 7)
	{
		f[n].loai = 4;
		if (f[n].mau == 0)f[n].s = hau_den; else f[n].s = hau_trang;
		return 1;
	}
	return 0;
}

vector<int>out;
//vector out để check xem có con nào bị ăn không, để tiện cho việc quay lại

//thực hiện bước đi, s gồm 2 ô
void move(int s)
{
	int a = s / 100, b = s % 100;
	out.push_back(B[b / 10][b % 10]);
	if (B[b / 10][b % 10] != -1)A[B[b / 10][b % 10]] = Vector2i(969, 969);
	B[b / 10][b % 10] = B[a / 10][a % 10];
	A[B[b / 10][b % 10]] = Vector2i(b / 10, b % 10);

	B[a / 10][a % 10] = -1;

	if (checkQueen(B[b / 10][b % 10]))out[out.size() - 1] += 1000;
	//nếu bước đi này có phong hậu thì sẽ có khác biệt 1 tí để nhận dạng
}

//quay lại trạng thái bàn cờ trước đó, dùng trong thuật toán minimax
void goBack()
{
	if (save.size() == 0)return;
	int top = save[save.size() - 1];
	save.pop_back();
	int a = top / 100, b = top % 100;

	int s = b * 100 + a;
	a = s / 100, b = s % 100;
	if (B[b / 10][b % 10] != -1)A[B[b / 10][b % 10]] = Vector2i(969, 969);
	B[b / 10][b % 10] = B[a / 10][a % 10];
	A[B[b / 10][b % 10]] = Vector2i(b / 10, b % 10);

	B[a / 10][a % 10] = -1;

	top = out[out.size() - 1];
	out.pop_back();
	//nếu top=-1 tức là bước này không có con nào bị ăn cả, nên không cần làm tiếp
	if (top == -1)return;

	//kiểm tra nếu nó được phong hậu mà có quay lại thì trả lại cho nó hình con chốt
	if (top > 32)
	{
		top -= 1000;
		int n = B[b / 10][b % 10];
		f[B[b / 10][b % 10]].loai = 6;
		if (f[n].mau == 0)
			f[n].s = chot_den; else f[n].s = chot_trang;
	}

	//trả lại cái con bị ăn lại bàn cờ
	B[a / 10][a % 10] = top;
	if (top != -1)
		A[B[a / 10][a % 10]] = Vector2i(a / 10, a % 10);

}
string getBoardStatus(); //kiểm tra các trạng thái bàn cờ có bị lặp lại hay không

//cái này dùng để quay lại khi người chơi muốn undo
//khác với hàm goBack chỗ trừ số lần của trạng thái bàn cờ
vector < int >redo;//chức năng đi lên trạng thái cũ sau khi bấm undo
void goBackPlayer()
{
	if (save.size() == 0)return;
	luotdi = 1;
	map_board[getBoardStatus()]--; //trừ số lần xuất hiện trạng thái cũ đi 1
	int top = save[save.size() - 1];

	redo.push_back(top);

	save.pop_back();
	int a = top / 100, b = top % 100;

	int s = b * 100 + a;
	a = s / 100, b = s % 100;
	if (B[b / 10][b % 10] != -1)A[B[b / 10][b % 10]] = Vector2i(969, 969);
	B[b / 10][b % 10] = B[a / 10][a % 10];
	A[B[b / 10][b % 10]] = Vector2i(b / 10, b % 10);

	B[a / 10][a % 10] = -1;

	top = out[out.size() - 1];
	out.pop_back();
	//nếu top=-1 tức là bước này không có con nào bị ăn cả, nên không cần làm tiếp
	if (top == -1)return;

	//kiểm tra nếu nó được phong hậu mà có quay lại thì trả lại cho nó hình con chốt
	if (top > 32)
	{
		top -= 1000;
		int n = B[b / 10][b % 10];
		f[B[b / 10][b % 10]].loai = 6;
		if (f[n].mau == 0)
			f[n].s = chot_den; else f[n].s = chot_trang;
	}

	//trả lại cái con bị ăn lại bàn cờ
	B[a / 10][a % 10] = top;
	if (top != -1)
		A[B[a / 10][a % 10]] = Vector2i(a / 10, a % 10);

}
//load các vị trí quân cờ
void loadPosition()
{
	thang = thua = hoa = false;
	demNuoc = 0;
	thoigian = AITime = 90 * 60;
	start = clock();
	out.clear();
	save.clear();//các bước đi đều trống
	int k = 0;
	for (int i = 0; i < 8; i++)for (int j = 0; j < 8; j++)
	{
		B[i][j] = -1;
		int n = board[i][j];
		if (!n) continue;
		int x = abs(n) - 1;
		int y = n > 0 ? 1 : 0;
		B[i][j] = k;
		A[k] = Vector2i(i, j);
		f[k].mau = y;
		f[k].loai = abs(n);
		if (playAsBlack)y = !y;
		f[k].s.setTextureRect(IntRect(x*leng, leng*y, leng, leng));
		f[k].s.setPosition(leng*j + leng / 2, leng*i + leng / 2);
		f[k].s.setOrigin(leng / 2, leng / 2);

		//chỗ này lưu sẵn các hình dạng để tiện thay đổi
		if (f[k].mau == 0 && f[k].loai == 6)chot_den = f[k].s;
		if (f[k].mau == 1 && f[k].loai == 6)chot_trang = f[k].s;
		if (f[k].mau == 0 && f[k].loai == 4)hau_den = f[k].s;
		if (f[k].mau == 1 && f[k].loai == 4)hau_trang = f[k].s;
		k++;
	}
	if (playAsBlack)luotdi = !luotdi;
}


vector< int > canGo(int n, bool check); //hàm sinh các nước đi kế tiếp
//kiểm tra quân màu đó có đang bị chiếu hay không
//sinh ra tất cả các nước đi kế của đối thủ vào một vector
//nếu con vua của mình trùng với bất cứ nước đi nào trong vector thì nghĩa là nó đang bị chiếu
bool chieuTuong(int mau)
{
	vector<int>tam;
	for (int i = 0; i < 32; i++)if (f[i].mau != mau)
	{
		vector<int> v = canGo(i, false);
		for (int i = 0; i < v.size(); i++)tam.push_back(v[i]);
	}
	int v;
	for (int i = 0; i < 32; i++)if (f[i].mau == mau && f[i].loai == 5)v = i;//chỉ số của con vua
	int k = A[v].x * 10 + A[v].y; //lấy tọa độ con vua của mình

	//nếu có bất cứ vị trí nào trùng với vua nghĩa là nó đang bị chiếu
	for (int i = 0; i < tam.size(); i++)if (k == tam[i])return true;

	return false;
}

//liệt kê các bước duy chuyển kế tiếp của quân có chỉ số n
//check=false nghĩa là đi nước đi kế mà không cần quan tâm vua có bị ăn hay không
//làm vậy để chạy được nhiều nước đi hơn
//check=true nghĩa là nước đi đó đảm bảo tướng không bị nguy hiểm
vector< int > canGo(int n, bool check)
{
	vector<int>rong;
	if (A[n].x > 7 || A[n].y > 7)return rong;//quân này nằm ngoài bàn cờ, số nước đi kế là 0
	int k = A[n].x * 10 + A[n].y;//lấy vị trí
	vector<int>s;
	//kiểm tra từng loại quân, liệt kê các nước đi kế đưa vào vector s
	if (f[n].loai == 5) //tướng
	{
		int dx[8] = { -1,0,1,0,-1,1,-1,1 };
		int dy[8] = { 0,-1,0,1,-1,1,1,-1 };
		for (int i = 0; i < 8; i++)
		{
			int x = k / 10 + dx[i];
			int y = k % 10 + dy[i];
			if (x < 0 || y < 0 || x>7 || y>7)continue;
			s.push_back(x * 10 + y);
		}
	}
	if (f[n].loai == 2) //mã
	{
		int dx[8] = { -1,2,1,2,-2,1,-1,-2 };
		int dy[8] = { 2,-1,2,1,1,-2,-2,-1 };
		for (int i = 0; i < 8; i++)
		{
			int x = k / 10 + dx[i];
			int y = k % 10 + dy[i];
			if (x < 0 || y < 0 || x>7 || y>7)continue;
			s.push_back(x * 10 + y);
		}
	}
	if (f[n].loai == 6) //chốt
	{
		int o;
		if (f[n].mau == 0)o = 1; else o = -1;
		int dx[4] = { o,o, o,o * 2 };//o*2 nghĩa là nước đi đầu nó được đi tận 2 ô
		int dy[4] = { 0,1,-1,0 };
		for (int i = 0; i < 4; i++)
		{
			int x = k / 10 + dx[i];
			int y = k % 10 + dy[i];
			if (x < 0 || y < 0 || x>7 || y>7)continue;
			int v = B[x][y];
			//nếu vị trí trên con chốt còn trống thì được đi lên
			//nếu hai bên chéo nó có quân thì được ăn
			if (v == -1 && i == 0 || v != -1 && i > 0 && i < 3)s.push_back(x * 10 + y);

			//xét coi con chốt có được đi 2 ô lên không
			//chỉ được đi với điều kiện nó đang nằm ở vị trí ban đầu
			if (i == 3 && v == -1)
			{
				if (f[n].mau == 0 && x == 3 && B[2][y] == -1)s.push_back(x * 10 + y);
				if (f[n].mau == 1 && x == 4 && B[5][y] == -1)s.push_back(x * 10 + y);
			}
		}
	}
	if (f[n].loai == 4)//hậu, không giới hạn độ dài đường đi
	{
		int dx[8] = { -1,0,1,0,-1,1,-1,1 };
		int dy[8] = { 0,-1,0,1,-1,1,1,-1 };
		for (int i = 0; i < 8; i++)
		{
			int x = k / 10 + dx[i];
			int y = k % 10 + dy[i];
			while (true)
			{
				if (x < 0 || y < 0 || x>7 || y>7)break;
				s.push_back(x * 10 + y);
				if (B[x][y] != -1)break;
				x += dx[i];
				y += dy[i];
			}
		}
	}
	if (f[n].loai == 1)//xe
	{
		int dx[4] = { -1,0,1,0 };
		int dy[4] = { 0,-1,0,1 };
		for (int i = 0; i < 4; i++)
		{
			int x = k / 10 + dx[i];
			int y = k % 10 + dy[i];
			while (true)
			{
				if (x < 0 || y < 0 || x>7 || y>7)break;
				s.push_back(x * 10 + y);
				if (B[x][y] != -1)break;
				x += dx[i];
				y += dy[i];
			}
		}
	}
	if (f[n].loai == 3)//tượng
	{
		int dx[4] = { -1,1,1,-1 };
		int dy[4] = { 1,-1,1,-1 };
		for (int i = 0; i < 4; i++)
		{
			int x = k / 10 + dx[i];
			int y = k % 10 + dy[i];
			while (true)
			{
				if (x < 0 || y < 0 || x>7 || y>7)break;
				s.push_back(x * 10 + y);
				if (B[x][y] != -1)break;
				x += dx[i];
				y += dy[i];
			}
		}
	}
	vector<int>res;//res sẽ là mảng kết quả của hàm này
	int start = A[n].x * 10 + A[n].y, stop; //start là điểm đang đứng, stop là điểm sẽ đi tới

	for (int i = 0; i < s.size(); i++)
	{
		bool ok = true;
		stop = s[i]; //dự là sẽ đi nước s[i]
		if (start == stop)ok = false; //nước đi không trùng với vị trí cũ
		int j = B[stop / 10][stop % 10];
		if (j != -1 && f[n].mau == f[j].mau)ok = false; //không được đi vào vị trí đang có quân cùng màu
		//tức là không được ăn quân của mình

		//nếu không xét sự nguy hiểm của con tướng 
		//đến đây nước đi thỏa các điều kiện trên sẽ là nước đi hợp lệ (ok=true)
		if (check == false)
		{
			if (ok)res.push_back(stop);
			continue; //làm tiếp, không xét bên dưới
		}

		//kiểm tra sau khi đi nước đó thì tướng có bị chiếu hay không
		move(start * 100 + stop);//đi thử để kiểm tra
		save.push_back(start * 100 + stop);//nạp nước đi vào save
		if (chieuTuong(f[n].mau))ok = false;//kiểm tra sau khi đi nước đó tướng mình có bị chiếu không
		if (ok)res.push_back(stop);//nếu thỏa tất cả yêu cầu tức là nước đi hợp lệ
		goBack();//đi xong rồi phải quay lại
	}
	return res;
}

//bước kiểm tra xem nước đi của người chơi có nằm trong những nước đi cho phép
bool check(Vector2f newPos)
{
	//newPos là vị trí trên màn hình
	vector<int> s = canGo(n, true);//liệt kê nước đi kế
	bool ok = false;
	int k = getBoardPosition(newPos);//k là vị trí trên bàn cờ
	for (int i = 0; i < s.size(); i++)if (s[i] == k)ok = true;//nếu nằm trong nước đi kế thì ok, được đi
	if (!ok)return false;
	return true;
}

//tất cả các nước đi kế, trạng thái kế của bàn cờ
//check cũng giống như check của canGo
vector<int> allWay(int mau, bool check)
{
	vector<int>s;
	for (int i = 0; i < 32; i++)if (f[i].mau == mau)
	{
		if (A[i].x > 7 || A[i].y > 7)continue;
		int start = A[i].x * 10 + A[i].y;
		vector<int> tam = canGo(i, check);
		if (tam.size() == 0)continue;
		for (int i = 0; i < tam.size(); i++)s.push_back(start * 100 + tam[i]);
	}
	return s;
}

//lấy trạng thái bàn cờ
//dùng để kiểm xem nước đi có bị lặp lại hay không
string getBoardStatus()
{
	string S = "";
	for (int i = 0; i < 32; i++)if (A[i].x > 7)S = S + char(78+48); else S = S + char(A[i].x * 10 + A[i].y+48);
	return S;
}
// giải thuật minimax + cắt tỉa alpha beta
//tham khảo https://jsfiddle.net/q76uzxwe/1/
int nextMove = -1;
int kt = -1;
int minimax(int max_d, int d, int mau, int alpha, int beta)
{
	int dau;
	if (!mau)dau = 1; else dau = -1;//mau sẽ quy định hàm minimax này làm hàm min hay hàm max
	int res = -dau * oo;//khởi tạo giá trị ban đầu là xấu nhất

	//mất vua coi như không tính nữa
	if (d != 0)for (int i = 0; i < 32; i++)if (f[i].loai == 5 && (A[i].x > 7 || A[i].y > 7))
	{
		if (mau == f[i].mau)kt = d - 2;
		return mau != f[i].mau ? -res - d : res - d;
	}

	if (max_d == d)return getValueBoard() - d; //đến độ sâu d thì ngưng

	string S = getBoardStatus();//lấy trạng thái
	if (map_board[S] == 3 && d != 0)return res / 2; //nếu như trạng thái đã lặp lại 3 lần thì không được đi nữa, trả về xấu nhất
	if (map_board[S] == 0)map_board.erase(S);

	vector<int> s = allWay(mau, !d);//lấy tất cả trạng thái kế, d=0 thì sẽ xét thêm tướng có bị chiếu hay không

	//minimax + cắt tỉa
	for (int i = 0; i < s.size(); i++)
	{
		string s1 = getBoardStatus();
		int tam = s[i];
		move(tam);//thực hiện nước đi
		save.push_back(tam);
		int val = minimax(max_d, d + 1, !mau, alpha, beta);
		goBack();//quay lại
		//if (d == kt)cout << d << endl;
		if (d == kt)
		{
			kt = -1;
			if (!chieuTuong(mau))if (allWay(mau, 1).size() == 0)return dau * oo - d;
		}
		if (val*dau > res*dau)//nếu tối ưu được res, dau cho biết sẽ so lớn hơn hay nhỏ hơn
		{
			res = val - d;
			if (d == 0)nextMove = tam; //chỉ các hàm có độ sâu =0 với gán nextMove
		}

		if (alpha >= beta)return res;
		if (!mau)alpha = max(alpha, res); else beta = min(beta, res);
	}

	//if (res == -dau * oo)if (!chieuTuong(mau))kt = true;
	return res;
}

int deep = 4;
bool sug = false;
int getBestMove(int mau)
{
	// dưới 5 giây thì được
	if (mau == 0)if (di[getBoardStatus()] != 0)return di[getBoardStatus()];
	nextMove = -1;
	if (mau == 1&&!sug)
	{
		vector<int>s = allWay(mau, true);
		int n = s.size();
		if (n == 0)return -1;
		srand(time(NULL));
		return s[rand() % n];
	}
	kt = -1;
	minimax(deep, 0, mau, -oo, +oo);//deep là độ khó
	//nếu sau hàm minimax mà nextMove vẵn là -1 thì có nghĩa là không đi được nữa
	return nextMove;
}

//làm cho bàn cờ random theo yêu cầu của thầy :3
void setRandom()
{
	int a[8] = { 0,1,2,3,4,5,6,7 };
	int b[8] = { 1,2,3,4,5,3,2,1 };
	bool check = false;
	while (!check)//check này kiểm tra là con tượng phải nằm ở 2 ô trắng và đen, nếu không thỏa thì xáo tiếp
	{
		for (int i = 0; i < 8; i++)swap(a[0], a[rand() % 8]);// tạo random hoán vị
		for (int i = 0; i < 8; i++)swap(b[i], b[a[i]]);
		int le = 0, chan = 0;
		for (int i = 0; i < 8; i++)if (i % 2 == 0 && b[i] == 3)chan = 1;
		else if (i % 2 == 1 && b[i] == 3)le = 1;
		check = chan && le;
	}
	for (int i = 0; i < 8; i++)
	{
		board[0][i] = -b[i];
		board[7][i] = b[i];
	}
}



//game đã kết thúc hay chưa
void checkEndGame()
{
	if (allWay(1, true).size() == 0)if (chieuTuong(1))thua = true;else hoa = true;
	if (allWay(0, true).size() == 0)if (chieuTuong(0))thang = true;else hoa = true;
}

void checkHoa(int k)
{
	int i;
	for (i = 0; i < 32; i++)if (A[i].x * 10 + A[i].y == k)break;
	demNuoc++;
	if (f[i].loai == 6)demNuoc = 0;
	vector<int>s = canGo(i, true);
	for (int i = 0; i < s.size(); i++)if (B[s[i] / 10][s[i] % 10] != -1)demNuoc = 0;
}
string toString(int i)
{
	string s = "";
	if (i == 0)return "0";
	while (i > 0)
	{
		s = char((i % 10) + 48) + s;
		i = i / 10;
	}
	return s;
}

void set(Text &t, Font &f, int size, string s, Color c, int x, int y, bool isUnderLine = false)
{
	t.setFont(f);
	t.setCharacterSize(size);
	t.setString(s);
	t.setFillColor(c);
	t.setPosition(Vector2f(x, y));
	if (isUnderLine)t.setStyle(Text::Underlined);
}

#include <stdio.h>  /* defines FILENAME_MAX */
// #define WINDOWS  /* uncomment this line to use it for windows.*/ 

#include <unistd.h>
#define GetCurrentDir getcwd

std::string GetCurrentWorkingDir( void ) {
  char buff[FILENAME_MAX];
  GetCurrentDir( buff, FILENAME_MAX );
  std::string current_working_dir(buff);
  return current_working_dir;
}


int changesize = 0;
int main()
{
	srand(time(NULL));
	RenderWindow window(VideoMode(893, 552), "Chess - Press H to see Instruction, Press Esc to Quit", Style::Default | Style::Titlebar);
	string address = GetCurrentWorkingDir();
	ifstream cin;
	cin.open(address + "/data/data.txt");
	string s1; int s2;
	do
	{
		cin >> s1 >> s2;
		if (di[s1] != 0)break;
		di[s1] = s2;
		
	} while (1);
	cin.close();

	Texture figures_image, board_image, fill_image, huongdan_image, end_image;

	//hình lấy từ https://commons.wikimedia.org/wiki/File:Chess_Pieces_Sprite.svg
	figures_image.loadFromFile(address + "/image/figures.png");

	board_image.loadFromFile(address + "/image/board.png");
	fill_image.loadFromFile(address + "/image/fill.png");
	huongdan_image.loadFromFile(address + "/image/huongdan.png");
	end_image.loadFromFile(address + "/image/end.png");
	Sprite board(board_image), huongdan(huongdan_image), end(end_image);

	huongdan.setPosition(-1000, -1000);
	end.setPosition(-1000, -1000);
	for (int i = 0; i < 32; i++)f[i].s.setTexture(figures_image);

	loadPosition();
	bool isGoing = false;//kiểm tra con cờ có đang bị kéo hay không
	float dx = 0, dy = 0;
	int go[32] = {};
	Vector2f oldPos, newPos;
	vector< Sprite > F;//các ô vuông gợi ý nước đi
	start = clock();

	Font f1, font, f3;
	f1.loadFromFile(address + "/font/VNI-Matisse.ttf");
	Text name_text;
	set(name_text, f1, 60, "CHESS", Color::Black, 665, 10);

	font.loadFromFile(address + "/font/VNTIFH.TTF");
	Text HumanTime_text; //thời gian của người
	set(HumanTime_text, font, 18, "", Color::Red, 573, 500);
	Text AITime_text; //thời gian của máy
	set(AITime_text, font, 18, "", Color::Red, 573, 520);

	f3.loadFromFile(address + "/font/VNI-Truck.ttf");
	Text new_text;
	set(new_text, f3, 30, "New Random Game", Color::Blue, 600, 100, true);
	Text redo_text;
	set(redo_text, f3, 30, "Redo", Color::Blue, 600, 150, true);
	Text undo_text;
	set(undo_text, f3, 30, "Undo", Color::Blue, 750, 150, true);
	Text AIGo_text;
	set(AIGo_text, f3, 30, "AI Go 1 Step", Color::Blue, 600, 200, true);
	Text changeDiff_text;
	set(changeDiff_text, f3, 30, "Change Difficult:", Color::Blue, 600, 250);
	Text diff_text[7];
	for (int i = 1; i <= 6; i++)
		set(diff_text[i], f3, 35, toString(i), Color::Blue, 600 + i * 30, 295, true);
	Text black_text;
	set(black_text, f3, 30, "Change Color", Color::Blue, 600, 350, true);
	Text suggest_text;
	set(suggest_text, f3, 30, "Suggest", Color::Blue, 600, 400, true);
	Text sur_text;
	set(sur_text, f3, 30, "Surrender", Color::Blue, 600, 450, true);

	Font f4;
	f4.loadFromFile(address + "/font/VHARIALB.TTF");
	Text win_text, lose_text, tie_text;

	while (window.isOpen())
	{
		int dem = 0, demvua=0, demma=0;
		for (int i = 0; i < 32; i++)if (A[i].x <= 7 && A[i].y <= 7)
		{
			dem++;
			if (f[i].loai == 5)demvua++;
			if (f[i].loai == 2)demma++;
		}
		if (dem == 2)hoa = true;
		if (dem == 3 && demma == 1)hoa = true;
		if (hoa || thang || thua)
		{
			if(thang)set(win_text, f4, 69, "You Win!", Color::Red, 140, 225);
			if(thua)set(lose_text, f4, 69, "You Lose!", Color::Red, 105, 225);
			if (hoa)set(tie_text, f4, 69, "Tie!", Color::Red, 230, 225);
			luotdi = 3;
			end.setPosition(0, 0);
		}
		else end.setPosition(-1000, -1000);
		if (!thang)win_text.setPosition(-1000, -1000);
		if (!thua)lose_text.setPosition(-1000, -1000);
		if (!hoa)tie_text.setPosition(-1000, -1000);

		if (demNuoc >= 50)hoa = true;	
		//đo thời gian
		bool check_time = true;
		if (luotdi != 3)
		{
			float t = thoigian - (double)(clock() - start) / CLOCKS_PER_SEC;
			if (t - int(t) > 0.5)check_time = true;
			if (t - int(t) < 0.05&&check_time)
			{
				int m = int(t + 0.5) / 60, s = int(t + 0.5) % 60;
				HumanTime_text.setString("Remaining time: " + toString(m) + "m " + toString(s) + "s\n");
				check_time = false;
				t = AITime;
				m = int(t + 0.5) / 60, s = int(t + 0.5) % 60;
				AITime_text.setString("AI Remaining time: " + toString(m) + "m " + toString(s) + "s\n");
			}
		}
		if (thoigian <= 0)thua = true;
		if (AITime <= 0)thang = true;

		Vector2i pos = Mouse::getPosition(window);
		//cout << pos.x << " " << pos.y << endl;
		int tamm = getBoardPosition(Vector2f(pos.x, pos.y));
		Event e;
		while (window.pollEvent(e))
		{
			if (e.type == Event::Closed)window.close();
			if (e.type == Event::KeyPressed&&e.key.code == Keyboard::Escape)window.close();

			//kiểm tra con cờ có đang bị bấm chuột, nếu bị thì nó sẽ thay đổi vị trí theo con chuột
			//và hiện các nước đi gợi ý kế tiếp
			if (e.type == Event::MouseButtonPressed)
				for (int i = 0; i < 32; i++)if (f[i].s.getGlobalBounds().contains(pos.x, pos.y))
				{
					isGoing = true;
					n = i;
					dx = pos.x - f[i].s.getPosition().x;
					dy = pos.y - f[i].s.getPosition().y;
					oldPos = f[i].s.getPosition();
					go[i] = 1;
					vector<int> s = canGo(n, true);

					for (int j = 0; j < s.size(); j++)
					{
						Sprite temp(fill_image);
						temp.setOrigin(leng / 2, leng / 2);
						temp.setPosition(getPosition(s[j]));
						if (F.size() < 64)F.push_back(temp);
					}
				}
			//thả chuột ra thì coi như thực hiện lượt đi
			if (e.type == Event::MouseButtonReleased&&isGoing)
			{
				F.clear();
				Vector2f p = f[n].s.getPosition();
				Vector2f newPos = Vector2f(leng*int(p.x / leng) + leng / 2, leng*int(p.y / leng) + leng / 2);

				//sử dụng chuột phải để di chuyển, dùng để tạo cờ thế
				if (e.key.code == Mouse::Right)
				{
					int tam = getBoardPosition(oldPos) * 100 + getBoardPosition(newPos);
					system("cls");
					cout << getBoardStatus() << " ";
					move(tam);
					f[n].s.setPosition(newPos);
					isGoing = false;//không theo con chuột nữa
					if (go[n])
					{
						go[n] = 0;
						save.push_back(tam);
					}
					
					 cout<< tam << endl;
					redo.clear();
					continue;
				}

				//nếu vị trí kéo vào không hợp lệ thì trả lại vị trí cũ cho quân cờ
				if (!check(newPos) || f[n].mau == 0)
				{
					f[n].s.setPosition(oldPos);
					isGoing = false;
				}
				else //nếu kéo vào vị trí hợp lệ thì thực hiện nước đi
				{
					int tam = getBoardPosition(oldPos) * 100 + getBoardPosition(newPos);
					move(tam);
					f[n].s.setPosition(newPos);
					isGoing = false;//không theo con chuột nữa
					if (go[n])
					{
						go[n] = 0;
						save.push_back(tam);
					}
					redo.clear();
					luotdi = !luotdi;//trả lại lượt đi cho đối thủ
					map_board[getBoardStatus()]++;//tăng số lần của trạng thái bàn cờ
					checkHoa(tam % 100);
					thoigian += 30;//cộng vào 30 giây
					
					checkEndGame();
				}
				
			}





			if (AIGo_text.getGlobalBounds().contains(pos.x, pos.y))
				AIGo_text.setFillColor(Color::Yellow); else AIGo_text.setFillColor(Color::Blue);
			if (new_text.getGlobalBounds().contains(pos.x, pos.y))
				new_text.setFillColor(Color::Yellow); else new_text.setFillColor(Color::Blue);
			if (undo_text.getGlobalBounds().contains(pos.x, pos.y))
				undo_text.setFillColor(Color::Yellow); else undo_text.setFillColor(Color::Blue);
			if (redo_text.getGlobalBounds().contains(pos.x, pos.y))
				redo_text.setFillColor(Color::Yellow); else redo_text.setFillColor(Color::Blue);
			if (sur_text.getGlobalBounds().contains(pos.x, pos.y))
				sur_text.setFillColor(Color::Yellow); else sur_text.setFillColor(Color::Blue);
			if (black_text.getGlobalBounds().contains(pos.x, pos.y))
				black_text.setFillColor(Color::Yellow); else black_text.setFillColor(Color::Blue);
			if (suggest_text.getGlobalBounds().contains(pos.x, pos.y))
				suggest_text.setFillColor(Color::Yellow); else suggest_text.setFillColor(Color::Blue);

			for (int i = 1; i <= 6; i++)if (i == deep)diff_text[i].setFillColor(Color(255, 79, 0));
			else diff_text[i].setFillColor(Color::Blue);
			for (int i = 1; i <= 6; i++)
				if (diff_text[i].getGlobalBounds().contains(pos.x, pos.y))
					diff_text[i].setFillColor(Color::Yellow); else if (i != deep) diff_text[i].setFillColor(Color::Blue);
			if (e.type == Event::MouseButtonPressed)changesize = 1;
			//đổi kích thước button
			if (changesize)
			{
				int si=30;
				if (AIGo_text.getGlobalBounds().contains(pos.x, pos.y)) AIGo_text.setCharacterSize(27);
				if (new_text.getGlobalBounds().contains(pos.x, pos.y)) new_text.setCharacterSize(27);
				if (redo_text.getGlobalBounds().contains(pos.x, pos.y)) redo_text.setCharacterSize(27);
				if (undo_text.getGlobalBounds().contains(pos.x, pos.y)) undo_text.setCharacterSize(27);
				if (sur_text.getGlobalBounds().contains(pos.x, pos.y)) sur_text.setCharacterSize(27);
				if (black_text.getGlobalBounds().contains(pos.x, pos.y)) black_text.setCharacterSize(27);
				if (suggest_text.getGlobalBounds().contains(pos.x, pos.y)) suggest_text.setCharacterSize(27);
				changesize = 0;
			}
			if (e.type == Event::MouseButtonReleased)
			{
				if (AIGo_text.getCharacterSize() == 27)AIGo_text.setCharacterSize(30);
				if (new_text.getCharacterSize() == 27)new_text.setCharacterSize(30);
				if (redo_text.getCharacterSize() == 27)redo_text.setCharacterSize(30);
				if (undo_text.getCharacterSize() == 27)undo_text.setCharacterSize(30);
				if (black_text.getCharacterSize() == 27)black_text.setCharacterSize(30);
				if (sur_text.getCharacterSize() == 27)sur_text.setCharacterSize(30);
				if (suggest_text.getCharacterSize() == 27)suggest_text.setCharacterSize(30);
			}


			if (e.type == Event::MouseButtonReleased && (AIGo_text.getGlobalBounds().contains(pos.x, pos.y)))luotdi = 0;
			for (int i = 1; i <= 6; i++)if (e.type == Event::MouseButtonReleased
				&&diff_text[i].getGlobalBounds().contains(pos.x, pos.y))deep = i;
			if (e.type == Event::MouseButtonReleased && (new_text.getGlobalBounds().contains(pos.x, pos.y)))
			{
				thoigian = 90 * 60;
				start = clock();
				luotdi = 1;
				deep = 4;
				setRandom();
				loadPosition();
			}
			if (e.type == Event::MouseButtonReleased && (undo_text.getGlobalBounds().contains(pos.x, pos.y)))
			{
				hoa = thang = thua = false;
				checkEndGame();
				thoigian -= 30;
				goBackPlayer();
				goBackPlayer();
			}
			if (e.type == Event::MouseButtonReleased && (redo_text.getGlobalBounds().contains(pos.x, pos.y)))
			{
				//redo
				if (redo.size() <= 0)continue;
				int x = redo[redo.size() - 1];
				move(x);
				save.push_back(x);
				redo.pop_back();
				x = redo[redo.size() - 1];
				move(x);
				save.push_back(x);
				redo.pop_back();

			}
			if (e.type == Event::MouseButtonReleased && (black_text.getGlobalBounds().contains(pos.x, pos.y)))
			{
				playAsBlack = !playAsBlack;
				thoigian = 90 * 60;
				start = clock();
				luotdi = 1;
				deep = 4;

				loadPosition();
			}
			if (e.type == Event::MouseButtonReleased && (sur_text.getGlobalBounds().contains(pos.x, pos.y)))thoigian = 0;
			if (e.type == Event::MouseButtonReleased && (suggest_text.getGlobalBounds().contains(pos.x, pos.y)))sug = true;
			
			//Event phím
			{
				if (e.type == Event::KeyReleased)if (e.key.code == Keyboard::U)//undo
				{
					hoa = thang = thua = false;
					checkEndGame();
					thoigian -= 30;
					goBackPlayer();
					goBackPlayer();
				}
				if (e.type == Event::KeyReleased)if (e.key.code == Keyboard::Z)//undo
				{
					goBack();
				}
				if (e.type == Event::KeyReleased)if (e.key.code == Keyboard::R)if (redo.size() > 0)//redo
				{
					if (redo.size() <= 0)continue;
					int x = redo[redo.size() - 1];
					move(x);
					save.push_back(x);
					redo.pop_back();
					x = redo[redo.size() - 1];
					move(x);
					save.push_back(x);
					redo.pop_back();
				}
				if (e.type == Event::KeyReleased)if (e.key.code == Keyboard::N)//new game
				{
					thoigian = 90 * 60;
					start = clock();
					luotdi = 1;
					deep = 4;
					setRandom();
					loadPosition();
				}
				if (e.type == Event::KeyReleased)if (e.key.code == Keyboard::P)luotdi = 0;
				if (e.type == Event::KeyReleased)if (e.key.code == Keyboard::S)thoigian = 0;
				if (e.type == Event::KeyReleased)if (e.key.code == Keyboard::B)
				{
					playAsBlack = !playAsBlack;
					thoigian = 90 * 60;
					start = clock();
					luotdi = 1;
					deep = 4;

					loadPosition();
				}
				if (e.type == Event::KeyReleased)if (e.key.code == Keyboard::A)TestAI = true;
				if (e.type == Event::KeyReleased)if (e.key.code == Keyboard::D)cout << getBoardStatus() << endl;
				if (e.type == Event::KeyReleased)if (e.key.code == Keyboard::Num1)deep = 1;
				if (e.type == Event::KeyReleased)if (e.key.code == Keyboard::Num2)deep = 2;
				if (e.type == Event::KeyReleased)if (e.key.code == Keyboard::Num3)deep = 3;
				if (e.type == Event::KeyReleased)if (e.key.code == Keyboard::Num4)deep = 4;
				if (e.type == Event::KeyReleased)if (e.key.code == Keyboard::Num5)deep = 5;
				if (e.type == Event::KeyReleased)if (e.key.code == Keyboard::Num6)deep = 6;
				if (e.type == Event::KeyPressed)if (e.key.code == Keyboard::H)huongdan.setPosition(25, 50);
				if (e.type == Event::KeyReleased)if (e.key.code == Keyboard::H)huongdan.setPosition(-1000, -1000);
			}
		}

		for (int i = 0; i < 32; i++)A[i] = Vector2i(969, 969);
		for (int i = 0; i < 8; i++)for (int j = 0; j < 8; j++)if (B[i][j] != -1)A[B[i][j]] = Vector2i(i, j);
		for (int i = 0; i < 32; i++)f[i].s.setPosition(getPosition(A[i].x * 10 + A[i].y));
		quan t[32];
		for (int i = 0; i < 32; i++)t[i] = f[i];
		if (isGoing) f[n].s.setPosition(pos.x - dx, pos.y - dy);//cho con chờ duy chuyển theo chuột

		//vẽ các đối tượng
		{
			window.clear();
			window.draw(board);
			for (int i = 0; i < F.size(); i++) window.draw(F[i]);
			for (int i = 0; i < 32; i++)window.draw(f[i].s);
			window.draw(f[n].s);
			window.draw(huongdan);
			window.draw(end);

			window.draw(name_text);
			window.draw(HumanTime_text);
			window.draw(AITime_text);
			window.draw(new_text);
			window.draw(undo_text);
			window.draw(redo_text);
			window.draw(AIGo_text);
			window.draw(changeDiff_text);
			window.draw(suggest_text);
			for (int i = 1; i <= 6; i++)window.draw(diff_text[i]);
			window.draw(sur_text);
			window.draw(black_text);
			window.draw(win_text);
			window.draw(lose_text);
			window.draw(tie_text);
			
			window.display();
		}

		//máy đi
			if (!luotdi || luotdi&&sug ||TestAI)
			{
				clock_t start2 = clock();

				int best = getBestMove(luotdi);//chọn nước đi kế cho máy
				checkEndGame();
				if (best == -1)continue;

				checkHoa(best % 100);
				//xử lí motion cho mượt mà
				int start = best / 100;
				int j = B[start / 10][start % 10];
				Vector2f luu = f[j].s.getPosition();
				Vector2f q = getPosition(best % 100) - getPosition(best / 100);
				for (int i = 0; i < 32; i++)f[i] = t[i];
				int loop = 100;
				q = Vector2f(q.x / loop, q.y / loop);
				for (int i = 0; i < loop; i++)
				{
					f[j].s.setPosition(Vector2f(getPosition(start)) + Vector2f(q.x*i, q.y*i));
					window.clear();
					window.draw(board);
					for (int i = 0; i < 32; i++)window.draw(f[i].s);
					window.draw(name_text);
					window.draw(HumanTime_text);
					window.draw(AITime_text);
					window.draw(new_text);
					window.draw(undo_text);
					window.draw(redo_text);
					window.draw(AIGo_text);
					window.draw(changeDiff_text);
					window.draw(suggest_text);
					for (int i = 1; i <= 6; i++)window.draw(diff_text[i]);
					window.draw(sur_text);
					window.draw(black_text);
					window.display();
				}
				f[j].s.setPosition(luu);
				move(best);
				save.push_back(best);
				luotdi = !luotdi;//trả lại lượt đi cho đối thủ
				checkEndGame();//kiểm tra game đã kết thúc chưa
				map_board[getBoardStatus()]++;//tăng trạng thái bàn cờ lên
				redo.clear();
				thoigian += (double)(clock() - start2) / CLOCKS_PER_SEC;
				AITime = AITime - (double)(clock() - start2) / CLOCKS_PER_SEC + 30;
				if (sug)luotdi = !luotdi;
				if (sug)sug = false;
			}
			

	}
	return 0;
}

//hiển thị con cờ trên bàn cờ và thao tác kéo thả, tham khảo ở:
//https://www.youtube.com/watch?v=_4EuZI8Q8cs&index=14&list=PLB_ibvUSN7mzUffhiay5g5GUHyJRO4DYr