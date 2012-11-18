#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <set>
#include <map>
#include <vector>
#include <list>
#include <iostream>

using namespace std;

inline char conv(char c) {
  return c == 'X' ? 1 : 0;
}

char vertices[8];
char edges[12][4];

char facet_vertices[6][4];
char facet_edges[6][4][4];

char pos_vertices[6][4];
char pos_edges[6][4];
char pos_edges_d[6][4];

bool facet_used[6];

int search_indices[] = {0, 1, 5, 4, 3, 2};

vector<int> verify_vertices[6];
vector<int> verify_edges[6];

void init_shape()
{
  // vertices relationships
  pos_vertices[0][0] = 4;
  pos_vertices[0][1] = 7;
  pos_vertices[0][2] = 1;
  pos_vertices[0][3] = 0;

  pos_vertices[1][0] = 7;
  pos_vertices[1][1] = 6;
  pos_vertices[1][2] = 2;
  pos_vertices[1][3] = 1;

  pos_vertices[2][0] = 6;
  pos_vertices[2][1] = 5;
  pos_vertices[2][2] = 3;
  pos_vertices[2][3] = 2;

  pos_vertices[3][0] = 5;
  pos_vertices[3][1] = 4;
  pos_vertices[3][2] = 0;
  pos_vertices[3][3] = 3;

  pos_vertices[4][0] = 4;
  pos_vertices[4][1] = 5;
  pos_vertices[4][2] = 6;
  pos_vertices[4][3] = 7;

  pos_vertices[5][0] = 0;
  pos_vertices[5][1] = 1;
  pos_vertices[5][2] = 2;
  pos_vertices[5][3] = 3;

  // edges relationships
  pos_edges[0][0] = 7;
  pos_edges[0][1] = 9;
  pos_edges[0][2] = 0;
  pos_edges[0][3] = 8;
  pos_edges_d[0][0] = -1;
  pos_edges_d[0][1] = -1;
  pos_edges_d[0][2] = -1;
  pos_edges_d[0][3] = 1;

  pos_edges[1][0] = 6;
  pos_edges[1][1] = 10;
  pos_edges[1][2] = 1;
  pos_edges[1][3] = 9;
  pos_edges_d[1][0] = -1;
  pos_edges_d[1][1] = -1;
  pos_edges_d[1][2] = -1;
  pos_edges_d[1][3] = 1;

  pos_edges[2][0] = 5;
  pos_edges[2][1] = 11;
  pos_edges[2][2] = 2;
  pos_edges[2][3] = 10;
  pos_edges_d[2][0] = -1;
  pos_edges_d[2][1] = -1;
  pos_edges_d[2][2] = -1;
  pos_edges_d[2][3] = 1;

  pos_edges[3][0] = 4;
  pos_edges[3][1] = 8;
  pos_edges[3][2] = 3;
  pos_edges[3][3] = 11;
  pos_edges_d[3][0] = -1;
  pos_edges_d[3][1] = -1;
  pos_edges_d[3][2] = -1;
  pos_edges_d[3][3] = 1;

  pos_edges[4][0] = 4;
  pos_edges[4][1] = 5;
  pos_edges[4][2] = 6;
  pos_edges[4][3] = 7;
  pos_edges_d[4][0] = 1;
  pos_edges_d[4][1] = 1;
  pos_edges_d[4][2] = 1;
  pos_edges_d[4][3] = 1;

  pos_edges[5][0] = 0;
  pos_edges[5][1] = 1;
  pos_edges[5][2] = 2;
  pos_edges[5][3] = 3;
  pos_edges_d[5][0] = 1;
  pos_edges_d[5][1] = 1;
  pos_edges_d[5][2] = 1;
  pos_edges_d[5][3] = 1;

  // verficiation sequences
  verify_edges[1].push_back(9);

  verify_vertices[5].push_back(1);
  verify_edges[5].push_back(0);
  verify_edges[5].push_back(1);

  verify_vertices[4].push_back(7);
  verify_edges[4].push_back(6);
  verify_edges[4].push_back(7);

  verify_vertices[3].push_back(0);
  verify_vertices[3].push_back(4);
  verify_edges[3].push_back(8);
  verify_edges[3].push_back(4);
  verify_edges[3].push_back(3);

  verify_vertices[2].push_back(2);
  verify_vertices[2].push_back(3);
  verify_vertices[2].push_back(5);
  verify_vertices[2].push_back(6);
  verify_edges[2].push_back(10);
  verify_edges[2].push_back(11);
  verify_edges[2].push_back(2);
  verify_edges[2].push_back(5);
}

inline int facet_v_conv(int i, int rotate, bool flip)
{
  return ((flip?-i:i) - rotate) & 0x3;
}

inline int facet_e_conv(int i, int rotate, bool flip)
{
  return ((flip?3-i:i) - rotate) & 0x3;
}

bool verify(int pos)
{
  vector<int>::iterator it = verify_vertices[pos].begin();
  while (it != verify_vertices[pos].end()) {
    if (vertices[*it] != 1) {
      return false;
    }

    ++it;
  }

  it = verify_edges[pos].begin();
  while (it != verify_edges[pos].end()) {
    for (int j = 0; j < 4; ++j) {
      if (edges[*it][j] != 1) {
        return false;
      }
    }

    ++it;
  }

  return true;
}

int modify_state(int pos, int facet, int rotate, bool flip, bool place)
{
  for (int i = 0; i < 4; ++i) {
    int vertex = pos_vertices[pos][i];
    int facet_v = facet_v_conv(i, rotate, flip);
    if (place) {
      vertices[vertex] += facet_vertices[facet][facet_v];
    }
    else {
      vertices[vertex] -= facet_vertices[facet][facet_v];
    }
  }

  for (int i = 0; i < 4; ++i) {
    int edge = pos_edges[pos][i];
    int direction = flip ? -pos_edges_d[pos][i] : pos_edges_d[pos][i];

    int facet_edge = facet_e_conv(i, rotate, flip);
    for (int j = 0; j < 4; ++j) {
      if (place) {
        if (direction > 0) {
          edges[edge][j] += facet_edges[facet][facet_edge][j];
        }
        else {
          edges[edge][j] += facet_edges[facet][facet_edge][3-j];
        }

      }
      else {
        if (direction > 0) {
          edges[edge][j] -= facet_edges[facet][facet_edge][j];
        }
        else {
          edges[edge][j] -= facet_edges[facet][facet_edge][3-j];
        }
      }
    }
  }
}

bool search(int ind)
{
  if (ind >= 6) {
    return true;
  }

  int pos = search_indices[ind];
  for (int f = 1; f < 6; ++f) {
    if (facet_used[f]) {
      continue;
    }

    facet_used[f] = true;
    for (int i = 0; i < 8; ++i) {
      bool flip = i >= 4;
      int rotate = i & 0x3;

      modify_state(pos, f, rotate, flip, true);
      if (verify(pos)) {
        if (search(ind+1)) {
          return true;
        }
      }

      modify_state(pos, f, rotate, flip, false);
    }

    facet_used[f] = false;
  }

  return false;
}

int main()
{
  init_shape();

  int cases;
  scanf("%d\n", &cases);

  for (int c = 1; c <= cases; ++c) {
    memset(vertices, 0, sizeof(vertices));
    memset(edges, 0, sizeof(edges));
    memset(facet_vertices, 0, sizeof(facet_vertices));
    memset(facet_edges, 0, sizeof(facet_edges));
    memset(facet_used, 0, sizeof(facet_used));

    char buf[128];

    for (int i = 0; i < 6; ++i) {
      fgets(buf, 128, stdin);

      for (int b = 0; b < 6; ++b) {
        int p = b * 7;
        if (i == 0) {
          facet_vertices[b][0] = conv(buf[p]);
          facet_vertices[b][1] = conv(buf[p+5]);
          for (int j = 1; j <= 4; ++j) {
            facet_edges[b][0][j-1] = conv(buf[p+j]);
          }
        }
        else if (i == 5) {
          facet_vertices[b][2] = conv(buf[p+5]);
          facet_vertices[b][3] = conv(buf[p]);
          for (int j = 1; j <= 4; ++j) {
            facet_edges[b][2][4-j] = conv(buf[p+j]);
          }
        }
        else {
          facet_edges[b][1][i-1] = conv(buf[p+5]);
          facet_edges[b][3][4-i] = conv(buf[p]);
        }
      }
    }

    // debug data reading
    // for (int i = 0; i < 6; ++i) {
    //   for (int j = 0; j < 4; ++j) {
    //     printf("%d %d %d %d %d %d\n", facet_vertices[i][j],
    //         facet_edges[i][j][0], facet_edges[i][j][1], facet_edges[i][j][2], facet_edges[i][j][3],
    //         facet_vertices[i][(j+1)%4]);
    //   }
    //   cout << endl;
    // }
    // cout << "case: " << cases << endl;

    modify_state(0, 0, 0, false, true);
    facet_used[0] = true;
    bool valid = search(1);

    printf("Scenario #%d:\n", c);
    cout << (valid?"Yes":"No") << endl << endl;

    fgets(buf, 128, stdin);
  }
}
