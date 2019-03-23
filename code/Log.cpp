#include "Log.hpp"

using namespace Log;

Log::Loger::Loger():path(def_path)
{
    stream = new std::ofstream();
    this->open(path);
}

void Loger::checknull()
{
    if (stream == nullptr)
        throw -1;
}
void Loger::open(std::string path)
{
    checknull();
    if (this->path != path )
        this->path = path;
    if (stream->is_open())
        stream->close();
    stream->open(path);
    if (!stream->is_open())
        throw -3;

}
void Loger::close()
{
    checknull();
    if (stream->is_open())
        stream->close();
}
Loger::Loger(std::string path) :path(path)
{
    if (stream == nullptr)
        stream  = new std::ofstream();
    this->open(path);
}
/*Loger::Loger(std::string path, Log::PrintType mod): type(mod), path(path)
{
    this->open(path);
}*/

void Loger::err(std::string &message)
{
    checknull();
    *stream << "error: " << message;
}
void Loger::info(std::string &message)
{
    checknull();
    *stream << message;
}
void Loger::warn(std::string &message)
{
    checknull();
    *stream << "warning: " <<message;
}
void Loger::endl()
{
    checknull();
    *stream << std::endl;
}
Loger& Loger::operator<<(const std::string &mess1)
{
    checknull();
    *stream << mess1;
    return *this;
}

Loger& Loger::operator<<(const Log::LogMod &mod)
{
    switch (mod.get_mode())
    {
        case INFO:
            info(mess);
            break;
        case WARN:
            warn(mess);
            break;
        case ERR:
            err(mess);
            break;
        case ENDL:
            endl();
            break;
        default:
            throw -1;
    }
    return *this;
}

Log::Loger::~Loger()
{
    
    this->close();
    checknull();
    delete stream;
}
