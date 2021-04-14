/// \file
/*
* Nirvana IDL front-end library.
*
* This is a part of the Nirvana project.
*
* Author: Igor Popov
*
* Copyright (c) 2021 Igor Popov.
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU Lesser General Public License as published by
* the Free Software Foundation; either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU Lesser General Public
* License along with this library.  If not, see <http://www.gnu.org/licenses/>.
*
* Send comments and/or bug reports to:
*  popov.nirvana@gmail.com
*/
#ifndef NIDL_INDENTEDOUT_H_
#define NIDL_INDENTEDOUT_H_

#include <fstream>
#include <filesystem>
#include <assert.h>

/// \brief Back-end support utility classes.
namespace BE {

/// Output file stream with indentation.
class IndentedOut : public std::ofstream
{
public:
	/// Default constructor.
	IndentedOut ();

	/// Constructor.
	/// 
	/// \param file File to open.
	/// \throw std::runtime_error
	IndentedOut (const std::filesystem::path& file);

	~IndentedOut ();

	/// \brief Opens the file.
	/// 
	/// \param file File to open.
	/// \throw std::runtime_error
	void open (const std::filesystem::path& file);

	/// Increase indentation.
	void indent () noexcept
	{
		isbuf_.indent ();
	}

	/// Decrease indentation.
	void unindent () noexcept
	{
		isbuf_.unindent ();
	}

	/// Ensure that empty line inserted at the current position.
	void empty_line ()
	{
		isbuf_.empty_line ();
	}

	/// \returns Current indentation.
	unsigned indentation () const noexcept
	{
		return isbuf_.indentation ();
	}

	/// \returns The last output character.
	char last_char () const noexcept
	{
		return isbuf_.last_char ();
	}

private:
	class IndentedStreambuf : public std::streambuf
	{
	public:
		IndentedStreambuf () :
			out_ (nullptr),
			indentation_ (0),
			bol_ (true),
			empty_line_ (false),
			last_char_ (0)
		{}

		void init (std::ostream& s)
		{
			out_ = s.rdbuf ();
			s.rdbuf (this);
		}

		void term (std::ostream& s)
		{
			s.rdbuf (out_);
		}

		void indent ()
		{
			++indentation_;
		}

		void unindent ()
		{
			assert (indentation_ > 0);
			if (indentation_ > 0)
				--indentation_;
		}

		unsigned indentation () const
		{
			return indentation_;
		}

		void empty_line ();

		const char last_char () const
		{
			return last_char_;
		}

	protected:
		virtual int overflow (int c);

	private:
		int put_char (char c);

	private:
		std::streambuf* out_;
		unsigned indentation_;
		char last_char_;
		bool bol_;
		bool empty_line_;
	};

	IndentedStreambuf isbuf_;
};

}

#endif
