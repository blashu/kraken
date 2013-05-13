#include "Settings.h"

#include <boost/log/expressions.hpp>
#include <boost/log/support/date_time.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <kraken/Decompiler.h>
#include <iostream>
#include <set>

using namespace kraken;
namespace logging = boost::log;
namespace keywords = boost::log::keywords;
namespace expr = boost::log::expressions;

void init_logger()
{
  logging::add_common_attributes();

  logging::add_file_log
  (
    keywords::file_name = "kraken.log",
    keywords::format =
    (
      expr::stream
        << "[" << expr::format_date_time<boost::posix_time::ptime>("TimeStamp", "%H:%M:%S.%f") << "]"
        << ": <" << expr::attr<logging::trivial::severity_level>("Severity")
        << "> " << expr::smessage
      )
  );
}
int main( int argc, const char** argv )
{
  Settings settings( argc, argv );

  if( false == settings.run() )
  {
    return EXIT_FAILURE;
  }

  init_logger();
  BOOST_LOG_TRIVIAL(info) << "Start app.";

  Decompiler decompiler;

  decompiler.set_target(settings.path_to_bin());
  decompiler.decompile();

  auto lll = decompiler.low_level_listing();

  for( int blockId = 0, blocksCount = lll->block_count(); blockId < blocksCount; blockId++ )
  {
    auto block = lll->get_block_by_id(blockId);

    cout << "=> Block " << blockId << endl << endl;
    for( int itemId = 0, itemsCount = block->get_item_count(); itemId < itemsCount; itemId++ )
    {
      cout << (block->get_item_by_id(itemId)->to_string()) << endl;
    }
    cout << "------------------" << endl;
  }

  return EXIT_SUCCESS;
}
