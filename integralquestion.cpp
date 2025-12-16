#include "integralquestion.h"
#include <fstream>
#include <iostream>

using namespace std;

string IntegralQuestion::getTopic() const {
    return "Integral";
}

vector<IntegralQuestion> IntegralQuestion::loadFromFile(const string& filename) {
    vector<IntegralQuestion> bank;
    ifstream in(filename);

    if (!in) {
        cerr << "Cannot open integral problems file: " << filename << "\n";
        return bank;
    }

    string q, a, b, c, d, idxLine, e, blank;

    while (getline(in, q)) {
        q = trim(q);
        if (q.empty()) continue;

        if (!getline(in, a)) break;
        if (!getline(in, b)) break;
        if (!getline(in, c)) break;
        if (!getline(in, d)) break;
        if (!getline(in, idxLine)) break;
        if (!getline(in, e)) e = "";
        getline(in, blank);

        a = trim(a); b = trim(b); c = trim(c); d = trim(d);
        idxLine = trim(idxLine);
        e = trim(e);

        if (e == "-") e = "";

        int idx = stoi(idxLine);

        bank.emplace_back(q, vector<string>{a, b, c, d}, idx, e);
    }

    return bank;
}
