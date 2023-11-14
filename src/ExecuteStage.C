#include "PipeRegArray.h"
#include "ExecuteStage.h"
#include "Instruction.h"
#include "Status.h"
#include "E.h"
#include "M.h"
#include "ConditionCodes.h"
#include "Tools.h"

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
    uint64_t valA = mreg->get(M_VALA);
    uint64_t e_valE = ereg->get(E_VALC);
	uint64_t e_dstE = ereg->get(E_DSTE);
	//setMInput(mreg, stat, icode, 0, 0, valA, RegisterFile::RNONE, dstM);  this was before changes

    setMInput(mreg, stat, icode, 0, e_valE, valA, e_dstE, dstM);
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


//LAB 8 
//==============================
uint64_t ExecuteStage::aluAComp(PipeReg * ereg)
{
    uint64_t e_icode = ereg->get(E_ICODE);
    uint64_t e_valA = ereg->get(E_VALA);
    uint64_t e_valC = ereg->get(E_VALC);

    if (e_icode == Instruction::IIRMOVQ || e_icode == Instruction::IOPQ)
    {
        return e_valA;
    }
    else if (e_icode == Instruction::IIRMOVQ || e_icode == Instruction::IRMMOVQ || e_icode == Instruction::IMRMOVQ)
    {
        return e_valC;
    }
    else if (e_icode == Instruction::ICALL || e_icode == Instruction::IPUSHQ)
    {
        return -8;
    }
    else if (e_icode == Instruction::IRET ||e_icode == Instruction::IPOPQ)
    {
        return 8;
    }
    else
    {
        return 0;
    }
}

uint64_t ExecuteStage::aluBComp(PipeReg * ereg)
{
    uint64_t e_icode = ereg->get(E_ICODE);
    uint64_t e_valB = ereg->get(E_VALB);

    if (e_icode == Instruction::IRMMOVQ || e_icode == Instruction::IMRMOVQ || e_icode == Instruction::IOPQ 
        || e_icode == Instruction::ICALL || e_icode == Instruction::IPUSHQ || e_icode == Instruction::IRET
        || e_icode == Instruction::IPOPQ)
    {
        return e_valB;
    }
    else if (e_icode == Instruction::IRRMOVQ || e_icode == Instruction::IIRMOVQ)
    {
        return 0;
    }
    else
    {
        return 0;
    }
}

uint64_t ExecuteStage::AluFunComp(PipeReg * ereg)
{
    uint64_t e_icode = ereg->get(E_ICODE);
    uint64_t e_ifun = ereg->get(E_IFUN);

    if (e_icode == Instruction::IOPQ)
    {
        return e_ifun;
    }
    else
    {
        return Instruction::ADDQ;
    }
}

bool ExecuteStage::setCC(PipeReg * ereg)
{
    uint64_t e_icode = ereg->get(E_ICODE);
    return (e_icode == Instruction::IOPQ);
}

uint64_t ExecuteStage::dstEComp(PipeReg * ereg)
{
    uint64_t e_icode = ereg->get(E_ICODE);
    uint64_t e_dstE = ereg->get(E_DSTE);

    if (e_icode == Instruction::IRRMOVQ && !e_Cnd)
    {
        return RegisterFile::RNONE;
    }
    else
    {
        return e_dstE;
    }
}

void ExecuteStage::ccMethod(bool setCC, uint64_t aluRes, uint64_t aluA, uint64_t aluB, uint64_t aluFun)
{
    bool error = false;
    if (!setCC)
    {
        return;
    }
    cc->setConditionCode(aluRes == 0, ConditionCodes::ZF, error);
    cc->setConditionCode(Tools::sign(aluRes), ConditionCodes::SF, error);
    if (aluFun == Instruction::ADDQ)
    {
        cc->setConditionCode(Tools::addOverflow(aluA, aluB), ConditionCodes::OF, error);
    }
    else if (aluFun == Instruction::SUBQ)
    {
        cc->setConditionCode(Tools::subOverflow(aluA, aluB), ConditionCodes::OF, error);
    }
    else
    {
        cc->setConditionCode(0, ConditionCodes::OF, error);
    }
}

uint64_t ExecuteStage::alu(uint64_t aluFun)
{

}


