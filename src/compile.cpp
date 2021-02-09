#include "compile.h"
#include "Options.h"
#include "simplecpp/simplecpp.h"
#include "FE/Driver.h"
#include <fstream>
#include <sstream>

FlexLexer* lexer;

using namespace std;

int compile (const string& file)
{
	istringstream preprocessed;

	{ // Perform preprocessing
		simplecpp::OutputList output_list;
		std::vector<std::string> files;
		std::ifstream f (file);
		simplecpp::TokenList rawtokens (f, files, file, &output_list);
		rawtokens.removeComments ();
		std::map<std::string, simplecpp::TokenList*> included = simplecpp::load (rawtokens, files, options.preprocessor, &output_list);
		for (auto i = included.begin (); i != included.end (); ++i)
			i->second->removeComments ();
		simplecpp::TokenList output_tokens (files);
		simplecpp::preprocess (output_tokens, rawtokens, files, included, options.preprocessor, &output_list);

    if (!output_list.empty ()) {
      for (const simplecpp::Output& output : output_list) {
        std::cerr << output.location.file () << ':' << output.location.line << ": ";
        switch (output.type) {
          case simplecpp::Output::ERROR:
            std::cerr << "#error: ";
            break;
          case simplecpp::Output::WARNING:
            std::cerr << "#warning: ";
            break;
          case simplecpp::Output::MISSING_HEADER:
            std::cerr << "Missing header: ";
            break;
          case simplecpp::Output::INCLUDE_NESTED_TOO_DEEPLY:
            std::cerr << "Include nested too deeply: ";
            break;
          case simplecpp::Output::SYNTAX_ERROR:
            std::cerr << "Syntax error: ";
            break;
          case simplecpp::Output::PORTABILITY_BACKSLASH:
            std::cerr << "Portability: ";
            break;
          case simplecpp::Output::UNHANDLED_CHAR_ERROR:
            std::cerr << "Unhandled char error: ";
            break;
          case simplecpp::Output::EXPLICIT_INCLUDE_NOT_FOUND:
            std::cerr << "Explicit include not found: ";
            break;
        }
        std::cerr << output.msg << std::endl;
      }
      return -1;
    }

		preprocessed.str (output_tokens.stringify ());
	}

	{
		Driver driver (file, preprocessed);
		int err = driver.parse ();
    if (err)
      return err;
	}

	return 0;
}
