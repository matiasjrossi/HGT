#include "logger.h"
#include <iostream>
#include <cstdlib>

Logger *Logger::logger = NULL;

Logger::Logger(QString filename, Level level) :
        file(new std::ofstream(filename.toStdString().c_str())),
        level(level),
        debugCounter(0),
        warningCounter(0),
        errorCounter(0)
{
}

Logger *Logger::get()
{
    if (logger == NULL)
        throw new UninitializedLoggerException();
    return logger;
}

void Logger::init(QString filename, Logger::Level level)
{
    logger = new Logger(filename, level);
}

QString Logger::toFile(QString text)
{
    (*file) << text.toStdString() << std::endl;
    return text;
}

QString Logger::toStdout(QString text)
{
    std::cout << text.toStdString() << std::endl;
    return text;
}

QString Logger::toStderr(QString text)
{
    std::cerr << text.toStdString() << std::endl;
    return text;
}

void Logger::debug(QString text)
{
    QString out = QString("DEBUG #%1: ").arg(++debugCounter).append(text);
    toFile(out);
    if (level >= DEBUG)
        toStderr(out);
}

void Logger::warning(QString text)
{
    QString out =QString("WARNING #%1: ").arg(++warningCounter).append(text);
    toFile(out);
    if (level >= WARNING)
        toStderr(out);
}

void Logger::error(QString text)
{
    QString out = QString("ERROR #%1: ").arg(++errorCounter).append(text);
    toFile(out);
    if (level >= ERROR)
        toStderr(out);
}

void Logger::fatal(QString text)
{
    QString out = QString("FATAL: ").append(text);
    toFile(out);
    if (level >= FATAL)
        toStderr(out);
}

void Logger::output(QString text)
{
    toFile(text);
    if (level >= OUTPUT)
        toStdout(text);
}
