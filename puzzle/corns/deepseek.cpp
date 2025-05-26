#include <iostream>
#include <vector>
#include <set>
#include <algorithm>
#include <map>
#include <tuple>

using namespace std;

// 定义玉米粒团的初始数据
vector<vector<vector<int>>> CORN_PIECES = {
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
    {{1, 1, 1}, {1, 1, 0}, {1, 0, 0}, {1, 0, 0}},
    {{0, 1, 1}, {1, 1, 1}, {0, 1, 0}},
    {{0, 1, 1}, {1, 1, 1}, {0, 0, 1}, {0, 0, 1}},
};

struct PieceVariant {
    vector<vector<int>> shape;
    int originalIndex;
};

vector<PieceVariant> generateVariants(const vector<vector<int>>& piece, int originalIndex) {
    vector<vector<int>> transformed;
    for (const auto& level : piece) {
        vector<int> revLevel(level.rbegin(), level.rend());
        transformed.push_back(revLevel);
    }
    reverse(transformed.begin(), transformed.end());

    vector<vector<vector<int>>> variants;
    variants.push_back(piece);
    variants.push_back(transformed);

    sort(variants.begin(), variants.end());
    variants.erase(unique(variants.begin(), variants.end()), variants.end());

    vector<PieceVariant> result;
    for (const auto& var : variants) {
        result.push_back({var, originalIndex});
    }
    return result;
}

vector<PieceVariant> allVariants;

void initVariants() {
    for (int i = 0; i < CORN_PIECES.size(); ++i) {
        auto variants = generateVariants(CORN_PIECES[i], i);
        for (const auto& pv : variants) {
            if (any_of(pv.shape[0].begin(), pv.shape[0].end(), [](int x) { return x == 1; })) {
                allVariants.push_back(pv);
            }
        }
    }
}

struct Placement {
    int varIndex;
    int x;
    int pieceId;
    vector<vector<int>> shape;
};

using Occupancy = vector<vector<bool>>;

set<vector<bool>> uniqueFirstLayers;

void rotateMask(vector<bool>& mask) {
    bool first = mask[0];
    for (int i = 0; i < 13; ++i) mask[i] = mask[i + 1];
    mask[13] = first;
}

vector<bool> normalizeMask(vector<bool> mask) {
    vector<bool> minMask = mask;
    for (int i = 0; i < 14; ++i) {
        rotateMask(mask);
        if (mask < minMask) minMask = mask;
    }
    return minMask;
}

void searchFirstLayer(int pos, vector<bool>& usedPieces, vector<Placement>& current, Occupancy& occupied, set<vector<bool>>& uniqueMasks) {
    if (pos == 14) {
        vector<bool> firstLayer(14, false);
        for (int i = 0; i < 14; ++i) {
            firstLayer[i] = occupied[0][i];
        }
        vector<bool> normalized = normalizeMask(firstLayer);
        if (uniqueMasks.count(normalized)) return;
        uniqueMasks.insert(normalized);
        return;
    }

    if (occupied[0][pos]) {
        searchFirstLayer(pos + 1, usedPieces, current, occupied, uniqueMasks);
        return;
    }

    for (int i = 0; i < allVariants.size(); ++i) {
        const auto& pv = allVariants[i];
        if (usedPieces[pv.originalIndex]) continue;
        int h = pv.shape.size();
        if (0 + h > 7) continue;

        for (int x = 0; x < 14; ++x) {
            bool canPlace = true;
            for (int dx = 0; dx < 3; ++dx) {
                int col = (x + dx) % 14;
                if (pv.shape[0][dx] && !occupied[0][col] && col == pos) {
                    canPlace = true;
                    break;
                }
                if (pv.shape[0][dx] && col == pos) {
                    canPlace = true;
                    break;
                }
            }
            if (!canPlace) continue;

            vector<pair<int, int>> placed;
            bool collision = false;
            for (int r = 0; r < pv.shape.size(); ++r) {
                for (int c = 0; c < 3; ++c) {
                    if (pv.shape[r][c]) {
                        int layer = 0 + r;
                        int column = (x + c) % 14;
                        if (occupied[layer][column]) {
                            collision = true;
                            break;
                        }
                    }
                }
                if (collision) break;
            }
            if (collision) continue;

            for (int r = 0; r < pv.shape.size(); ++r) {
                for (int c = 0; c < 3; ++c) {
                    if (pv.shape[r][c]) {
                        int layer = 0 + r;
                        int column = (x + c) % 14;
                        occupied[layer][column] = true;
                        placed.emplace_back(layer, column);
                    }
                }
            }

            vector<bool> newUsed = usedPieces;
            newUsed[pv.originalIndex] = true;
            current.push_back({i, x, pv.originalIndex, pv.shape});
            searchFirstLayer(pos + 1, newUsed, current, occupied, uniqueMasks);
            current.pop_back();

            for (const auto& p : placed) {
                occupied[p.first][p.second] = false;
            }
        }
    }
}

int main() {
    initVariants();
    vector<bool> usedPieces(CORN_PIECES.size(), false);
    vector<Placement> current;
    Occupancy occupied(7, vector<bool>(14, false));
    set<vector<bool>> uniqueMasks;

    searchFirstLayer(0, usedPieces, current, occupied, uniqueMasks);

    cout << "Unique first layer solutions: " << uniqueMasks.size() << endl;

    return 0;
}
