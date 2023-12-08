#include "PipeRegArray.h"
#include "MemoryStage.h"
#include "M.h"
#include "W.h"
#include "Instruction.h"
#include "Status.h"

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
    uint64_t newStat = 0;
    bool mem_error = false;
    //Get the memory address
    uint64_t memAddRes = memAddr(mreg);
    //Check if we need to read or write
    bool memReadRes = memRead(icode);
    bool memWriteRes = memWrite(icode);

    //Get value out of memory or write to memory
	Stage::m_valM = 0;
    if (memReadRes)
    {
        Stage::m_valM = Stage::mem->getLong(memAddRes, mem_error);
    }
    else if (memWriteRes)
    {
        Stage::mem->putLong(mreg->get(M_VALA), memAddRes, mem_error);
    }
    //Check if memory error after the read or write
    Stage::m_stat = m_stat(mem_error);
   setWInput(wreg, Stage::m_stat, icode, valE, Stage::m_valM, dstE, dstM);

   return false;
}

/**
 * setWInput
 * 
 * sends the updated values to the writeBack reg
 * 
 * @param wreg - writeBack reg
 * @param stat - current stat
 * @param icode - current icode
 * @param valE - updated valE
 * @param valM - updated valM
 * @param dstE - updated dstE
 * @param dstM - updated dstM
*/
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

/**
 * memAddr
 * 
 * determine the memAddress needed
 * 
 * @param mreg - memory register to get the value of icode, valE, and valA
 * @return uint64_t
*/
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

/**
 * memRead 
 * 
 * determines whether the instruction will need to read from memory
 * 
 * @param icode - current instruction code
 * @return bool 
*/
bool MemoryStage::memRead(uint64_t icode)
{
    return (icode == Instruction::IMRMOVQ || icode == Instruction::IPOPQ || icode == Instruction::IRET);
}

/**
 * memWrite 
 * 
 * determines whether the instruction will need to write to memory
 * 
 * @param icode - current instruction code
 * @return bool 
*/
bool MemoryStage::memWrite(uint64_t icode)
{
    return (icode == Instruction::IRMMOVQ || icode == Instruction::IPUSHQ || icode == Instruction::ICALL);
}

/**
 * m_stat
 * 
 * Updates m_stat if there is a memory error
 * 
 * @param stat - original stat variable
 * @param mem_error - bool state true if error occured
 * @return uint64_t
*/
uint64_t MemoryStage::m_stat(bool mem_error)
{
    if (mem_error)
    {
        return Status::SADR;
    }
    else
    {
        return Stage::m_stat;
    }
}

