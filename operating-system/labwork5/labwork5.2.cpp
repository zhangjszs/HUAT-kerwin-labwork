#include <iostream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <list>
#include <algorithm>
#include <ctime>
#include <climits>

class PageReplacementAlgorithms
{
public:
    // 最佳淘汰算法 (OPT)
    double OPT(const std::vector<int> &pages, int capacity)
    {
        int hits = 0;
        std::unordered_set<int> s;
        std::unordered_map<int, int> index;

        for (int i = 0; i < pages.size(); i++)
        {
            if (s.size() < capacity)
            {
                if (s.find(pages[i]) != s.end())
                {
                    hits++;
                }
                else
                {
                    s.insert(pages[i]);
                }
            }
            else
            {
                if (s.find(pages[i]) != s.end())
                {
                    hits++;
                }
                else
                {
                    int farthest = i + 1, val = -1;
                    for (auto it : s)
                    {
                        int j;
                        for (j = i + 1; j < pages.size(); j++)
                        {
                            if (pages[j] == it)
                            {
                                if (j > farthest)
                                {
                                    farthest = j;
                                    val = it;
                                }
                                break;
                            }
                        }
                        if (j == pages.size())
                        {
                            val = it;
                            break;
                        }
                    }
                    s.erase(val);
                    s.insert(pages[i]);
                }
            }
            index[pages[i]] = i;
        }

        return 1.0 - (double)(pages.size() - hits) / pages.size();
    }

    // FIFO Algorithm
    double FIFO(const std::vector<int> &pages, int capacity)
    {
        int hits = 0;
        std::unordered_set<int> s;
        std::queue<int> indexes;

        for (int i = 0; i < pages.size(); i++)
        {
            if (s.size() < capacity)
            {
                if (s.find(pages[i]) != s.end())
                {
                    hits++;
                }
                else
                {
                    s.insert(pages[i]);
                    indexes.push(pages[i]);
                }
            }
            else
            {
                if (s.find(pages[i]) != s.end())
                {
                    hits++;
                }
                else
                {
                    int val = indexes.front();
                    indexes.pop();
                    s.erase(val);
                    s.insert(pages[i]);
                    indexes.push(pages[i]);
                }
            }
        }

        return 1.0 - (double)(pages.size() - hits) / pages.size();
    }

    // LRU Algorithm
    double LRU(const std::vector<int> &pages, int capacity)
    {
        int hits = 0;
        std::unordered_set<int> s;
        std::list<int> indexes;

        for (int i = 0; i < pages.size(); i++)
        {
            if (s.size() < capacity)
            {
                if (s.find(pages[i]) != s.end())
                {
                    hits++;
                    indexes.remove(pages[i]);
                }
                else
                {
                    s.insert(pages[i]);
                }
            }
            else
            {
                if (s.find(pages[i]) != s.end())
                {
                    hits++;
                    indexes.remove(pages[i]);
                }
                else
                {
                    int val = indexes.back();
                    indexes.pop_back();
                    s.erase(val);
                    s.insert(pages[i]);
                }
            }
            indexes.push_front(pages[i]);
        }

        return 1.0 - (double)(pages.size() - hits) / pages.size();
    }

    // LFU Algorithm
    double LFU(const std::vector<int> &pages, int capacity)
    {
        int hits = 0;
        std::unordered_map<int, int> freq;
        std::unordered_map<int, std::list<int>::iterator> pos;
        std::list<int> lru;

        for (int i = 0; i < pages.size(); i++)
        {
            if (freq.size() < capacity)
            {
                if (freq.find(pages[i]) != freq.end())
                {
                    hits++;
                    freq[pages[i]]++;
                }
                else
                {
                    freq[pages[i]] = 1;
                }
            }
            else
            {
                if (freq.find(pages[i]) != freq.end())
                {
                    hits++;
                    freq[pages[i]]++;
                }
                else
                {
                    int minFreq = INT_MAX, val = -1;
                    for (auto &p : freq)
                    {
                        if (p.second < minFreq)
                        {
                            minFreq = p.second;
                            val = p.first;
                        }
                    }
                    freq.erase(val);
                    freq[pages[i]] = 1;
                }
            }
        }

        return 1.0 - (double)(pages.size() - hits) / pages.size();
    }

    // NUR Algorithm
    double NUR(const std::vector<int> &pages, int capacity)
    {
        int hits = 0;
        std::unordered_set<int> s;
        std::unordered_map<int, bool> ref_bits;
        std::queue<int> fifo;

        for (int i = 0; i < pages.size(); i++)
        {
            if (s.size() < capacity)
            {
                if (s.find(pages[i]) != s.end())
                {
                    hits++;
                    ref_bits[pages[i]] = true;
                }
                else
                {
                    s.insert(pages[i]);
                    ref_bits[pages[i]] = false;
                    fifo.push(pages[i]);
                }
            }
            else
            {
                if (s.find(pages[i]) != s.end())
                {
                    hits++;
                    ref_bits[pages[i]] = true;
                }
                else
                {
                    while (!fifo.empty() && ref_bits[fifo.front()])
                    {
                        ref_bits[fifo.front()] = false;
                        fifo.pop();
                        fifo.push(fifo.front());
                    }
                    int val = fifo.front();
                    fifo.pop();
                    s.erase(val);
                    ref_bits.erase(val);
                    s.insert(pages[i]);
                    ref_bits[pages[i]] = false;
                    fifo.push(pages[i]);
                }
            }
        }

        return 1.0 - (double)(pages.size() - hits) / pages.size();
    }
};

int main()
{
    PageReplacementAlgorithms algorithms;
    std::vector<int> pages = {7, 0, 1, 2, 0, 3, 0, 4, 2, 3, 0, 3, 2};
    int capacity = 4;
    std::cout << "一个页地址流,为";
    for (auto i : pages)
        std::cout << i << " ";
    std::cout << std::endl;
    std::cout << "内存的容量：" << capacity << std::endl;
    std::cout << "OPT hit 命中率: " << algorithms.OPT(pages, capacity) << std::endl;
    std::cout << "FIFO hit 命中率: " << algorithms.FIFO(pages, capacity) << std::endl;
    std::cout << "LRU hit 命中率: " << algorithms.LRU(pages, capacity) << std::endl;
    std::cout << "LFU hit 命中率: " << algorithms.LFU(pages, capacity) << std::endl;
    std::cout << "NUR hit 命中率: " << algorithms.NUR(pages, capacity) << std::endl;

    return 0;
}
