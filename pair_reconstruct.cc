#include <iostream>
#include <iterator>
#include <algorithm>
#include <vector>
#include <list>
#include <set>
#include <queue>
#include <unordered_map>
#include <stack>
#include <fstream>
#include <sstream>

using namespace std;

typedef unsigned long UL;
typedef std::unordered_map<UL, std::queue<UL> > IOq;
typedef std::unordered_map<UL, std::pair<string, string> > KmerHash;

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

UL strHash(std::string s)
{
    const char *str = s.c_str();
    UL hash = 0 ;
    int c;

    while(c = *str++){
        hash = c + (hash << 6) + (hash << 16) - hash;
    }
    return hash;
}

int main(int argc, char* argv[])
{
    ifstream fin(argv[1]);
    UL src, dst;
    string s, res;
    pair<string, string> s_src, s_dst;
    IOq out_q;
    list<UL> tour;
    set<UL> non_first;
    int k, d;
    KmerHash kmerHash;

    //read edges from input
    getline(fin, s, ' '); //read k
    k = atoi(s.c_str()) - 1;

    getline(fin, s); //read d
    d = atoi(s.c_str());

    while(getline(fin, s)){
        s_src = make_pair(s.substr(0, k), s.substr(k+2, k));
        s_dst = make_pair(s.substr(1, k), s.substr(k+3, k));
        src = strHash(s_src.first + s_src.second);
        dst = strHash(s_dst.first + s_dst.second);
        kmerHash[src] = s_src;
        kmerHash[dst] = s_dst;
        out_q[src].push(dst);
        non_first.insert(dst);
    }

    IOq::iterator a_i;
    for(a_i = out_q.begin(); a_i != out_q.end(); a_i ++){
        if(non_first.find(a_i->first) == non_first.end())
        {
            src = a_i->first;
            break;
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
    it = tour.begin();
    res = kmerHash[*it].first;
    it++;
    while(it != tour.end()){
        res+=kmerHash[*it].first[k - 1];
        it++;
    }
    it--;
    string tail = kmerHash[*it].second;
    for(int i = 0; i < d + 1; i++){
        it--;
        tail = kmerHash[*it].second[0] + tail;
    }
    std::cout << res << tail << std::endl;
    return 0;
}
