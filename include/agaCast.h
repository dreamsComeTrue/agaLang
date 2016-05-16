#ifndef _AGA_CAST_H_
#define _AGA_CAST_H_

#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Type.h>
#include <llvm/IR/Value.h>

namespace aga
{
    class agaCast
    {
      public:
        static llvm::Value *CreateCast (llvm::Value *expression, llvm::Type *castType, llvm::IRBuilder<> &builder)
        {
            llvm::Value *castValue = nullptr;

            if (expression->getType () == castType)
            {
                castValue = expression;
            }
            else if (expression->getType ()->getScalarSizeInBits () > castType->getScalarSizeInBits ())
            {
                castValue = builder.CreateCast (llvm::Instruction::Trunc, expression, castType);
            }
            else if (expression->getType ()->getScalarSizeInBits () < castType->getScalarSizeInBits ())
            {
                castValue = builder.CreateCast (llvm::Instruction::SExt, expression, castType);
            }

            return castValue;
        }
    };
}

#endif //	_AGA_CAST_H_
