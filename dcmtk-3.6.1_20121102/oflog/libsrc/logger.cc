// Module:  Log4CPLUS
// File:    logger.cxx
// Created: 6/2001
// Author:  Tad E. Smith
//
//
// Copyright 2001-2009 Tad E. Smith
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "dcmtk/oflog/logger.h"
#include "dcmtk/oflog/appender.h"
#include "dcmtk/oflog/hierarchy.h"
#include "dcmtk/oflog/helpers/loglog.h"
#include "dcmtk/oflog/spi/logimpl.h"

using namespace dcmtk::log4cplus::spi;

namespace dcmtk
{

namespace log4cplus
{


Logger
DefaultLoggerFactory::makeNewLoggerInstance (const tstring & name,
    Hierarchy& h)
{
    return Logger (new LoggerImpl (name, h));
}


//////////////////////////////////////////////////////////////////////////////
// static Logger Methods
//////////////////////////////////////////////////////////////////////////////
//
Hierarchy &
Logger::getDefaultHierarchy ()
{
    static Hierarchy defaultHierarchy;

    return defaultHierarchy;
}


bool
Logger::exists (const tstring & name)
{
    return getDefaultHierarchy().exists(name);
}


LoggerList
Logger::getCurrentLoggers ()
{
    return getDefaultHierarchy ().getCurrentLoggers ();
}


Logger
Logger::getInstance (const tstring& name)
{
    return getDefaultHierarchy().getInstance(name);
}


Logger
Logger::getInstance (const tstring& name,
    LoggerFactory& factory)
{
    return getDefaultHierarchy().getInstance(name, factory);
}


Logger
Logger::getRoot ()
{
    return getDefaultHierarchy ().getRoot ();
}


void
Logger::shutdown ()
{
    getDefaultHierarchy ().shutdown ();
}



//////////////////////////////////////////////////////////////////////////////
// Logger ctors and dtor
//////////////////////////////////////////////////////////////////////////////

Logger::Logger ()
    : value (0)
{ }


Logger::Logger (LoggerImpl * ptr)
    : value (ptr)
{
    if (value)
        value->addReference ();
}


Logger::Logger (const Logger& rhs)
    : AppenderAttachable (rhs)
    , value (rhs.value)
{
    if (value)
        value->addReference ();
}


Logger &
Logger::operator = (const Logger& rhs)
{
    Logger (rhs).swap (*this);
    return *this;
}


Logger::~Logger ()
{
    if (value)
        value->removeReference ();
}


//////////////////////////////////////////////////////////////////////////////
// Logger Methods
//////////////////////////////////////////////////////////////////////////////

void
Logger::swap (Logger & other)
{
    LoggerImpl* tmp = value;
    value = other.value;
    other.value = tmp;
    //STD_NAMESPACE swap (value, other.value);
}


Logger
Logger::getParent () const
{
    if (value->parent)
        return Logger (value->parent.get ());
    else
    {
        value->getLogLog().error(DCMTK_LOG4CPLUS_TEXT("********* This logger has no parent: " + getName()));
        return *this;
    }
}


void
Logger::addAppender (SharedAppenderPtr newAppender)
{
    value->addAppender(newAppender);
}


SharedAppenderPtrList
Logger::getAllAppenders ()
{
    return value->getAllAppenders();
}


SharedAppenderPtr
Logger::getAppender (const tstring& name)
{
    return value->getAppender (name);
}


void
Logger::removeAllAppenders ()
{
    value->removeAllAppenders ();
}


void
Logger::removeAppender (SharedAppenderPtr appender)
{
    value->removeAppender(appender);
}


void
Logger::removeAppender (const tstring& name)
{
    value->removeAppender (name);
}


void
Logger::assertion (bool assertionVal, const tstring& msg) const
{
    if (! assertionVal)
        log (FATAL_LOG_LEVEL, msg);
}


void
Logger::closeNestedAppenders () const
{
    value->closeNestedAppenders ();
}


bool
Logger::isEnabledFor (LogLevel ll) const
{
    return value->isEnabledFor (ll);
}


void
Logger::log (LogLevel ll, const tstring& message, const char* file,
    int line, const char* function) const
{
    value->log (ll, message, file, line, function);
}


void
Logger::forcedLog (LogLevel ll, const tstring& message,
    const char* file, int line, const char* function) const
{
    value->forcedLog (ll, message, file, line, function);
}


void
Logger::callAppenders (const InternalLoggingEvent& event) const
{
    value->callAppenders (event);
}


LogLevel
Logger::getChainedLogLevel () const
{
    return value->getChainedLogLevel ();
}


LogLevel
Logger::getLogLevel() const
{
    return value->getLogLevel ();
}


void
Logger::setLogLevel (LogLevel ll)
{
    value->setLogLevel (ll);
}


Hierarchy &
Logger::getHierarchy () const
{
    return value->getHierarchy ();
}


tstring
Logger::getName () const
{
    return value->getName ();
}


bool
Logger::getAdditivity () const
{
    return value->getAdditivity ();
}


void
Logger::setAdditivity (bool additive)
{
    value->setAdditivity (additive);
}


} // namespace log4cplus

} // namespace dcmtk
