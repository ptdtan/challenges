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

void dfs(ADJ &adj_o, ADJ &adj_i, UL u, set<UL> &c, map<UL, bool> &visited) //nodes is not reference
{
    std::stack<UL> s;
    list<UL>::iterator it;
    UL v;

    s.push(u);
    while(!s.empty()){
        v = s.top();
        s.pop();
        for(it = adj_o[v].begin(); it != adj_o[v].end(); it++){
            if(visited[*it])
                continue;
            s.push(*it);
            c.insert(*it);
            visited[*it] = true;
        }
        for(it = adj_i[v].begin(); it != adj_i[v].end(); it++){
            if(visited[*it])
                continue;
            s.push(*it);
            c.insert(*it);
            visited[*it] = true;
        }
    }
}
vector<vector<UL> > elongate(ADJ &adj_o, ADJ &adj_i, set<UL> &nodes)
{
    set<UL>::iterator it = nodes.begin();
    list<UL>::iterator it_l;
    UL e;
    vector<vector<UL> > contigs;
    
    while(it != nodes.end()){
        if(adj_o[*it].size() == 1 && adj_i[*it].size() == 1){
            it++;
            continue;
        }
        it_l = adj_o[*it].begin();
        while(it_l != adj_o[*it].end()){
            vector<UL> path; //init new path
            path.push_back(*it);
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
    return contigs;
}

int main(int argc, char* argv[])
{
    ifstream fin(argv[1]);
    UL src, dst;
    string s, res, s_src, s_dst, buf, s1;
    ADJ adj_o, adj_i;
    int k, d, i, j;
    map<UL, bool> visited;

    while(getline(fin, s)){
        stringstream ss(s);
        vector<string> vs;
        while(ss >> buf)
            vs.push_back(buf);
        src = atoi(vs[0].c_str());
        stringstream ss1(vs[2]);
        while(getline(ss1, s1, ',')){
            dst = atoi(s1.c_str());
            adj_o[src].push_back(dst);
            adj_i[dst].push_back(src);
            visited[src] = false;
            visited[dst] = false;
        }
    }

    k--; //for printing kmer
    map<UL, bool>::iterator u_it = visited.begin();
    while(u_it != visited.end()){
        if(u_it->second){
            u_it++;
            continue;
        }
        set<UL> c;
        dfs(adj_o, adj_i, u_it->first, c, visited);
        vector<vector<UL> > contigs = elongate(adj_o, adj_i, c);
        if(c.size() > 0 && contigs.size() == 0){
            vector<UL> path;
            UL u = u_it->first;
            path.push_back(u);
            while(path.size() < c.size()+1){
                path.push_back(adj_o[u].front());
                u = adj_o[u].front();
            }
            contigs.push_back(path);
        }

        for(i = 0; i < contigs.size(); ++i){
            for(j = 0; j < contigs[i].size() - 1; ++j){
                std::cout << contigs[i][j] << "->";
            }
            std::cout << contigs[i][j] << std::endl;
        }
        u_it++;
    }
    return 0;
}
