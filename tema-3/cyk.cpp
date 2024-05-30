#include <fstream>
#include<iostream>
#include<vector>
#include<map>
#include<set>
#include<string>
using namespace std;
int main()
{
    const int MAXN=4000;
    std::map<char, std::vector<pair<char, char> > > prod;///productile sunt, invariabil, de forma Simbol -> alte 2 simboluri, din care poate unul e 0
    std::ifstream fin("cyk.in");
    std::ofstream fout("cyk.out");
    int termSymbols, nontermSymbols, prodCount;
    fin>>termSymbols>>nontermSymbols>>prodCount;
    for(int i=0; i<prodCount; i++){
        char start, f1, f2; fin>>start>>f1>>f2;
        prod[start].push_back({f1, f2});
    }
    std::set<char> acc[MAXN][MAXN];///acc[i][j] - multimea simbolurilor UNICE de la care, pornind, se paote obtine subcuvantul dintre i si j
    int testedWords; fin>>testedWords;
    for(int word=0; word<testedWords; word++){
        std::string s; fin>>s;
        int n=s.size();
        for(int i=0; i<n; i++){
            char start;
            vector<pair<char, char> >ext;
            for(auto x: prod){///intai generam ce se obtine cu neterminale
                start=x.first, ext=x.second;
                for(int j=0; j<ext.size(); j++){
                    if(ext[j].second=='0'&&ext[j].first==s[i]){ ///e productie cu terminal
                        acc[i][i].insert(start);///din neterminalul de inceput se va genera litera
                    }
                }
            }
            for(int j=i; j>=0; j--){
                for(int k=j; k<=i; k++){///aici se intampla magia: s[i-j] se genereaza dintr-un neterminal daca si numai daca  s[i-k] iese din unul, s[k-j] din altul si ambele din original
                   for(auto currProd: prod){
                       start=currProd.first, ext=currProd.second;
                       for(int k2=0; k2<ext.size(); k2++){
                           if(acc[j][k].find(ext[k2].first)!=acc[j][k].end() &&
                              acc[k+1][i].find(ext[k2].second)!=acc[k+1][i].end()
                              && ext[k2].second!='0')
                                acc[k+1][i].insert(ext[k2].first);
                       }
                   }
                }
            }
        }
        if(acc[0][n-1].find('S')!=acc[0][n-1].end()){///raspunsul la intrebarea magica: e in limbaj <=> s[0-(n-1)] iese din el
            fout<<"ACCEPTA\n";
        }
        else{
            fout<<"RESPINGE\n";
        }
    }
    return 0;
}
