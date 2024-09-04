#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

// Naive String Matching
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

// Knuth-Morris-Pratt (KMP) Algorithm
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

// Boyer-Moore Algorithm
vector<int> boyerMooreSearch(const string& text, const string& pattern) {
    vector<int> result;
    int n = text.length();
    int m = pattern.length();
    if (m == 0) return result;

    vector<int> charArr(256, -1);
    for (int i = 0; i < m; ++i) charArr[pattern[i]] = i;

    int s = 0;
    while (s <= n - m) {
        int j = m - 1;
        while (j >= 0 && pattern[j] == text[s + j]) j--;
        if (j < 0) {
            result.push_back(s);
            s += (s + m < n) ? m - charArr[text[s + m]] : 1;
        } else {
            s += max(1, j - charArr[text[s + j]]);
        }
    }
    return result;
}

// Rabin-Karp Algorithm
vector<int> rabinKarpSearch(const string& text, const string& pattern, int d = 256, int q = 101) {
    vector<int> result;
    int n = text.length();
    int m = pattern.length();
    if (m > n) return result;

    int h = 1;
    for (int i = 0; i < m - 1; ++i) h = (h * d) % q;

    int p = 0; // Hash value for pattern
    int t = 0; // Hash value for text
    for (int i = 0; i < m; ++i) {
        p = (d * p + pattern[i]) % q;
        t = (d * t + text[i]) % q;
    }

    for (int i = 0; i <= n - m; ++i) {
        if (p == t) {
            int j = 0;
            while (j < m && text[i + j] == pattern[j]) j++;
            if (j == m) result.push_back(i);
        }
        if (i < n - m) {
            t = (d * (t - text[i] * h) + text[i + m]) % q;
            if (t < 0) t += q;
        }
    }
    return result;
}

// Main function
int main(int argc, char* argv[]) {
    if (argc != 4) {
        cerr << "Usage: " << argv[0] << " <algorithm> <filename> <pattern>" << endl;
        return 1;
    }

    string algorithm = argv[1];
    string filename = argv[2];
    string pattern = argv[3];

    ifstream file(filename);
    if (!file) {
        cerr << "Error opening file." << endl;
        return 1;
    }
    string text((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
    file.close();

    vector<int> result;
    if (algorithm == "naive") {
        result = naiveSearch(text, pattern);
    } else if (algorithm == "kmp") {
        result = kmpSearch(text, pattern);
    } else if (algorithm == "boyer-moore") {
        result = boyerMooreSearch(text, pattern);
    } else if (algorithm == "rabin-karp") {
        result = rabinKarpSearch(text, pattern);
    } else {
        cout << "Unknown algorithm." << endl;
        return 1;
    }
    if(result.empty()==true){
        cout << "No results found" << "\n";
        return 0;
    }
    for (int pos : result) {
        cout << "Entered text '" << pattern << "' found at position " << pos << endl;
    }

    double patternSize = pattern.size();
    double textSize = text.size();
    // Total operations for each algorithm
    double totalOperationsOnNaive = textSize * patternSize;
    double totalOperationsOnBoyerMoore = textSize / patternSize; // Average case
    double totalOperationsOnKMP = textSize + patternSize;
    double totalOperationsOnRabin = textSize + patternSize; // Average case

    // Efficiency calculations
    double effNaiveOverKMP = (totalOperationsOnKMP - totalOperationsOnNaive) / totalOperationsOnNaive;
    double effNaiveOverBoyerMoore = (totalOperationsOnBoyerMoore - totalOperationsOnNaive) / totalOperationsOnNaive;
    double effNaiveOverRabin = (totalOperationsOnRabin - totalOperationsOnNaive) / totalOperationsOnNaive;
    
    double effKMPOverNaive = (totalOperationsOnNaive - totalOperationsOnKMP) / totalOperationsOnKMP;
    double effKMPOverBoyerMoore = (totalOperationsOnBoyerMoore - totalOperationsOnKMP) / totalOperationsOnKMP;
    double effKMPOverRabin = (totalOperationsOnRabin - totalOperationsOnKMP) / totalOperationsOnKMP;
    
    double effBoyerMooreOverNaive = (totalOperationsOnNaive - totalOperationsOnBoyerMoore) / totalOperationsOnBoyerMoore;
    double effBoyerMooreOverKMP = (totalOperationsOnKMP - totalOperationsOnBoyerMoore) / totalOperationsOnBoyerMoore;
    double effBoyerMooreOverRabin = (totalOperationsOnRabin - totalOperationsOnBoyerMoore) / totalOperationsOnBoyerMoore;
    
    double effRabinOverNaive = (totalOperationsOnNaive - totalOperationsOnRabin) / totalOperationsOnRabin;
    double effRabinOverKMP = (totalOperationsOnKMP - totalOperationsOnRabin) / totalOperationsOnRabin;
    double effRabinOverBoyerMoore = (totalOperationsOnBoyerMoore - totalOperationsOnRabin) / totalOperationsOnRabin;

    // Output the efficiency percentages
    cout << "\n";
    cout << "USING NAIVE APPROACH" << "\n";
    cout << "Efficiency using Naive over KMP: " << effNaiveOverKMP * 100 << "%" << endl;
    cout << "Efficiency using Naive over Boyer-Moore: " << effNaiveOverBoyerMoore * 100 << "%" << endl;
    cout << "Efficiency using Naive over Rabin-Karp: " << effNaiveOverRabin * 100 << "%" << endl;
    cout << "\n";
    cout << "USING KMP APPROACH" << "\n";
    cout << "Efficiency using KMP over Naive: " << effKMPOverNaive * 100 << "%" << endl;
    cout << "Efficiency using KMP over Boyer-Moore: " << effKMPOverBoyerMoore * 100 << "%" << endl;
    cout << "Efficiency using KMP over Rabin-Karp: " << effKMPOverRabin * 100 << "%" << endl;
    cout << "\n";
    cout << "USING BOYER-MOORE APPROACH" << "\n";
    cout << "Efficiency using Boyer-Moore over Naive: " << effBoyerMooreOverNaive * 100 << "%" << endl;
    cout << "Efficiency using Boyer-Moore over KMP: " << effBoyerMooreOverKMP * 100 << "%" << endl;
    cout << "Efficiency using Boyer-Moore over Rabin-Karp: " << effBoyerMooreOverRabin * 100 << "%" << endl;
    cout << "\n";
    cout << "USING RABIN-KARP APPROACH" << "\n";
    cout << "Efficiency using Rabin-Karp over Naive: " << effRabinOverNaive * 100 << "%" << endl;
    cout << "Efficiency using Rabin-Karp over KMP: " << effRabinOverKMP * 100 << "%" << endl;
    cout << "Efficiency using Rabin-Karp over Boyer-Moore: " << effRabinOverBoyerMoore * 100 << "%" << endl;

    return 0;
}
    // g++ -o text_search text_search.cpp 
    // ./text_search kmp input.txt "nisl" 