#include <iostream>
#include <fstream>
#include <unordered_map>
#include <string>
#include <stdlib.h>
#include <vector>

using namespace std;
typedef std::unordered_map<unsigned long , vector<unsigned long> > AdjList;
typedef std::unordered_map<unsigned long , string > hashKmer;

unsigned long strHash(const char *str)
{
    unsigned long hash = 0 ;
    int c;

    while(c = *str++){
        hash = c + (hash << 6) + (hash << 16) - hash;
    }
    return hash;
}

int main(int argc, char *argv[])
{
    int k=0, i;
    string kmer, kmer_l, kmer_r;
    AdjList adj;
    hashKmer khash;
    ifstream fin(argv[1]);

    while(getline(fin, kmer)){
        if(k == 0)
            k = kmer.length();
        kmer_l = kmer.substr(0, k-1);
        kmer_r = kmer.substr(1, k-1);
        unsigned long h_l = strHash(kmer_l.c_str()), h_r = strHash(kmer_r.c_str());
        khash[h_l] = kmer_l;
        khash[h_r] = kmer_r;
        adj[h_l].push_back(h_r);
        }
    AdjList::iterator a_it;

    for(a_it = adj.begin(); a_it != adj.end(); a_it++){
        cout << khash[a_it->first] << "-> ";
        for(i = 0; i < (a_it->second).size()-1; ++i){
            cout << khash[adj[a_it->first][i]] << ",";
        }
        cout << khash[adj[a_it->first][i]] << endl;
    }
   return 0; 
}
