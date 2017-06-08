//
// Created by Richard Remus on 02.06.17.
//

#ifndef NBODY_PSYS17_SIGNALHANDLER_H
#define NBODY_PSYS17_SIGNALHANDLER_H

#include <stdexcept>

using std::runtime_error;

class SignalException : public runtime_error
{
public:
   SignalException(const std::string& _message)
      : std::runtime_error(_message)
   {}
};

class SignalHandler
{
protected:
    static bool mbGotExitSignal;

public:
    SignalHandler();
    ~SignalHandler();

    static bool gotExitSignal();
    static void setExitSignal(bool _bExitSignal);

    void        setupSignalHandlers();
    static void exitSignalHandler(int _ignored);
};

#endif //NBODY_PSYS17_SIGNALHANDLER_H
