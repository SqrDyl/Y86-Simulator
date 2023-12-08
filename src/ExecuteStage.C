#include "PipeRegArray.h"
#include "ExecuteStage.h"
#include "Instruction.h"
#include "Status.h"
#include "E.h"
#include "M.h"
#include "ConditionCodes.h"
#include "Tools.h"
#include "W.h"
#include "Stage.h"

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
	PipeReg * wreg = pipeRegs->getWritebackReg();
   
	uint64_t stat = ereg->get(E_STAT);
	uint64_t icode = ereg->get(E_ICODE);
    uint64_t dstM = ereg->get(E_DSTM);
    uint64_t valA = ereg->get(E_VALA);

    Stage::e_Cnd = cond(ereg);
    Stage::e_dstE = dstEComp(ereg);
    bool ccRes = setCC(ereg, wreg);
    uint64_t fun = aluFunComp(ereg);
    uint64_t op1 = aluAComp(ereg);
    uint64_t op2 = aluBComp(ereg);
    Stage::e_valE = alu(fun, op1, op2);
    ccMethod(ccRes, Stage::e_valE, op1, op2, fun);
    
	M_bubble = calculateControlSignals(wreg);
    //  v LAB 9 QUESTION FOR OFFICE HOURS v
    //  If Stage::e_Cnd is instead set to 0, the andq runs? v LIKE THIS v
    // setMInput(mreg, stat, icode, 0, Stage::e_valE, valA, Stage::e_dstE, dstM);

    setMInput(mreg, stat, icode, Stage::e_Cnd, Stage::e_valE, valA, Stage::e_dstE, dstM);

	return false;
}
/**
 * setMInput
 * 
 * sends the updated values to the memory stage
 * 
 * @param mreg - memory register
 * @param stat - stat value from execute stage
 * @param icode - icode from execute stage
 * @param Cnd - condition value from stage
 * @param valE - valE computed from the ALU
 * @param valA - valA from execute stage
 * @param dstE - destination if operation was conducted in execute stage
 * @param dstM - destination if operation was conducted in memory
**/
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
	PipeReg * wreg = pipeRegs->getWritebackReg();
	if (M_bubble)
	{
		((M *)mreg)->bubble();
	}
	else
	{
		mreg->normal();
	}
}


//LAB 8 
//==============================

/**
 * aluAComp
 * 
 * figures out value of op1
 * 
 * @param ereg - execute reg in order to get the icode, valA, valC
 * @return uint64_t
*/
uint64_t ExecuteStage::aluAComp(PipeReg * ereg)
{
    uint64_t e_icode = ereg->get(E_ICODE);
    uint64_t e_valA = ereg->get(E_VALA);
    uint64_t e_valC = ereg->get(E_VALC);

    if (e_icode == Instruction::IRRMOVQ || e_icode == Instruction::IOPQ)
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

/**
 * aluBComp
 * 
 * gets the value for op2
 * 
 * @param ereg - execute reg in order to get the values or icode, and valB
 * @return uint64_t
*/
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

/**
 * aluFunComp 
 * 
 * gets the function based on the icode and the ifun
 * 
 * @param ereg - execute reg in order to get the icode and the ifun values
 * @return uint64_t 
*/
uint64_t ExecuteStage::aluFunComp(PipeReg * ereg)
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

/**
 * setCC
 * 
 * grabs the stat and icode in order to see if the condition
 * codes will need to be updated
 * 
 * @param ereg - execute reg to get the icode
 * @param wreg - writeBack reg to get the stat value
 * @return bool returns if the condition codes need to be updated or not
*/
bool ExecuteStage::setCC(PipeReg * ereg, PipeReg * wreg)
{
	uint64_t w_stat = wreg->get(W_STAT);
	uint64_t e_icode = ereg->get(E_ICODE);
	return ((e_icode == Instruction::IOPQ) && 
		(!Stage::m_stat == Status::SADR || !Stage::m_stat == Status::SINS|| !Stage::m_stat == Status::SHLT)
         && (!w_stat == Status::SADR|| !w_stat == Status::SINS || !w_stat == Status::SHLT));
}
/**
 * dstEComp
 * 
 * method to get the dstEComp
 * 
 * @param ereg - execute reg to get the icode, and the dstE
 * @return uint64_t
*/
uint64_t ExecuteStage::dstEComp(PipeReg * ereg)
{
    uint64_t e_icode = ereg->get(E_ICODE);

    if (e_icode == Instruction::IRRMOVQ && !e_Cnd)
    {
        return RegisterFile::RNONE;
    }
    else
    {
        return ereg->get(E_DSTE);
    }
}
/**
 * ccMethod
 * 
 * updates condition codes 
 * 
 * @param setCC - bool value to determine if we need to update condtion codes
 * @param aluRes - alu result
 * @param aluA - op1 from aluAComp
 * @param aluB - op2 from aluBComp
 * @param aluFun - uses the function to determine which overflow to use
*/
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

/**
 * alu
 * 
 * conducts the operation if it is IOPQ
 * 
 * @param aluFun - what operation is being performed
 * @param op1 - value from aluAComp
 * @param op2 - value from aluBComp
 * @return uint64_t - result
*/
uint64_t ExecuteStage::alu(uint64_t aluFun, uint64_t op1, uint64_t op2)
{
    if (aluFun == Instruction::ADDQ)
    {
        return op1 + op2;
    }
    else if (aluFun == Instruction::SUBQ)
    {
        return op2 - op1;
    }
    else if (aluFun == Instruction::ANDQ)
    {
        return op1 & op2;
    }
    else if (aluFun == Instruction::XORQ)
    {
        return op1 ^ op2;
    }
    else
    {
        return 0;
    }
}

/**
 * cond
 * 
 * updates the execute stage condition variable
 * 
 * @param ereg - execute reg to get the icode and the ifun variables
 * @return uint64_t
*/
uint64_t ExecuteStage::cond(PipeReg * ereg)
{
    bool error = false;
    uint64_t icode = ereg->get(E_ICODE);
    uint64_t ifun = ereg->get(E_IFUN);

    if (icode == Instruction::IJXX || icode == Instruction::IRRMOVQ)
    {
        if (ifun == Instruction::UNCOND)
        {
            return 1;
        }
        else if (ifun == Instruction::LESSEQ)
        {
            return (cc->getConditionCode(ConditionCodes::SF, error) ^ cc->getConditionCode(ConditionCodes::OF, error)) 
                | cc->getConditionCode(ConditionCodes::ZF, error);
        }
        else if (ifun == Instruction::LESS)
        {
            return (cc->getConditionCode(ConditionCodes::SF, error) ^ cc->getConditionCode(ConditionCodes::OF, error));
        }   
        else if (ifun == Instruction::EQUAL)
        {
            return cc->getConditionCode(ConditionCodes::ZF, error);
        }
        else if (ifun == Instruction::NOTEQUAL)
        {
            return !(cc->getConditionCode(ConditionCodes::ZF, error));
        }
        else if (ifun == Instruction::GREATER)
        {
            return (!(cc->getConditionCode(ConditionCodes::SF, error) ^ cc->getConditionCode(ConditionCodes::OF, error)) 
                & !(cc->getConditionCode(ConditionCodes::ZF, error)));
        }
        else if (ifun == Instruction::GREATEREQ)
        {
            return !(cc->getConditionCode(ConditionCodes::SF, error) ^ cc->getConditionCode(ConditionCodes::OF, error));    
        }
    }
    return 0;
}

/**
 * calculateControlSignals
 * 
 * calculate the status based on the stat in the writeback stage
 * 
 * @param wreg - writeBack reg in order to attain the stat value
 * @return uin64_t
*/
uint64_t ExecuteStage::calculateControlSignals(PipeReg * wreg)
{
	uint64_t w_stat = wreg->get(W_STAT);

	if ((Stage::m_stat == Status::SADR || Stage::m_stat == Status::SINS 
		|| Stage::m_stat == Status::SHLT) || (w_stat == Status::SADR
		|| w_stat == Status::SINS || w_stat == Status::SHLT))
	{
		return true;
	}
	else 
	{
		return false;
	}

}
