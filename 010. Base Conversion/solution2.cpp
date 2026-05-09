#include <bits/stdc++.h>

using namespace std;

bool zero(vector<long long> &v){
	for (auto i: v){
		if (i != 0) return false;
	}
	return true;
}

int main(){    

    long long k, t, a; 
    unsigned long long buf;
    cin >> k >> t;

    vector<long long> v;
    vector<unsigned long long> ans;

    while(cin >> a) v.push_back(a);    

    while(!zero(v)){
    	buf = 0;
    	for (auto &i: v){
    		buf = buf * k + i;
    		i = buf / t;
    		buf = buf % t;
    	}
    	ans.push_back(buf);
    }
    

    for(int i = (int)ans.size() - 1; i > -1; i--){
    	cout << ans[i] << " ";
    } 

    return 0;
}
