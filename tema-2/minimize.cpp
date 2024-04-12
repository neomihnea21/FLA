#include <fstream>
#include<iostream>
#include<map>
#include<set>
#include<vector>
using namespace std;
int find(int x, int sef[]){///dupa ce avem tabla echivalentei Nerode, le dam merge cu union-find
    if(sef[x]==x) return x;
    return sef[x]=find(sef[x], sef);
}
void unify(int x, int y, int sef[]){
    int sefX=find(x, sef), sefY=find(y, sef);
    sef[sefY]=sefX;
}
class DFA{///nu are rost sa mai defimim o clasa separata pentru stare, nu e povestea asa de complexa
    int start, nodes; ///alfabetul e by default a-z
    map<int, map<char, int> > m;
    set<int> finale;
    set<int> reach; ///tinem un std::set cu stari unde se poate ajunge, nu
public:
    DFA(int start_=0, int nodes_=0, map<int, map<char, int> >m_={}, set<int> finale_={}): start(start_), nodes(nodes_), m(m_), finale(finale_) {}
    int getStart(){return start;}
    void addFinal(int k){
        finale.insert(k);
    }
    void print(std::ostream &out){///nu mai stau sa supraincarc operatorul <<
       for(map<int, map<char, int> >::iterator it1=m.begin(); it1!=m.end(); it1++){
          map<char, int> curr=it1->second;
          for(map<char, int>::iterator it2=curr.begin(); it2!=curr.end(); it2++){
            if(it2->second!=0)///0 e pus automat, ca sa fie complet AFD-ul, deci se ignora
               out<<it1->first<<" "<<it2->second<<" "<<it2->first<<"\n";
          }
       }
    }
    void addMove(int start, int finish, char key){
        m[start][finish]=key;
    }
    void dfs(int state){///verifica toate nodurile unde se poate merge dintr-o stare, cu orice litere
        reach.insert(state);
        for(char c='a'; c<='z'; c++){
            int newState=m[state][c];
            if(reach.find(newState)==reach.end()&&newState>0)///nu vrem sa faca dfs in eter
                dfs(newState);
        }
    }
    DFA minimize(){///nu vom mergeui toate starile, ci doar pe acelea unde se si ajunge
       vector<int> stari;
       DFA result;///il umplem la final, e aici pt debugging
       for(set<int>::iterator it=reach.begin(); it!=reach.end(); it++){
          stari.push_back(*it);///eliminam starile unde nu se poate ajunge
       }///noi vrem sa comasam niste noduri echivalente (ie. din ele cu acelasi cuvant se ajunge in acelasi tip de stare)
       map<pair<int, int>, int> egal;///si pentru asta vom determina cine NU POATE fi echivalent
       for(int i=0; i<stari.size(); i++){
          for(int j=i+1; j<stari.size(); j++){
              bool iFinal=(finale.find(stari[i])==finale.end()),
                   jFinal=(finale.find(stari[j])==finale.end());
              if(iFinal^jFinal){///daca una e finala si alta nu e, nu or sa fie echivlaente
                  egal[make_pair(stari[i], stari[j])]=1;
              }
          }
       }
       for(int lenWord=0; lenWord<stari.size()-1; lenWord++)
         for(char c='a'; c<='z'; c++){///daca un singur caracter nu le strica feng shuiul, nici mai multe n-or sa o faca
            for(int i=0; i<stari.size(); i++){
                for(int j=i+1; j<stari.size(); j++){
                    int nextI=m[stari[i]][c], nextJ=m[stari[j]][c];///tranzitiile astea trebuie sa si existe
                    if(nextI!=0&&nextJ!=0&&egal[make_pair(nextI, nextJ)]==1){
                        egal[{stari[i], stari[j]}]=1;///daca o litera le duce in stari neechivalente, sunt neechivalente
                    }
                }
            }
         }///si acum, bagam un simplu DSU pentru a vedea cum arata noul automat
       const int MAX_STATES=3000;///nu pot compacta mari automate
       int sefi[MAX_STATES], newNodes=nodes;
       for(int i=1; i<=nodes; i++) sefi[i]=i;///la inceput, fiecare e propria multime
       for(int i=1; i<=nodes; i++){
          for(int j=i+1; j<=nodes; j++){///le unim dupa clasa de echivalenta
            if(egal[{i, j}]==0&&find(i, sefi)!=find(j, sefi)){
                unify(i, j, sefi);
                newNodes--;
            }
          }
       }
       int newStart=sefi[start];
       map<int, map<char, int> > newM;
       for(map<int, map<char, int> >::iterator it1=m.begin(); it1!=m.end(); it1++){
          map<char, int> curr=it1->second;
          for(map<char, int>::iterator it2=curr.begin(); it2!=curr.end(); it2++){
            newM[sefi[it1->first]][it2->first]=sefi[it2->second];
          }
       }
       set<int> newFinals;
       for(int i=1; i<=nodes; i++) newFinals.insert(sefi[i]);
       result=DFA(newStart, newNodes, newM, newFinals);
       return result;
    }
    friend std::istream& operator>>(std::istream &in, DFA &d);
};
std::istream& operator>>(std::istream &in, DFA &d){
    int ctMoves, ctFinal; in>>d.nodes>>ctMoves;
    in>>d.start;
    for(int i=0; i<ctMoves; i++){
        int x, y; char key; in>>x>>y>>key;
        d.m[x][key]=y;
    }
    in>>ctFinal;
    for(int i=0; i<ctFinal; i++){
        int temp; in>>temp;
        d.finale.insert(temp);
    }
    return in;
}
int main()
{
    ifstream fin("minimize.in");
    ofstream fout("minimize.out");
    DFA automat; fin>>automat;
    automat.dfs(automat.getStart());
    DFA minim=automat.minimize();
    minim.print(fout);///facem o schema: bag parametru locul unde sa scrie
    return 0;
}
