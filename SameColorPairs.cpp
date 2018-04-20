// C++11
#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <map>
#include <sstream>
#include <vector>
#include <set>
#include <string>

#define PRINT_DEBUG 0

using namespace std;

void print_matrix(vector<vector<int>> &im) {

    #if PRINT_DEBUG == 1
      fprintf(stderr, "Printing matrix...\n");
    #endif

    for(int i = 0; i < im.size(); i++) {
        for(int j = 0; j < im[i].size(); j++) {

            #if PRINT_DEBUG == 1
              fprintf(stderr, "%3d ", im[i][j]);
            #endif
        }
        
        #if PRINT_DEBUG == 1
          fprintf(stderr, "\n");
        #endif
    }
}

class SameColorPairs {
public:
  int m_r;
  int m_c;
  vector<string> m_solution;
  vector<vector<int>> m_board;

  SameColorPairs();


  vector<vector<int>> set_m_board(vector<string> &board);

  void adjacent_horizontal_tiles();
  void adjacent_vertical_tiles();

  vector<string> removePairs(vector<string> board);
};


SameColorPairs::SameColorPairs() {
  m_r = 0;
  m_c = 0;
  m_solution = vector<string>(0);
  m_board = vector<vector<int>>(0, vector<int>(0));
}


vector<vector<int>> SameColorPairs::set_m_board(vector<string> &board) {
  m_r = board.size();
  m_c = board[0].length();

  #if PRINT_DEBUG == 1
    fprintf(stderr, "Board dimensions (r x c): (%d x %d)\n", m_r, m_c);
  #endif

  vector<vector<int>> b(m_r, vector<int>(m_c, 0));

  for(int ir = 0; ir < m_r; ir++) {
    for(int ic = 0; ic < m_c; ic++) {
      b[ir][ic] = board[ir][ic] - '0';
    }
  }

  print_matrix(b);
  return b;
}


void SameColorPairs::adjacent_horizontal_tiles() {

  int r = 0;
  while (r < m_r) {

    int c = 0;
    while (c < m_c - 1) {

      #if PRINT_DEBUG == 1
        fprintf(stderr, "m_board[r = %2d][c = %2d]: %3d m_board[r = %2d][c + 1 = %2d]: %3d - ", r, c, m_board[r][c], r, c + 1, m_board[r][c+1]);
      #endif

      if (m_board[r][c] == -1) {
        c = c + 1;
        continue;
      }

      if (m_board[r][c + 1] == -1) {
        c = c + 2;
        continue;
      }

      if( m_board[r][c] == m_board[r][c+1] ) {

        #if PRINT_DEBUG == 1
          fprintf(stderr, "Removing tiles!\n");
        #endif

        string s = to_string(r) + " " + to_string(c) + " " + to_string(r) + " " + to_string(c + 1);
        m_solution.push_back(s);

        m_board[r][c] = -1;
        m_board[r][c+1] = -1;

        c = c + 2;
        continue;
      }

      #if PRINT_DEBUG == 1
        fprintf(stderr, "No action!\n");
      #endif

      c = c + 1;

    }
    r = r + 1;
  }

}


void SameColorPairs::adjacent_vertical_tiles() {

  int c = 0;
  while (c < m_c) {

    int r = 0;
    while (r < m_r - 1) {

      if (m_board[r][c] == -1) {
        r = r + 1;
        continue;
      }

      if (m_board[r + 1][c] == -1) {
        r = r + 2;
        continue;
      }

      #if PRINT_DEBUG == 1
        fprintf(stderr, "m_board[r = %2d][c = %2d]: %3d m_board[r + 1 = %2d][c = %2d]: %3d - ", r, c, m_board[r][c], r, c + 1, m_board[r + 1][c]);
      #endif

      if( m_board[r][c] == m_board[r + 1][c] ) {

        #if PRINT_DEBUG == 1
          fprintf(stderr, "Removing tiles!\n");
        #endif

        string s = to_string(r) + " " + to_string(c) + " " + to_string(r + 1) + " " + to_string(c);
        m_solution.push_back(s);

        m_board[r][c] = -1;
        m_board[r + 1][c] = -1;

        r = r + 2;
        continue;
      }

      #if PRINT_DEBUG == 1
        fprintf(stderr, "No action!\n");
      #endif

      r = r + 1;

    }
    c = c + 1;
  }

}


vector<string> SameColorPairs::removePairs(vector<string> board) {

  m_board = set_m_board(board);

  adjacent_horizontal_tiles();
  adjacent_vertical_tiles();

  return m_solution;
}

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
