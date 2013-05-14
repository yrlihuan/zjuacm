#include <vector>
#include <iostream>
#include <utility>
#include <set>

using namespace std;

class Solution {
public:
    typedef vector<vector<char> > Matrix;
    typedef pair<int, int> Size;

    int findMaxRectAt(int left, int top, Matrix &mat) {
        if (mat[left][top] == '0') {
            return 0;
        }
        
        int width = mat.size();
        int height = mat[0].size();
        
        set<Size> searched;
        vector<Size> queue;
        
        searched.insert(make_pair(1, 1));
        queue.push_back(make_pair(1, 1));
        int maxArea = 1;
        
        while (queue.size() > 0) {
            Size s = queue.back();
            queue.pop_back();
                
            int right = left + s.first;
            int bottom = top + s.second;
            
            // Try expand horizontal
            Size sizeH = make_pair(s.first+1, s.second);
            if (right < width && searched.find(sizeH) == searched.end()) {
                
                bool allOnes = true;
                for (int y = top; y < bottom; ++y) {
                    if (mat[right][y] == '0') {
                        allOnes = false;
                        break;
                    }
                }
                
                if (allOnes) {
                    searched.insert(sizeH);
                    queue.push_back(sizeH);
                    int area = sizeH.first * sizeH.second;
                    maxArea = area > maxArea ? area : maxArea;

                    cout << "expanding h: " << maxArea << endl;
                }
            }

            
            // Try expand vertical
            Size sizeV = make_pair(s.first, s.second+1);
            if (bottom < height && searched.find(sizeV) == searched.end()) {
                
                bool allOnes = true;
                for (int x = left; x < right; ++x) {
                    if (mat[x][bottom] == '0') {
                        allOnes = false;
                        break;
                    }
                }
                
                if (allOnes) {
                    searched.insert(sizeV);
                    queue.push_back(sizeV);
                    int area = sizeV.first * sizeV.second;
                    maxArea = area > maxArea ? area : maxArea;

                    cout << "expanding v: " << maxArea << endl;
                }
            }

        }
        
        return maxArea;
    }

    int maximalRectangle(vector<vector<char> > &matrix) {
        // Start typing your C/C++ solution below
        // DO NOT write int main() function
        
        int width = matrix.size();
        if (width == 0) {
            return 0;
        }
        
        int height = matrix[0].size();
        if (height == 0) {
            return 0;
        }
        
        int max = 0;
        for (int x = 0; x < width; ++x) {
            for (int y = 0; y < height; ++y) {
                cout << "start: " << x << ", " << y << endl;
                int area = findMaxRectAt(x, y, matrix);
                
                max = area > max ? area : max;
            }
        }
        
        return max;
    }
};

int main() {
  vector<vector<char> > mat;
  mat.push_back(vector<char>(2, '1'));
  mat.push_back(vector<char>(2, '1'));

  Solution solver;
  cout << solver.maximalRectangle(mat) << endl;
}
