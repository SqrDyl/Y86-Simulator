#include "PipeRegArray.h"
#include "WritebackStage.h"
#include "Status.h"
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
bool WritebackStage::doClockLow(PipeRegArray * pipeRegs)
{
    PipeReg * wreg = pipeRegs->getWritebackReg();
    uint64_t stat = wreg->get(W_STAT);
    if (stat != Status::SAOK)
    {
        return true;
    }
    return false;
}

/* doClockHigh
 *
 * applies the appropriate control signal to the F
 * and D register intances
 * 
 * @param: pipeRegs - array of the pipeline register (F, D, E, M, W instances)
*/
void WritebackStage::doClockHigh(PipeRegArray * pipeRegs)
{
    bool error = false;
    PipeReg * wreg = pipeRegs->getWritebackReg();
    uint64_t w_valE = wreg->get(W_VALE);
    uint64_t w_dstE = wreg->get(W_DSTE);
    RegisterFile * reg = RegisterFile::getInstance();
    reg->writeRegister(w_valE, w_dstE, error);
    
    //  v Lab 9 Code v
    //uint64_t m_valM = mreg->get(W_VALM);
    uint64_t w_dstM = wreg->get(W_DSTM);
    reg->writeRegister(m_valM, w_dstM, error);
}



