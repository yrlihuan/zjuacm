import copy

# 玉米粒团定义
CORN_PIECES = [
    [[1, 1, 1], [0, 1, 1]],
    [[1, 1, 1], [0, 1, 1]],
    [[1, 1, 1], [0, 0 ,1]],
    [[1, 1, 0], [1, 1, 1], [0, 1, 1]],
    [[1, 1, 0], [0, 1, 0], [0, 1, 0], [0, 1, 1]],
    [[1, 1, 1], [0, 1, 0], [0, 1, 0], [0, 1, 0]],
    [[1, 1, 1], [0, 0, 1]],
    [[1, 1, 1], [1, 1, 0]],
    [[1, 1, 1], [0, 0, 1], [0, 0, 1]],
    [[1, 1, 1], [1, 0, 1], [0, 0, 1]],
    [[1, 1, 1], [1, 0, 1], [0, 0, 1]],
    [[1, 1, 1], [1, 0, 1], [1, 0, 1]],
    [[1, 1, 0], [1, 1, 1], [0, 0, 1]],
    [[1, 1, 1], [1, 1, 0], [1, 0, 0]],
    [[1, 1, 1], [1, 1, 0], [1, 0, 0], [1, 0, 0]],
    [[0, 1, 1], [1, 1, 1], [0, 1, 0]],
    [[0, 1, 1], [1, 1, 1], [0, 0, 1], [0, 0, 1]],
]

ROWS = 7
COLS = 14

def reverse_piece(piece):
    return [row[::-1] for row in piece]

def get_piece_variants(piece):
    # 只考虑左右翻转
    variants = []
    normal = piece
    flipped = reverse_piece(piece)
    for p in [normal, flipped]:
        if p not in variants:
            variants.append(p)
    return variants

def can_place(board, piece, row, col):
    for i, prow in enumerate(piece):
        for j, val in enumerate(prow):
            if val:
                r = row + i
                c = (col + j) % COLS
                if r >= ROWS or board[r][c]:
                    return False
    return True

def place_piece(board, piece, row, col, val):
    for i, prow in enumerate(piece):
        for j, v in enumerate(prow):
            if v:
                r = row + i
                c = (col + j) % COLS
                board[r][c] = val

def search(board, pieces, solution):
    if not pieces:
        return True, solution
    for idx, piece in enumerate(pieces):
        for variant in get_piece_variants(piece):
            ph = len(variant)
            pw = len(variant[0])
            for row in range(ROWS - ph + 1):
                for col in range(COLS):
                    if can_place(board, variant, row, col):
                        place_piece(board, variant, row, col, len(solution)+1)
                        found, sol = search(board, pieces[:idx]+pieces[idx+1:], solution+[(variant, row, col)])
                        if found:
                            return True, sol
                        place_piece(board, variant, row, col, 0)
    return False, None

def print_board(board):
    for row in board:
        print(' '.join(str(x) if x else '.' for x in row))

def main():
    board = [[0]*COLS for _ in range(ROWS)]
    found, solution = search(board, CORN_PIECES, [])
    if found:
        print("Solution found!")
        print_board(board)
        # 可选：输出每个玉米粒团的放置方式
        # for idx, (piece, row, col) in enumerate(solution):
        #     print(f"Piece {idx+1} at ({row},{col})")
    else:
        print("No solution found.")

if __name__ == "__main__":
    main()