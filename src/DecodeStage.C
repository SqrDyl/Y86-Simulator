#include "PipeRegArray.h"
#include "DecodeStage.h"
#include "Instruction.h"
#include "Status.h"
#include "D.h"
#include "E.h"
#include "W.h"
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
bool DecodeStage::doClockLow(PipeRegArray * pipeRegs)
{
    PipeReg * dreg = pipeRegs->getDecodeReg();
    PipeReg * ereg = pipeRegs->getExecuteReg();

    uint64_t stat = dreg->get(D_STAT);
	uint64_t icode = dreg->get(D_ICODE);
	uint64_t ifun = dreg->get(D_IFUN);
    uint64_t valC = dreg->get(D_VALC);

    //Lab 8 function calls
    uint64_t destE = dstE(dreg);
	d_srcA = srcA(dreg);
	d_srcB = srcB(dreg);
	executeBubble = e_bubble(ereg);

    setEInput(ereg, stat, icode, ifun, valC, fwdA(pipeRegs), fwdB(pipeRegs), destE, dstM(dreg),
	   d_srcA, d_srcB);


    return false;
}
/**
 * setEInput
 * 
 * @param ereg - execute reg
 * @param stat - stat value in execute reg
 * @param icode - stat value in execute reg
 * @param ifun - ifun value in execute reg
 * @param valC - updated valC value
 * @param valA - updated valA value
 * @param valB - updated valB value
 * @param destE - updated destE value
 * @param destM - updated destM value
 * @param srcA - updated srcA value
 * @param srcB - updated srcB value
*/
void DecodeStage::setEInput(PipeReg * ereg, uint64_t stat, uint64_t icode,
                           uint64_t ifun, uint64_t valC, uint64_t valA, uint64_t valB,
                           uint64_t destE, uint64_t destM, uint64_t srcA, uint64_t srcB)
{
 
    ereg->set(E_STAT, stat);
    ereg->set(E_ICODE, icode);
    ereg->set(E_IFUN, ifun);
    ereg->set(E_VALC, valC);
    ereg->set(E_VALA, valA);
    ereg->set(E_VALB, valB);
    ereg->set(E_DSTE, destE);
    ereg->set(E_DSTM, destM);
    ereg->set(E_SRCA, srcA);
    ereg->set(E_SRCB, srcB);

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
    PipeReg * ereg = pipeRegs->getExecuteReg(); 
    //ereg->normal();
    if (executeBubble)
    {
        ((E *)ereg)->bubble();
    } 
    else
    {
	    ereg->normal();
    }
}

/**
 * srcA
 * 
 * grabs the correct srcA value based on the icode in the decode stage.
 * 
 * @param dreg - decode register that has the d_icode, and the RA value.
 * @return uint64_t 
*/
uint64_t DecodeStage::srcA(PipeReg * dreg)
{
	uint64_t d_Icode = dreg->get(D_ICODE);
	if (d_Icode == Instruction::IRRMOVQ || d_Icode == Instruction::IRMMOVQ ||
		d_Icode == Instruction::IOPQ || d_Icode == Instruction::IPUSHQ)
	{
		return dreg->get(D_RA);	
	}
	else if (d_Icode == Instruction::IPOPQ || d_Icode == Instruction::IRET)
	{
		return RegisterFile::rsp;
	}
	else
	{

		return RegisterFile::RNONE;
	}
}

/**
 * srcB
 * 
 * grabs the correct srcB value based on the icode in the decode stage.
 * 
 * @param dreg - decode register that has the d_icode, and the RB value.
 * @return uint64_t 
*/
uint64_t DecodeStage::srcB(PipeReg * dreg)
{
	uint64_t d_Icode = dreg->get(D_ICODE);
	if (d_Icode == Instruction::IOPQ || d_Icode == Instruction::IRMMOVQ || 
			d_Icode == Instruction::IMRMOVQ)
	{
		return dreg->get(D_RB);
	}
	else if (d_Icode == Instruction::IPUSHQ || d_Icode == Instruction::IPOPQ ||
			d_Icode == Instruction::ICALL || d_Icode == Instruction::IRET)
	{
		return RegisterFile::rsp;
	}
	else
	{
		return RegisterFile::RNONE;
	}
}

/**
 * dstE
 * 
 * grabs the appropriate destination for to send to the execute stage
 * (if it will be conducted there) based on the icode in the decode stage.
 * 
 * @param dreg - decode register that has the d_icode, and the RB value.
 * @return uint64_t 
*/
uint64_t DecodeStage::dstE(PipeReg * dreg)
{
	uint64_t d_Icode = dreg->get(D_ICODE);
	if (d_Icode == Instruction::IRRMOVQ || d_Icode == Instruction::IIRMOVQ ||
			d_Icode == Instruction::IOPQ)
	{
		return dreg->get(D_RB);
	}
	else if (d_Icode == Instruction::IPUSHQ || d_Icode == Instruction::IPOPQ ||
			d_Icode == Instruction::ICALL || d_Icode == Instruction::IRET)
	{
		return RegisterFile::rsp;
	}
	else 
	{
		return RegisterFile::RNONE;
	}
}

/**
 * dstM
 * 
 * grabs the appropriate destination in memory for to send to the execute stage
 * (if it will be conducted in memory) based on the icode in the decode stage.
 * 
 * @param dreg - decode register that has the d_icode, and the RB value.
 * @return uint64_t 
**/
uint64_t DecodeStage::dstM(PipeReg * dreg)
{
	uint64_t d_Icode = dreg->get(D_ICODE);
	if (d_Icode == Instruction::IMRMOVQ || d_Icode == Instruction::IPOPQ)
	{
		return dreg->get(D_RA);
	}
	else
	{
		return RegisterFile::RNONE;
	}
}

/**
 * fwdA 
 * 
 * grabs the correct value for valA in the execute stage
 * 
 * @param PipeRegs - passes the memory, writeBack, and decode reg.
 * @return uint64_t 
*/
uint64_t DecodeStage::fwdA(PipeRegArray * PipeRegs)
{
	bool error; 
	PipeReg * mreg = PipeRegs->getMemoryReg();
	PipeReg * wreg = PipeRegs->getWritebackReg();
    PipeReg * dreg = PipeRegs->getDecodeReg();
    uint64_t icode = dreg->get(D_ICODE);
	if (icode == Instruction::IJXX || icode == Instruction::ICALL)
    {
        return dreg->get(D_VALP);
    }
    else if (d_srcA == RegisterFile::RNONE)
    {
        return 0;
    }
    else if (d_srcA == e_dstE)
	{
		return e_valE;
	}
    else if (d_srcA == mreg->get(M_DSTM))
    {
        return m_valM;
    }
	else if (d_srcA == mreg->get(M_DSTE))
	{
		return mreg->get(M_VALE);
	}
    else if (d_srcA == wreg->get(W_DSTM))
    {
        return wreg->get(W_VALM);
    }
	else if (d_srcA == wreg->get(W_DSTE))
	{
		return wreg->get(W_VALE);
	}
	else
	{
		return rf->readRegister(d_srcA, error);
	}
}

/**
 * fwdB
 * 
 * grabs the correct value for valB in the execute stage
 * 
 * @param PipeRegs - passes the memory, writeBack, and decode reg.
 * @return uint64_t 
*/
uint64_t DecodeStage::fwdB(PipeRegArray * PipeRegs)
{
	bool error = false;
	PipeReg * mreg = PipeRegs->getMemoryReg();
	PipeReg * wreg = PipeRegs->getWritebackReg();
    
    if (d_srcB == RegisterFile::RNONE)
    {
        return 0;
    }
	else if (d_srcB == e_dstE)
	{
		return e_valE;
	}
    else if (d_srcB == mreg->get(M_DSTM))
    {
        return m_valM;
    }
	else if (d_srcB == mreg->get(M_DSTE))
	{
		return mreg->get(M_VALE);
	}
    else if (d_srcB == wreg->get(W_DSTM))
    {
        return wreg->get(W_VALM);
    }
	else if (d_srcB == wreg->get(W_DSTE))
	{
		return wreg->get(W_VALE);
	}
	else 
	{
		return rf->readRegister(d_srcB, error);
	}
}

/**
 * 
*/
/**
 * f_stall
 * 
 * determines whether fetch needs to stall
 * 
 * @param ereg - allows the use of the execute stage icode and the dstM
 * @return bool
*/
bool DecodeStage::e_bubble(PipeReg * ereg)
{
    uint64_t e_icode = ereg->get(E_ICODE);
    uint64_t e_dstM = ereg->get(E_DSTM);
    return ((e_icode == Instruction::IMRMOVQ || e_icode == Instruction::IPOPQ) 
        && (e_dstM == Stage::d_srcA || e_dstM == Stage::d_srcB)) 
        || (e_icode == Instruction::IJXX && !Stage::e_Cnd);
}
