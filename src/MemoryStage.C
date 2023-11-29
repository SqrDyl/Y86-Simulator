#include "PipeRegArray.h"
#include "MemoryStage.h"
#include "M.h"
#include "W.h"
#include "Instruction.h"

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

    bool error = false;
    uint64_t memAddRes = memAddr(mreg);
    //Stage::m_valM = memAddr(mreg);
    bool memReadRes = memRead(icode);
    bool memWriteRes = memWrite(icode);
	Stage::m_valM = 0;
    if (memReadRes)
    {
        Stage::m_valM = Stage::mem->getLong(memAddRes, error);
    }
    else if (memWriteRes)
    {
        Stage::mem->putLong(mreg->get(M_VALA), memAddRes, error);
        //m_valM = 0;
    }
    

   setWInput(wreg, stat, icode, valE, Stage::m_valM, dstE, dstM);

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

uint64_t MemoryStage::memAddr(PipeReg * mreg)
{
    uint64_t icode = mreg->get(M_ICODE);
    if (icode == Instruction::IRMMOVQ || icode == Instruction::IPUSHQ || icode == Instruction::ICALL || icode == Instruction::IMRMOVQ)
    {
        return mreg->get(M_VALE);
    }
    else if (icode == Instruction::IPOPQ || icode == Instruction::IRET)
    {
        return mreg->get(M_VALA);
    }
    else 
    {
        return 0;
    }
}

bool MemoryStage::memRead(uint64_t icode)
{
    return (icode == Instruction::IMRMOVQ || icode == Instruction::IPOPQ || icode == Instruction::IRET);
}

bool MemoryStage::memWrite(uint64_t icode)
{
    return (icode == Instruction::IRMMOVQ || icode == Instruction::IPUSHQ || icode == Instruction::ICALL);
}


