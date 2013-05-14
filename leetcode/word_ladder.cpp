class Solution {
public:
    typedef vector<int> Conn;
    typedef vector<Conn> ConnMatrix;
    
    const static int pathInfinity = ~(1<<31);
    
    int shortestPath(int start, int end, const ConnMatrix &conn) {
        int size = conn.size();
        
        vector<int> shortestPath(size, pathInfinity);
        vector<int> prevNode(size, -1);
        vector<bool> visited(size, false);
        
        shortestPath[start] = 1;
        visited[start] = true;
        
        int current = start;
        int pathLen = shortestPath[current];
        while (true) {
            int j = 0;
            while (j < size) {
                int cost = conn[current][j];
                if (cost >= 0 && !visited[j] && pathLen + cost < shortestPath[j]) {
                    shortestPath[j] = pathLen + cost;
                    prevNode[j] = current;
                }
                
                ++j;
            }
            
            int next = -1;
            int nextLen = pathInfinity;
            j = 0;
            while (j < size) {
                if (!visited[j] && shortestPath[j] < nextLen) {
                    next = j;
                    nextLen = shortestPath[j];
                }
                
                ++j;
            }
            
            if (next != -1) {
                current = next;
                pathLen = nextLen;
                visited[current] = true;
            }
            else {
                break;
            }
        }
        
        return shortestPath[end] != pathInfinity ? shortestPath[end] : 0;
    }
    
    int wordDist(const string &a, const string &b) {
        int len = a.size();
        
        int diff = 0;
        int p = 0;
        while (diff < 2 && p < len) {
            diff += a[p] != b[p] ? 1 : 0;
            ++p;
        }
        
        return diff < 2 ? 1 : -1;
    }
    
    ConnMatrix buildConnMatrix(set<string> &dict) {
        vector<string> nodes;
        nodes.insert(nodes.begin(), dict.begin(), dict.end());
        
        int size = nodes.size();
        
        // init result
        ConnMatrix result;
        for (int i = 0; i < size; ++i) {
            result.push_back(Conn(size, -1));
        }
        
        for (int i = 0; i < size; ++i) {
            string w1 = nodes[i];
            for (int j = i+1; j < size; ++j) {
                string w2 = nodes[j];
                
                int dist = wordDist(w1, w2);
                result[i][j] = dist;
                result[j][i] = dist;
            }
        }
        
        return result;
    }

    int ladderLength(string start, string end, unordered_set<string> &dict) {
        // Start typing your C/C++ solution below
        // DO NOT write int main() function
        
        set<string> newDict(dict.begin(), dict.end());
        ConnMatrix conn = buildConnMatrix(newDict);
        
        int startP = 0;
        int endP = 0;
        
        int p = 0;
        set<string>::iterator it = newDict.begin();
        while (it != newDict.end()) {
            if (*it == start) {
                startP = p;
            }
            else if (*it == end) {
                endP = p;
            }
            
            ++p;
            ++it;
        }
        return shortestPath(startP, endP, conn);
        
    }
};
