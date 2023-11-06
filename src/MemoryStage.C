#include "PipeRegArray.h"
#include "MemoryStage.h"
#include "M.h"
#include "W.h"

/*
 * doClockLow
 *
 * Performs the Fetch stage combinational logic that is performed when
 * the clock edge is low.
 *
 * @param: pipeRegs - array of the pipeline register 
                      (F, D, E, M, W instances)
 */
bool MemoryStage::doClockLow(PipeRegArray * pipeRegs)
{
   PipeReg * mreg = pipeRegs->getMemoryReg();
   PipeReg * wreg = pipeRegs->getWritebackReg();
   uint64_t stat = mreg->get(M_STAT);
   uint64_t icode = mreg->get(M_ICODE);
   uint64_t valE = mreg->get(M_VALE);
   uint64_t dstE = mreg->get(M_DSTE);
   uint64_t dstM = mreg->get(M_DSTM);
   // Undefined Reference Error for below???? 
   setWInput(wreg, stat, icode, valE, 0, dstE, dstM);

   return false;
}

void MemoryStage::setWInput(PipeReg * wreg, uint64_t stat, uint64_t icode, uint64_t valE,
		uint64_t valM, uint64_t dstE, uint64_t dstM)
{
	wreg->set(W_STAT, stat);
	wreg->set(W_ICODE, icode);
	wreg->set(W_VALE, valE);
	wreg->set(W_VALM, valM);
	wreg->set(W_DSTE, dstE);
	wreg->set(W_DSTM, dstM);
}

/* doClockHigh
 *
 * applies the appropriate control signal to the F
 * and D register intances
 * 
 * @param: pipeRegs - array of the pipeline register (F, D, E, M, W instances)
*/
void MemoryStage::doClockHigh(PipeRegArray * pipeRegs)
{
	PipeReg * wreg = pipeRegs->getWritebackReg();
	wreg->normal();
}



