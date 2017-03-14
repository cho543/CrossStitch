// C++11
#include <bits/stdc++.h>
using namespace std;

#define REP(i,n) for (int i=0;i<(n);i++)

const long long int CYCLE_PER_SEC = 2400000000;

unsigned long long int getCycle()
{
  unsigned int low, high;
  __asm__ volatile ("rdtsc" : "=a" (low), "=d" (high));
  return ((unsigned long long int)low) | ((unsigned long long int)high << 32);
}

double getTime(unsigned long long int begin_cycle)
{
  return (double)(getCycle() - begin_cycle) / CYCLE_PER_SEC;
}

unsigned long long int startCycle = getCycle();

const int C_MAX = 22;
int S;
vector<pair<int, int> > colors[C_MAX];
vector<int> ans[C_MAX];
vector<string> ret;
int scores[C_MAX];
int xx[4] = {0, 1, 0, 1};
int yy[4] = {0, 1, 1, 0};
int NC;
mt19937 mt(1);

int eval_color(int color) {
    int score = 0;
    REP(i, ans[color].size()-1) {
        int r1 = colors[color][ans[color][i]].first;
        int c1 = colors[color][ans[color][i]].second;
        int r2 = colors[color][ans[color][i+1]].first;
        int c2 = colors[color][ans[color][i+1]].second;
        score += (r1 - r2) * (r1 - r2) + (c1 - c2) * (c1 - c2);
    }
    return score;
}


void search() {
    const int UNIT = 100;
    int a_cand[UNIT], b_cand[UNIT];
    uniform_int_distribution<int> randcolor(0, NC-1);
    int color, old_score, a, b, new_score;

    while(getTime(startCycle) < 9.5) {
        color = randcolor(mt);
        if (ans[color].size() <= 1) continue;

        old_score = scores[color];
        uniform_int_distribution<int> randrand(0, ans[color].size()-1);

        REP(i, UNIT) {
            a = randrand(mt);
            b = a;
            while (b == a) b = randrand(mt);
            swap(ans[color][a], ans[color][b]);
            a_cand[i] = a;
            b_cand[i] = b;
        }

        new_score = eval_color(color);
        if (new_score <= old_score) {
            scores[color] = new_score;
        }
        else {
            for (int i = UNIT-1; i >= 0; i--) {
                swap(ans[color][a_cand[i]], ans[color][b_cand[i]]);
            }
        }
    }
}


void make_ret() {
    REP(i, NC) {
        if (ans[i].size() == 0) continue;

        ret.push_back(string(1, 'a'+i));

        if (ans[i].size() == 1) {
            int r = colors[i][0].first;
            int c = colors[i][0].second;
            REP(j, 4) ret.push_back(to_string(r+xx[j]) +  " " + to_string(c+yy[j]));
            continue;
        }

        int start = -10;

        REP(j, ans[i].size()-1) {
            int r1 = colors[i][ans[i][j]].first;
            int c1 = colors[i][ans[i][j]].second;
            int r2 = colors[i][ans[i][j+1]].first;
            int c2 = colors[i][ans[i][j+1]].second;
            int md = 1 << 30;
            int ma = -1;
            int mb = -1;
            REP(a, 4) REP(b, 4) {
                if (a/2 == start/2) continue;
                int d = (r1+xx[a]-r2-xx[b]) * (r1+xx[a]-r2-xx[b]) +
                        (c1+yy[a]-c2-yy[b]) * (c1+yy[a]-c2-yy[b]);
                if (d != 0 && d < md) {
                    md = d; ma = a; mb = b;
                }
            }

            if (start < 0) {
                if (ma <= 1) start = 2;
                else start = 0;
            }

            int s1 = start;
            int s2 = (start % 2 == 1) ? start - 1 : start + 1;
            int s3 = (ma % 2 == 1) ? ma - 1 : ma + 1;
            int s4 = ma;
            ret.push_back(to_string(r1+xx[s1]) + " " + to_string(c1+yy[s1]));
            ret.push_back(to_string(r1+xx[s2]) + " " + to_string(c1+yy[s2]));
            ret.push_back(to_string(r1+xx[s3]) + " " + to_string(c1+yy[s3]));
            ret.push_back(to_string(r1+xx[s4]) + " " + to_string(c1+yy[s4]));
            start = mb;
        }

        int r1 = colors[i][ans[i].back()].first;
        int c1 = colors[i][ans[i].back()].second;
        int s1 = start;
        int s2 = (start % 2 == 1) ? start - 1 : start + 1;
        int s3 = (start / 2 == 1) ? 0 : 2;
        int s4 = s3 + 1;
        ret.push_back(to_string(r1+xx[s1]) + " " + to_string(c1+yy[s1]));
        ret.push_back(to_string(r1+xx[s2]) + " " + to_string(c1+yy[s2]));
        ret.push_back(to_string(r1+xx[s3]) + " " + to_string(c1+yy[s3]));
        ret.push_back(to_string(r1+xx[s4]) + " " + to_string(c1+yy[s4]));

    }
}


class CrossStitch {
public:
    vector<string> embroider(vector<string> pattern) {

        S = pattern.size();
        REP(r, S) REP(c, S) {
            if (pattern[r][c] != '.') {
                colors[pattern[r][c]-'a'].push_back(make_pair(r, c));
            }
        }

        NC = 0;
        REP(i, C_MAX) {
            ans[i] = vector<int>(colors[i].size());
            iota(ans[i].begin(), ans[i].end(), 0);
            //shuffle(ans[i].begin(), ans[i].end(), mt19937());
            if (ans[i].size() > 0) NC += 1;
        }


        REP(i, NC) {
            //scores[i] = 1 << 30;
            scores[i] = eval_color(i);
            //cerr << 'a' + i << " " << scores[i] << endl;
        }

        //while(getTime(startCycle) < 9.5)
        search();

        make_ret();


        return ret;
    }
};
// -------8<------- end of solution submitted to the website -------8<-------

template<class T> void getVector(vector<T>& v) {
    for (int i = 0; i < v.size(); ++i)
        cin >> v[i];
}

int main() {
    int S;
    cin >> S;
    vector<string> pattern(S);
    getVector(pattern);

    CrossStitch cs;
    vector<string> ret = cs.embroider(pattern);
    cout << ret.size() << endl;
    for (int i = 0; i < (int)ret.size(); ++i)
        cout << ret[i] << endl;
    cout.flush();
}
