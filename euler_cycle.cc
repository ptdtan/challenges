#include <iostream>
#include <iterator>
#include <algorithm>
#include <vector>
#include <list>
#include <queue>
#include <unordered_map>
#include <stack>
#include <fstream>
#include <sstream>

using namespace std;

typedef unsigned long UL;
typedef std::unordered_map<UL, std::queue<UL> > IOq;

void insert_after(list<UL> &tour, list<UL>::iterator &it, UL e)
{
    if(it==tour.end())
    {
        tour.push_back(e);
    }else{
        it++;
        tour.insert(it, e);
        it--;
    }
}
void next_node(list<UL> &tour, list<UL>::iterator &it, IOq &out_q)
{
    while(it != tour.end()){    
        if(out_q[*it].size() > 0)
            break;
        it++;
    }
}

void eulerian(list<UL> &tour, list<UL>::iterator &it, IOq &out_q)
{
    UL next = *it;
    while(out_q[next].size() > 0){
        insert_after(tour, it, out_q[next].front());
        out_q[next].pop();
        next = *it;
    }
}
int main(int argc, char* argv[])
{
    ifstream fin(argv[1]);
    UL src, dst;
    string s, s1, buf, res;
    IOq out_q;
    list<UL> tour;

    //read edges from input
    while(getline(fin, s)){
        stringstream ss(s);
        vector<string> vs;
        while(ss >> buf)
            vs.push_back(buf);
        src = atoi(vs[0].c_str());
        stringstream ss1(vs[2]);
        while(getline(ss1, s1, ',')){
            dst = atoi(s1.c_str());
            out_q[src].push(dst);
        }
    }
    //find eulerian tour
    tour.push_back(src);
    list<UL>::iterator it_last = tour.begin();
    list<UL>::iterator it = it_last;
    while(!(it_last == tour.end() && tour.size() > 1)){
        eulerian(tour, it, out_q);
        next_node(tour, it_last, out_q);
        it = it_last;
    }

    //output
    stringstream ss;
    std::copy(tour.begin(), tour.end(), std::ostream_iterator<UL>(ss, "->"));
    res = ss.str();
    res.resize(res.size() - 2);
    std::cout << res; 
    return 0;
}
