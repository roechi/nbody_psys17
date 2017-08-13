//
// Created by Richard Remus on 02.06.17.
//

#include <signal.h>

#include "SignalHandler.h"

bool SignalHandler::mbGotExitSignal = false;

SignalHandler::SignalHandler() {}

SignalHandler::~SignalHandler() {}

bool SignalHandler::gotExitSignal()
{
    return mbGotExitSignal;
}

void SignalHandler::setExitSignal(bool _bExitSignal)
{
    mbGotExitSignal = _bExitSignal;
}

void SignalHandler::exitSignalHandler(int _ignored)
{
    mbGotExitSignal = true;
}

/**
* Set up the signal handlers for CTRL-C.
*/
void SignalHandler::setupSignalHandlers()
{
    if (signal((int) SIGINT, SignalHandler::exitSignalHandler) == SIG_ERR)
    {
        throw SignalException("!!!!! Error setting up signal handlers !!!!!");
    }
}