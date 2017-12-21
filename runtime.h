#ifndef _RUNTIME_H
#define _RUNTIME_H

#include <map>
#include <string>
#include <vector>

namespace runtime
{
  // temporary to allow program compilation
  typedef int func;
  namespace opcodes
  {
    extern std::map<std::string, func>;
  }

  namespace datatypes
  {
    class NullType;
    class StringType;
    class BoolType;
    class CharType;
    class ShortType;
    class IntType;
    class LongType;
    class FloatType;
    class DoubleType;
    class UnsignedCharType;
    class UnsignedShortType;
    class UnsignedIntType;
    class UnsignedLongType;
    class StructureType;

    class DataType
    {
    private:
    protected:
      const int CType;
      bool array;

    public:

      virtual std::string toString();
      virtual bool isArray();
      virtual int getDataType();

      // Data Type Definitions - used to determine the type of data
    public:
      static const int Undefined = -1;
      static const int Null = 0;
      static const int String = 1;
      static const int Bool = 2;
      static const int Char = 3;
      static const int Short = 4;
      static const int Int = 5;
      static const int Long = 6;
      static const int Float = 7;
      static const int Double = 8;
      static const int UnsignedChar = 9;
      static const int UnsignedShort = 10;
      static const int UnsignedInt = 11;
      static const int UnsignedLong = 12;
      static const int Structure = 13;        // A structure can hold a variety of fields within
    };

    class NullType : public DataType
    {
    public:
      virtual std::string toString();
      virtual int getDataType();
    };

    class StringType : public DataType
    {
    protected:
      std::vector<std::string> data;

    public:
      virtual std::string toString();
      virtual int getDataType();

      // Allow easy casting to string!
      inline operator std::string()
      {
        return toString();
      }
    };

    class BoolType : public DataType
    {
    protected:
      std::vector<bool> data;

    public:
      virtual std::string toString();
      virtual int getDataType();
    };

    class CharType : public DataType
    {
    protected:
      std::vector<char> data;

    public:
      virtual std::string toString();
      virtual int getDataType();
    };

    class ShortType : public DataType
    {
    protected:
      std::vector<short int> data;

    public:
      virtual std::string toString();
      virtual int getDataType();
    };

    class IntType : public DataType
    {
    protected:
      std::vector<int> data;

    public:
      virtual std::string toString();
      virtual int getDataType();
    };

    class LongType : public DataType
    {
    protected:
      std::vector<long> data;

    public:
      virtual std::string toString();
      virtual int getDataType();
    };

    class FloatType : public DataType
    {
    protected:
      std::vector<float> data;

    public:
      virtual std::string toString();
      virtual int getDataType();
    };

    class DoubleType : public DataType
    {
    protected:
      std::vector<double> data;

    public:
      virtual std::string toString();
      virtual int getDataType();
    };

    class UnsignedCharType : public DataType
    {
    protected:
      std::vector<unsigned char> data;

    public:
      virtual std::string toString();
      virtual int getDataType();
    };

    class UnsignedShortType : public DataType
    {
    protected:
      std::vector<unsigned short int> data;

    public:
      virtual std::string toString();
      virtual int getDataType();
    };

    class UnsignedIntType : public DataType
    {
    protected:
      std::vector<unsigned int> data;

    public:
      virtual std::string toString();
      virtual int getDataType();
    };

    class UnsignedLongType : public DataType
    {
    protected:
      std::vector<unsigned long> data;

    public:
      virtual std::string toString();
      virtual int getDataType();
    };

    class StructureType : public DataType
    {
    protected:
      std::vector<DataType> data;

    public:
      virtual std::string toString();
      virtual int getDataType();
    };
  }

  /*
  Execution context of a program; Contains the following:
    - Program code (processed)
    - Execution pipeline (stack/queue/whatever's fastest)
    - Function table
    - Tickno
    - Data storage
  */
  class CircuitLangCTX
  {
  };


  int run_program(const char* filename);
}

#endif