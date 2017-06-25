#include <iostream>
#include <iterator>
#include <algorithm>
#include <vector>
#include <list>
#include <set>
#include <queue>
#include <map>
#include <stack>
#include <fstream>
#include <sstream>

using namespace std;

typedef unsigned long UL;
typedef std::map<UL, list<UL> > ADJ;
typedef std::map<UL, string> KmerHash;
typedef std::map<UL, int> EdgeCounter;

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

void elongate(vector<vector<UL> >&contigs, ADJ &adj_o, ADJ &adj_i)
{
    ADJ::iterator it = adj_o.begin();
    list<UL>::iterator it_l;
    UL e;
    
    while(it != adj_o.end()){
        if(it->second.size() == 1 && adj_i[it->first].size() == 1){
            it++;
            continue;
        }
        it_l = it->second.begin();
        while(it_l != it->second.end()){
            vector<UL> path; //init new path
            path.push_back(it->first);
            e = *it_l;
            while(1){
                path.push_back(e);
                if(!(adj_o[e].size() == 1 && adj_i[e].size() == 1))
                    break;
                e = adj_o[e].front();
            }
            contigs.push_back(path);
            it_l++;
        }
        it++;
    }
}

int main(int argc, char* argv[])
{
    ifstream fin(argv[1]);
    UL src, dst;
    string s, res, s_src, s_dst;
    ADJ adj_o, adj_i;
    int k, d;
    KmerHash kmerHash;

    while(getline(fin, s)){
        k = s.length();
        s_src = s.substr(0, k-1);
        s_dst = s.substr(1, k-1);
        src = strHash(s_src);
        dst = strHash(s_dst);
        kmerHash[src] = s_src;
        kmerHash[dst] = s_dst;
        adj_o[src].push_back(dst);
        adj_i[dst].push_back(src);
    }
    vector<vector<UL> > contigs;
    elongate(contigs, adj_o, adj_i);
    k--;
    for(int i = 0; i < contigs.size(); ++i){
        std::cout << kmerHash[contigs[i][0]];
        for(int j = 1; j < contigs[i].size(); ++j){
            std::cout << kmerHash[contigs[i][j]][k-1];
        }
        std::cout << std::endl;
    }
    return 0;
}
