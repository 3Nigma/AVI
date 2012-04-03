#include "exceptions.hpp"

Exception::Exception(const std::string &m) : mMsg(m) {}
Exception::~Exception() noexcept {}
const char* Exception::what() const noexcept { return mMsg.c_str(); }

IOException::IOException(const std::string &info) : Exception(info) {}
LuaException::LuaException(const std::string &info) : Exception(info) {}

LuaMemAllocException::LuaMemAllocException(const std::string &msg) : LuaException(msg) {}
LuaMemAllocException::~LuaMemAllocException() noexcept {}

LuaExecCodeException::LuaExecCodeException(const std::string &msg) : LuaException(msg) {}
LuaExecCodeException::~LuaExecCodeException() noexcept {}

LuaFileException::LuaFileException(const std::string &msg) : LuaException(msg), IOException(msg) {}
LuaFileException::~LuaFileException() noexcept {}

LuaErrorFuncException::LuaErrorFuncException(const std::string &msg) : LuaException(msg) {}
LuaErrorFuncException::~LuaErrorFuncException() noexcept {}

LuaRuntimeException::LuaRuntimeException(const std::string &msg) : LuaException(msg) {}
LuaRuntimeException::~LuaRuntimeException() noexcept {}
