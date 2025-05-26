import copy
from typing import List, Tuple, Optional

# 环形玉米棒子参数
ROWS = 7
COLS = 14

# 示例玉米粒团，可以自行添加更多
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

def rotate_piece(piece: List[List[int]]) -> List[List[int]]:
    """顺时针旋转玉米粒团90度"""
    return [list(row) for row in zip(*piece[::-1])]

def get_piece_variants(piece: List[List[int]]) -> List[List[List[int]]]:
    """获取玉米粒团的所有旋转形态（0, 90, 180, 270度）"""
    variants = []
    current = piece
    for _ in range(4):
        if current not in variants:
            variants.append(copy.deepcopy(current))
        current = rotate_piece(current)
    return variants

def can_place(board: List[List[int]], piece: List[List[int]], row: int, col: int) -> bool:
    """判断玉米粒团能否放在指定位置"""
    for i, prow in enumerate(piece):
        for j, val in enumerate(prow):
            if val:
                r = row + i
                c = (col + j) % COLS  # 环形
                if r >= ROWS or board[r][c]:
                    return False
    return True

def place_piece(board: List[List[int]], piece: List[List[int]], row: int, col: int, pid: int):
    """放置玉米粒团到棋盘上"""
    for i, prow in enumerate(piece):
        for j, val in enumerate(prow):
            if val:
                r = row + i
                c = (col + j) % COLS
                board[r][c] = pid

def remove_piece(board: List[List[int]], piece: List[List[int]], row: int, col: int):
    """移除玉米粒团"""
    for i, prow in enumerate(piece):
        for j, val in enumerate(prow):
            if val:
                r = row + i
                c = (col + j) % COLS
                board[r][c] = 0

def find_empty(board: List[List[int]]) -> Optional[Tuple[int, int]]:
    """找到第一个空格"""
    for i in range(ROWS):
        for j in range(COLS):
            if board[i][j] == 0:
                return i, j
    return None

def solve(board: List[List[int]], pieces: List[List[List[int]]], used: List[bool], solution: List[Tuple[int, int, int, int]]) -> Optional[List[Tuple[int, int, int, int]]]:
    """递归搜索解决方案"""
    empty = find_empty(board)
    if empty is None:
        return solution  # 全部填满
    row, col = empty
    for idx, piece in enumerate(pieces):
        if used[idx]:
            continue
        for variant in get_piece_variants(piece):
            for offset in range(COLS):  # 环形每一列都尝试
                if can_place(board, variant, row, (col + offset) % COLS):
                    place_piece(board, variant, row, (col + offset) % COLS, idx + 1)
                    used[idx] = True
                    solution.append((idx, row, (col + offset) % COLS, get_piece_variants(piece).index(variant)))
                    result = solve(board, pieces, used, solution)
                    if result:
                        return result
                    solution.pop()
                    used[idx] = False
                    remove_piece(board, variant, row, (col + offset) % COLS)
    return None

def print_board(board: List[List[int]]):
    for row in board:
        print(' '.join(str(x) if x else '.' for x in row))

def main():
    board = [[0 for _ in range(COLS)] for _ in range(ROWS)]
    used = [False] * len(CORN_PIECES)
    solution = []
    result = solve(board, CORN_PIECES, used, solution)
    if result:
        print("找到解决方案:")
        print_board(board)
        print("放置顺序: (piece_id, row, col, variant_id)")
        print(result)
    else:
        print("无解")

if __name__ == "__main__":
    main()