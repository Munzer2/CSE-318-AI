#include<bits/stdc++.h>
#include<dirent.h>
#include "TSP.h"
#define ll long long
#define all(x) x.begin(), x.end()
#define pb(x) push_back(x)
#define fs first
#define sc second

using namespace std;

typedef pair<double, vector<long long>> (TSP::*Construct)();
typedef double (TSP::*Perturbation)(vector< ll >); 


void print_vect(vector< ll > tour) {
    for(auto x : tour) cout << x << " "; 
    cout << endl; 
    return; 
}


ll checkTSP(string &filename) {
    ll ind = -1; 
    for(ll i = 0 ; i < filename.size(); ++i) {
        if(filename[i] == '.') {
            ind = i;
        }
    }
    if(ind == -1) return 0;
    return filename.substr(ind+1) == "tsp"; 
}

int main() {
    // cout << "hello munzer\n"; 
    // string path = "TSP_assignment_task_benchmark_data\\a280.tsp"; 
    // TSP tsp(path); 
    // pair< double , vector<ll >> p1 = tsp.semi_greedy_Nearest_neighbors();  
    // // double _final = tsp.twoOpt(p1.sc);
    // cout << p1.fs <<"\n"; 

    string folder_path = "TSP_assignment_task_benchmark_data";
    vector< string > tsp_files;
    DIR* dir = opendir(folder_path.c_str());
    if (!dir) {
        cerr << "Error: Unable to open directory " << folder_path << endl;
        return 1;
    }

    struct dirent* entry;
    while ((entry = readdir(dir)) != nullptr) {
        string file_name = entry->d_name;

        if (file_name == "." || file_name == "..") continue;

        if (checkTSP(file_name)) {
            string file_path = folder_path + "/" + file_name;
            tsp_files.push_back(file_path);
        }
    }
    closedir(dir);
    vector< Construct > Constructs = {&TSP::Nearest_neighbors, &TSP::Nearest_insertion, &TSP::cheapest_insertion, &TSP::semi_greedy_Nearest_neighbors};
    vector< string > ConstructNames = {"Nearest Neighbors", "Nearest Insertion", "Cheapest Insertion", "Semi greedy nearest neighbors"};

    vector< Perturbation > perturbs = {&TSP::twoOpt,&TSP::nodeswap,&TSP::nodeshift};
    vector< string > perturbNames = {"twoOpt","nodeswap","nodeshift"};

    
    map< string, ll > mp;

    for(auto path : tsp_files) {
        TSP tsp(path);
        cout << "Experimenting on " << path.substr(35) << "\n";
        double _min = 1e10, best_i = -1, best_j = -1;
        for(ll i = 0; i < ConstructNames.size(); ++i) {
            pair< double, vector< ll > > con = (tsp.*Constructs[i])();
            for(ll j = 0 ; j < perturbNames.size(); ++j) {
                double ans = (tsp.*perturbs[j])(con.sc);
                cout << ConstructNames[i] << " and " << perturbNames[j] << ". Tour length: " << ans << "\n";
                if(_min > ans) {
                    _min = ans; 
                    best_i = i, best_j = j;
                }
            }
        }
        cout << "\n"; 
        cout << ConstructNames[best_i] << " and " << perturbNames[best_j] << " wins\n";
        mp[perturbNames[best_j]]++;
        cout << "-----------------------------------------------------------------------------------------------\n\n";
    }

    
    return 0;
}