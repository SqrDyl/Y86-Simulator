#include "PipeRegArray.h"
#include "DecodeStage.h"
#include "Instruction.h"
#include "Status.h"
#include "D.h"
#include "E.h"


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

	uint64_t srcAR = srcA(dreg);
	uint64_t srcBR = srcB(dreg);
	
    setEInput(ereg, stat, icode, ifun, valC, 0, 0, dstE(dreg), dstM(dreg),
	    fwdA(dreg, srcAR), fwdB(dreg, srcBR));


    return false;
}

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
	ereg->normal();
}


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
		return dreg->get(RegisterFile::rsp);
	}
	else
	{

		return RegisterFile::RNONE;
	}
}

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
		return dreg->get(RegisterFile::rsp);
	}
	else
	{
		return RegisterFile::RNONE;
	}
}

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
		return dreg->get(RegisterFile::rsp);
	}
	else 
	{
		return RegisterFile::RNONE;
	}
}

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

uint64_t DecodeStage::fwdA(PipeReg * dreg, uint64_t srcA1)
{
	return dreg->get(D_RA);
}

uint64_t DecodeStage::fwdB(PipeReg * dreg, uint64_t srcB1)
{
	return dreg->get(D_RB);
}