#include "PipeRegArray.h"
#include "DecodeStage.h"
#include "Instruction.h"
#include "Status.h"
#include "D.h"


/*
 * doClockLow
 *
 * Performs the Fetch stage combinational logic that is performed when
 * the clock edge is low.
 *
 * @param: pipeRegs - array of the pipeline register 
                      (F, D, E, M, W instances)
 */
bool DecodeStage::doClockLow(PipeRegArray * pipeRegs)
{
    PipeReg * dreg = pipeRegs->getDecodeReg();

    bool mem_error = false;
    uint64_t icode = Instruction::INOP, ifun = Instruction::FNONE;
    uint64_t rA = RegisterFile::RNONE, rB = RegisterFile::RNONE;
    uint64_t valC = 0, valP = 0, stat = Status::SAOK, predPC = 0;
    bool needvalC = false;
    bool needregId = false;

    setEInput(dreg, stat, icode, ifun, rA, rB, valC, valP);
    return false;
}

void DecodeStage::setEInput(PipeReg * ereg, uint64_t stat, uint64_t icode,
                           uint64_t ifun, uint64_t rA, uint64_t rB,
                           uint64_t valC, uint64_t valP)
{
   ereg->set(D_STAT, stat);
   ereg->set(D_ICODE, icode);
   ereg->set(D_IFUN, ifun);
   ereg->set(D_RA, rA);
   ereg->set(D_RB, rB);
   ereg->set(D_VALC, valC);
   ereg->set(D_VALP, valP);
}

/* doClockHigh
 *
 * applies the appropriate control signal to the F
 * and D register intances
 * 
 * @param: pipeRegs - array of the pipeline register (F, D, E, M, W instances)
*/
void DecodeStage::doClockHigh(PipeRegArray * pipeRegs)
{
    PipeReg * freg = pipeRegs->getFetchReg();  
    PipeReg * dreg = pipeRegs->getDecodeReg();
    freg->normal();
    dreg->normal();
}



