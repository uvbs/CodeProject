/*	
 *	C++ Reflection Framework.
 *
 *	Copyright (c) 2004,2005 - Fabio Lombardelli (lombarde@users.sf.net).
 *	The author makes no representations about the suitability of this software
 *	for any purpose. It is provided "as is" without express or implied warranty.
 *
 */

#ifndef SERIALIZATION_ARCHIVE_H
#define SERIALIZATION_ARCHIVE_H

namespace Serialization
{

	class Archive
	{
	protected:
		Archive();
		char* _buf;
		char* _cursor;
	};
	
	class OutArchive : public Archive
	{
	public:
		OutArchive(int size);
		~OutArchive();
		void write(const void* data, int size);
		void* get();
		int size();
	private:
		int _size;
	};
	
	class InArchive : public Archive
	{
	public:
		InArchive(void* buf);
		void* read(int size);
	};
	
} // namespace Serialization

#endif

