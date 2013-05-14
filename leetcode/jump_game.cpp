class Solution
{
public:
    bool canJump(int jumpPowers[], int n)

    {

        vector<bool> canReachEnd(n, false);

        canReachEnd[n-1] = true;

        int p = n - 1;

        while (true) {

            if (canReachEnd[0]) {

                return true;

            }

            if (p == 0) {

                return false;

            }
            

            for (int i = 0; i < p; ++i) {

                if (canReachEnd[i]) {

                    continue;

                }

                if (jumpPowers[i] + i >= p) {

                    canReachEnd[i] = true;

                }

            }

           

            while (p > 0) {

                if (canReachEnd[--p]) {

                    break;

                }

            }

        }

    }

};
