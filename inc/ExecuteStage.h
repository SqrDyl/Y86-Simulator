#include "PipeRegArray.h"
#include "Stage.h"

#ifndef EXECUTESTAGE_H
#define EXECUTESTAGE_H
class ExecuteStage: public Stage
{
   private:
      //TODO: provide declarations for new methods
	  void setMInput(PipeReg * mreg, uint64_t stat, uint64_t icode,
                           uint64_t Cnd, uint64_t valE, uint64_t valA,
                           uint64_t destE, uint64_t destM);
      uint64_t aluAComp(PipeReg * ereg);
      uint64_t aluBComp(PipeReg * ereg);
      uint64_t AluFunComp(PipeReg * ereg);
      bool setCC(PipeReg * ereg);
      uint64_t dstEComp(PipeReg * ereg);
      void ccMethod(bool setCC, uint64_t aluRes, uint64_t aluA, uint64_t aluB, uint64_t aluFun);
      uint64_t alu(uint64_t aluFun);
   public:
      //These are the only methods called outside of the class
      bool doClockLow(PipeRegArray * pipeRegs);
      void doClockHigh(PipeRegArray * pipeRegs);
};
#endif
