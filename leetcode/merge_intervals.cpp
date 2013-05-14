vector<Interval> merge(vector<Interval> &intervals) {

    if (intervals.size() == 0) {

        return intervals;

    }

    vector<Interval> intCopy = intervals;

    stable_sort(intCopy.begin(), intCopy.end(), compInterval);

    vector<Interval> result;

    Interval prev = intCopy.front();

    int i = 1;

    int size = intCopy.size();

    while (i != size) {

        Interval &current = intCopy[i];

        if (prev.end >= current.start) {

            prev.end = max(prev.end, current.end);

        }

        else {

            result.push_back(prev);

            prev = current;

        }

        ++i;

    }

    result.push_back(prev);

    return result;
}
};
