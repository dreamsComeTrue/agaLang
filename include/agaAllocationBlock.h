#ifndef _AGA_ALLOCATIONBLOCK_H_
#define _AGA_ALLOCATIONBLOCK_H_

#include <string>

namespace aga
{
	class agaAllocationBlock
	{
	public:
		agaAllocationBlock () :
			m_Code (""), m_RegisterIndex (-1) {}

		const std::string &GetCode () const
		{
			return m_Code;
		}

		void SetCode (const std::string &code)
		{
			m_Code = code;
		}

		int GetRegisterIndex () const
		{
			return m_RegisterIndex;
		}

		void SetRegisterIndex (int index)
		{
			m_RegisterIndex = index;
		}

	protected:
		int				m_RegisterIndex;
		std::string		m_Code;
	};
}

#endif	//	_AGA_ALLOCATIONBLOCK_H_	
