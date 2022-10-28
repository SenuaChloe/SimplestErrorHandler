# SimplestErrorHandler
A very small piece of code that can generically handle error.
It holds two functions and one class inside its public namespace.

# Public functions
Every function is templated on (at least) a type that must be constructible using a `std::string`.
This is supposed to be an exception class. It is always thrown at the end with the generated message.
This template has a default value, which is a home-made exception called `ErrorHandler::BasicException` (see bellow).

Here are the two functions that are part of the public interface :

    template<typename TExceptionType = BasicException, typename ...TArgs>
    static void raise_error(const TArgs & ...args);

    template<typename TExceptionType = BasicException, typename ...TArgs>
    static void assert(bool predicate, const TArgs & ...args);

The `raise_error(const TArgs & ...args)` function concatenates every given arguments using the `<<` operator and uses that as the error message. A `TExceptionType` is constructed using this message then thrown. This is most usefull function.

The `assert(bool predicate, const TArgs & ...args)` checks if the predicate is true or false. If it is false, then it calls `raise_error` with the remaining arguments. Else it does nothing. You can use this to make one-liners instead of `if (/*...*/) { raise_error(/*...*/); }`. Beware of the syntax: `assert` does NOTHING is the predicate is TRUE.

In every case, the error message that is thrown with the exception is also printed into `cerr`.

# Public classes
`BasicException` is a very generic and simple class that is meant to be used as a default exception class. It inherits from `std::exception` and can be constructed directly using a `std::string`.

You can either use `BasicException` as it is, implement a children exception class that inherits from `BasicException` or use your own home-made exception classes.
