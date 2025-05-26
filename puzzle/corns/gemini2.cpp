#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <set>
#include <map>
#include <tuple>
#include <numeric> // For std::iota if needed, not strictly here

// --- Configuration ---
const int NUM_LAYERS = 7;
const int NUM_CELLS_PER_LAYER = 14;
const int PIECE_WIDTH = 3; // All pieces have this width

// --- Piece Definition ---
struct Piece {
    int id; // Index in the original CORN_PIECES_RAW
    std::vector<std::vector<int>> shape;
    std::vector<std::vector<int>> flipped_shape;
    bool is_flipped_different;
    int height;
    // width is implicitly PIECE_WIDTH

    Piece(int _id, const std::vector<std::vector<int>>& _shape)
        : id(_id), shape(_shape) {
        height = static_cast<int>(shape.size());

        // Create flipped_shape
        flipped_shape = shape;
        // 1. Reverse the order of rows
        std::reverse(flipped_shape.begin(), flipped_shape.end());
        // 2. Reverse each row
        for (auto& row : flipped_shape) {
            std::reverse(row.begin(), row.end());
        }

        is_flipped_different = (shape != flipped_shape);
    }

    // Get the specific orientation (original or flipped)
    const std::vector<std::vector<int>>& get_shape(bool flipped) const {
        return flipped ? flipped_shape : shape;
    }
};

std::vector<Piece> ALL_PIECES;

// Raw data for corn pieces from the problem description
const std::vector<std::vector<std::vector<int>>> CORN_PIECES_RAW = {
    {{1, 1, 1}, {0, 1, 1}},
    {{1, 1, 1}, {0, 1, 1}},
    {{1, 1, 1}, {0, 0 ,1}},
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

void initialize_pieces() {
    ALL_PIECES.clear();
    for (int i = 0; i < CORN_PIECES_RAW.size(); ++i) {
        ALL_PIECES.emplace_back(i, CORN_PIECES_RAW[i]);
    }
}

// --- Board Representation and Operations ---
using Board = std::vector<std::vector<int>>; // 0 for empty, piece_id+1 for filled

void print_board(const Board& board) {
    std::cout << "Board state:" << std::endl;
    for (int r = 0; r < NUM_LAYERS; ++r) {
        // std::cout << "Layer " << r << ": "; // More compact printing
        for (int c = 0; c < NUM_CELLS_PER_LAYER; ++c) {
            if (board[r][c] == 0) {
                std::cout << ". ";
            } else {
                // Print piece_id % 10 for single digit, or use letters
                char display_char = 'A' + (board[r][c] - 1);
                 if (board[r][c] -1 >= 26) display_char = 'a' + (board[r][c] -1 - 26); // more pieces
                 std::cout << display_char << " ";
            }
        }
        std::cout << std::endl;
    }
}

bool can_place(const Board& board, const Piece& piece, bool flipped, int R, int C) {
    const auto& p_shape = piece.get_shape(flipped);
    // Check vertical bounds first
    if (R < 0 || R + piece.height > NUM_LAYERS) return false;

    for (int r_offset = 0; r_offset < piece.height; ++r_offset) {
        for (int c_offset = 0; c_offset < PIECE_WIDTH; ++c_offset) {
            if (p_shape[r_offset][c_offset] == 1) {
                int board_r = R + r_offset;
                int board_c = (C + c_offset + NUM_CELLS_PER_LAYER) % NUM_CELLS_PER_LAYER; // Ensure positive modulo
                if (board[board_r][board_c] != 0) {
                    return false; // Collision
                }
            }
        }
    }
    return true;
}

void place_piece(Board& board, const Piece& piece, bool flipped, int R, int C, int mark_value) {
    const auto& p_shape = piece.get_shape(flipped);
    for (int r_offset = 0; r_offset < piece.height; ++r_offset) {
        for (int c_offset = 0; c_offset < PIECE_WIDTH; ++c_offset) {
            if (p_shape[r_offset][c_offset] == 1) {
                int board_r = R + r_offset;
                int board_c = (C + c_offset + NUM_CELLS_PER_LAYER) % NUM_CELLS_PER_LAYER;
                board[board_r][board_c] = mark_value;
            }
        }
    }
}

void remove_piece(Board& board, const Piece& piece, bool flipped, int R, int C) {
    const auto& p_shape = piece.get_shape(flipped);
    for (int r_offset = 0; r_offset < piece.height; ++r_offset) {
        for (int c_offset = 0; c_offset < PIECE_WIDTH; ++c_offset) {
            if (p_shape[r_offset][c_offset] == 1) {
                int board_r = R + r_offset;
                int board_c = (C + c_offset + NUM_CELLS_PER_LAYER) % NUM_CELLS_PER_LAYER;
                board[board_r][board_c] = 0; // Clear cell
            }
        }
    }
}

// --- First Layer Logic ---
std::set<std::vector<int>> canonical_first_layer_boards_set;
std::map<std::vector<int>, std::vector<std::tuple<int, int, bool>>> first_layer_configs_details;

std::vector<int> get_canonical_layer_representation(const std::vector<int>& layer_row) {
    if (layer_row.empty()) return {};
    std::vector<int> canonical = layer_row;
    std::vector<int> current = layer_row;
    for (int i = 1; i < NUM_CELLS_PER_LAYER; ++i) {
        std::rotate(current.begin(), current.begin() + 1, current.end());
        if (current < canonical) {
            canonical = current;
        }
    }
    return canonical;
}

void find_first_layer_configs_recursive(
    std::vector<int>& current_layer0_cells, // Stores piece_id+1 for filled cells on layer 0
    std::vector<bool>& pieces_used_mask,     // Which pieces from ALL_PIECES are used
    std::vector<std::tuple<int, int, bool>>& current_placements // (piece_id, col_start, is_flipped)
) {
    int next_empty_col = -1;
    for (int c = 0; c < NUM_CELLS_PER_LAYER; ++c) {
        if (current_layer0_cells[c] == 0) {
            next_empty_col = c;
            break;
        }
    }

    if (next_empty_col == -1) { // Layer 0 is full
        std::vector<int> canonical_repr = get_canonical_layer_representation(current_layer0_cells);
        if (canonical_first_layer_boards_set.find(canonical_repr) == canonical_first_layer_boards_set.end()) {
            canonical_first_layer_boards_set.insert(canonical_repr);
            first_layer_configs_details[canonical_repr] = current_placements;
            // std::cout << "Found canonical first layer config #" << canonical_first_layer_boards_set.size() << std::endl;
        }
        return;
    }

    // Try to place an unused piece that covers 'next_empty_col'
    for (int i = 0; i < ALL_PIECES.size(); ++i) {
        if (pieces_used_mask[i]) continue;

        const auto& piece = ALL_PIECES[i];
        for (int orient_idx = 0; orient_idx < (piece.is_flipped_different ? 2 : 1); ++orient_idx) {
            bool is_f = (orient_idx == 1);
            const auto& current_shape = piece.get_shape(is_f);

            if (current_shape.empty() || current_shape[0].empty()) continue;

            // Try to align one of the piece's first-row cells with `next_empty_col`.
            for (int piece_col_anchor = 0; piece_col_anchor < PIECE_WIDTH; ++piece_col_anchor) {
                if (current_shape[0][piece_col_anchor] == 1) { // This part of the piece potentialy covers `next_empty_col`
                    int actual_col_start_for_piece = (next_empty_col - piece_col_anchor + NUM_CELLS_PER_LAYER) % NUM_CELLS_PER_LAYER;

                    bool can_place_on_layer0 = true;
                    std::vector<int> cells_this_piece_fills_on_layer0;

                    for (int c_offset = 0; c_offset < PIECE_WIDTH; ++c_offset) {
                        if (current_shape[0][c_offset] == 1) {
                            int board_c = (actual_col_start_for_piece + c_offset) % NUM_CELLS_PER_LAYER;
                            if (current_layer0_cells[board_c] != 0) {
                                can_place_on_layer0 = false;
                                break;
                            }
                            cells_this_piece_fills_on_layer0.push_back(board_c);
                        }
                    }
                    
                    if (can_place_on_layer0) {
                        if (cells_this_piece_fills_on_layer0.empty()) { // Should not happen if piece_col_anchor was valid
                            continue;
                        }
                        // Place on current_layer0_cells
                        for (int board_c_to_fill : cells_this_piece_fills_on_layer0) {
                            current_layer0_cells[board_c_to_fill] = piece.id + 1;
                        }
                        pieces_used_mask[i] = true;
                        current_placements.emplace_back(piece.id, actual_col_start_for_piece, is_f);

                        find_first_layer_configs_recursive(current_layer0_cells, pieces_used_mask, current_placements);

                        // Backtrack
                        current_placements.pop_back();
                        pieces_used_mask[i] = false;
                        for (int board_c_to_clear : cells_this_piece_fills_on_layer0) {
                            current_layer0_cells[board_c_to_clear] = 0;
                        }
                    }
                }
            }
        }
    }
}

// --- Main Solver for Full Corn ---
int solutions_found_count = 0; // To stop after first few solutions if needed
const int MAX_SOLUTIONS_TO_FIND = 1;

bool solve_full_corn_recursive(
    Board& board,
    std::vector<bool>& pieces_overall_used_mask,
    int num_pieces_placed_total) {

    if (solutions_found_count >= MAX_SOLUTIONS_TO_FIND) return true;

    if (num_pieces_placed_total == ALL_PIECES.size()) {
        std::cout << "--- Solution Found! (" << ++solutions_found_count << ") ---" << std::endl;
        print_board(board);
        return true; 
    }

    // Find first empty cell (r_empty, c_empty)
    int r_empty = -1, c_empty = -1;
    for (int r_scan = 0; r_scan < NUM_LAYERS; ++r_scan) {
        for (int c_scan = 0; c_scan < NUM_CELLS_PER_LAYER; ++c_scan) {
            if (board[r_scan][c_scan] == 0) {
                r_empty = r_scan;
                c_empty = c_scan;
                goto found_empty_cell_full_solve;
            }
        }
    }

found_empty_cell_full_solve: // Label for goto
    if (r_empty == -1) { 
        // No empty cell, but not all pieces placed (num_pieces_placed_total < ALL_PIECES.size())
        // This means the board is full but pieces remain, which is a contradiction if pieces fit.
        // Or, if piece volume < board volume, this point means board is full, and if all pieces placed, it's a solution.
        // This specific path (no empty cell AND not all pieces placed) is a dead end.
        return false; 
    }

    // Try to place an unused piece such that it covers (r_empty, c_empty)
    for (int i = 0; i < ALL_PIECES.size(); ++i) {
        if (pieces_overall_used_mask[i]) continue;

        const auto& piece = ALL_PIECES[i];
        for (int orient_idx = 0; orient_idx < (piece.is_flipped_different ? 2 : 1); ++orient_idx) {
            bool is_f = (orient_idx == 1);
            const auto& current_shape = piece.get_shape(is_f);

            // Try to align any '1' of the piece with (r_empty, c_empty)
            for(int pr_offset = 0; pr_offset < piece.height; ++pr_offset) {
                for (int pc_offset = 0; pc_offset < PIECE_WIDTH; ++pc_offset) {
                    if (current_shape[pr_offset][pc_offset] == 1) {
                        // If this piece cell (pr_offset, pc_offset) is placed at board cell (r_empty, c_empty)
                        // then the piece's reference point (0,0) must be placed at:
                        int piece_origin_R = r_empty - pr_offset;
                        int piece_origin_C = (c_empty - pc_offset + NUM_CELLS_PER_LAYER) % NUM_CELLS_PER_LAYER;

                        if (can_place(board, piece, is_f, piece_origin_R, piece_origin_C)) {
                            place_piece(board, piece, is_f, piece_origin_R, piece_origin_C, piece.id + 1);
                            pieces_overall_used_mask[i] = true;

                            if (solve_full_corn_recursive(board, pieces_overall_used_mask, num_pieces_placed_total + 1)) {
                                return true; // Propagate solution found
                            }

                            // Backtrack
                            remove_piece(board, piece, is_f, piece_origin_R, piece_origin_C);
                            pieces_overall_used_mask[i] = false;
                        }
                    }
                }
            }
        }
         if (solutions_found_count >= MAX_SOLUTIONS_TO_FIND) return true; // Check again after trying a piece and its orientations
    }
    return false; // No unused piece could be placed to cover (r_empty, c_empty) successfully
}

int main() {
    initialize_pieces();
    std::cout << "Corn Puzzle Solver" << std::endl;
    std::cout << "Total pieces to place: " << ALL_PIECES.size() << std::endl;
    
    long long total_piece_volume = 0;
    for(const auto& p : ALL_PIECES) {
        for(const auto& row : p.shape) {
            for(int cell : row) {
                total_piece_volume += cell;
            }
        }
    }
    std::cout << "Total piece volume (sum of 1s): " << total_piece_volume << std::endl;
    std::cout << "Board volume (7x14): " << NUM_LAYERS * NUM_CELLS_PER_LAYER << std::endl;
    if (total_piece_volume > NUM_LAYERS * NUM_CELLS_PER_LAYER) {
        std::cout << "Warning: Piece volume > board volume. Impossible to place all pieces." << std::endl;
    }

    std::cout << "\nPhase 1: Searching for first layer configurations..." << std::endl;
    std::vector<int> initial_layer0_cells(NUM_CELLS_PER_LAYER, 0);
    std::vector<bool> initial_pieces_used_mask(ALL_PIECES.size(), false);
    std::vector<std::tuple<int, int, bool>> initial_placements;
    find_first_layer_configs_recursive(initial_layer0_cells, initial_pieces_used_mask, initial_placements);

    std::cout << "Phase 1 complete. Found " << canonical_first_layer_boards_set.size() << " unique canonical first layer configurations." << std::endl;

    if (canonical_first_layer_boards_set.empty()) {
        std::cout << "No configurations found that fill the first layer completely." << std::endl;
        std::cout << "Attempting full solve directly on an empty board (this might be slow)..." << std::endl;
        Board empty_board(NUM_LAYERS, std::vector<int>(NUM_CELLS_PER_LAYER, 0));
        std::vector<bool> all_pieces_unused(ALL_PIECES.size(), false);
        solutions_found_count = 0; // Reset for this attempt
        if (solve_full_corn_recursive(empty_board, all_pieces_unused, 0)) {
             // Solution already printed
        } else {
             std::cout << "No solution found with direct full solve attempt." << std::endl;
        }
        return solutions_found_count > 0 ? 0 : 1;
    }
    
    std::cout << "\nPhase 2: Attempting full solve using pre-computed first layer configurations." << std::endl;
    int config_idx = 0;
    solutions_found_count = 0; // Reset for this phase
    for (const auto& canonical_repr_pair : first_layer_configs_details) {
        // const auto& canon_board0_repr = canonical_repr_pair.first; // Key
        const auto& placements_for_this_config = canonical_repr_pair.second;
        config_idx++;
        std::cout << "\nTrying full solve with first layer configuration " << config_idx 
                  << "/" << first_layer_configs_details.size() << "..." << std::endl;

        Board current_board(NUM_LAYERS, std::vector<int>(NUM_CELLS_PER_LAYER, 0));
        std::vector<bool> pieces_used_in_this_solve(ALL_PIECES.size(), false);
        int num_initial_pieces_placed = 0;

        for (const auto& p_info : placements_for_this_config) {
            int piece_id    = std::get<0>(p_info);
            int col_start   = std::get<1>(p_info);
            bool is_flipped = std::get<2>(p_info);
            
            const auto& piece_obj = ALL_PIECES[piece_id]; // piece_id is the index
            
            // Place the FULL piece onto the board, starting at layer 0, col_start
            place_piece(current_board, piece_obj, is_flipped, 0, col_start, piece_obj.id + 1);
            
            pieces_used_in_this_solve[piece_id] = true;
            num_initial_pieces_placed++;
        }
        
        // std::cout << "Initial board for this configuration:" << std::endl;
        // print_board(current_board);

        if (solve_full_corn_recursive(current_board, pieces_used_in_this_solve, num_initial_pieces_placed)) {
            // Solution already printed. Stop if MAX_SOLUTIONS_TO_FIND reached.
            if (solutions_found_count >= MAX_SOLUTIONS_TO_FIND) {
                 std::cout << "Reached max solutions to find (" << MAX_SOLUTIONS_TO_FIND << ")." << std::endl;
                 return 0;
            }
        } else {
            // std::cout << "No solution for this specific first-layer configuration." << std::endl;
        }
    }

    if (solutions_found_count == 0) {
        std::cout << "\nSearched all " << first_layer_configs_details.size() 
                  << " first layer configurations. No full solution found through this strategy." << std::endl;
        return 1; // Indicate failure
    }
    
    return 0; // Indicate success or completion
}
