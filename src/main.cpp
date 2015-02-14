#include "IndexProcessor.h"
#include "SearchProcessor.h"
#include "optionparser.h"

#include <iostream>

#include <string>
#include <vector>

using namespace std;

struct Arg : public option::Arg
{
  static option::ArgStatus NonEmpty (const option::Option& option, bool message);
};

enum programOptions {UNKNOWN, HELP, PATTERNFILE, COUNT};

const option::Descriptor usage[] =
  {
    {UNKNOWN, 0, "" , "", Arg::None, "USAGE: findpat2 MODE [OPTIONS] PATTERN TEXTFILE [TEXTFILE...]" },
    {UNKNOWN, 0, "" , "", Arg::None, "Search for PATTERN in each TEXTFILE\n"
                                             "If --pattern option is set, a list of patterns will be used instead of PATTERN\n"
                                             "Multiple files can be indicated for TEXTFILE by using wildcards\n\n"
                                             "Options:" },
    {HELP, 0, "h" , "help", Arg::None, "  -h, --help  \tPrint usage and exit" },
    {PATTERNFILE, 0, "p", "pattern", Arg::NonEmpty, "  -p, --pattern  \tSpecify a file with patterns to be searched, one per line," 
                                                                          " instead of using PATTERN" },
    {COUNT, 0, "c", "count", Arg::None, "  -c, --count  \tInstead of printing the TEXTFILE lines in which the patterns occur,"
                                                             " the total count of occurrences per file will be shown" },
    {UNKNOWN, 0, "",  "", Arg::None, "\nExamples:\n"
                                             "  findpat2 ababc textfile1.txt textfile2.txt\n"
                                             "  findpat2 -e 2 -p patterfile.txt textfile.txt\n"
                                             "  findpat2 -c -e 2 ababc textfile*.txt\n" },
    {0, 0, 0, 0, 0, 0}
 };

int main(int argc, char** argv) {

  string mode; // execution mode: index or search

  // Optional parameters
  string patternFile; // search
  bool count; // search

  // Required parameters (positional options)
  string textFile; // index
  string pattern; // search
  string indexFile; // search

  // Skip program name
  if (argc > 0) {
    argc -= 1;
    argv += 1;
  }

  // Get and check execution mode
  if (argc > 0) {
    mode = argv[0];

    if ((mode != "index") && (mode != "search")) { // invalid mode option
      if ((mode != "-h") && (mode != "--help")) { // cheking if it is the help option
        cerr << "Invalid execution mode. You have to specify it either as search or index. Try 'findpat2 --help' for more information." << endl;

        return 1;
      }
    }
    else {
      // skip it from parsing
      argc -= 1;
      argv += 1;
    }
  }

  // Initializing option parser structures
  option::Stats  stats(usage, argc, argv);
  option::Option* options = new option::Option[stats.options_max];
  option::Option* buffer = new option::Option[stats.buffer_max];
  option::Parser parse(usage, argc, argv, options, buffer);

  // Error in option parsing
  if (parse.error()){
    return 1;
  }

  // Print usage table if help option was set or no options at all
  if (options[HELP] || argc == 0) {
    option::printUsage(cout, usage);

    return 0;
  }

  // Pattern file option
  if (options[PATTERNFILE]) {
    patternFile = options[PATTERNFILE].last()->arg;
  }

  // Count option
  count = options[COUNT];

  // Positional options (required parameters)
  int positionalOptionsCount = parse.nonOptionsCount();

  if (mode == "index") { // Index mode
    if (positionalOptionsCount == 1) {
      textFile = parse.nonOption(0);
    }
    else {
      cerr << "In index mode, you have to specity a text file to be indexed. Try 'findpat2 --help' for more information." << endl;

      return 1;
    }

    IndexProcessor::processParameters(textFile);
  }
  else { // Search mode
    if (options[PATTERNFILE]) {
      if (positionalOptionsCount == 1) {
        indexFile = parse.nonOption(0);
      }
      else {
        cerr << "In search mode, you have to specity a pattern (or a patternfile) and a index file. Try 'findpat2 --help' for more information." << endl;

        return 1;
      }
    }
    else {
      if (positionalOptionsCount == 2) {
        pattern = parse.nonOption(0);
        indexFile = parse.nonOption(1);
      }
      else {
        cerr << "In search mode, you have to specity a pattern (or a patternfile) and a index file. Try 'findpat2 --help' for more information." << endl;

        return 1;
      }
    }

    SearchProcessor::processParameters(patternFile, count, pattern, indexFile);
  }

  // Unknown options
  for (option::Option* option = options[UNKNOWN]; option; option = option->next()) {
    cout << "Unknown option: " << option->name << endl;
  }

  return 0;
}

// Checking logic for non-empty argument
option::ArgStatus Arg::NonEmpty(const option::Option& option, bool message) {

  if (option.arg != 0 && option.arg[0] != 0) {
    return option::ARG_OK;
  }

  if (message) {
    (cerr << "Option ").write(option.name, option.namelen) << " requires a non-empty argument" << endl;
  }

  return option::ARG_ILLEGAL;
}