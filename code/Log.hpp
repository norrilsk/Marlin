#ifndef MARLIN_LOG_HPP
#define MARLIN_LOG_HPP
/*
 * Loger Class
 * usage:
 * Log::Logger log("path to log file");
 * log << "message"; -- you will see "message" in your file
 * log << Log::err << " message 1 " << Log::endl << "message 2" ; -- you will see  error: message 1
 *                                                                                 message 2
 *
 */
#include<string>
#include<iostream>
#include<fstream>
//Logger log("file");
//log << Log::err << "
namespace Log
{
    enum PrintType
    {
        ERR,
        INFO,
        WARN,
        ENDL
    };
    class LogMod
    {
       
        int mod;
    public:
        
        PrintType get_mode() const {return static_cast<PrintType>(mod);};
        explicit LogMod(int mod): mod(mod) {};
        ~LogMod() = default;
    };
    const LogMod endl(PrintType::ENDL);
    const LogMod err(PrintType::ERR);
    const LogMod info(PrintType::INFO);
    const LogMod warn(PrintType::WARN);
    const std::string def_path("../../MarlinLog.txt");
    class Loger
    {
      //  PrintType type = PrintType::INFO;
        std::string path ;
        std::string mess;
        std::ofstream* stream = nullptr;
        void checknull();
    public:
        void open(std::string path);
        void err(std::string& message);
        void warn(std::string& message);
        void info(std::string& message);
        void endl();
        void close();
        Loger& operator << (const std::string& mess);
        Loger& operator << (const LogMod& mod);

    

        explicit Loger(std::string path);
        Loger(std::string path, PrintType mod);
        Loger();
        ~Loger();
    };

}
#endif

