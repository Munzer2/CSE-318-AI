#include<bits/stdc++.h> 
#include "gameState.h"
#define NEG_INF -1e15
#define INF 1e15 

using namespace std; 
ll w[4];

pair< ll, ll > minimax(State curr,ll depth, ll alpha, ll beta, ll check_max,ll heu) {
    vector< ll > legal = curr.LegalMoves(); 
    if(depth == 0 || legal.size() == 0) {
        if(heu == 1) return {curr.heuristic1(), -1}; 
        else{
            if(heu == 2) return {curr.heuristic2(w[0],w[1]), -1}; 
            else if(heu == 3) return {curr.heuristic3(w[0], w[1], w[2]), -1};
            else return {curr.heuristic4(w[0], w[1], w[2], w[3]), -1};  
        }
    }
    if(!check_max) {
        pair< ll ,ll > ans; 
        ll mx = NEG_INF;
        for(auto move : legal) {
            State _new = curr;
            _new.ApplyMove(move);
            pair< ll , ll > check = minimax(_new, depth-1, alpha, beta, _new.curr_pl, heu);
            if(mx < check.first) {
                mx = check.first;
                ans = {mx, move};
            }
            alpha = max(alpha, mx);
            if(beta <= alpha ) break; 
        }
        return ans;
    }
    else{
        ll mn = INF; 
        pair< ll , ll > ans; 
        for(auto move : legal) {
            State _new = curr; 
            _new.ApplyMove(move);
            pair< ll, ll > check  = minimax(_new, depth-1, alpha, beta, _new.curr_pl,heu);
            if(mn > check.first) {
                mn = check.first; 
                ans = {mn,move};
            }
            beta = min(beta, mn);
            if(beta <= alpha ) break;
        }
        return ans;
    } 
}


pair< ll, ll > minimax2(State curr,ll depth, ll alpha, ll beta, ll check_max,ll heu) { /////This one corporates diffrent move according to heuristics.
    vector< ll > legal = curr.LegalMoves(); 

    vector< pair< ll, ll >> move_ordering;
    for(auto move: legal) {
        State temp = curr; 
        temp.ApplyMove(move);
        ll _h; 
        if(heu == 1) _h = temp.heuristic1(); 
        else if(heu == 2) _h = temp.heuristic2(w[0],w[1]); 
        else if(heu == 3) _h = temp.heuristic3(w[0],w[1],w[2]); 
        else _h = temp.heuristic4(w[0],w[1],w[2],w[3]); 
        move_ordering.push_back({_h,move}); 
    }

    if(!check_max) sort(move_ordering.rbegin(), move_ordering.rend()); 
    else sort(move_ordering.begin(), move_ordering.end());

    if(depth == 0 || legal.size() == 0) {
        if(heu == 1) return {curr.heuristic1(), -1}; 
        else{
            if(heu == 2) return {curr.heuristic2(w[0],w[1]), -1}; 
            else if(heu == 3) return {curr.heuristic3(w[0], w[1], w[2]), -1};
            else return {curr.heuristic4(w[0], w[1], w[2], w[3]), -1};  
        }
    }
    if(!check_max) {
        pair< ll ,ll > ans; 
        ll mx = NEG_INF;
        for(auto move : move_ordering) {
            State _new = curr;
            _new.ApplyMove(move.second);
            pair< ll , ll > check = minimax2(_new, depth-1, alpha, beta, _new.curr_pl, heu);
            if(mx < check.first) {
                mx = check.first;
                ans = {mx, move.second};
            }
            alpha = max(alpha, mx);
            if(beta <= alpha ) break; 
        }
        return ans;
    }
    else{
        ll mn = INF; 
        pair< ll , ll > ans; 
        for(auto move : move_ordering) {
            State _new = curr; 
            _new.ApplyMove(move.second);
            pair< ll, ll > check  = minimax2(_new, depth-1, alpha, beta, _new.curr_pl,heu);
            if(mn > check.first) {
                mn = check.first; 
                ans = {mn,move.second};
            }
            beta = min(beta, mn);
            if(beta <= alpha ) break;
        }
        return ans;
    } 
}



ll Play(ll depth, ll heu1, ll heu2, ll &first_game) {
    State init;
    ll curr = 0;
    ///game starts.
    ///randomize all weights.
    ll val = 0;
    for(ll i = 0 ; i < 4; ++i) w[i] = rand()%100 + 1; 
    if(first_game == 1) {
        cout << "Player-1 is using heuristic-" << heu1 << "\n"; 
        cout << "Player-2 is using heuristic-" << heu2 << "\n"; 
    }
    while(!init.IsOver()) {
        ll best; 
        pair< ll, ll > check = minimax(init, depth,NEG_INF, INF,curr,(curr == 0? heu1 : heu2));
        best = check.second;
        init.ApplyMove(best);
        if(first_game == 1) {
            cout << "Player " << curr + 1 << " choses move : " << best+1 << "\n";
            init.printMax(); 
            init.printMin();
        }
        curr = init.curr_pl;
    }
    if(first_game == 1) {
        first_game++ ;
        cout << "Player-" << init.Winner() << " wins!\n"; 
    } 
    return init.Winner();
}


void Simulation() {
    ll first_game =1;
    map< ll, ll > draws, wins, loss; 
    for(ll i = 0 ; i < 100 ; ++i) {
        ll rand_heu1 = rand()%4 + 1;
        ll rand_heu2; 
        while(1){
            rand_heu2 = rand()%4 + 1; 
            if(rand_heu2 !=  rand_heu1) break; 
        }
        ll depth = rand()%7 + 1; 
        // cout << "Depth: " << depth << "\n"; 
        ll ans = Play(depth, rand_heu1, rand_heu2,first_game);
        if(!ans) {
            draws[rand_heu1]++; draws[rand_heu2]++; 
        }
        else{
            if(ans == 1) { 
                wins[rand_heu1]++ ; 
                loss[rand_heu2]++ ; 
            }
            else{
                wins[rand_heu2]++ ; 
                loss[rand_heu1]++ ;
            }
        }
    }
    for(ll i = 1; i <= 4; ++i) {
        cout << "Heuristic-" << i << " won: " << wins[i] << " times\n";
        double d = (wins[i]*1.0)/((loss[i]!=0 ? loss[i] : 1)*1.0);
        cout << "win-loss ratio for heuristic-"<< i << ":  " << d << "\n";  
    }
    cout << endl; 
}

int main() { 
    srand(time(0)); 
    Simulation(); 
    return 0;
}