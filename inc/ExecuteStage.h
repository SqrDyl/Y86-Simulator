#include "PipeRegArray.h"
#include "Stage.h"

#ifndef EXECUTESTAGE_H
#define EXECUTESTAGE_H
class ExecuteStage: public Stage
{
   private:
      //TODO: provide declarations for new methods
	  void setMInput(PipeReg * mreg, uint64_t stat, uint64_t icode,
                           uint64_t ifun, uint64_t valC, uint64_t valA, uint64_t valB,
                           uint64_t destE, uint64_t destM, uint64_t srcA, uint64_t srcB);
   public:
      //These are the only methods called outside of the class
      bool doClockLow(PipeRegArray * pipeRegs);
      void doClockHigh(PipeRegArray * pipeRegs);
};
#endif
