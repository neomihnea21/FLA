#include <fstream>
#include<iostream>
#include<string>
#include<set>
#include<map>
using namespace std;
ifstream fin("input.txt");
ofstream fout("output.txt");
const int MAX_STATES=2000, ALPHA_SIZE=26, NIL=-1;
map<int, map<char, int> > a;

set<int> stari; ///lookup table de stari finale
int main()
{
    int n, m, start, finalStates; fin>>n;
    for(int i=0; i<n; i++){
        fin>>start;///nu tinem starile niciaieri, doar initializam  mapul daca o fi cazul
    }
    fin>>m;
    for(int i=0; i<m; i++){
        int x, y; char c; fin>>x>>y>>c; ///c poate fi si 0, la patea a treia
        a[x][c-'a']=y;
    }
    fin>>start>>finalStates;
    for(int i=0; i<finalStates; i++){
        int temp; fin>>temp;
        stari.insert(temp);
    }
    int countWords; fin>>countWords;
    for(int i=0; i<countWords; i++){
      string s; fin>>s;
      int len=s.size(), currState=start;
      bool accept=true;
      for(int i=0; i<len&&accept; i++){
         if(a[currState][s[i]-'a']!=NIL)
            currState=a[currState][s[i]-'a'];
         else
            accept=false;
      }
      if(accept&&(stari.find(currState)!=stari.end()))
        cout<<"ACCEPTA\n";
    else
        cout<<"RESPINGE\n";
    }
    return 0;
}
