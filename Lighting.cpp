// C++11
#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <map>
#include <sstream>
#include <vector>
#include <set>
#include <string>
#include <cmath>
#include <random>
#include <cassert>

using namespace std;

// const int MAX_S = 50;
// const int MAX_L = 20;

// class Node {
// public:
//
//     int field[MAX_S][MAX_S];
//     Node parent;
//     string output;
//     int score;
//
//     Node() {
//         memset(field, 0, sizeof(field));
//         score = 0;
//     }
//
//     // 比較用
//     bool operator<(const Node& n1) const {
//         return this->score < n1.score;
//     }
//
// };
//
// class Player {
// public:
//
//     const int SEARCH_DEPTH = MAX_L; // Lターン読む
//
//     // 探索
//     Node search(Node now) {
//
//     }
//
//     // 評価
//     int eval(Node next) {
//
//     }
//
//
// };

class Lighting {
public:
    vector<string> setLights(vector<string> map, int D, int L) {

        //
        vector<string> ret;

        //
        int S = map.size();
        s = S;
        d = D;
        l = L;
        ns = s * P;
        nd = d * P;

        // 0 - Sまでの乱数
        mt19937 mt(rnd());
        uniform_int_distribution<> randS(0, S);

        //
        //initField(map);
        initFieldMini(map);

        // ライトを置く場所を一つ決める
        for (int i = 0; i < L; ++i) {

            pair<int, int> mpos = { -1, -1 };
            int illuminated = -1;
            for (int y = max(0, s/2-nd); y < min(ns, s/2+nd); ++y) {
                for (int x = max(0, s/2-nd); x < min(ns, s/2+nd); ++x) {

                    if (field[y][x] == Wall) {
                        x += P-1;
                        continue;
                    }

                    pair<double, double> l_pos = getCellCenter(y, x);   // ライト置く場所（中心）をとりあえずここにする

                    int t = getIlluminatedCells(l_pos);
                    if (illuminated < t && t > 0) {
                        illuminated = t;
                        mpos = { y, x };
                    }

                }
            }

            onIlluminated(getCellCenter(mpos));

            // 整形
            string Y = "", X ="";
            if (mpos.first == -1.0 || mpos.second == -1.0 || field[mpos.first][mpos.second] == Wall) {
                bool update = true;

                while (update) {
                    update = false;
                    X += to_string(randS(mt) % S) + "." + to_string(randS(mt) % 90 + 10);
                    Y += to_string(randS(mt) % S) + "." + to_string(randS(mt) % 90 + 10);

                    int tx = atol(X.c_str()), ty = atol(Y.c_str());
                    if (field[ty][tx] == Wall) {
                        update = true;
                        X = "";
                        Y = "";
                    }
                }
            }
            else {
                pair<double, double> t = getCellCenter(mpos);
                Y += to_string(t.first);
                X += to_string(t.second);
            }

            bool f=false; int p=0, id=0;
            for (int i=0;i<X.size();++i) {
                if (X[i]=='.') {
                    p=i;
                    f=true;
                }
                if (f) {
                    id++;
                    if (id == 2) break;
                }
            }
            for (int i=id; i<2; ++i) X += "0";

            int p2=0; id=0; f=false;
            for (int i=0;i<Y.size();++i) {
                if (Y[i]=='.') {
                    p2=i;
                    f=true;
                }
                if (f) {
                    id++;
                    if (id == 2) break;
                }
            }
            for (int i=id; i<2; ++i) Y += "0";

            bool ok=false;
            int c=0;
            string XX="";
            for (int j = 0; j < X.size(); ++j) {
                XX += X[j];
                if (X[j]=='.') {
                    ok=true;
                }
                if (ok) {
                    ++c;
                    if (c == 3) break;
                }
            }

            if (!ok) {
                XX = to_string(randS(mt) % S) + "." + to_string(randS(mt) % 90 + 10);
            }

            ok=false; c=0;
            string YY="";
            for (int j = 0; j < Y.size(); ++j) {
                YY += Y[j];
                if (Y[j]=='.') {
                    ok=true;
                }
                if (ok) {
                    ++c;
                    if (c==3) break;
                }
            }

            if (!ok) {
                YY = to_string(randS(mt) % S) + "." + to_string(randS(mt) % 90 + 10);
            }


            //ret.push_back(X.substr(0, p+3) + " " + Y.substr(0, p2+3));
            //cerr << "a = " << XX << " " << YY << "(" << X << ", " << Y << ") = " << field[(int)atol(YY.c_str())][(int)atol(XX.c_str())] << "(" << field[(int)atol(Y.c_str())][(int)atol(X.c_str())] << ")"  << endl;
            ret.push_back(XX + " " + YY);
        }

        return ret;
    }

private:

    random_device rnd;

    const int P = 1;

    int s, d, l, ns, nd;                            // s == S, d == D, l == L, ns == S * P, nd == D * P;
    const int cell_length = 1*P;                    // セルの1辺の長さ(100倍)
    const int subcell_length = cell_length / P;     // サブセルの1辺の長さ(100倍)

    char field[50][50];                           // [CELL_ID][SUB_CELL_ID]

    //
    const char Wall = '#';
    const char Empty = '.';
    const char Illuminated = 'l';


    void initField(const vector<string>& map);      // mapをP倍に広げたもの（サブセルを含めたマップをセットする）
    void initFieldMini(const vector<string>& map);  //

    pair<double, double> getCellCenter(const int y, const int x);
    pair<double, double> getCellCenter(const pair<int, int>& p) {
        return getCellCenter(p.first, p.second);
    }
    pair<double, double> getCellCenter(const pair<double, double>& p) {
        return getCellCenter((int)p.first, (int)p.second);
    }


    int getIlluminatedCells(const pair<double, double>& l);
    void onIlluminated(const pair<double, double>& l);  // テスト用 getIlluminatedCellsで、illuminatedがプラスされるところをそのままIlluminatedにする


    // ユークリッド距離
    double distance(int y, int x, int y2, int x2) {
        return sqrt(pow(y-y2, 2) + pow(x-x2, 2));
    }

    double distance(const pair<int, int> p1, const pair<int, int> p2) {
        return distance(p1.first, p1.second, p2.first, p2.second);
    }

};

void Lighting::initField(const vector<string>& map) {
    for (int y = 0; y < ns; ++y) {
        for (int x = 0; x < ns; ++x) {

            char obj;
            switch (map[y][x]) {
                case '#': obj = Wall; break;
                case '.': obj = Empty; break;
                default: assert(false); // ここは入ってこないので
            }

            // 100倍に広げたmap
            for (int ny = y*P; ny < y*P+cell_length; ++ny) {
                for (int nx = x*P; nx < x*P+cell_length; ++nx) {
                    field[ny][nx] = obj;
                }
            }

        }
    }
}

void Lighting::initFieldMini(const vector<string>& map) {
    for (int y = 0; y < s; ++y) {
        for (int x = 0; x < s; ++x) {

            char obj;
            switch (map[y][x]) {
                case '#': obj = Wall; break;
                case '.': obj = Empty; break;
                default: assert(false);
            }

            field[y][x] = obj;
        }
    }
}

pair<double, double> Lighting::getCellCenter(const int y, const int x) {
    double c = cell_length / 2.0;    // subcellの辺の半分
    double a = y+c, b = x+c;
    return { a, b };
}

int Lighting::getIlluminatedCells(const pair<double, double>& l) {
    int y = l.first, x = l.second;  // index
    int illuminated = 0;

    // 1辺50の長さの正方形分、調べる
    for (int i = max(0, y - nd/2); i < min(ns, y + nd/2); ++i) {
        for (int j = max(0, x - nd/2); j < min(ns, x + nd/2); ++j) {

            if (field[i][j] == Empty && distance(l, getCellCenter(i, j)) <= nd) illuminated++;

        }
    }

    return illuminated;
}

void Lighting::onIlluminated(const pair<double, double>& l) {
    int y = l.first, x = l.second;  // index

    // 1辺ndの長さの正方形分、調べる
    for (int i = max(0, y - nd/2); i < min(ns, y + nd/2); ++i) {
        for (int j = max(0, x - nd/2); j < min(ns, x + nd/2); ++j) {

            if (field[i][j] == Empty && distance(l, getCellCenter(i, j)) <= nd) field[i][j] = Illuminated;

        }
    }
}

// -------8<------- end of solution submitted to the website -------8<-------

namespace std {
    ostream& operator<<(ostream& os, const pair<int, int>& p) {
        os << "(" << p.second << ", " << p.first << ")";
        return os;
    }

    ostream& operator<<(ostream& os, const pair<double, double>& p) {
        os << "(" << p.second << ", " << p.first << ")";
        return os;
    }
}


template <typename T> void print2d(T a, int h, int w, int width = -1, int br = 0) {
    for (int i = 0; i < h; ++i) {
        for (int j = 0; j < w; ++j) {
            if (width != -1) cerr.width(width);
            cerr << a[i][j] << ' ';
        }
        cerr << endl;
    }
    while (br--) cerr << endl;
}

template<class T> void getVector(vector<T>& v) {
    for (int i = 0; i < v.size(); ++i)
        cin >> v[i];
}

int main() {
    Lighting l;
    int S;
    cin >> S;
    vector<string> map(S);
    getVector(map);

    int D;
    cin >> D;

    int maxL;
    cin >> maxL;

    vector<string> ret = l.setLights(map, D, maxL);
    cout << ret.size() << endl;
    for (int i = 0; i < (int)ret.size(); ++i)
        cout << ret[i] << endl;

    cout.flush();
}
