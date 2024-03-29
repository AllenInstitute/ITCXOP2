#pragma once

#include <memory>
#include <string>

void AddLogEntry(const json &doc);
void AddLogEntry(const std::string &str);
void SetLoggingTemplate(const std::string &loggingTemplate);

// Logging class creating JSONL formatted log files in Igor Preferences folder
// under `package`
//
// Callers must ensure proper threadsafe usage!
class Logging
{
public:
  Logging(const std::string &package, const std::string &loggingTemplate);
  Logging(const std::string &package);
  ~Logging();

  void AddLogEntry(const json &doc) const;
  void AddLogEntry(const std::string &str) const;

private:
  Logging(const Logging &)        = delete;
  Logging(const Logging &&)       = delete;
  Logging &operator=(Logging &)   = delete;
  Logging &&operator=(Logging &&) = delete;

  class Implementation;
  std::unique_ptr<Logging::Implementation> m_impl;
};
