#ifndef LIMITQUESTION_H
#define LIMITQUESTION_H

#include "Question.h"
#include <vector>

using namespace std;

class LimitQuestion : public Question {
public:
    using Question::Question;

    string getTopic() const override;
    static vector<LimitQuestion> loadFromFile(const string& filename);
};

#endif
