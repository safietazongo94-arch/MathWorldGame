#ifndef INTEGRALQUESTION_H
#define INTEGRALQUESTION_H

#include "Question.h"
#include <vector>

using namespace std;

class IntegralQuestion : public Question {
public:
    using Question::Question;

    string getTopic() const override;
    static vector<IntegralQuestion> loadFromFile(const string& filename);
};

#endif
