#include <iostream>
#include<fstream>
#include<map>
#include<vector>
#include<set>
using namespace std;
class State{
   bool isStart, isFinal;///are voie sa fie recursiva, cu pointer la sine
   static int id;
   vector<pair<char, State*> > v;///fiecaare stare isi tine si tranzitiile
public:
   void setID(){id=0;}
   void assignID(map<State*, int> m){
       m[this]=id;
       id++;
   }
   State(bool start=0, bool ending=0, vector<pair<char, State*> >v_={}): isStart(start), isFinal(ending), v(v_){}
   bool getStart(){ return isStart; }
   bool getFinal(){ return isFinal; }
   void setStartFinal(int isStart_=false, int isFinal_=false){
       isStart=isStart_, isFinal=isFinal_;
   }
   void addTransition(State* finish, char key){
       v.push_back(make_pair(key, finish));
   }
};
class NFA{
    int n; ///nr de stari
    vector<State*> v;///tinem un std::vector de pointeri la stari, ca starile sunt mari
public:
    int getN(){ return n;}
    State* getStart(){
        for(int i=0; i<n; i++){
            if(v[i]->getStart())
                return v[i];
        }
        ///va ex intotdeauna start
    }
    State* getState(int i) const {
        return v[i];
    }
    NFA(int n_=0){
        n=n_;
        for(int i=1; i<=n; i++){
            State s1;
            v.push_back(&s1);
        }
    }
    NFA(const NFA& other){
        n=other.n;
        for(int i=0; i<n; i++){
            v.push_back(new State(*other.getState(i)));
        }
    }
    void addEdge(State* s1, State* s2, char key){
        s1->addTransition(s2, key);
    }
    void addState(State s1){
        v.push_back(&s1);
    }
    NFA kleeneStar(){
        NFA answer=(*this);
        State newStart(true, true);
        answer.addState(newStart);
        for(int i=0; i<n; i++){
            if(v[i]->getFinal())
                answer.addEdge(v[i], &newStart, '0');
        }
        return answer;
    }
    NFA concat(NFA &other){
        NFA answer(*this);
        for(int i=0; i<other.getN(); i++){
            answer.addState(other.getState(i));///punem si restul starilor in automat
        }
        for(int i=0; i<n; i++){
            if(v[i]->getFinal()){///daca e finala
                answer.addEdge(v[i], other.getStart(), '0');
            }
        }
        return answer;
    }
    NFA unite(NFA other){
        NFA answer(*this);
        for(int i=0; i<other.getN(); i++){
            answer.addState(other.getState(i));
        }
        State newState;
        newState.addTransition(other.getStart(), '0');
        newState.addTransition(this->getStart(), '0');
        answer.addState(newState);
        return answer;
    }
    void print(){
        for(int i=0; i<n; i++){

        }
    }
};
NFA parseRegex(string s){///legenda de la regex: . e concatenare, * e stelare, + e reuniune
    NFA answer;
    if(s=="0"){///cu lambda e o singura stare
        State s1(true, true);
        answer.addState(s1);
        return answer;
    }
    else if(s.size()==1){///daca e un singur caracter
        State s1(1, 0), s2(0, 1);
        s1.addTransition(&s2, s[0]);
        answer.addState(s1);
        answer.addState(s2);
        return answer;
    }
    else{
        int breakpoint=0; char breakSign;
        NFA n1, n2;
        for(int i=1; i<s.size()-1; i++){///ne uitam dupa un breakpoint
            if(s[i-1]==')'&&s[i+1]=='('){
                if(s[i]=='.'||s[i]=='+')
                    breakpoint=i, breakSign=s[i];
            }
        }
        if(breakpoint>0){///daca are ceva de concatenat sau de reunit, s-o faca pe aia prima, ca e invers, de fapt
           string s1=s.substr(1, breakpoint-2), s2=s.substr(breakpoint+2, s.size()-2);
           n1=parseRegex(s1), n2=parseRegex(s2);
           if(s[breakpoint]=='.'){
               NFA ans=n1.concat(n2);
               return ans;
           }
           else if(s[breakpoint]=='+'){
               return n1.unite(n2);
           }
        }
        ///daca a intrat pe asta, nu mai sunt breakpointuri nerezolvate care se puteau face imediat
        else if(s[s.size()-2]=='*'){ ///daca e cu stea kleene, are aritate 1
           string sStar=s.substr(2, s.size()-4);
           n1=parseRegex(sStar);
           n1=n1.kleeneStar();
           return n1;
        }
    }
}
int main()
{
    State trash;
    ifstream fin("regex.in");
    ofstream fout("regex.out");
    string expr; fin>>expr;
    NFA answer=parseRegex(expr);
    return 0;
}
