#include <fstream>
#include <iostream>
#include<set>
#include<map>
#include<string>
#include<vector>
#include<queue>
using namespace std;
ifstream fin("input.txt");
ofstream fout("output.txt");
const int MAX_STATES=2000, ALPHA_SIZE=26, NIL=-1;
map<int, map<char, vector<int> > > a;
set<int> stari; ///lookup table de stari finale)
void bfs(int node, string s, bool &accept){
    queue <pair<int, string> > q;
    accept=false;
    q.push(make_pair(node, s));
    while(!q.empty()){
        pair<int, string> p=q.front();
        q.pop();
        int currNode=p.first;
        string s=p.second;
        char key=s[0];
        if(s.size()>0){
          string s1=s.substr(1, s.size()-1);
          for(int i=0; i<a[currNode][key].size(); i++)
             q.push(make_pair(a[currNode][key][i], s1));
        }
        else{
            if(stari.find(currNode)!=stari.end())
                accept=true;
        }
    }
}
int main()
{
    int n, m, start, finalStates; fin>>n;
    for(int i=0; i<n; i++){
        fin>>start;///nu tinem starile niciaieri, doar initializam  mapul daca o fi cazul
    }
    fin>>m;
    for(int i=0; i<m; i++){
        int x, y; char c; fin>>x>>y>>c; ///c poate fi si 0, la patea a treia
        a[x][c].push_back(y);
    }
    fin>>start>>finalStates;
    for(int i=0; i<finalStates; i++){
        int temp; fin>>temp;
        stari.insert(temp);
    }
    int countWords; fin>>countWords;
    for(int i=0; i<countWords; i++){
      string s; fin>>s;
      bool check;
      bfs(start, s, check);
      if(check)
         fout<<"ACCEPTA\n";
      else
          fout<<"RESPINGE\n";
    }
    return 0;
}
