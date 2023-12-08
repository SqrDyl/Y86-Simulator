#include "PipeRegArray.h"
#include "PipeReg.h"
#include "Stage.h"

#ifndef FETCHSTAGE_H
#define FETCHSTAGE_H
class FetchStage: public Stage
{
   private:
      //TODO: provide declarations for new methods
      bool fetchStall;
      bool decodeStall;
      void setDInput(PipeReg * dreg, uint64_t stat, uint64_t icode, uint64_t ifun, 
                     uint64_t rA, uint64_t rB,
                     uint64_t valC, uint64_t valP);
      uint64_t selectPC(PipeReg * freg, PipeReg * mreg, PipeReg * wreg);
	  bool needRegIds(uint64_t f_icode);
      bool need_valC(uint64_t f_icode);
	  uint64_t predictPC(uint64_t f_icode, uint64_t f_valC, uint64_t f_valP);
      uint64_t PCincrement(uint64_t f_pc, bool needReg, bool needValC);
      void getRegs(bool needReg, uint64_t f_pc, uint64_t &rA, uint64_t &rB);
      uint64_t buildValC(bool needValC, bool needRegs, uint64_t f_pc);
      bool instr_valid(uint64_t icode);
      uint64_t f_stat(bool instrValid, uint64_t icode, bool memError);
      uint64_t f_icode(uint64_t icode, bool mem_error);
      uint64_t f_ifun(uint64_t ifun, bool mem_error);
      bool f_stall(PipeReg * ereg);
      bool d_stall(PipeReg * ereg);

   public:
      //These are the only methods called outside of the class
      bool doClockLow(PipeRegArray * pipeRegs);
      void doClockHigh(PipeRegArray * pipeRegs);
};
#endif
