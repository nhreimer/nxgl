#ifndef INC_3AA8F1E76F9B4B01BDE1B28D96DCC9E4
#define INC_3AA8F1E76F9B4B01BDE1B28D96DCC9E4

#if !defined(__PRETTY_FUNCTION__) && !defined(__GNUC__)
#define __PRETTY_FUNCTION__ __FUNCSIG__
#endif

#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE
//#define SPDLOG_FUNCTION __PRETTY_FUNCTION__

#include <spdlog/spdlog.h>
#include <spdlog/logger.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/null_sink.h>
#include <iostream>
#include <mutex>

#ifndef PLOG_LOGGER
#define PLOG_LOGGER

#define LOG_TRACE( ... ) SPDLOG_LOGGER_TRACE( nxgl::SLog::log, __VA_ARGS__ )
#define LOG_DEBUG( ... ) SPDLOG_LOGGER_DEBUG( nxgl::SLog::log, __VA_ARGS__ )
#define LOG_INFO( ... ) SPDLOG_LOGGER_INFO( nxgl::SLog::log, __VA_ARGS__ )
#define LOG_WARN( ... ) SPDLOG_LOGGER_WARN( nxgl::SLog::log, __VA_ARGS__ )
#define LOG_ERROR( ... ) SPDLOG_LOGGER_ERROR( nxgl::SLog::log, __VA_ARGS__ )
#define LOG_CRITICAL( ... ) SPDLOG_LOGGER_CRITICAL( nxgl::SLog::log, __VA_ARGS__ )

#endif

namespace nxgl
{
class SLog
{
public:
  inline static std::shared_ptr< spdlog::logger > log;

  static void initializeConsole()
  {
    std::unique_lock< std::mutex > lock( m_mutex );

    if ( !log )
    {
      ensureLoggerExists();

      auto console = std::make_shared< spdlog::sinks::stdout_color_sink_mt >();

      console->set_level( spdlog::level::debug );
      console->set_pattern( "[%L][%t][%H:%M:%S.%e][%!:%#] %v" );
      log->sinks().push_back( console );
    }
  }

  static void initializeLogger( const std::string& filename )
  {
    std::unique_lock< std::mutex > lock( m_mutex );

    if ( !log )
    {
      ensureLoggerExists();
      // this can throw an error if the file can't be created or written to
      auto file = std::make_shared< spdlog::sinks::basic_file_sink_mt >( filename, true );
      file->set_pattern( "[%L][%t][%H:%M:%S.%e][%!:%#] %v" );
      log->sinks().push_back( file );
    }
  }

  static void initializeNullLogger()
  {
    std::unique_lock< std::mutex > lock( m_mutex );

    if ( !log )
    {
      ensureLoggerExists();
      log->sinks().push_back( std::make_shared< spdlog::sinks::null_sink_mt >() );
    }
  }

private:

  static void ensureLoggerExists()
  {
    if ( log )
    {
      LOG_WARN( "request to initialize log when log already exists. ignoring request." );
      return;
    }

    log = std::make_shared< spdlog::logger >( "nxgl" );
    log->set_level( spdlog::level::debug );
    log->flush_on( spdlog::level::debug );
    log->set_pattern( "[%L][%t][%H:%M:%S.%e][%!:%#] %v" );
  }

  static inline std::mutex m_mutex;
};
}

#endif //INC_3AA8F1E76F9B4B01BDE1B28D96DCC9E4
