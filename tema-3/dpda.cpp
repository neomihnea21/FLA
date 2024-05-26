#include <fstream>
#include<stack>
#include<vector>
#include<set>
#include<map>
#include<string>
using namespace std;
class State{
    int no;
    map<pair<char, char>, pair<string, int> > moves;///miscarea e cu input+vf stivei, si iese ce urca pe stiva+noua stare
public:
    void addMove(pair<char, char> in, pair<string, int> target){
        moves[in]=target;
    }
    pair<string, int> getDestination(pair<char, char> input){
        return moves[input];
    }
};
class PDA{
    vector<State> v;
    stack<char> s;///intrebarea magica este: cum accepta?
    set<int> finalStates;
    int start;
public:
    PDA(int n=0, char startSymbol='$'){
        for(int i=0; i<n; i++){
            State tempState;
            v.push_back(tempState);
        }
        s.push(startSymbol);
    }
    bool isFinal(int stare){
        return finalStates.find(stare)!=finalStates.end();
    }
    void addFinal(int stare){
        finalStates.insert(stare);
    }
    State getState(int k){
        return v[k];
    }
    void addMove(int src, int dest, char input, char stackTop, string newSymbols){
        v[src].addMove(make_pair(input, stackTop), make_pair(newSymbols, dest));
    }
    int makeMove(int startState, char input){
        pair<string, int> ans=v[startState].getDestination(make_pair(input, s.top()));
        string newSymbols=ans.first;
        s.pop();
        for(int i=0; i<newSymbols.size(); i++)
            s.push(newSymbols[i]);
        return  ans.second;
    }
};
int main()
{
    std::ifstream fin("dpda.in");
    std::ofstream fout("dpda.out");
    int noStates, startState, noMoves;
    fin>>noStates>>startState>>noMoves;///starile sunt NUMEROTATE DE LA 0 !!!
    PDA a(noStates);
    for(int i=0; i<noMoves; i++){
        int start, finish; char memory, input; string output;
        fin>>start>>finish>>input>>memory>>output;
        a.addMove(start, finish, input, memory, output);
    }
    int noFinals; fin>>noFinals;
    for(int i=0; i<noFinals; i++){
        int currFinal; fin>>currFinal;
        a.addFinal(currFinal);
    }
    int testedWords; fin>>testedWords;
    for(int w=0; w<testedWords; w++){
      std::string word; fin>>word;
      int statePointer=startState;
      for(int i=0; i<word.size(); i++){///automatul accepta prin stare finala
         if(word[i]!='0')
            statePointer=a.makeMove(statePointer, word[i]);
         ///0 reprezinta cuvantul lambda
      }
      if(a.isFinal(statePointer)){
          fout<<"ACCEPTA\n";
      }
      else{
        fout<<"RESPINGE\n";
      }
    }
    return 0;
}
