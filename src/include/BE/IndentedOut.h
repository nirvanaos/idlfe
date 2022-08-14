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
#ifndef IDLFE_BE_INDENTEDOUT_H_
#define IDLFE_BE_INDENTEDOUT_H_
#pragma once

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

	/// Destructor.
	~IndentedOut ()
	{
		close ();
	}

	/// \brief Opens the file.
	/// 
	/// Also creates all parent directories.
	/// 
	/// \param file File to open.
	/// \throw std::runtime_error
	void open (const std::filesystem::path& file);

	/// \brief Closes the file.
	void close ();

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

	/// \returns The number of characters written.
	size_t size () const noexcept
	{
		return isbuf_.size ();
	}

private:
	class IndentedStreambuf : public std::streambuf
	{
	public:
		IndentedStreambuf ();

		void init (std::ostream& s);
		void term (std::ostream& s);

		void indent () noexcept
		{
			++indentation_;
		}

		void unindent () noexcept
		{
			assert (indentation_ > 0);
			if (indentation_ > 0)
				--indentation_;
		}

		unsigned indentation () const noexcept
		{
			return indentation_;
		}

		void empty_line ();

		const char last_char () const noexcept
		{
			return last_char_;
		}

		size_t size () const noexcept
		{
			return size_;
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
		size_t size_;
	};

	IndentedStreambuf isbuf_;
};

}

#endif
