#ifndef DERIVATIVEQUESTION_H
#define DERIVATIVEQUESTION_H

#include "Question.h"
#include <vector>

using namespace std;

class DerivativeQuestion : public Question {
public:
    using Question::Question;

    string getTopic() const override;
    static vector<DerivativeQuestion> loadFromFile(const string& filename);
};

#endif
