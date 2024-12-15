#include <gtest/gtest.h>

#include "../ErrorHandler.h"

namespace Test0
{
  // ---- Test class (empty)

  class GenericTests: public testing::Test
  {};


  // ---- Streamable class

  class PrintableMessage
  {
    public:
      const char* message;
      PrintableMessage(const char* message): message(message) {}
  };

  std::ostream& operator<<(std::ostream &out, const PrintableMessage & pmessage)
  {
    return out << pmessage.message;
  }


  // ---- Tests with basic exception

  TEST(GeneralTests, BasicRaiseError)
  {
    ASSERT_THROW( ErrorHandler::raise_error( ), ErrorHandler::BasicException);
    ASSERT_THROW( ErrorHandler::raise_error( "This is an error!" ), ErrorHandler::BasicException);
    ASSERT_THROW( ErrorHandler::raise_error( "There are ", 42, " errors!" ), ErrorHandler::BasicException);
    ASSERT_THROW( ErrorHandler::raise_error( PrintableMessage("Foo") ), ErrorHandler::BasicException);
  }

  TEST(GeneralTests, BasicAssert)
  {
    ASSERT_NO_THROW( ErrorHandler::assert_p( true, "You shoudln't see this" ) );
    ASSERT_THROW( ErrorHandler::assert_p( false ), ErrorHandler::BasicException);
    ASSERT_THROW( ErrorHandler::assert_p( false, "This is an error!" ), ErrorHandler::BasicException);
    ASSERT_THROW( ErrorHandler::assert_p( false, "There are ", 42, " errors!" ), ErrorHandler::BasicException);
    ASSERT_THROW( ErrorHandler::assert_p( false, PrintableMessage("Foo") ), ErrorHandler::BasicException);
  }


  // ---- Custom exception

  class CustomException : public std::exception
  {
  public:
      CustomException(const std::string &) {}
      CustomException(std::string &&) {}
      const char * what() const noexcept override { return "You are alone, in the dark..."; };
  };


  // ---- Tests with custom exception

  TEST(GeneralTests, CustomRaiseError)
  {
    ASSERT_THROW( ErrorHandler::raise_error<CustomException>( ), CustomException);
    ASSERT_THROW( ErrorHandler::raise_error<CustomException>( "This is an error!" ), CustomException);
    ASSERT_THROW( ErrorHandler::raise_error<CustomException>( "There are ", 42, " errors!" ), CustomException);
    ASSERT_THROW( ErrorHandler::raise_error<CustomException>( PrintableMessage("Foo") ), CustomException);
  }

  TEST(GeneralTests, CustomAssert)
  {
    ASSERT_NO_THROW( ErrorHandler::assert_p<CustomException>( true, "You shoudln't see this" ) );
    ASSERT_THROW( ErrorHandler::assert_p<CustomException>( false ), CustomException);
    ASSERT_THROW( ErrorHandler::assert_p<CustomException>( false, "This is an error!" ), CustomException);
    ASSERT_THROW( ErrorHandler::assert_p<CustomException>( false, "There are ", 42, " errors!" ), CustomException);
    ASSERT_THROW( ErrorHandler::assert_p<CustomException>( false, PrintableMessage("Foo") ), CustomException);
  }


  // ---- main

  int main(int argc, char **argv)
  {
    ::testing::InitGoogleTest( &argc, argv );
    return RUN_ALL_TESTS();
  }
};
