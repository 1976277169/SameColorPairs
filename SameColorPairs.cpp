#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <map>
#include <sstream>
#include <vector>
#include <set>
#include <string>
#include <random>
#include <utility>
#include <cassert>
#include <chrono>

#define PRINT_DEBUG 0
#define MAX_NUMBER_OF_COLORS 6

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

class Point {
  public:
    Point();
    Point(int r, int c);
    int p_r;
    int p_c;
};

Point::Point() {
  p_r = -1;
  p_c = -1;
}

Point::Point(int r, int c) {
  p_r = r;
  p_c = c;
}

class SameColorPairs {
public:
  random_device m_rd;
  mt19937 m_random_engine;
  uniform_int_distribution<> m_coin_dist;

  int m_r;
  int m_c;
  int m_number_of_colors;
  int m_total_number_of_tiles;
  int m_number_of_removed_tiles;
  vector<string> m_solution;
  vector<vector<int>> m_board;
  vector<vector<Point>> m_remaining_points_on_the_board;

  SameColorPairs();


  vector<vector<int>> set_m_board(vector<string> &board);

  void update_solution(string &s);

  void adjacent_horizontal_tiles();
  void adjacent_vertical_tiles();

  void set_number_of_colors();
  void fill_remaining_points_on_the_board();
  void print_remaining_points_on_the_board();


  pair<int, int> draw_random_remaining_tile();
  bool check_bounding_box(int &r1, int &c1, int &r2, int &c2, int &color);
  int find_tile_index(int &tile_color, int &tile_r, int &tile_c);
  void search_tile_neighbourhood(pair<int, int> &one_of_the_remaining_tiles, int &neighbourhood_size);
  void loop_random_remove();

  vector<string> removePairs(vector<string> board);
};


SameColorPairs::SameColorPairs() {

  m_random_engine.seed(m_rd());
  m_coin_dist = uniform_int_distribution<>(0, 1);


  m_r = 0;
  m_c = 0;
  m_number_of_colors = 0;
  m_total_number_of_tiles = 0;
  m_number_of_removed_tiles = 0;
  m_solution = vector<string>(0);
  m_board = vector<vector<int>>(0, vector<int>(0));
  m_remaining_points_on_the_board = vector<vector<Point>>(MAX_NUMBER_OF_COLORS, vector<Point>(0));
}


vector<vector<int>> SameColorPairs::set_m_board(vector<string> &board) {
  m_r = board.size();
  m_c = board[0].length();
  m_total_number_of_tiles = m_r * m_c;

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

void SameColorPairs::update_solution(string &s) {

  m_number_of_removed_tiles = m_number_of_removed_tiles + 2;
  m_solution.push_back(s);

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
        update_solution(s);

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
        update_solution(s);

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


void SameColorPairs::set_number_of_colors() {

  for(int i = 0; i < MAX_NUMBER_OF_COLORS; i++) {
    if(m_remaining_points_on_the_board[i].size() != 0) {
      m_number_of_colors++;
    }
  }
}


void SameColorPairs::fill_remaining_points_on_the_board() {

  for(int r = 0; r < m_r; r++) {
    for(int c = 0; c < m_c; c++) {

      if(m_board[r][c] == -1)
        continue;

      int color_index = m_board[r][c];
      m_remaining_points_on_the_board[color_index].push_back( Point(r, c) );
    }
  }

  set_number_of_colors();
}

void SameColorPairs::print_remaining_points_on_the_board() {

  for(int i = 0; i < MAX_NUMBER_OF_COLORS; i++) {
    int n_points = m_remaining_points_on_the_board[i].size();
    #if PRINT_DEBUG == 1
      fprintf(stderr, "There are %d remaining tiles of color %d\n", n_points, i);
    #endif

    for(int j = 0; j < n_points; j++) {
      #if PRINT_DEBUG == 1
        int p_r = m_remaining_points_on_the_board[i][j].p_r;
        int p_c = m_remaining_points_on_the_board[i][j].p_c;
        fprintf(stderr, "Point (r x c): (%2d x %2d)\n", p_r, p_c);
      #endif

    }
  }

  #if PRINT_DEBUG == 1
    fprintf(stderr, "Number of colors: %2d\n", m_number_of_colors);
  #endif
}


pair<int, int> SameColorPairs::draw_random_remaining_tile() {

  uniform_int_distribution<> color_dist(0, m_number_of_colors - 1);
  int chosen_color = color_dist(m_random_engine);

  int number_of_tiles_of_chosen_color = m_remaining_points_on_the_board[chosen_color].size();

  if (number_of_tiles_of_chosen_color == 0)
    return make_pair(-1, -1);

  uniform_int_distribution<> tile_dist(0, number_of_tiles_of_chosen_color - 1);
  int chosen_tile = tile_dist(m_random_engine);

  return make_pair(chosen_color, chosen_tile);
}


bool SameColorPairs::check_bounding_box(int &r1, int &c1, int &r2, int &c2, int &color) {

  if (r1 <= r2 && c1 <= c2) {
    #if PRINT_DEBUG == 1
      fprintf(stderr, "Met condition: r1 <= r2 && c1 <= c2\n");
    #endif

    for(int i = r1; i <= r2; i++) {
      for(int j = c1; j <= c2; j++) {
        if(m_board[i][j] == -1)
            continue;

        if(m_board[i][j] != color)
          return false;

        if( ((i == r1) && (j == c1)) || ((i == r2) && (j == c2)) )
          continue;

      }
    }
    return true;
  } else if (r1 >= r2 && c1 >= c2) {
    #if PRINT_DEBUG == 1
      fprintf(stderr, "Met condition: r1 >= r2 && c1 >= c2\n");
    #endif

    for(int i = r2; i <= r1; i++) {
      for(int j = c2; j <= c1; j++) {
        if(m_board[i][j] == -1)
            continue;

        if(m_board[i][j] != color)
          return false;

        if( ((i == r1) && (j == c1)) || ((i == r2) && (j == c2)) )
          continue;
      }
    }
    return true;
  } else if (r1 >= r2 && c1 <= c2) {
    #if PRINT_DEBUG == 1
      fprintf(stderr, "Met condition: r1 >= r2 && c1 <= c2\n");
    #endif

    for(int i = r2; i <= r1; i++) {
      for(int j = c1; j <= c2; j++) {
        if(m_board[i][j] == -1)
            continue;

        if(m_board[i][j] != color)
          return false;

        if( ((i == r1) && (j == c1)) || ((i == r2) && (j == c2)) )
          continue;
      }
    }
    return true;
  } else if (r1 <= r2 && c1 >= c2) {
    #if PRINT_DEBUG == 1
      fprintf(stderr, "Met condition: r1 <= r2 && c1 >= c2\n");
    #endif

    for(int i = r1; i <= r2; i++) {
      for(int j = c2; j <= c1; j++) {
        if(m_board[i][j] == -1)
            continue;

        if(m_board[i][j] != color)
          return false;

        if( ((i == r1) && (j == c1)) || ((i == r2) && (j == c2)) )
          continue;
      }
    }
    return true;
  }
}


int SameColorPairs::find_tile_index(int &tile_color, int &tile_r, int &tile_c) {

  for(int i = 0; i < m_remaining_points_on_the_board[tile_color].size(); i++) {

    int r = m_remaining_points_on_the_board[tile_color][i].p_r;
    int c = m_remaining_points_on_the_board[tile_color][i].p_c;

    if(tile_r == r && tile_c == c)
      return i;
  }
  return -1;
}


void SameColorPairs::search_tile_neighbourhood(pair<int, int> &one_of_the_remaining_tiles, int &neighbourhood_size) {

  int tile_color = one_of_the_remaining_tiles.first;
  int tile_position = one_of_the_remaining_tiles.second;

  int r1 = m_remaining_points_on_the_board[tile_color][tile_position].p_r;
  int c1 = m_remaining_points_on_the_board[tile_color][tile_position].p_c;

  #if PRINT_DEBUG == 1
    fprintf(stderr, "Searching neighbourhood of tile (r x c): (%2d x %2d)\n", r1, c1);
  #endif

  // Search the tile's neighbourhood for tiles of the same color
  // and remove if possible.
  for(int i = r1 - neighbourhood_size; i <= r1 + neighbourhood_size; i++) {
    if( (i < 0) || (i >= m_r) )
      continue;

    for(int j = c1 - neighbourhood_size; j <= c1 + neighbourhood_size; j++) {
      if( (j < 0) || (j >= m_c) )
        continue;

      if( (i == r1) && (j == c1) )
        continue;

      if(m_board[i][j] == tile_color) {
        // Possible tile pair to remove found.
        #if PRINT_DEBUG == 1
          fprintf(stderr, "Tile with the same color found at (r x c): (%2d x %2d)\n", i, j);
        #endif

        bool bounding_box_status = check_bounding_box(r1, c1, i, j, tile_color);


        #if PRINT_DEBUG == 1
          fprintf(stderr, "Bounding box status: %d\n", bounding_box_status);
        #endif

        if (bounding_box_status == true && m_coin_dist(m_random_engine) == 1) {
          #if PRINT_DEBUG == 1
            fprintf(stderr, "Neighbouring tile accepted!\n");
          #endif

          auto begining = m_remaining_points_on_the_board[tile_color].begin();
          m_remaining_points_on_the_board[tile_color].erase(begining + tile_position);

          int tile_index = find_tile_index(tile_color, i, j);
          assert(tile_index >= 0);

          begining = m_remaining_points_on_the_board[tile_color].begin();
          m_remaining_points_on_the_board[tile_color].erase(begining + tile_index);

          m_board[r1][c1] = -1;
          m_board[i][j] = -1;

          string s = to_string(r1) + " " + to_string(c1) + " " + to_string(i) + " " + to_string(j);
          update_solution(s);

          print_remaining_points_on_the_board();
          return;
        }
      }
    }
  }
}


void SameColorPairs::loop_random_remove() {

  for(int i = 0; i < 1000000; i++) {

    #if PRINT_DEBUG == 1
      fprintf(stderr, "Total number of tiles: %d\n", m_total_number_of_tiles);
      fprintf(stderr, "Total number of removed tiles: %d\n", m_number_of_removed_tiles);
    #endif

    pair<int, int> one_of_the_remaining_tiles = draw_random_remaining_tile();

    if (one_of_the_remaining_tiles.first == -1)
      continue;

    #if PRINT_DEBUG == 1
      fprintf(stderr, "Chosen color: %d, chosen tile: %d\n", one_of_the_remaining_tiles.first,
                                                             one_of_the_remaining_tiles.second);
    #endif


    int neighbourhood_size = 10;
    search_tile_neighbourhood(one_of_the_remaining_tiles, neighbourhood_size);
  }

}


vector<string> SameColorPairs::removePairs(vector<string> board) {

  chrono::time_point<std::chrono::system_clock> start, end;
  start = std::chrono::system_clock::now();

  m_board = set_m_board(board);

  adjacent_horizontal_tiles();
  adjacent_vertical_tiles();

  fill_remaining_points_on_the_board();
  print_remaining_points_on_the_board();

  loop_random_remove();

  end = chrono::system_clock::now();
  chrono::duration<double> elapsed_seconds = end - start;

  cerr << "elapsed time (removePairs): " << elapsed_seconds.count() << " sec" << endl;

  return m_solution;

  //vector<string> ret;
  //return ret;

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
