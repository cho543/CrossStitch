// C++11
#include <bits/stdc++.h>
using namespace std;

#define REP(i,n) for (int i=0;i<(n);i++)
const int C_MAX = 20;
mt19937 mt(1);


class CrossStitch {
private:
    clock_t start_time = clock();
    int S;
    vector<pair<int, int> > colors[C_MAX];
    vector<int> ans[C_MAX];
    vector<string> ret;
    int scores[C_MAX];
    int xx[4] = {0, 1, 0, 1};
    int yy[4] = {0, 1, 1, 0};
    int NC;



    int eval_color(int color) {
        scores[color] = 0;
        REP(i, ans[color].size()-1) {
            int r1 = colors[color][ans[color][i]].first;
            int c1 = colors[color][ans[color][i]].second;
            int r2 = colors[color][ans[color][i+1]].first;
            int c2 = colors[color][ans[color][i+1]].second;
            scores[color] += (r1 - r2) * (r1 - r2) + (c1 - c2) * (c1 - c2);
        }
        return scores[color];
    }


    void search() {
        uniform_int_distribution<int> randcolor(0, NC);
        int color = randcolor(mt);
        if (ans[color].size() <= 1) return;

        int old_score = eval_color(color);
        uniform_int_distribution<int> randrand(0, ans[color].size()-1);
        int a = randrand(mt);
        int b = a;
        while (b == a) b = randrand(mt);
        swap(ans[color][a], ans[color][b]);
        int new_score = eval_color(color);
        if (new_score > old_score) swap(ans[color][a], ans[color][b]);
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
            shuffle(ans[i].begin(), ans[i].end(), mt19937());
            if (ans[i].size() > 0) NC += 1;
        }


        REP(i, NC) {
            eval_color(i);
        }

        while(((double)clock() - start_time) / CLOCKS_PER_SEC < 5.0)
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
