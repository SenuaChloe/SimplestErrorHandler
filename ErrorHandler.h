#pragma once

#include <iostream>
#include <sstream>

namespace ErrorHandler
{   
    template<typename TExceptionType, typename ...TArgs>
    concept TemplatedTypesConstraints = requires(std::string s, std::ostringstream oss, TArgs... args)
    {
        TExceptionType(s); // TExceptionType must be constructible using a std::string
        (oss << ... << args); // All args must be streamable
    };

    class BasicException : public std::exception
    {
    protected:
        std::string m_what;
    public:
        BasicException(const std::string & what): m_what(what) {}
        BasicException(std::string && what): m_what(std::forward<std::string>(what)) {}
        const char * what() const noexcept override { return m_what.c_str(); };
    };

    template<typename TExceptionType = BasicException, typename ...TArgs>
    requires TemplatedTypesConstraints<TExceptionType, TArgs...>
    void raise_error(const TArgs & ...args)
    {
        std::ostringstream oss;
        (oss << ... << args);
        const std::string error_str = oss.str();
        std::cerr << error_str << std::endl;
        throw TExceptionType(error_str);
    }

    template<typename TExceptionType = BasicException, typename ...TArgs>
    requires TemplatedTypesConstraints<TExceptionType, TArgs...>
    void assert(bool predicate, const TArgs & ...args)
    {
        if (!predicate)
            raise_error<TExceptionType>(args...);
    }
};
