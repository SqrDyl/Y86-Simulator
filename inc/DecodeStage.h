#include "PipeRegArray.h"
#include "Stage.h"

#ifndef DECODESTAGE_H
#define DECODESTAGE_H
class DecodeStage: public Stage
{
   private:
    void setEInput(PipeReg * ereg, uint64_t stat, uint64_t icode,
                           uint64_t ifun, uint64_t valC, uint64_t valA,uint64_t valB,
                           uint64_t destE, uint64_t destM, uint64_t srcA, uint64_t srcB);
	uint64_t srcA(PipeReg * dreg);
	uint64_t srcB(PipeReg * dreg);
	uint64_t dstE(PipeReg * dreg);
	uint64_t dstM(PipeReg * dreg);
	uint64_t fwdA(PipeRegArray * PipeReg);
	uint64_t fwdB(PipeRegArray * PipeReg);
      //TODO: provide declarations for new methods
   public:
      //These are the only methods called outside of the class
      bool doClockLow(PipeRegArray * pipeRegs);
      void doClockHigh(PipeRegArray * pipeRegs);
};
#endif
