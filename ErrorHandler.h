#pragma once

#include <iostream>
#include <sstream>


class ErrorHandler
{
    template<typename TExceptionType, typename THead>
    static void raise_error_recursion(std::ostringstream & error_string_stream, const THead & arg_head)
    {
        error_string_stream << arg_head;
        const std::string current_error_str = error_string_stream.str();

        std::cerr << current_error_str << std::endl;
        throw TExceptionType(current_error_str);
    }

    template<typename TExceptionType, typename THead, typename ...TTail>
    static void raise_error_recursion(std::ostringstream & error_string_stream, const THead & arg_head, const TTail & ...arg_tail)
    {
        error_string_stream << arg_head;
        raise_error_recursion<TExceptionType>(error_string_stream, arg_tail...);
    }

public:

    class BasicException : std::exception
    {
    protected:
        std::string m_what;
    public:
        BasicException(const std::string & what): m_what(what) {}
        BasicException(std::string && what): m_what(std::forward<std::string>(what)) {}
        virtual const char * what() const noexcept override { return m_what.c_str(); };
    };

    template<typename TExceptionType = BasicException>
    static void raise_error()
    {
        std::ostringstream error_string_stream;
        raise_error_recursion<TExceptionType>(error_string_stream, "<Unknown error>");
    }

    template<typename TExceptionType = BasicException, typename ...TArgs>
    static void raise_error(const TArgs & ...args)
    {
        std::ostringstream error_string_stream;
        raise_error_recursion<TExceptionType>(error_string_stream, args...);
    }

    template<typename TExceptionType = BasicException, typename ...TArgs>
    static void assert(bool predicate, const TArgs & ...args)
    {
        if (!predicate)
            raise_error<TExceptionType>(args...);
    }
};
