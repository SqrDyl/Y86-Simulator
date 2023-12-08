//TODO add more #includes as you need them
#include <cstdint>
#include "PipeRegArray.h"
#include "PipeReg.h"
#include "Memory.h"
#include "FetchStage.h"
#include "Instruction.h"
#include "RegisterFile.h"
#include "Status.h"
#include "F.h"
#include "D.h"
#include "M.h"
#include "W.h"
#include "Tools.h"
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
bool FetchStage::doClockLow(PipeRegArray * pipeRegs)
{
   PipeReg * freg = pipeRegs->getFetchReg();
   PipeReg * dreg = pipeRegs->getDecodeReg();
   PipeReg * mreg = pipeRegs->getMemoryReg();
   PipeReg * wreg = pipeRegs->getWritebackReg();
   PipeReg * ereg = pipeRegs->getExecuteReg();
   bool mem_error = false;
   uint64_t icode = Instruction::INOP, ifun = Instruction::FNONE;
   uint64_t valC = 0, valP = 0, stat = Status::SAOK, predPC = 0;
   bool needValC = false;
   bool needRegId = false;

   //TODO: read lab assignment
   //TODO 
   //select PC value and read byte from memory
   //set icode and ifun using byte read from memory
   //uint64_t f_pc =  .... call your select pc function

   //set stat for this instruction to Status::SHLT if
   //icode is Instruction::IHALT; otherwise leave it is
   //as initialized to Status::SAOK

   //TODO
   //In order to calculate the address of the next instruction,
   //you'll need to know whether this current instruction has an
   //immediate field and a register byte. (Look at the instruction encodings.)
   //needvalC =  .... call your need valC function
   //needregId = .... call your need regId function
    uint64_t f_pc = selectPC(freg, mreg, wreg);
    uint64_t rA = RegisterFile::RNONE, rB = RegisterFile::RNONE;
    uint8_t insByte = mem->getByte(f_pc, mem_error);
    icode = Tools::getBits(insByte, 4, 7);
    ifun = Tools::getBits(insByte, 0, 3);
    
    bool validInstr = instr_valid(icode);
    stat = f_stat(validInstr, icode, mem_error);
    
    //Lab10 code
    icode = f_icode(icode, mem_error);
    ifun = f_ifun(ifun, mem_error);
    //????
    //Old code -- do I need this?
    /*if (icode == Instruction::IHALT)
    {
        stat = Status::SHLT;
    }*/

    needRegId = FetchStage::needRegIds(icode);
    needValC = FetchStage::need_valC(icode);
    
   //TODO
   //determine the address of the next sequential function
   //valP = ..... call your PC increment function 
    valP = PCincrement(f_pc, needRegId, needValC);
   
   //TODO
   //calculate the predicted PC value
   //predPC = .... call your function that predicts the next PoC   
    predPC = predictPC(icode, valC, valP);
   //set the input for the PREDPC pipe register field in the F register
    freg->set(F_PREDPC, predPC);
    //Lab7 calls
    getRegs(needRegId, f_pc, rA, rB);

    fetchStall = f_stall(ereg);
    decodeStall = d_stall(ereg);

   //set the inputs for the D register
    setDInput(dreg, stat, icode, ifun, rA, rB, buildValC(needValC, needRegId, f_pc), valP);
    return false;
}

/* doClockHigh
 *
 * applies the appropriate control signal to the F
 * and D register intances
 * 
 * @param: pipeRegs - array of the pipeline register (F, D, E, M, W instances)
*/
void FetchStage::doClockHigh(PipeRegArray * pipeRegs)
{
    PipeReg * freg = pipeRegs->getFetchReg();  
    PipeReg * dreg = pipeRegs->getDecodeReg();
    if (!fetchStall)
    {
        freg->normal();
    }    
    if (!decodeStall)
    {
        dreg->normal();
    }
}

/* setDInput
 * provides the input to potentially be stored in the D register
 * during doClockHigh
 *
 * @param: dreg - pointer to the D register instance
 * @param: stat - value to be stored in the stat pipeline register within D
 * @param: icode - value to be stored in the icode pipeline register within D
 * @param: ifun - value to be stored in the ifun pipeline register within D
 * @param: rA - value to be stored in the rA pipeline register within D
 * @param: rB - value to be stored in the rB pipeline register within D
 * @param: valC - value to be stored in the valC pipeline register within D
 * @param: valP - value to be stored in the valP pipeline register within D
*/
void FetchStage::setDInput(PipeReg * dreg, uint64_t stat, uint64_t icode,
                           uint64_t ifun, uint64_t rA, uint64_t rB,
                           uint64_t valC, uint64_t valP)
{
   dreg->set(D_STAT, stat);
   dreg->set(D_ICODE, icode);
   dreg->set(D_IFUN, ifun);
   dreg->set(D_RA, rA);
   dreg->set(D_RB, rB);
   dreg->set(D_VALC, valC);
   dreg->set(D_VALP, valP);
}

/**
 * selectPC
 * 
 * gets the value for the pc based on the icodes in the writeBack 
 * and the memory regs
 * 
 * @param freg - fetchStage reg for predpc
 * @param mreg - memory reg for icode, valA, and condition value
 * @param wreg - writeBack reg for icode and valM
 * @return uint64_t
*/
uint64_t FetchStage::selectPC(PipeReg * freg, PipeReg * mreg, PipeReg * wreg)
{
    uint64_t M_icode = mreg->get(M_ICODE);
    uint64_t W_icode = wreg->get(W_ICODE);
    uint64_t M_valA = mreg->get(M_VALA);
    uint64_t M_Cnd = mreg->get(M_CND);
    uint64_t W_valM = wreg->get(W_VALM);
    uint64_t pred_pc = freg->get(F_PREDPC);

    if (M_icode == Instruction::IJXX && !M_Cnd)
    {
		return M_valA;
    }
    else if (W_icode == Instruction::IRET) 
    {
		return W_valM;
    }
    else
    {
		return pred_pc;
    }
}
/**
 * needRegIds
 * 
 * determines if you need regIds based off the icode in fetch stage
 * 
 * @param icode - fetch stage icode
 * @return bool
*/
bool FetchStage::needRegIds(uint64_t icode)
{
    //needRegIds  method: input is f_icode
    //bool need_regids = f_icode in { IRRMOVQ, IOPQ, IPUSHQ, IPOPQ, IIRMOVQ, IRMMOVQ, IMRMOVQ };
    //uint64_t num = Tools::getBits(f_icode, 0, 4);

    return ((icode == Instruction::IRRMOVQ) || (icode == Instruction::IOPQ) || (icode == Instruction::IPUSHQ) 
        || (icode == Instruction::IPOPQ) || (icode == Instruction::IIRMOVQ) || (icode == Instruction::IRMMOVQ) 
        || (icode == Instruction::IMRMOVQ));
}

/**
 * needValC
 * 
 * returns 1 if you need valC based off the icode
 * 
 * @param icode - fetch stage icode
 * @return bool 
*/
bool FetchStage::need_valC(uint64_t icode)
{
    //needValC method: input is f_icode
    //bool need_valC = f_icode in { IIRMOVQ, IRMMOVQ, IMRMOVQ, IJXX, ICALL };   
    //uint64_t num = Tools::getBits(f_icode, 0, 4); //will return 0x61

    return (icode == Instruction::IIRMOVQ || icode == Instruction::IRMMOVQ 
        || icode == Instruction::IMRMOVQ || icode == Instruction::IJXX || icode == Instruction::ICALL);
}

/**
 * predictPC
 * 
 * predicts the value of the next PC based on icode, 
 * accounts for jumps and calls
 * 
 * @param icode - operation being fetched right now
 * @param f_valC - fetchStage value that will have the value of the jump/call address
 * @param f_valP - fetchStage value that will have the value of the next operation in the 
 *      .yo file
 * @return uint64_t
*/
uint64_t FetchStage::predictPC(uint64_t icode, uint64_t f_valC, uint64_t f_valP)
{
	//uint64_t num = Tools::getBits(f_icode, 0, 4);
    
	if (icode == Instruction::ICALL || icode == Instruction::IJXX)
	{
		return f_valC;
	}
	else
	{
		return f_valP;
	}
}
/**
 * PCIncrement
 * 
 * determines the amount to increment the PC based on if we need regIds or valC
 * 
 * @param f_pc - current PC
 * @param needRegRes - bool result of needRegIds function
 * @param needValCRes - bool restul of needValC function
 * @return uint64_t
*/
uint64_t FetchStage::PCincrement(uint64_t f_pc, bool needRegRes, bool needValCRes)
{
	// F_PC + length of current intruction (Get length from if needRegRes or needValCRes)
	if (needRegRes == true)
	{
	    f_pc++;
	}
	if (needValCRes == true)
	{
        f_pc += 8;	
	}
	return f_pc + 1; 
	
}


//Lab7
//=============================================================================
/**
 * getRegs
 * 
 * gets the values of regs based on the operation and needRegResult
 * 
 * @param needReg - bool result of needRegIds
 * @param f_pc - current PC
 * @param rA - the address of rA
 * @param rB - the address of rB
*/
void FetchStage::getRegs(bool needReg, uint64_t f_pc, uint64_t &rA, uint64_t &rB)
{
    bool error = false;
    if (needReg)
    {
        uint8_t regs = mem->getByte(f_pc + 1, error);
        rA = Tools::getBits(regs, 4, 7);
        rB = Tools::getBits(regs, 0, 3);
    }
}

/**
 * buildValC 
 * 
 * aligns valC
 * 
 * @param needValC - determines if we need to buildValC
 * @param needRegs - result of needRegIds function
 * @param f_pc - current PC
 * @return uint64_t
*/
uint64_t FetchStage::buildValC(bool needValC, bool needRegs, uint64_t f_pc)
{
    bool error = false;
    if (needValC)
    {
        f_pc++; //move past the instruction and function byte
        uint8_t bytes[8];
        if (needRegs)
        {
            f_pc++;
        }
        for (int i = 0; i < 8; i++)
        {
            bytes[i] = mem->getByte(f_pc, error);
            f_pc++;   
        }
        uint64_t valC = Tools::buildLong(bytes);
        return valC;
    }
    else
    {
        return 0;
    }
}

/**
 * instr_valid
 * 
 * returns true if the instruction is valid
 * 
 * @param icode - current icode
*/
bool FetchStage::instr_valid(uint64_t icode)
{
    return (icode == Instruction::INOP || icode == Instruction::IHALT || icode == Instruction::IRRMOVQ 
    || icode == Instruction::IIRMOVQ || icode == Instruction::IRMMOVQ || icode == Instruction::IMRMOVQ 
    || icode == Instruction::IOPQ || icode == Instruction::IJXX || icode == Instruction::ICALL 
    || icode == Instruction::IRET || icode == Instruction::IPUSHQ || icode == Instruction::IPOPQ);
}

/**
 * f_stat 
 * 
 * updates the status variable if there is a memError
 * or if instructions isn't valid, or halt is hit
 * 
 * @param instrValid - bool result if the current instruction is valid
 * @param icode - current icode
 * @param memError - will occur if error in memory
 * @return uint64_t
*/
uint64_t FetchStage::f_stat(bool instrValid, uint64_t icode, bool memError)
{
    if (memError)
    {
        return Status::SADR;
    }
    else if (!instrValid)
    {
        return Status::SINS;
    }
    else if (icode == Instruction::IHALT)
    {
        return Status::SHLT;
    }
    else 
    {
        return Status::SAOK;
    }
}

/**
 * f_icode
 * 
 * changes icode if there is a memory error
 * 
 * @param icode - current icode
 * @param mem_error - bool if there was a memory error
 * @return uint64_t
*/
uint64_t FetchStage::f_icode(uint64_t icode, bool mem_error)
{
    if (mem_error)
    {
        return Instruction::INOP;
    }
    else 
    {
        return icode;
    }
}
/**
 * f_ifun
 * 
 * changes ifun if there was a memory error
 * 
 * @param ifun - current ifun
 * @param mem_error - bool if there was a memory error
 * @return uint64_t
*/
uint64_t FetchStage::f_ifun(uint64_t ifun, bool mem_error)
{
    if (mem_error)
    {
        return Instruction::FNONE;
    }
    else
    {
        return ifun;
    }
}

/**
 * f_stall
 * 
 * determines whether fetch needs to stall
 * 
 * @param ereg - allows the use of the execute stage icode and the dstM
 * @return bool
*/
bool FetchStage::f_stall(PipeReg * ereg)
{
    uint64_t e_icode = ereg->get(E_ICODE);
    uint64_t e_dstM = ereg->get(E_DSTM);
    return ((e_icode == Instruction::IMRMOVQ || e_icode == Instruction::IPOPQ) 
        && (e_dstM == Stage::d_srcA) || e_dstM == Stage::d_srcB);
}

/**
 * d_stall
 * 
 * determines whether fetch needs to stall
 * 
 * @param ereg - allows the use of the execute stage icode and the dstM
 * @return bool
*/
bool FetchStage::d_stall(PipeReg * ereg)
{
    uint64_t e_icode = ereg->get(E_ICODE);
    uint64_t e_dstM = ereg->get(E_DSTM);
    return ((e_icode == Instruction::IMRMOVQ || e_icode == Instruction::IPOPQ) 
        && (e_dstM == Stage::d_srcA) || e_dstM == Stage::d_srcB);
}
