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

class SameColorPairs {
public:
    vector<string> removePairs(vector<string> board) {
        vector<string> ret;
        int H = board.size(), W = board[0].size();
        // find the first pair of horizontally adjacent tiles and remove them
        for (int i = 0; i < H; ++i) {
          for (int j = 1; j < W; ++j) {
            if (board[i][j] == board[i][j-1]) {
                ret.push_back(to_string(i) + " " + to_string(j) + " " + to_string(i) + " " + to_string(j-1));
                //return ret;
            }
          }
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
    SameColorPairs scp;
    int H;
    cin >> H;
    vector<string> board(H);
    getVector(board);

    vector<string> ret = scp.removePairs(board);
    cout << ret.size() << endl;
    for (int i = 0; i < (int)ret.size(); ++i)
        cout << ret[i] << endl;
    cout.flush();
}
