#include<bits/stdc++.h>
using namespace std;

vector<int> computeLPSArray(const string& pattern) {
    int m = pattern.length();
    vector<int> lps(m, 0);
    int len = 0;
    int i = 1;
    while (i < m) {
        if (pattern[i] == pattern[len]) {
            len++;
            lps[i] = len;
            i++;
        } else {
            if (len != 0) len = lps[len - 1];
            else i++;
        }
    }
    return lps;
}

vector<int> kmpSearch(const string& text, const string& pattern) {
    vector<int> result;
    vector<int> lps = computeLPSArray(pattern);
    int n = text.length();
    int m = pattern.length();
    int i = 0;
    int j = 0;
    while (i < n) {
        if (pattern[j] == text[i]) {
            i++;
            j++;
        }
        if (j == m) {
            result.push_back(i - j);
            j = lps[j - 1];
        } else if (i < n && pattern[j] != text[i]) {
            if (j != 0) j = lps[j - 1];
            else i++;
        }
    }
    return result;
}
int main() {
    string text;
    string pattern;
    cin >> text >> pattern; 
    
    vector<int> ans =kmpSearch(text, pattern);
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
