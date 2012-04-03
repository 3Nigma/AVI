#ifndef _EXCEPTIONS_HPP_
#define _EXCEPTIONS_HPP_

#include <iostream>
#include <exception>

class Exception : public std::exception {
public:
  Exception(const std::string &m);
  virtual ~Exception() noexcept;
  virtual const char* what() const noexcept;
  
private:
  std::string mMsg;
};

class IOException : public Exception {
public:
  IOException(const std::string &info);
};

class LuaException : public Exception {
public:
  LuaException(const std::string &info);
};

class LuaMemAllocException : public LuaException {
public:
  LuaMemAllocException(const std::string &msg);
  virtual ~LuaMemAllocException() noexcept;
};

class LuaExecCodeException : public LuaException {
public:
  LuaExecCodeException(const std::string &msg);
  virtual ~LuaExecCodeException() noexcept;
};

class LuaFileException : public LuaException, public IOException {
public:
  LuaFileException(const std::string &msg);
  virtual ~LuaFileException() noexcept;
};

class LuaErrorFuncException : public LuaException {
public:
  LuaErrorFuncException(const std::string &msg);
  virtual ~LuaErrorFuncException() noexcept;
};

class LuaRuntimeException : public LuaException {
public:
  LuaRuntimeException(const std::string &msg);
  virtual ~LuaRuntimeException() noexcept;
};

#endif
