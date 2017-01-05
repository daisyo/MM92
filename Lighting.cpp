// C++11
#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <map>
#include <sstream>
#include <vector>
#include <set>
#include <string>

using namespace std;

class Lighting {
public:
    vector<string> setLights(vector<string> map, int D, int L) {
        vector<string> ret;
        srand(123);
        int S = map.size();
        for (int i = 0; i < L; ++i) {
            ret.push_back(to_string(rand() % S) + "." + to_string(rand() % 90 +10) + " " 
                        + to_string(rand() % S) + "." + to_string(rand() % 90 +10));
        }
        return ret;
    }
};
// -------8<------- end of solution submitted to the website -------8<-------

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
