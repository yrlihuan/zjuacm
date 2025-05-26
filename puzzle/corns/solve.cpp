#include <iostream>
#include <vector>
#include <array>
#include <algorithm>
#include <optional>

using namespace std;

// Constants for the board
constexpr int ROWS = 7;
constexpr int COLS = 14;

// Type definitions
using Board = array<array<int, COLS>, ROWS>;
using Piece = vector<vector<int>>;

// The corn pieces
vector<Piece> CORN_PIECES = {
    {{1, 1, 1}, {0, 1, 1}},
    {{1, 1, 1}, {0, 1, 1}},
    {{1, 1, 1}, {0, 0, 1}},
    {{1, 1, 0}, {1, 1, 1}, {0, 1, 1}},
    {{1, 1, 0}, {0, 1, 0}, {0, 1, 0}, {0, 1, 1}},
    {{1, 1, 1}, {0, 1, 0}, {0, 1, 0}, {0, 1, 0}},
    {{1, 1, 1}, {0, 0, 1}},
    {{1, 1, 1}, {1, 1, 0}},
    {{1, 1, 1}, {0, 0, 1}, {0, 0, 1}},
    {{1, 1, 1}, {1, 0, 1}, {0, 0, 1}},
    {{1, 1, 1}, {1, 0, 1}, {0, 0, 1}},
    {{1, 1, 1}, {1, 0, 1}, {1, 0, 1}},
    {{1, 1, 0}, {1, 1, 1}, {0, 0, 1}},
    {{1, 1, 1}, {1, 1, 0}, {1, 0, 0}},
    {{1, 1, 1}, {1, 1, 0}, {1, 0, 0}, {1, 0, 0}},
    {{0, 1, 1}, {1, 1, 1}, {0, 1, 0}},
    {{0, 1, 1}, {1, 1, 1}, {0, 0, 1}, {0, 0, 1}},
};

// Reverse a piece (mirror horizontally and vertically)
Piece reverse_piece(const Piece& p) {
    Piece rev = p;
    reverse(rev.begin(), rev.end());
    for (auto& row : rev) reverse(row.begin(), row.end());
    return rev;
}

// Generate all unique orientations for a piece (original and reversed)
vector<Piece> generate_orientations(const Piece& p) {
    vector<Piece> res;
    res.push_back(p);
    Piece rev = reverse_piece(p);
    if (rev != p) res.push_back(rev);
    return res;
}

// Check if a piece can be placed at (row, col) on the board
bool can_place(const Board& board, const Piece& piece, int row, int col) {
    int h = piece.size();
    int w = piece[0].size();
    for (int i = 0; i < h; ++i) {
        int r = row + i;
        if (r >= ROWS) return false;
        for (int j = 0; j < w; ++j) {
            if (piece[i][j]) {
                int c = (col + j) % COLS;
                if (board[r][c]) return false;
            }
        }
    }
    return true;
}

// Place a piece on the board
void place(Board& board, const Piece& piece, int row, int col, int val) {
    int h = piece.size();
    int w = piece[0].size();
    for (int i = 0; i < h; ++i) {
        int r = row + i;
        for (int j = 0; j < w; ++j) {
            if (piece[i][j]) {
                int c = (col + j) % COLS;
                board[r][c] = val;
            }
        }
    }
}

// Print the board
void print_board(const Board& board) {
    for (int i = 0; i < ROWS; ++i) {
        for (int j = 0; j < COLS; ++j) {
            cout << (board[i][j] ? "#" : ".");
        }
        cout << endl;
    }
}

// Recursive DFS search
bool dfs(Board& board, vector<vector<Piece>>& all_orients, vector<bool>& used, vector<pair<int,int>>& solution) {
    // Find the first empty cell
    for (int r = 0; r < ROWS; ++r) {
        for (int c = 0; c < COLS; ++c) {
            if (board[r][c] == 0) {
                // Try all unused pieces
                for (int p = 0; p < all_orients.size(); ++p) {
                    if (used[p]) continue;
                    for (const auto& orient : all_orients[p]) {
                        int h = orient.size();
                        int w = orient[0].size();
                        // Try all possible positions for this orientation at (r, c)
                        for (int offset = 0; offset < COLS; ++offset) {
                            int cc = (c - offset + COLS) % COLS;
                            if (can_place(board, orient, r, cc)) {
                                place(board, orient, r, cc, p+1);
                                used[p] = true;
                                solution.push_back({p, cc});
                                if (dfs(board, all_orients, used, solution)) return true;
                                solution.pop_back();
                                used[p] = false;
                                place(board, orient, r, cc, 0);
                            }
                        }
                    }
                }
                return false; // No piece fits here
            }
        }
    }
    // All cells filled
    return true;
}

int main() {
    Board board = {};
    vector<vector<Piece>> all_orients;
    for (const auto& p : CORN_PIECES) {
        all_orients.push_back(generate_orientations(p));
    }
    vector<bool> used(CORN_PIECES.size(), false);
    vector<pair<int,int>> solution;
    if (dfs(board, all_orients, used, solution)) {
        cout << "Solution found:\n";
        print_board(board);
    } else {
        cout << "No solution found.\n";
    }
    return 0;
}