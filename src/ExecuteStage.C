#include "PipeRegArray.h"
#include "ExecuteStage.h"
#include "Instruction.h"
#include "Status.h"
#include "E.h"
#include "M.h"

/*
 * doClockLow
 *
 * Performs the Fetch stage combinational logic that is performed when
 * the clock edge is low.
 *
 * @param: pipeRegs - array of the pipeline register 
                      (F, D, E, M, W instances)
 */
bool ExecuteStage::doClockLow(PipeRegArray * pipeRegs)
{
	PipeReg * ereg = pipeRegs->getExecuteReg();
	PipeReg * mreg = pipeRegs->getMemoryReg();
   
	uint64_t stat = ereg->get(E_STAT);
	uint64_t icode = ereg->get(E_ICODE);
    uint64_t dstM = ereg->get(E_DSTM);


	setMInput(mreg, stat, icode, 0, 0, RegisterFile::RNONE, RegisterFile::RNONE, dstM);

	return false;
}

void ExecuteStage::setMInput(PipeReg * mreg, uint64_t stat, uint64_t icode,
                           uint64_t Cnd, uint64_t valE, uint64_t valA, 
                           uint64_t dstE, uint64_t dstM)
{
	mreg->set(M_STAT, stat);
    mreg->set(M_ICODE, icode);
	mreg->set(M_CND, Cnd);
	mreg->set(M_VALE, valE);
	mreg->set(M_VALA, valA);
	mreg->set(M_DSTE, dstE);
	mreg->set(M_DSTM, dstM);
}

/* doClockHigh
 *
 * applies the appropriate control signal to the F
 * and D register intances
 * 
 * @param: pipeRegs - array of the pipeline register (F, D, E, M, W instances)
*/
void ExecuteStage::doClockHigh(PipeRegArray * pipeRegs)
{
	PipeReg * mreg = pipeRegs->getMemoryReg();
	mreg->normal();
}



