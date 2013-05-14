class Solution {
public:
    typedef vector<string> VS;

void appendNextChar(int n, int left, int right, string &str, VS &result)

{

    if (right == n) {

        result.push_back(str);

    }

    if (left > right) {

        str += ')';

        appendNextChar(n, left, right+1, str, result);
        
        str = str.substr(0, str.size()-1);

    }

    if (left < n) {

        str += '(';

        appendNextChar(n, left+1, right, str, result);
        str = str.substr(0, str.size()-1);
    }

}

VS generateParenthesis(int n) {

VS result;

string str;

appendNextChar(n, 0, 0, str, result);

return result;
}
};
