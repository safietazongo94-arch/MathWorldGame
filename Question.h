#ifndef QUESTION_H
#define QUESTION_H

#include <string>
#include <vector>

using namespace std;

// Utility helpers (used by loaders)
string trim(const string& s);

class Question {
protected:
    string prompt;
    vector<string> choices;   // size 4
    int correctIndex = -1;    // 0..3
    string explanation;

public:
    Question() = default;

    Question(const string& p,
             const vector<string>& c,
             int idx,
             const string& expl)
        : prompt(p), choices(c), correctIndex(idx), explanation(expl) {}

    virtual ~Question() = default;

    virtual string getTopic() const = 0;

    const string& getPrompt() const { return prompt; }
    const vector<string>& getChoices() const { return choices; }
    int getCorrectIndex() const { return correctIndex; }
    const string& getExplanation() const { return explanation; }
    void setCorrectIndex(int idx) { correctIndex = idx; }

    bool checkAnswer(int selectedIndex) const {
        return selectedIndex == correctIndex;
    }

};

#endif // QUESTION_H
