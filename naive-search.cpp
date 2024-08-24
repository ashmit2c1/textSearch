#include <bits/stdc++.h>
using namespace std;

vector<int> naiveSearch(const string& text, const string& pattern) {
    vector<int> result;
    int n = text.length();
    int m = pattern.length();
    for (int i = 0; i <= n - m; ++i) {
        int j;
        for (j = 0; j < m; ++j) {
            if (text[i + j] != pattern[j]) break;
        }
        if (j == m) result.push_back(i);
    }
    return result;
}

int main() {
    string text;
    string pattern;
    cin >> text >> pattern; 
    
    vector<int> ans = naiveSearch(text, pattern);
    int n = ans.size();
    
    if (ans.empty()) {
        cout << "No results found" << "\n";
    } else {
        for (int i = 0; i < n; ++i) {
            cout << "Pattern '" << pattern << "' found at position " << ans[i] << "\n";
        }
    }
    
    return 0;
}
