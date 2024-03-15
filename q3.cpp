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
map<int, map<char, vector<int> > > a;///since the state id's can get large, we store them as (startState : map from inputs to endStates)
map<int, vector<int> > lambdaClosure;
set<int> stari; ///lookup table for final states
void bfs(int node, string s, bool &accept){
    queue <pair<int, string> > q;
    accept=false;
    q.push(make_pair(node, s));///the BFS memorizes meta-states: (state, word) pairs
    for(int i=1; i<lambdaClosure[node].size(); i++)
        q.push(make_pair(lambdaClosure[node][i], s));///we need the lambda-moves dealt with separately
    while(!q.empty()){
        pair<int, string> p=q.front();
        q.pop();
        int currNode=p.first;
        s=p.second;
        char key=s[0];
        if(s.size()>0){
          string s1=s.substr(1, s.size()-1);
          for(int i=0; i<a[currNode][key].size(); i++){
             q.push(make_pair(a[currNode][key][i], s1));///whenever we cut down a character
             for(int j=1; j<lambdaClosure[a[currNode][key][i]].size(); j++)
                q.push(make_pair(lambdaClosure[a[currNode][key][i]][j], s1));///we add its full lambda closure, not just itself
          }
        }
        else{
            if(stari.find(currNode)!=stari.end())
                accept=true;
        }
    }
}
void dfsLambda(int node, int start){///this determines the lambda-closure of a node
    map<int, int> vis;
    vis[start]=1;
    for(int i=0; i<a[node]['0'].size(); i++){
        if(vis[a[node]['0'][i]]!=1){
            lambdaClosure[start].push_back(a[node]['0'][i]);
            vis[a[node]['0'][i]]=1;
            dfsLambda(a[node]['0'][i], start);
        }
    }
}
int main()
{
    int n, m, start, finalStates; fin>>n;
    for(int i=0; i<n; i++){
        fin>>start;///nu tinem starile niciaieri, doar initializam  mapul daca o fi cazul
        lambdaClosure[start].push_back(start);
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
    map<int, vector<int> >::iterator it;
    for(it=lambdaClosure.begin(); it!=lambdaClosure.end(); it++){
        dfsLambda(it->first, it->first);
    }
    int countWords; fin>>countWords;
    for(int i=0; i<countWords; i++){
      string s; fin>>s;
      bool check;
      if(s.length()==1&&s[0]=='0'){///a zero input is denoted by a lone char, 0
         check=false;
         for(int j=0; j<lambdaClosure[start].size(); j++){
            int currState=lambdaClosure[start][j];
            if(stari.find(currState)!=stari.end()){
                check=true;
            }
         }
      }
      else
        bfs(start, s, check);
      if(check)
         fout<<"ACCEPTA\n";
      else
          fout<<"RESPINGE\n";
    }
    return 0;
}
