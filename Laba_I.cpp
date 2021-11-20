#include <iostream>
#include <set>
#include <random>
#include <limits>
#include <fstream>
using namespace std;

template <typename T>
class State {
public:
    virtual bool contains(T s) const = 0;
};

template <typename T>
class DiscreteState: public State<T> {
private:
    T const state;

public:
    DiscreteState(T state) : state(state) { }

    bool contains(T s) const {
        return s == state;
    }
};

template <typename T>
class SegmentState: public State<T> {
private:
    T const beg, end;

public:
    SegmentState() : beg(0), end(-1) { }
    SegmentState(T beg, T end) : beg(beg), end(end) { }

    bool contains(T s) const {
        return s >= beg && s <= end;
    }
};

template <typename T>
class SetState: public State<T> {
private:
    set<T> const states;

public:
    SetState() : states() { }
    SetState(set<T> const& src) : states(src) { }

    bool contains(T s) const {
        return states.count(s) > 0;
    }
};

template <typename T>
class OrState : public State<T> {
private:
    State<T>& s1;
    State<T>& s2;

public:
    OrState(State<T>& s1, State<T>& s2) : s1(s1), s2(s2) { }

    bool contains(T s) const {
        return s1.contains(s) || s2.contains(s);
    }
};

template <typename T>
class AndState : public State<T> {
private:
    State<T>& s1;
    State<T>& s2;

public:
    AndState(State<T>& s1, State<T>& s2) : s1(s1), s2(s2) { }

    bool contains(T s) const {
        return s1.contains(s) && s2.contains(s);
    }
};

template <typename T>
class AndNotState : public State<T> {
private:
    State<T>& s1;
    State<T>& s2;

public:
    AndNotState(State<T>& s1, State<T>& s2) : s1(s1), s2(s2) { }

    bool contains(T s) const {
        return s1.contains(s) && not s2.contains(s);
    }
};

template <typename T>
class NotAndState : public State<T> {
private:
    State<T>& s1;
    State<T>& s2;

public:
    NotAndState(State<T>& s1, State<T>& s2) : s1(s1), s2(s2) { }

    bool contains(T s) const {
        return not s1.contains(s) && s2.contains(s);
    }
};


template <typename T>
class ProbabilityTest {
private:
    unsigned seed;
    T test_min, test_max;
    unsigned test_count;

public:
    ProbabilityTest(unsigned seed, T test_min, T test_max, unsigned test_count) : seed(seed), test_min(test_min), test_max(test_max), test_count(test_count) { }

    float operator()(State<T> const& s) const {
        default_random_engine rng(seed);
        uniform_int_distribution<T> dstr(test_min, test_max);
        unsigned good = 0;
        for (unsigned cnt = 0; cnt != test_count; ++cnt)
            if (s.contains(dstr(rng))) ++good;

        return static_cast<float>(good) / static_cast<float>(test_count);
    }
    /*
    float operator()(DiscreteState const& s) const {
        default_random_engine rng(seed);
        uniform_int_distribution<int> dstr(test_min, test_max);
        unsigned good = 0;
        for (unsigned cnt = 0; cnt != test_count; ++cnt)
            if (s.contains(dstr(rng))) ++good;

        return static_cast<float>(good) / static_cast<float>(test_count);
    }

    float operator()(SegmentState const& s) const {
        default_random_engine rng(seed);
        uniform_int_distribution<int> dstr(test_min, test_max);
        unsigned good = 0;
        for (unsigned cnt = 0; cnt != test_count; ++cnt)
            if (s.contains(dstr(rng))) ++good;

        return static_cast<float>(good) / static_cast<float>(test_count);
    }

    float operator()(SetState const& s) const {
        default_random_engine rng(seed);
        uniform_int_distribution<int> dstr(test_min, test_max);
        unsigned good = 0;
        for (unsigned cnt = 0; cnt != test_count; ++cnt)
            if (s.contains(dstr(rng))) ++good;

        return static_cast<float>(good) / static_cast<float>(test_count);
    }*/
};


int main() {
    DiscreteState<int> d(3);
    SegmentState<int> seg(8, 19);
    SetState<int> set({1, 5, 7, 11, 15, 19, 21});
    SetState<int> set2({0, 1, 2, 5, 6, 7, 8, 11, 19, 22, 33});
    
    State<int> const& s = OrState<int>(d, set2);


    ofstream out_file1("out1.txt");
    for (int i = 0; i < 1000000; i += 10000) {
        //ProbabilityTest pt(10, numeric_limits<int>::min(), numeric_limits<int>::max(), i);
        ProbabilityTest<int> pt(10, -1000, 1000, i);
        out_file1 << i << ' ' << pt(seg) << endl;
    }

    ofstream out_file2("out2.txt");
    for (int i = 0; i < 1000000; i += 10000) {
        //ProbabilityTest pt(10, numeric_limits<int>::min(), numeric_limits<int>::max(), i);
        ProbabilityTest<int> pt(10, -1000, 1000, i);
        out_file2 << i << ' ' << pt(s) << endl;
    }

    return 0;
};