#include<bits/stdc++.h>
#define ll long long
using namespace std;

vector< vector< ll >> grid; ///A global 2D vector to store initial state/ input. 
set< string > visited;
map< string, string> par;


string getStringOfGrid(vector< vector< ll >> grid) {
    string ans = "" ; 
    ll n = grid.size() ; 
    for(ll i = 0 ; i < n; ++i) {
        for(ll j = 0 ; j < n; ++j) {
            ans += to_string(grid[i][j]) ;
            if(i == n-1 && j == n-1 );
            else ans += "," ; 
        }
    }
    return ans;
}

void init(ll n) {
    grid.resize(n, vector< ll > (n));
    return; 
}

void clean(ll n) {
    for(ll i = 0 ; i < n; ++i) grid[i].clear(); 
    grid.clear();
    return; 
}

void print(vector< vector< ll >> grid) {
    ll n = grid.size(); 
    for(ll i = 0 ; i < n ; ++i) {
        for(ll j = 0 ;j  < n ; ++j ) cout << grid[i][j] << " " ;
        cout << endl ; 
    }
    cout << endl;
}

ll get_inv(ll n) {
    vector< ll > check;
    for(ll i = 0 ; i < n; ++i) {
        for(ll j = 0 ; j < n; ++j) {
            if(grid[i][j] == 0) continue;
            check.push_back(grid[i][j]); 
        }
    }
    ll inv = 0; 
    for(ll i = 0 ;i < check.size() -1; ++i) {
        for(ll j = i+1; j< check.size(); ++j) inv += (check[i]> check[j]);
    }
    return inv ; 
}

ll getBlankRow(ll n) { 
    for(ll i = n-1; i>= 0; --i) {
        for(ll j = 0 ; j < n; ++j) {
            if(grid[i][j] == 0) {
                return n-i; 
            }
        }
    }
    return -1;  
}

ll check() { 
    ll n = grid.size();
    ll inv_cnt = get_inv(n);
    if(n%2){
        return (inv_cnt%2 == 0);
    }
    ll row = getBlankRow(n);
    return (row%2 && inv_cnt%2 == 0) || (row%2 == 0 && inv_cnt%2);
}


ll isGoalState(vector< vector< ll >> grid){
    ll cnt = 1;
    ll ok = 1; 
    ll n = grid.size();
    if(grid[n-1][n-1] != 0) {
        return 0;
    }
    for(ll i = 0; i < n; ++i) {
        for(ll j = 0 ; j< n; ++j) {
            if(grid[i][j] == 0) {
                cnt++;
                continue; 
            }
            if(grid[i][j] == cnt || cnt == n*n) {
                cnt++;
            }
            else {
                ok = 0; break;
            }
        }
    }
    return ok;
}

ll manhattan(vector< vector< ll >> curr) {
    ll dist = 0; 
    ll n = curr.size(); 
    for(ll i = 0 ; i < n; ++i) {
        for(ll j = 0 ; j < n; ++j) {
            ll val = curr[i][j];
            ll row = (val-1)/n;
            ll col = (val-1)%n;
            dist += abs(row-i) + abs(col-j);
        }
    }
    return dist;
}

ll hamming(vector< vector< ll >> curr) {
    ll cnt = 1;
    ll tot = 0; 
    ll n = curr.size(); 
    for(ll i =0 ; i < n ; ++i ) {
        for(ll j = 0 ; j < n ; ++j) {
            if(curr[i][j] == 0 ) { cnt++ ; continue; } 
            tot += !(curr[i][j] == cnt);
            cnt++ ;
        }
    }
    return tot; 
}


vector< vector< vector< ll>>> genNeighbours(vector< vector< ll >> curr) {
    ll blank_x = -1, blank_y = -1; 
    ll n = curr.size();
    for(ll i = 0 ; i < n; ++i) {
        for(ll j= 0 ; j < n; ++j) {
            if(curr[i][j] == 0) {
                blank_x = i; 
                blank_y = j;
            }
        }
    }
    vector< vector< vector< ll >>> all_states; 
    vector< pair< ll, ll >> possible = {{-1,0},{1,0}, {0,-1}, {0,1}};
    for(auto x : possible) {
        ll place_x = blank_x + x.first, place_y = blank_y + x.second; 
        if(place_x < n && place_y < n && place_x >= 0 && place_y >= 0) {
            vector< vector< ll >> new_grid = curr;
            swap(new_grid[blank_x][blank_y], new_grid[place_x][place_y]);
            all_states.push_back(new_grid);
        } 
    }
    return all_states; 
}

void printGridForm(string s ) {
    ll n = grid.size();
    vector< vector< ll >> _grid;
    vector< ll > row;
    stringstream _s(s); 
    string val; 
    while(getline(_s,val,',')) {
        row.push_back(stoll(val));
        if(row.size() == n){
            _grid.push_back(row);
            row.clear(); 
        }
    }
    print(_grid);
    cout << "--------------------------\n";
}

void recur(string s) {
    vector< string > strs; 
    while(1) { 
        strs.push_back(s);
        s = par[s];
        if(s == "NONE") break;
    }
    for(ll i = strs.size()-1; i >= 0 ; --i) printGridForm(strs[i]);
    return;
}

ll A_star(ll choice){
    ll explored = 0, expanded = 0;
    set<pair< ll , pair< ll , vector< vector< ll >>>>> Nodes; 
    Nodes.insert({0,{0,grid}});
    string init = getStringOfGrid(grid);
    visited.insert(init);
    par[init] = "NONE";
    while(!Nodes.empty()){
        pair< ll, pair< ll, vector< vector< ll>>>> curr = *Nodes.begin();
        Nodes.erase(Nodes.begin());
        explored++;         
        string curr_str = getStringOfGrid(curr.second.second);
        if(isGoalState(curr.second.second)) {
            recur(curr_str);
            cout << "Total explored: " << explored << "\n"; 
            cout << "Total expanded: " << expanded << "\n"; 
            return curr.second.first; 
        }
        ll moves = curr.second.first;
        vector< vector< vector< ll >>> Neighs = genNeighbours(curr.second.second);
        for(auto x : Neighs) {
            string ch = getStringOfGrid(x); 
            if(visited.find(ch) != visited.end()) {
                continue;
            }
            expanded++;
            ll heu = (choice ? manhattan(x) : hamming(x)); 
            Nodes.insert({moves + heu + 1, {moves+1,x}});
            par[ch] = curr_str;
            visited.insert(ch);
        }
    }
    return -1;
}


int main(){
    ll n ; 
    cin >> n ;
    init(n);
    for(ll i = 0; i < n; ++i) {
        for(ll j =0 ; j < n; ++j) {
            string x ; 
            cin >> x;
            grid[i][j] = (x == "*" ? 0 : stoi(x));
        }
    }
    if(check()){
        cout << "The given grid is solvable.\n\n";
        for(ll i =  0; i < 2; ++i) {
            ll c = A_star(i);
            cout << "Optimal cost using " << (i ? "manhattan" : "hamming") << " is : " << c << "\n\n";
            visited.clear(); 
            par.clear();
        } 

    }
    else cout << "The given grid is not solvable\n";
    clean(n);
    return 0;
}

