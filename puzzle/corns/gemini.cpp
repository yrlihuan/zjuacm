// prompt v2.

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <set>
#include <numeric> // For std::iota if needed

// --- Constants ---
const int BOARD_HEIGHT = 7;
const int BOARD_WIDTH = 14;
const int NUM_PIECES = 17; // Total pieces to place

// --- Piece Definition ---
using Shape = std::vector<std::vector<int>>;

struct Piece {
    int id;
    Shape original_shape;
    Shape flipped_shape;
    bool is_symmetrical; // True if original_shape == flipped_shape
    int height;
    int num_orientations;

    Piece(int i, const Shape& s) : id(i), original_shape(s) {
        height = original_shape.size();

        // Compute flipped shape: reverse columns of each row, then reverse rows
        flipped_shape = original_shape;
        for (auto& row : flipped_shape) {
            std::reverse(row.begin(), row.end());
        }
        std::reverse(flipped_shape.begin(), flipped_shape.end());

        is_symmetrical = (original_shape == flipped_shape);
        num_orientations = is_symmetrical ? 1 : 2;
    }

    const Shape& get_shape(int orientation_idx) const {
        if (orientation_idx == 0) return original_shape;
        return flipped_shape;
    }
};

std::vector<Piece> ALL_PIECES;

// --- Board Representation ---
using Board = std::vector<std::vector<int>>; // 0 for empty, piece_id+1 for filled

// --- Utility Functions ---
void print_board(const Board& board) {
    std::cout << "Board state:" << std::endl;
    for (int r = BOARD_HEIGHT - 1; r >= 0; --r) {
        for (int c = 0; c < BOARD_WIDTH; ++c) {
            std::cout << board[r][c] << (board[r][c] < 10 ? "  " : " ");
        }
        std::cout << std::endl;
    }
    std::cout << "---------------------" << std::endl;
}

// Serializes board to a string for canonical representation
std::string board_to_string(const Board& board) {
    std::string s = "";
    for (int r = 0; r < BOARD_HEIGHT; ++r) {
        for (int c = 0; c < BOARD_WIDTH; ++c) {
            s += std::to_string(board[r][c]) + ",";
        }
    }
    return s;
}

// Rotates board state for canonical representation
Board rotate_board(const Board& board, int shift) {
    Board rotated = board;
    for (int r = 0; r < BOARD_HEIGHT; ++r) {
        for (int c = 0; c < BOARD_WIDTH; ++c) {
            rotated[r][c] = board[r][(c - shift + BOARD_WIDTH) % BOARD_WIDTH];
        }
    }
    return rotated;
}

// Gets the canonical string representation of a board by trying all rotations
std::string get_canonical_board_string(const Board& board) {
    std::string min_s = board_to_string(board);
    for (int shift = 1; shift < BOARD_WIDTH; ++shift) {
        Board rotated = rotate_board(board, shift);
        min_s = std::min(min_s, board_to_string(rotated));
    }
    return min_s;
}


bool can_place(const Board& board, const Piece& piece, int orientation_idx, int start_row, int start_col) {
    const Shape& current_shape = piece.get_shape(orientation_idx);
    if (start_row + current_shape.size() > BOARD_HEIGHT) {
        return false; // Exceeds board height
    }

    for (int r = 0; r < current_shape.size(); ++r) {
        for (int c = 0; c < current_shape[r].size(); ++c) {
            if (current_shape[r][c] == 1) {
                int board_r = start_row + r;
                int board_c = (start_col + c) % BOARD_WIDTH;
                if (board[board_r][board_c] != 0) {
                    return false; // Collision
                }
            }
        }
    }
    return true;
}

void place_or_remove_piece(Board& board, const Piece& piece, int orientation_idx,
                           int start_row, int start_col, bool place) {
    const Shape& current_shape = piece.get_shape(orientation_idx);
    int piece_id_on_board = place ? (piece.id + 1) : 0;

    for (int r = 0; r < current_shape.size(); ++r) {
        for (int c = 0; c < current_shape[r].size(); ++c) {
            if (current_shape[r][c] == 1) {
                int board_r = start_row + r;
                int board_c = (start_col + c) % BOARD_WIDTH;
                board[board_r][board_c] = piece_id_on_board;
            }
        }
    }
}

// --- Phase 1: Fill First Layer ---
std::set<std::string> processed_first_layer_canonical_hashes;
std::vector<std::pair<Board, int>> first_layer_configs; // Pair: {board_state, used_pieces_mask}

void dfs_fill_first_layer(Board& current_board, int used_pieces_mask) {
    int first_empty_col_layer0 = -1;
    for (int c = 0; c < BOARD_WIDTH; ++c) {
        if (current_board[0][c] == 0) {
            first_empty_col_layer0 = c;
            break;
        }
    }

    if (first_empty_col_layer0 == -1) { // First layer is completely filled
        std::string canonical_hash = get_canonical_board_string(current_board);
        if (processed_first_layer_canonical_hashes.find(canonical_hash) == processed_first_layer_canonical_hashes.end()) {
            processed_first_layer_canonical_hashes.insert(canonical_hash);
            first_layer_configs.push_back({current_board, used_pieces_mask});
            // std::cout << "Found a unique first layer config. Total: " << first_layer_configs.size() << std::endl;
        }
        return;
    }

    // Try to place an unused piece to cover board[0][first_empty_col_layer0]
    for (int piece_idx = 0; piece_idx < NUM_PIECES; ++piece_idx) {
        if (!((used_pieces_mask >> piece_idx) & 1)) { // If piece is not used
            const Piece& p = ALL_PIECES[piece_idx];
            for (int ori = 0; ori < p.num_orientations; ++ori) {
                const Shape& current_s = p.get_shape(ori);
                if (current_s.empty() || current_s[0].empty()) continue;

                // Try to align piece's first row cells with board[0][first_empty_col_layer0]
                for (int piece_col_anchor = 0; piece_col_anchor < current_s[0].size(); ++piece_col_anchor) {
                    if (current_s[0][piece_col_anchor] == 1) {
                        int board_start_col = (first_empty_col_layer0 - piece_col_anchor + BOARD_WIDTH) % BOARD_WIDTH;

                        if (can_place(current_board, p, ori, 0, board_start_col)) {
                            place_or_remove_piece(current_board, p, ori, 0, board_start_col, true);
                            dfs_fill_first_layer(current_board, used_pieces_mask | (1 << piece_idx));
                            place_or_remove_piece(current_board, p, ori, 0, board_start_col, false); // Backtrack
                        }
                    }
                }
            }
        }
    }
}


// --- Phase 2: Solve Remaining ---
bool find_first_empty_cell(const Board& board, int& r_out, int& c_out) {
    for (int r = 0; r < BOARD_HEIGHT; ++r) {
        for (int c = 0; c < BOARD_WIDTH; ++c) {
            if (board[r][c] == 0) {
                r_out = r;
                c_out = c;
                return true;
            }
        }
    }
    return false; // No empty cells
}

int solutions_found = 0;

bool solve_remaining(Board& current_board, int used_pieces_mask) {
    if (used_pieces_mask == (1 << NUM_PIECES) - 1) { // All pieces placed
        solutions_found++;
        std::cout << "Solution " << solutions_found << " found!" << std::endl;
        print_board(current_board);
        // return true; // Found one solution, uncomment to stop after first
    }

    // Optimization: if we have too many pieces left for too few cells, prune.
    // Or if we cannot possibly place remaining pieces. (More complex to check)

    int r_empty, c_empty;
    if (!find_first_empty_cell(current_board, r_empty, c_empty)) {
        // Board is full, but not all pieces placed (if we didn't hit the success condition above)
        return false;
    }

    for (int piece_idx = 0; piece_idx < NUM_PIECES; ++piece_idx) {
        if (!((used_pieces_mask >> piece_idx) & 1)) { // If piece is not used
            const Piece& p = ALL_PIECES[piece_idx];
            for (int ori = 0; ori < p.num_orientations; ++ori) {
                const Shape& current_s = p.get_shape(ori);
                if (current_s.empty()) continue;

                // Try to place the piece such that one of its cells covers (r_empty, c_empty)
                // Iterate over each '1' cell in the piece as an anchor
                for (int pr = 0; pr < current_s.size(); ++pr) {
                    for (int pc = 0; pc < current_s[pr].size(); ++pc) {
                        if (current_s[pr][pc] == 1) {
                            // This piece cell (pr, pc) will align with board cell (r_empty, c_empty)
                            int board_start_row = r_empty - pr;
                            int board_start_col = (c_empty - pc + BOARD_WIDTH) % BOARD_WIDTH;

                            if (board_start_row < 0) continue; // Piece would start above board

                            if (can_place(current_board, p, ori, board_start_row, board_start_col)) {
                                place_or_remove_piece(current_board, p, ori, board_start_row, board_start_col, true);
                                if (solve_remaining(current_board, used_pieces_mask | (1 << piece_idx))) {
                                    // return true; // Found one solution
                                }
                                place_or_remove_piece(current_board, p, ori, board_start_row, board_start_col, false); // Backtrack
                            }
                        }
                    }
                }
            }
        }
    }
    return false; // No solution from this path
}


// --- Main Function ---
int main() {
    std::vector<Shape> corn_piece_shapes = {
        {{1, 1, 1}, {0, 1, 1}}, {{1, 1, 1}, {0, 1, 1}}, {{1, 1, 1}, {0, 0 ,1}},
        {{1, 1, 0}, {1, 1, 1}, {0, 1, 1}}, {{1, 1, 0}, {0, 1, 0}, {0, 1, 0}, {0, 1, 1}},
        {{1, 1, 1}, {0, 1, 0}, {0, 1, 0}, {0, 1, 0}}, {{1, 1, 1}, {0, 0, 1}},
        {{1, 1, 1}, {1, 1, 0}}, {{1, 1, 1}, {0, 0, 1}, {0, 0, 1}},
        {{1, 1, 1}, {1, 0, 1}, {0, 0, 1}}, {{1, 1, 1}, {1, 0, 1}, {0, 0, 1}},
        {{1, 1, 1}, {1, 0, 1}, {1, 0, 1}}, {{1, 1, 0}, {1, 1, 1}, {0, 0, 1}},
        {{1, 1, 1}, {1, 1, 0}, {1, 0, 0}}, {{1, 1, 1}, {1, 1, 0}, {1, 0, 0}, {1, 0, 0}},
        {{0, 1, 1}, {1, 1, 1}, {0, 1, 0}}, {{0, 1, 1}, {1, 1, 1}, {0, 0, 1}, {0, 0, 1}}
    };

    if (corn_piece_shapes.size() != NUM_PIECES) {
        std::cerr << "Error: Mismatch in NUM_PIECES and defined shapes." << std::endl;
        return 1;
    }

    for (int i = 0; i < NUM_PIECES; ++i) {
        ALL_PIECES.emplace_back(i, corn_piece_shapes[i]);
    }

    std::cout << "Starting Phase 1: Generating first layer configurations..." << std::endl;
    Board initial_board(BOARD_HEIGHT, std::vector<int>(BOARD_WIDTH, 0));
    dfs_fill_first_layer(initial_board, 0);
    std::cout << "Phase 1 complete. Found " << first_layer_configs.size() << " unique starting configurations." << std::endl;

    if (first_layer_configs.empty()) {
        std::cout << "No way to fill the first layer. No solution possible." << std::endl;
        return 0;
    }

    std::cout << "\nStarting Phase 2: Solving for remaining pieces..." << std::endl;
    for (size_t i = 0; i < first_layer_configs.size(); ++i) {
        if (i % 10000 == 0) {
          std::cout << "Trying initial configuration " << (i + 1) << "/" << first_layer_configs.size() << std::endl;
        }
        Board board_copy = first_layer_configs[i].first;
        int used_mask = first_layer_configs[i].second;
        // print_board(board_copy); // Optional: print the starting board for this attempt
        if (solve_remaining(board_copy, used_mask)) {
            std::cout << "A solution was found starting from this configuration." << std::endl;
            // if you want to stop after the very first solution across all initial configs:
            // break;
        }
    }

    if (solutions_found == 0) {
        std::cout << "No solution found after checking all configurations." << std::endl;
    } else {
        std::cout << "Total solutions found: " << solutions_found << std::endl;
    }

    return 0;
}
