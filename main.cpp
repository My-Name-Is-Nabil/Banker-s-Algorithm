#include <iostream>
#include <vector> 
#include <cmath>
using namespace std;

struct Answer {
    bool is_safe;
    vector<int> process_sequence;
};

bool less_than(vector<int>& need, vector<int>& work) {
    for (int i = 0; i < work.size(); i++) {
        if (need[i] > work[i])
            return  false;
    }
    return true;
}
Answer is_in_safe_state(vector<int> work, vector<vector<int>>& allocation, vector<vector<int>>& need) {
    Answer answer;
    int n = allocation.size();
    int m = allocation[0].size();
    vector<bool> finish(n, false);
    bool loop_again = false;
    for (int i = 0; i < n; i++) {
        if (!finish[i] && less_than(need[i], work)) {
            finish[i] = true;
            loop_again = true;
            answer.process_sequence.push_back(i);
            for (int j = 0; j < m; j++)
                work[j] += allocation[i][j];
        }
        if (i == n - 1) {
            if (loop_again) {
                i = -1;
                loop_again = false;
            }
            else {
                for (int j = 0; j < n; j++) {
                    if (!finish[j]) {
                        answer.is_safe = false;
                        return answer;
                    }
                }
            }
        }
    }
    answer.is_safe = true;
    return answer;
}

Answer grant_request(vector<int> available, vector<vector<int>> allocation, vector<vector<int>> need, vector<int>& request, int& p){
    if (!less_than(request,available)){
        Answer answer;
        answer.is_safe = false;
        return answer;
    }
    int m = available.size();
    for(int i=0;i<m;i++)
        available[i] -= request[i];
    for(int i=0;i<m;i++){
        allocation[p][i] += request[i];
        need[p][i] -= request[i];
    }
    return is_in_safe_state(available, allocation, need);
}

int main() {
    while (true) {
        int m, n;
        cout << "Please Enter the number of resources followed by the number of process\n";
        cin >> m >> n;
        vector<vector<int>> allocation(n, vector<int>(m, 0)), max(n, vector<int>(m, 0));
        cout << "Enter the allocation matrix\n";
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++)
                cin >> allocation[i][j];
        }
        cout << "Enter the max matrix\n";
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++)
                cin >> max[i][j];
        }
        vector<int> available(m, 0);
        cout << "Enter the available matrix\n";
        for (int i = 0; i < m; i++)
            cin >> available[i];
        vector<vector<int>> need(n, vector<int>(m, 0));
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++)
                need[i][j] = max[i][j] - allocation[i][j];
        }
        cout<<"Need matrix:\n";
        cout<<"   ";
        for(int i = 0; i < m; i++)
            cout<<"R"<<i<<"  ";
        cout<<'\n';
        for (int i = 0; i < n; i++) {
            cout<<"P"<<i<<" ";
            for (int j = 0; j < m; j++)
                cout<<need[i][j]<<"   ";
            cout<<'\n';
        }
        cout<<"If you want to check the safety of the state enter s/S, if you want to check the possibility of a request enter r/R and if you want to exit enter e/E\n";
        char c;
        cin>>c;
        if ( c=='s' || c=='S' ){
            Answer answer = is_in_safe_state(available, allocation, need);
            if (answer.is_safe) {
                cout<<"Yes, Safe state <";
                for (int i = 0; i < n; i++)
                    cout << 'P' << answer.process_sequence[i] << (i==n-1 ? "" : ",");
                cout << ">\n";
            }
            else{
                cout<<"Not Safe\n";
            }
        }
        else if ( c=='r' || c=='R'){
            int p;
            cout<<"Which process is making the request?\n";
            cin>>p;
            vector<int> request(m,0);
            cout<<"Enter the request matrix\n";
            for(int i=0;i<m;i++)
                cin>>request[i];
            Answer answer = grant_request(available, allocation, need, request, p);
            if (answer.is_safe) {
                cout<<"Yes, request can be granted with safe state, Safe state <";
                cout<<"P"<<p<<"req,";
                for (int i = 0; i < n; i++){
                        cout << 'P' << answer.process_sequence[i] << (i==n-1 ? "" : ",");
                }
                cout << ">\n";
            }
            else{
                cout<<"Not safe to make this request\n";
            }
        }
        else if ( c=='e' || c=='E')
            break;
        else
            cout<<"Incorrect Input!\n";
    }
    system("pause");
    return 0;
}