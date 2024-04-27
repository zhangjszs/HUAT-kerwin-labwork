#include <iostream>
#include <fstream>
#include <algorithm>

using namespace std;

struct node
{
    int s, f;
    int name;
    bool operator<(const node &b) const
    {
        return f < b.f;
    }
};

void GreedySelector(int n, node *act, bool *A)
{
    A[1] = true;
    int j = 1;
    for (int i = 2; i <= n; i++)
    {
        if (act[i].s >= act[j].f)
        {
            A[i] = true;
            j = i;
        }
        else
        {
            A[i] = false;
        }
    }
}

int main()
{
    int n;
    bool a[100];
    node act[100];

    ifstream infile("input.txt");
    if (!infile.is_open())
    {
        cout << "Error opening the file." << endl;
        return 1;
    }

    infile >> n;
    for (int i = 1; i <= n; i++)
        infile >> act[i].name;
    for (int i = 1; i <= n; i++)
        infile >> act[i].s;
    for (int i = 1; i <= n; i++)
        infile >> act[i].f;

    infile.close();

    sort(act + 1, act + n + 1);

    GreedySelector(n, act, a);

    int totalDays = 0;
    int lastEnd = -1;
    for (int i = 1; i <= n; i++)
    {
        if (a[i])
        {
            int profit = act[i].f - act[i].s;
            totalDays += profit;
            lastEnd = max(lastEnd, act[i].f);
            cout << "Activity " << act[i].name << " start: " << act[i].s << " end: " << act[i].f << " profit: " << profit << endl;
        }
    }

    cout << "Total days used: " << totalDays << endl;
    cout << "Last activity ends at day: " << lastEnd << endl;

    return 0;
}
