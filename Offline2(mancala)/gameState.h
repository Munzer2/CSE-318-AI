#include<bits/stdc++.h>
#define ll long long
#define PITS 6

using namespace std; 


class State { 
public:
    vector< ll > min , max;
    ll curr_pl; 
    ll max_score,min_score;
    vector< ll > _extra, _capStones; 
    State() {
        min.resize(PITS,4);
        max.resize(PITS,4);
        _extra.resize(2,0); 
        _capStones.resize(2,0); 
        curr_pl = 0;
        max_score = 0; 
        min_score = 0;
    }
    void ApplyMove(ll ind) {
        ll stones; 
        if(curr_pl) {
            stones = min[ind] ;
            if(!stones) return; 
            min[ind] = 0;
            if(ind == 0) {
                min_score++;
                stones--;
                if(!stones) { _extra[curr_pl]++;   return;}   
                makeMove(ind,stones,(curr_pl+1)%2,curr_pl); 
            }
            else makeMove(ind-1, stones,curr_pl, curr_pl); 
        }
        else{
            stones = max[ind] ; 
            if(!stones) return ;   
            max[ind] = 0;
            if(ind == PITS-1) {
                max_score++;
                stones--;
                if(!stones) { _extra[curr_pl]++;   return;}  
                makeMove(ind,stones,(curr_pl+1)%2,curr_pl); 
            }
            else makeMove(ind+1, stones,curr_pl, curr_pl); 
        }
        return ; 
    }
    void makeMove(ll ind,ll stones, ll curr, ll& curr_pl) {
        if(!stones) return; 
        if(curr == 0) {
            for(ll i = ind ; i < PITS; ++i) {
                max[i]++; 
                stones--; 
                if(!stones) {
                    if(max[i] == 1 && min[i] > 0 && curr_pl == 0) {
                        max_score += (min[i] + max[i]);
                        _capStones[curr_pl] += min[i]; 
                        max[i] = 0; 
                        min[i] = 0; 
                    }
                    curr_pl = (curr_pl + 1)%2; 
                    return; 
                }
            }
            if(stones) {
                if(curr_pl == 0) {
                    max_score++ ; 
                    stones--;
                    if(!stones){ _extra[curr_pl]++; }
                }
                makeMove(PITS-1,stones, (curr+1)%2, curr_pl);
            }
        }
        else{
            for(ll i = ind; i >= 0 ; --i) {
                min[i]++; 
                stones--; 
                if(!stones) {
                    if(min[i] == 1 && max[i] > 0 && curr_pl == 1) {
                        min_score += (max[i] + min[i]); 
                        _capStones[curr_pl] += max[i]; 
                        max[i] = 0; 
                        min[i] = 0; 
                    }
                    curr_pl = (curr_pl+1)%2;
                    return;  
                }
            }
            if(stones) {
                if(curr_pl) {
                    stones--; 
                    min_score++; 
                    if(!stones) _extra[curr_pl]++;
                }
                makeMove(0,stones,(curr+1)%2, curr_pl);
            }
        }
        return; 
    }


    vector< ll > LegalMoves() {
        vector< ll > ans; 
        for(ll i = 0 ; i  < PITS; ++i) {
            if(curr_pl == 0 && max[i] > 0 ) ans.push_back(i); 
            else if(curr_pl == 1 && min[i] > 0) ans.push_back(i); 
        }
        return ans; 
    }

    ll heuristic1() {
        return max_score - min_score;
    }

    ll heuristic2(ll w1, ll w2) {
        ll d = w1 * heuristic1();
        ll mySide = accumulate(max.begin(), max.end(), 0); 
        ll oppSide = accumulate(min.begin(), min.end(), 0); 
        return d + w2*(mySide - oppSide);  
    }

    ll heuristic3(ll w1, ll w2, ll w3) {
        ll d = w1 * heuristic1();
        ll mySide = accumulate(max.begin(), max.end(), 0); 
        ll oppSide = accumulate(min.begin(), min.end(), 0); 
        return d + w2*(mySide-oppSide) + w3*(_extra[0]- _extra[1]);  
    }

    ll heuristic4(ll w1, ll w2, ll w3, ll w4) {
        ll d = w1 * heuristic1();
        ll mySide = accumulate(max.begin(), max.end(), 0); 
        ll oppSide = accumulate(min.begin(), min.end(), 0); 
        return d + w2*(mySide - oppSide) + w3*(_extra[0]- _extra[1]) + w4*_capStones[curr_pl]; 
    }

    bool IsOver(){
        ll ok1 = 1, ok2= 1; 
        for(ll i = 0 ; i < PITS; ++i) {
            if(max[i]!=0){
                ok1 = 0; 
                break; 
            }
        }
        for(ll i = 0 ; i < PITS; ++i){
            if(min[i] != 0) {
                ok2 = 0 ; 
                break; 
            }
        }
        return (ok1 || ok2); 
    }

    void finalize_game() {
        for(ll i = 0 ; i < PITS; ++i) max_score+= (max[i]); 
        for(ll i = 0; i < PITS; ++i) min_score += (min[i]);
    }

    ll Winner() {
        if(max_score > min_score) return 1; 
        else if(min_score == max_score) return 0; 
        else return 2; 
    }

    void printMax() {
        cout << "Max : " << max_score << "\n"; 
        for(ll i = 0 ; i < PITS; ++i) cout << max[i] << " "; 
        cout << endl; 
    }
    void printMin() {
        cout << "Min : " << min_score << "\n"; 
        for(ll i = 0 ; i < PITS; ++i) cout << min[i] << " "; 
        cout << endl; 
    }
}; 