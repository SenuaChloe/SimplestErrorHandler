#include <iostream>
#include <sstream>
#include <exception>
#include <type_traits>

namespace ErrorHandler
{
    template<typename T, typename = void>
    struct is_streamable : std::false_type {};

    template<typename T>
    struct is_streamable<T, std::void_t<decltype(std::declval<std::ostringstream&>() << std::declval<T>())>> : std::true_type {};

    template<typename T>
    constexpr bool is_streamable_v = is_streamable<T>::value;

    template<typename TExceptionType, typename... TArgs>
    struct are_streamable : std::conjunction<is_streamable<TArgs>...> {};

    class BasicException : public std::exception
    {
    protected:
        std::string m_what;
    public:
        BasicException(const std::string & what) : m_what(what) {}
        BasicException(std::string && what) : m_what(std::move(what)) {}
        const char * what() const noexcept override { return m_what.c_str(); }
    };

    template<typename TExceptionType = BasicException, typename... TArgs>
    typename std::enable_if<are_streamable<TExceptionType, TArgs...>::value, void>::type
    raise_error(const TArgs&... args)
    {
        std::ostringstream oss;
        (oss << ... << args);
        const std::string error_str = oss.str();
        std::cerr << error_str << std::endl;
        throw TExceptionType(error_str);
    }

    template<typename TExceptionType = BasicException, typename... TArgs>
    typename std::enable_if<are_streamable<TExceptionType, TArgs...>::value, void>::type
    assert(bool predicate, const TArgs&... args)
    {
        if (!predicate)
            raise_error<TExceptionType>(args...);
    }
}

int main() {
    try {
        ErrorHandler::assert<ErrorHandler::BasicException>(false, "Error: ", "Something went wrong! ", 42);
    }
    catch (const std::exception &e) {
        std::cerr << "Caught exception: " << e.what() << std::endl;
    }
    return 0;
}
