#ifndef VERSION_PROCESSOR_H
#define VERSION_PROCESSOR_H

#include "AimlProcessor.h"

#include <string>

#define VERSION_STRING "Reeve AIML Interpreter v0.5 (built: __DATE__)"

using namespace std;

class VersionProcessor : public AimlProcessor
{
public:
    ~VersionProcessor() { }

    string process(Match *, PElement, Responder *, const string &)
    {
        return VERSION_STRING;
    }
};

#endif
