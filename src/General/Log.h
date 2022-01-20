#pragma once

namespace Log {
enum class MessageType {
    Info,
    Warning,
    Error,
    Debug,
    Console, // Only displayed in the console
    Script,  // Script output
    Any
};

/*
Black:  \u001b[30m.
Red:    \u001b[31m.
Green:  \u001b[32m.
Yellow: \u001b[33m.
Blue:   \u001b[34m.
Magenta:\u001b[35m.
Cyan:   \u001b[36m.
White:  \u001b[37m.
*/
static const int levelColors[] = {
    38,
    33,
    31,
    34,
    37,
    32,
    36
};


struct Message {
    string message;
    MessageType type;
    int level;
    wxDateTime timestamp;

    /* add additional debug info */
    string file;
    string func;
    int line;


    string formattedMessageLine(bool colorize = false) const;


    string getDebugInfo() const;


    string typeToString() const;
};


const vector<Message> &history();


int verbosity();


void setVerbosity(int verbosity);


void init();


void message(MessageType type, int level, const char *text, const char *_file = "", const char *_func = "", int _line = 0);


void message(MessageType type, int level, const wxString &text, const char *_file = "", const char *_func = "", int _line = 0);


void message(MessageType type, const char *text, const char *_file = "", const char *_func = "", int _line = 0);


void message(MessageType type, const wxString &text, const char *_file = "", const char *_func = "", int _line = 0);


vector<Message *> since(time_t time, MessageType type = MessageType::Any);


inline void info(int level, const char *text) { message(MessageType::Info, level, text); }


inline void info(int level, const wxString &text) { message(MessageType::Info, level, text); }


inline void info(const char *text) { message(MessageType::Info, text); }


inline void info(const wxString &text) { message(MessageType::Info, text); }


inline void warning(int level, const char *text) { message(MessageType::Warning, level, text); }


inline void warning(int level, const wxString &text) { message(MessageType::Warning, level, text); }


inline void warning(const char *text) { message(MessageType::Warning, text); }


inline void warning(const wxString &text) { message(MessageType::Warning, text); }


inline void error(int level, const char *text) { message(MessageType::Error, level, text); }


inline void error(int level, const wxString &text) { message(MessageType::Error, level, text); }


inline void error(const char *text) { message(MessageType::Error, text); }


inline void error(const wxString &text) { message(MessageType::Error, text); }


void debug(int level, const char *text);


void debug(int level, const wxString &text);


void debug(const char *text);


void debug(const wxString &text);


inline void console(const char *text) { message(MessageType::Console, text); }


inline void console(const wxString &text) { message(MessageType::Console, text); }
}

// Try to avoid using these and use Log::message/error/warning with S_FMT instead
#define LOG_MESSAGE(level, ...) Log::message(Log::MessageType::Info, level, wxString::Format(__VA_ARGS__), __FILE__, __FUNCTION__, __LINE__)
#define LOG_WARNING(level, ...) Log::message(Log::MessageType::Warning, level, wxString::Format(__VA_ARGS__), __FILE__, __FUNCTION__, __LINE__)
#define LOG_ERROR(level, ...) Log::message(Log::MessageType::Error, level, wxString::Format(__VA_ARGS__), __FILE__, __FUNCTION__, __LINE__)
// move LOG_DEBUG here?
