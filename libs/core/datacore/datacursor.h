#pragma once

// TODO
#include "abstractdata.h"

class AbstractCommand
{
public:
    AbstractCommand();
    ~AbstractCommand();

    virtual void applyOn(t_DataBlockP dataObj) = 0;
    virtual AbstractCommand* combineWith(AbstractCommand * cmd);
};

class CommandQueue
{
public:
    void addCommand(AbstractCommand* );
    void flush();

};

// class CommandCreate
// class CommandAdd
// class CommandSet
// class CommandView
// class CommandRemove
// class CommandDestroy
