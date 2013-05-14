#include <QApplication>
#include "MainWindow.h"

#include <boost/log/expressions.hpp>
#include <boost/log/support/date_time.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/sinks.hpp>
#include <boost/log/utility/empty_deleter.hpp>
#include <fstream>
#include <iostream>

using namespace kraken;
namespace logging = boost::log;
namespace keywords = boost::log::keywords;
namespace expr = boost::log::expressions;
namespace sinks = boost::log::sinks;

void init_logger()
{
  logging::add_common_attributes();

  boost::shared_ptr< logging::core > core = logging::core::get();

  // Create a backend and attach a couple of streams to it
  boost::shared_ptr< sinks::text_ostream_backend > backend =
      boost::make_shared< sinks::text_ostream_backend >();

  backend->add_stream(
          boost::shared_ptr< std::ostream >(&std::cout, logging::empty_deleter()));
  backend->add_stream(
      boost::shared_ptr<std::ostream >(new std::ofstream("kraken.log")));

  // Enable auto-flushing after each log record written
  backend->auto_flush(true);

  // Wrap it into the frontend and register in the core.
  // The backend requires synchronization in the frontend.
  typedef sinks::synchronous_sink< sinks::text_ostream_backend > sink_t;
  boost::shared_ptr< sink_t > sink(new sink_t(backend));

  sink->set_formatter
  (
    expr::format("[%1%] [%2%] %3%")
        % expr::format_date_time<boost::posix_time::ptime>("TimeStamp", "%H:%M:%S.%f")
        % expr::attr<logging::trivial::severity_level>("Severity")
        % expr::xml_decor[ expr::stream << expr::smessage ]
  );

  core->add_sink(sink);
}

int main(int argc, char *argv[])
{
  init_logger();

  QApplication app(argc, argv);
  MainWindow mainWindow;
  mainWindow.show();
  return app.exec();
}
