
// ----------------------------------------------------------------------------
// SLADE - It's a Doom Editor
// Copyright(C) 2008 - 2017 Simon Judd
//
// Email:       sirjuddington@gmail.com
// Web:         http://slade.mancubus.net
// Filename:    Log.cpp
// Description: The SLADE logging implementation
//
// This program is free software; you can redistribute it and/or modify it
// under the terms of the GNU General Public License as published by the Free
// Software Foundation; either version 2 of the License, or (at your option)
// any later version.
//
// This program is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or 
// FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
// more details.
//
// You should have received a copy of the GNU General Public License along with
// this program; if not, write to the Free Software Foundation, Inc.,
// 51 Franklin Street, Fifth Floor, Boston, MA  02110 - 1301, USA.
// ----------------------------------------------------------------------------


// ----------------------------------------------------------------------------
//
// Includes
//
// ----------------------------------------------------------------------------
#include "Main.h"
#include "App.h"
#include <fstream>


// ----------------------------------------------------------------------------
//
// Variables
//
// ----------------------------------------------------------------------------
namespace Log {
vector<Message> log;
std::ofstream log_file;
}

CVAR(Int, log_verbosity, 1, CVAR_SAVE)      // log-level
CVAR(Bool, log_console, false, CVAR_SAVE);   // print to console


// ----------------------------------------------------------------------------
//
// FreeImage Error Handler
//
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// FreeImageErrorHandler
//
// Allows us to catch FreeImage errors and log them
// ----------------------------------------------------------------------------
void FreeImageErrorHandler(FREE_IMAGE_FORMAT fif, const char *message) {
    string error = "FreeImage: ";
    if (fif != FIF_UNKNOWN)
        error += S_FMT("[%s] ", FreeImage_GetFormatFromFIF(fif));
    error += message;

    Log::error(error);
}


// ----------------------------------------------------------------------------
//
// Log::Message Struct Functions
//
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// Log::Message::formattedMessageLine
//
// Returns the log entry as a formatted string:
// [HH:MM:SS:mmm] <message>
// ----------------------------------------------------------------------------
string Log::Message::formattedMessageLine() const {
    return S_FMT("%s%s%s", wxDateTime(timestamp).Format("[%H:%M:%S:%l] "), getDebugInfo(), CHR(message));
}


// ----------------------------------------------------------------------------
// Log::Message::getDebugInfo
//
// Returns the log entry as a formatted string:

// ----------------------------------------------------------------------------
string Log::Message::getDebugInfo() const {
    auto fname = wxString(file).AfterLast('/');
    auto fmt = wxString::Format("<%s->%s(%d)> ", fname, func, line);

    if (fname.size() <= 1) return "";

    return fmt;
}

// ----------------------------------------------------------------------------
//
// Log Namespace Functions
//
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// Log::init
//
// Initialises the log file and logging stuff
// ----------------------------------------------------------------------------
void Log::init() {
    // Redirect sf::err output to the log file
    log_file.open(CHR(App::path("slade3.log", App::Dir::User)));
    sf::err().rdbuf(log_file.rdbuf());

    // Write logfile header
    string year = wxNow().Right(4);
    info("SLADE - It's a Doom Editor");
    info(S_FMT("Version %s", App::version().toString()));
    if (Global::sc_rev != "")
        info(S_FMT("Git Revision %s", Global::sc_rev));
    if (App::platform() == App::Platform::Windows)
        info(S_FMT("%s Windows Build", App::isWin64Build() ? "64bit" : "32bit"));
    info(S_FMT("Written by Simon Judd, 2008-%s", year));
    info(S_FMT("Apple M1 Version by Patrick Lindenberg, 2021-%s", year));
#ifdef SFML_VERSION_MAJOR
    info(
        S_FMT(
            "Compiled with wxWidgets %i.%i.%i and SFML %i.%i.%i",
            wxMAJOR_VERSION,
            wxMINOR_VERSION,
            wxRELEASE_NUMBER,
            SFML_VERSION_MAJOR,
            SFML_VERSION_MINOR,
            SFML_VERSION_PATCH
        ));
#else
    info(S_FMT(
        "Compiled with wxWidgets %i.%i.%i",
        wxMAJOR_VERSION,
        wxMINOR_VERSION,
        wxRELEASE_NUMBER
    ));
#endif
    info("--------------------------------");

    // Set up FreeImage to use our log:
    FreeImage_SetOutputMessage(FreeImageErrorHandler);
}


// ----------------------------------------------------------------------------
// Log::history
//
// Returns the log message history
// ----------------------------------------------------------------------------
const vector<Log::Message> &Log::history() {
    return log;
}


// ----------------------------------------------------------------------------
// Log::verbosity
//
// Returns the current log verbosity level, log messages with a
// higher level than the current verbosity will not be logged
// ----------------------------------------------------------------------------
int Log::verbosity() {
    return log_verbosity;
}


// ----------------------------------------------------------------------------
// Log::setVerbosity
//
// Sets the log verbosity level to [verbosity]
// ----------------------------------------------------------------------------
void Log::setVerbosity(int verbosity) {
    log_verbosity = verbosity;
}


// ----------------------------------------------------------------------------
// Log::message
//
// Logs a message [text] of [type]
// ----------------------------------------------------------------------------
void Log::message(MessageType type, const char *text, const char *_file, const char *_func, int _line) {
    // Add log message
    log.push_back({ text, type, wxDateTime::UNow(), _file, _func, _line });

    // Write to log file
    if (log_file.is_open() && type != MessageType::Console)
        sf::err() << log.back().formattedMessageLine() << "\n";

    if (log_console)
        std::cout << log.back().formattedMessageLine() << "\n";
}


void Log::message(MessageType type, const wxString &text, const char *_file, const char *_func, int _line) {
    message(type, CHR(text), _file, _func, _line);
}


// ----------------------------------------------------------------------------
// Log::since
//
// Returns a list of log messages of [type] that have been recorded since
// [time]
// ----------------------------------------------------------------------------
vector<Log::Message *> Log::since(time_t time, MessageType type) {
    vector<Message *> list;
    for (auto &msg : log)
        if (msg.timestamp >= time && (type == MessageType::Any || msg.type == type))
            list.push_back(&msg);
    return list;
}


// ----------------------------------------------------------------------------
// Log::debug
//
// Logs a debug message [text] at verbosity [level] only if debug mode is on
// ----------------------------------------------------------------------------
void Log::debug(int level, const char *text) {
    if (Global::debug)
        message(MessageType::Debug, level, text);
}


void Log::debug(int level, const wxString &text) {
    debug(level, CHR(text));
}


// ----------------------------------------------------------------------------
// Log::debug
//
// Logs a debug message [text] only if debug mode is on
// ----------------------------------------------------------------------------
void Log::debug(const char *text) {
    if (Global::debug)
        message(MessageType::Debug, text);
}


void Log::debug(const wxString &text) {
    debug(CHR(text));
}


// ----------------------------------------------------------------------------
// Log::message
//
// Logs a message [text] of [type] at verbosity [level]
// ----------------------------------------------------------------------------
void Log::message(MessageType type, int level, const char *text, const char *_file, const char *_func, int _line) {
    if (level > log_verbosity)
        return;

    // Add log message
    log.push_back({ text, type, wxDateTime::UNow(), _file, _func, _line });

    // Write to log file
    if (log_file.is_open() && type != MessageType::Console)
        sf::err() << log.back().formattedMessageLine() << "\n";

    if (log_console) {
        std::cout << log.back().formattedMessageLine() << "\n";
    }

}


void Log::message(MessageType type, int level, const wxString &text, const char *_file, const char *_func, int _line) {
    message(type, level, CHR(text), _file, _func, _line);
}
