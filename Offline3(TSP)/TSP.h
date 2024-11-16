#include<bits/stdc++.h>
#define ll long long
#define all(x) x.begin(), x.end()
#define pb(x) push_back(x)

using namespace std; 

class TSP{
public:
    vector<pair< double, double >> points; 
    vector<vector< double >> dist; 
    ll sz;  
    TSP(string &filepath) {
        parsePath(filepath);
        sz = points.size(); 
        dist.resize(sz, vector< double >(sz, 0)); 
        computeDistance(); 
    }
    void parsePath(string &filepath) {
        ifstream in(filepath); 
        string line; 

        if(!in.is_open()) {
            cout << "The file doesn't exist.\n";
            return;  
        }

        while(getline(in, line)) {
            if(line == "NODE_COORD_SECTION") {
                break;
            }
        }

        while(getline(in,line)){
            if(line == "EOF") break;
            istringstream _in(line);
            int ind; 
            double x,y;
            _in >> ind >> x >> y ;
            points.push_back({x,y});  
        }
        return;
    }

    void computeDistance() {
        for(ll i = 0 ; i < sz; ++i) {
            for(ll j = 0 ; j < sz; ++j) {
                dist[i][j] = calcDist(i,j); 
            }
        }
        return;
    }

    double calcDist(ll i, ll j) {
        double x1 = points[i].first,x2 = points[j].first,y1 = points[i].second,y2 = points[j].second;   
        double dist = sqrt((x1-x2)*(x1-x2) + (y1-y2)*(y1-y2));
        return dist; 
    }

    pair< double, vector< ll >> Nearest_neighbors() {
        ll curr = 0;
        vector< ll > tour;
        vector< ll > vis(sz, 0);
        tour.pb(curr);  
        vis[curr] = 1; 
        ll next = -1;
        for(ll i = 1; i < sz; ++i) {
            double min_dist = 1e9;
            for(ll j = 0 ; j < sz; ++j) {
                if(!vis[j] && dist[curr][j] < min_dist) {
                    min_dist = dist[curr][j];
                    next = j; 
                }
            }
            vis[next] = 1; 
            curr = next;
            tour.pb(next);  
        }
        return {TourLen(tour), tour};
    }

    pair<double, vector< ll >> Nearest_insertion() {
        vector< ll > tour; 
        vector< ll > vis(sz, 0);
        vis[0] = 1; vis[1] = 1;
        tour.pb(0); tour.pb(1); 
        while(tour.size() < sz) {
            double _min = 1e9; 
            ll best = -1, best_pos = -1;
            for(ll i = 0; i < sz; ++i) {
                if(!vis[i]){
                    for(ll j = 0; j < tour.size(); ++j) {
                        double d = dist[i][tour[j]];
                        if(d < _min) {
                            _min = d; 
                            best = i;
                        }
                    }
                }
            } 
            double _len = 1e9;
            for(ll j = 0 ; j < tour.size()-1; ++j) {
                double _dist = dist[tour[j]][best] + dist[best][tour[j+1]] - dist[tour[j]][tour[j+1]]; 
                if( _dist < _len) {
                    best_pos = j+1; 
                    _len = _dist;
                }
            }
            tour.insert(tour.begin() + best_pos, best); 
            vis[best] = 1; 
        }
        return {TourLen(tour), tour}; 
    }

    pair< double, vector< ll >> cheapest_insertion() {
        vector< ll > tour; 
        tour.pb(0); tour.pb(1); 
        vector< ll > vis(sz,0); 
        vis[0] = vis[1] = 1;
        while(tour.size() < sz) {
            double actual_min = 1e9;
            ll best_pos = -1, best = -1; 
            for(ll i = 0 ; i < sz; ++i) {
                if(!vis[i]) {
                    double _min = 1e9;
                    ll _bestPos = -1;
                    for(ll j = 0 ; j < tour.size()-1; ++j) {
                        double increase = dist[tour[j]][i] + dist[i][tour[j+1]] - dist[tour[j]][tour[j+1]];
                        if(increase < _min) {
                            _min = increase; 
                            _bestPos = j+1;
                        }
                    }
                    if(_min < actual_min) {
                        actual_min = _min; 
                        best = i;
                        best_pos = _bestPos;
                    }
                }
            }
            tour.insert(tour.begin() + best_pos, best);
            vis[best] = 1;
        }
        return {TourLen(tour), tour}; 
    }

    pair< double, vector< ll >> farthest_insertion() {
        vector< ll > tour; 
        vector< ll > vis(sz, 0);
        vis[0] = 1; vis[1] = 1;
        tour.pb(0); tour.pb(1); 
        while(tour.size() < sz) {
            double _max = -1; 
            ll best = -1, best_pos = -1;
            for(ll i = 0; i < sz; ++i) {
                if(!vis[i]){
                    double _min = 1e9;
                    for(ll j = 0; j < tour.size(); ++j) {
                        double d = dist[i][tour[j]];
                        if(d < _min) {
                            _min = d;
                        }
                    }
                    if(_min > _max) {
                        best = i;
                        _max = _min;
                    }
                }
            } 
            double _len = 1e9;
            for(ll j = 0 ; j < tour.size()-1; ++j) {
                double _dist = dist[tour[j]][best] + dist[best][tour[j+1]] - dist[tour[j]][tour[j+1]]; 
                if( _dist < _len) {
                    best_pos = j+1; 
                    _len = _dist;
                }
            }
            tour.insert(tour.begin() + best_pos, best); 
            vis[best] = 1; 
        }
        return {TourLen(tour), tour};
    }

    double TourLen(vector< ll > tour) {
        double len = 0; 
        for(ll i = 1; i < sz; ++i) len += (dist[tour[i-1]][tour[i]]);
        if(tour[0] != tour.back())  len += dist[tour.back()][tour[0]];  
        return len; 
    }

    void reverseSegment(vector<ll> &tour, ll strt, ll end) {
        while(strt < end) {
            swap(tour[strt], tour[end]);
            strt++ ; 
            end--;
        }
        return; 
    }


    double twoOpt(vector< ll > init) {
        if(init.size() <= 2) return TourLen(init); 
        vector< ll > tour = init;
        tour.pb(tour[0]); //assuming I didnt add the first element to last. 
        ll n = tour.size(); 
        while(1) {
            ll check = 0; 
            for(ll i = 0; i < n-2; ++i) {
                for(ll j = i+2; j < n; ++j) {
                    if(!i && j == n-1) break;
                    double curr_dist = dist[tour[i]][tour[i+1]] + dist[tour[j]][tour[(j+1)%n]];
                    double new_dist = dist[tour[i]][tour[j]] + dist[tour[i+1]][tour[(j+1)%n]]; 
                    if(curr_dist > new_dist) {
                        reverseSegment(tour,i+1,j);
                        check = 1; 
                    }
                }
            }
            if(!check) break; 
        }
        return TourLen(tour); 
    }

    double nodeshift(vector< ll > tour) {
        ll n = tour.size();
        if(n <= 2) return TourLen(tour); 
        while(1) {
            ll check = 0;
            double curr_len = TourLen(tour);

            for(ll i = 0 ; i < n; ++i) {
                ll best_pos = -1; 
                double best_len = curr_len;
                ll node = tour[i];  
                tour.erase(tour.begin() + i);
                for(ll j = 0; j < n; ++j) {
                    tour.insert(tour.begin() + j, node); 
                    double _new = TourLen(tour);
                    if(best_len > _new) {
                        best_len = _new; 
                        best_pos = j; 
                    }
                    tour.erase(tour.begin() + j); 
                }
                if(best_pos == -1) {
                    tour.insert(tour.begin() + i, node);
                }
                else{
                    check = 1; 
                    tour.insert(tour.begin() + best_pos, node);
                    curr_len = best_len; 
                }
            }
            if(!check) break;
        }
        return TourLen(tour); 
    }

    double nodeswap(vector< ll > tour) {
        ll n = tour.size();
        if(n <= 2) return TourLen(tour); 
        while(1) {
            ll check = 0; 
            for(ll i = 0; i < n; ++i) {
                for(ll j = i+1; j < n; ++j) {
                    double curr = TourLen(tour); 
                    swap(tour[i], tour[j]); 
                    double _new = TourLen(tour);
                    if(curr > _new) {
                        check = 1; 
                    }
                    else swap(tour[i], tour[j]);
                }
            }
            if(!check) break;
        }
        return TourLen(tour);  
    }


    double threeOpt(vector<ll> tour) {
        ll n = tour.size();
        while (true) {
            ll check = 0; 
            vector<ll> _temp = tour; 
            double best_len = TourLen(_temp);

            for (ll i = 0; i < n - 2; ++i) {
                for (ll j = i + 1; j < n - 1; ++j) {
                    for (ll k = j + 1; k < n; ++k) {
                        for (ll opt = 0; opt < 4; ++opt) {
                            vector<ll> tempTour = _temp; 
                            if (opt == 0) {
                                continue; 
                            } 
                            else if (opt == 1) {
                                reverseSegment(tempTour, j + 1, k);
                            } 
                            else if (opt == 2) {
                                reverseSegment(tempTour, k + 1, n - 1); 
                            } 
                            // else if (opt == 3) {
                            //     reverseSegment(tempTour, j + 1, k); 
                            //     reverseSegment(tempTour, k + 1, n - 1); 
                            // }
                            // else if (opt == 3) {
                            //     reverseSegment(tempTour, i + 1, j);  
                            // }
                            else if (opt == 3) {
                                reverseSegment(tempTour, i + 1, j); 
                                reverseSegment(tempTour, j + 1, k);  
                            }
                            // else if (opt == 6) {
                            //     reverseSegment(tempTour, i + 1, j); 
                            //     reverseSegment(tempTour, j + 1, k);
                            //     reverseSegment(tempTour, k + 1, n-1);  
                            // }
                            double _new = TourLen(tempTour);

                            if (best_len > _new) {
                                check = 1; 
                                best_len = _new;
                                _temp = tempTour; 
                            }
                        }
                    }
                }
            }

            if (!check) break; 
            tour = _temp; 
        }

        return TourLen(tour); 
    }


    print(){
        // for(auto c : points) cout << c.first << " " << c.second << "\n";
        for(ll i = 0 ; i < sz; ++i) {
            for(ll j = 0 ; j < sz; ++j ) cout << dist[i][j] << " " ; 
            cout << "\n"; 
        } 
    }
};