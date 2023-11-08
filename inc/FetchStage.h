#include "PipeRegArray.h"
#include "PipeReg.h"
#include "Stage.h"

#ifndef FETCHSTAGE_H
#define FETCHSTAGE_H
class FetchStage: public Stage
{
   private:
      //TODO: provide declarations for new methods
      
      void setDInput(PipeReg * dreg, uint64_t stat, uint64_t icode, uint64_t ifun, 
                     uint64_t rA, uint64_t rB,
                     uint64_t valC, uint64_t valP);
      uint64_t selectPC(PipeReg * freg, PipeReg * mreg, PipeReg * wreg);
	  bool needRegIds(uint64_t f_icode);
      bool need_valC(uint64_t f_icode);
	  uint64_t predictPC(uint64_t f_icode, uint64_t f_valC, uint64_t f_valP);

   public:
      //These are the only methods called outside of the class
      bool doClockLow(PipeRegArray * pipeRegs);
      void doClockHigh(PipeRegArray * pipeRegs);
};
#endif
