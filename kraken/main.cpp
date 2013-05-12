#include "Settings.h"

#include <boost/log/expressions.hpp>
#include <boost/log/support/date_time.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <iomanip>
#include <iostream>
#include <kraken/Decompiler.h>
#include <set>

using namespace kraken;
namespace logging = boost::log;
namespace keywords = boost::log::keywords;
namespace expr = boost::log::expressions;

std::string complete2short_instr( const char* completeInstruction )
{
  const char* whiteCharPtr;

  if( NULL == ( whiteCharPtr = strchr( completeInstruction, ' ' ) ) )
  {
    return completeInstruction;
  }

  return std::string( completeInstruction, whiteCharPtr );
}

void show_all_instr(const Disassembler& disassem)
{
  int count = 0;

  disassem.go_through_instructions([&count](const AsmCode& asmCode){
    unsigned int rva = asmCode.VirtualAddr - 0x400000;

    cout << "0x" << std::setw(8) << std::hex << std::setfill('0')
         << rva << "\t" << asmCode.CompleteInstr << endl;

    count++;
  });

  cout << "Cound of disassembled instructions: " << std::dec << count << endl;
}

void show_instr_set(const Disassembler& disassem)
{
  std::set<std::string> instructionList;
  cout << "Exe file contains next list of instruction:" << endl;

  disassem.go_through_instructions([&instructionList](const AsmCode& asmCode){
    instructionList.insert( complete2short_instr( asmCode.CompleteInstr ) );
  });

  for( auto it = instructionList.begin(), end = instructionList.end(); it != end; ++it )
  {
    cout << *it << endl;
  }
  cout << instructionList.size() << endl;
}

void init_logging()
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
  init_logging();
  BOOST_LOG_TRIVIAL(info) << "Start app.";

  Settings settings( argc, argv );

  if( false == settings.run() )
  {
    return EXIT_FAILURE;
  }

  Decompiler decompiler;

  decompiler.set_target(settings.path_to_bin());
  decompiler.decompile();

  return EXIT_SUCCESS;
}
