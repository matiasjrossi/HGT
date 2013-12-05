#ifndef LOGGER_H
#define LOGGER_H

#include <QString>
#include <fstream>

class UninitializedLoggerException {};

class Logger
{
public:
    typedef enum { QUIET=100, OUTPUT, FATAL, ERROR, WARNING, DEBUG } Level;
    static Logger *get();
    static void init(QString filename, Level level = FATAL);
    void debug(QString text);
    void warning(QString text);
    void error(QString text);
    void fatal(QString text);
    void output(QString text);
private:
    Logger(QString filename, Level level);
    static Logger *logger;
    std::ofstream *file;
    int level;
    QString toFile(QString text);
    QString toStdout(QString text);
    QString toStderr(QString text);
    unsigned long long debugCounter, warningCounter, errorCounter;
};

#endif // LOGGER_H
