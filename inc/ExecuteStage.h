#include "PipeRegArray.h"
#include "Stage.h"

#ifndef EXECUTESTAGE_H
#define EXECUTESTAGE_H
class ExecuteStage: public Stage
{
   private:
      //TODO: provide declarations for new methods
      bool M_bubble;
	  void setMInput(PipeReg * mreg, uint64_t stat, uint64_t icode,
                           uint64_t Cnd, uint64_t valE, uint64_t valA,
                           uint64_t destE, uint64_t destM);
      uint64_t aluAComp(PipeReg * ereg);
      uint64_t aluBComp(PipeReg * ereg);
      uint64_t aluFunComp(PipeReg * ereg);
      bool setCC(PipeReg * ereg, PipeReg * wreg);
      uint64_t dstEComp(PipeReg * ereg);
      void ccMethod(bool setCC, uint64_t aluRes, uint64_t aluA, uint64_t aluB, uint64_t aluFun);
      uint64_t alu(uint64_t aluFun, uint64_t op1, uint64_t op2);
      uint64_t cond(PipeReg * ereg);
	  uint64_t calculateControlSignals(PipeReg * wreg, PipeReg * mreg);
   public:
      //These are the only methods called outside of the class
      bool doClockLow(PipeRegArray * pipeRegs);
      void doClockHigh(PipeRegArray * pipeRegs);
};
#endif
