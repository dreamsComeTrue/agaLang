#ifndef _AGA_TYPEINFO_H_
#define _AGA_TYPEINFO_H_

#include <string>

namespace aga
{
	struct agaTypeInfo
	{
	public:
		agaTypeInfo (long value) : m_LongValue (value) {}
		agaTypeInfo (double value) : m_DoubleValue (value) {}
		agaTypeInfo (char* value) : m_StringValue (value) {}

	private:
		union
		{
			long		m_LongValue;
			double		m_DoubleValue;
			char*		m_StringValue;
		};
	};
}

#endif	//	_AGA_TYPEINFO_H_
