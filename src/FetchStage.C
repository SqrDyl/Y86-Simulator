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
   bool mem_error = false;
   uint64_t icode = Instruction::INOP, ifun = Instruction::FNONE;
   uint64_t rA = RegisterFile::RNONE, rB = RegisterFile::RNONE;
   uint64_t valC = 0, valP = 0, stat = Status::SAOK, predPC = 0;
   bool needvalC = false;
   bool needregId = false;

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

    uint8_t insByte = mem->getByte(f_pc, mem_error);
    icode = Tools::getBits(insByte, 0, 4);
    ifun = Tools::getBits(insByte, 5, 9);

    if (icode == Instruction::IHALT)
    {
        stat = Status::SHLT;
    }
    bool needRegRes = FetchStage::needRegIds(icode);
    bool needValRes = FetchStage::need_valC(icode);
    
   //TODO
   //determine the address of the next sequential function
   //valP = ..... call your PC increment function 
   valP = PCincrement(f_pc, needRegRes, needValRes);
   
   //TODO
   //calculate the predicted PC value
   //predPC = .... call your function that predicts the next PoC   
   uint64_t predicted = predictPC(icode, valC, valP);

   //set the input for the PREDPC pipe register field in the F register
   freg->set(F_PREDPC, predicted);

   //set the inputs for the D register
   setDInput(dreg, stat, icode, ifun, rA, rB, valC, valP);
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
   freg->normal();
   dreg->normal();
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

uint64_t FetchStage::selectPC(PipeReg * freg, PipeReg * mreg, PipeReg * wreg)
{
    uint64_t M_icode = mreg->get(M_ICODE);
    uint64_t W_icode = wreg->get(W_ICODE);
    uint64_t M_valA = mreg->get(M_VALA);
    uint64_t M_Cnd = mreg->get(M_CND);
    uint64_t W_valM = wreg->get(W_VALM);
    //uint64_t F_predPC = freg->get(F_PREDPC);
    if (M_icode == Tools::getByte(M_icode, 0) && !M_Cnd)
    {
        freg->set(F_PREDPC, M_valA);
		return freg->get(F_PREDPC);
    }
    else if (W_icode == Tools::getByte(W_icode, 0))
    {
        freg->set(F_PREDPC, W_valM);
		return freg->get(F_PREDPC);
    }
    else
    {
        freg->set(F_PREDPC, 1);
		return freg->get(F_PREDPC);
    }
    //Uncomment this block
    /*word f_pc = [
    M_icode == IJXX && !M_Cnd : M_valA;
    W_icode == IRET : W_valM;
    1: F_predPC;
    ];*/

}

bool FetchStage::needRegIds(uint64_t f_icode)
{
    //needRegIds  method: input is f_icode
    //bool need_regids = f_icode in { IRRMOVQ, IOPQ, IPUSHQ, IPOPQ, IIRMOVQ, IRMMOVQ, IMRMOVQ };
    uint64_t num = Tools::getBits(f_icode, 0, 4);
    return ((num == Instruction::IRRMOVQ) || (num == Instruction::IOPQ) || (num == Instruction::IPUSHQ) 
        || (num == Instruction::IPOPQ) || (num == Instruction::IIRMOVQ) || (num == Instruction::IRMMOVQ) 
        || (num == Instruction::IMRMOVQ));
}

bool FetchStage::need_valC(uint64_t f_icode)
{
    //needValC method: input is f_icode
    //bool need_valC = f_icode in { IIRMOVQ, IRMMOVQ, IMRMOVQ, IJXX, ICALL };   
    uint64_t num = Tools::getBits(f_icode, 0, 4); //will return 0x61
    return (num == Instruction::IIRMOVQ || num == Instruction::IRMMOVQ 
        || num == Instruction::IMRMOVQ || num == Instruction::IJXX || num == Instruction::ICALL);
}


uint64_t FetchStage::predictPC(uint64_t f_icode, uint64_t f_valC, uint64_t f_valP)
{
	uint64_t num = Tools::getBits(f_icode, 0, 4);
	if (num == Instruction::ICALL || num == Instruction::IJXX)
	{
		return f_valC;
	}
	else
	{
		return f_valP;
	}
}

uint64_t FetchStage::PCincrement(uint64_t f_pc, bool needRegRes, bool needValCRes)
{
	// F_PC + length of current intruction (Get length from if needRegRes or needValCRes)
	if (needRegRes == true)
	{
		f_pc += 2;
	}
	if (needValCRes == true)
	{
        f_pc += 8;	
	}
	return f_pc++; 
	
}
//TODO
//Write your selectPC, needRegIds, needValC, PC increment, and predictPC methods
//Remember to add declarations for these to FetchStage.h

// Here is the HCL describing the behavior for some of these methods. 
/*

//selectPC method: input is F, M, and W registers
word f_pc = [
    M_icode == IJXX && !M_Cnd : M_valA;
    W_icode == IRET : W_valM;
    1: F_predPC;
];

//needRegIds  method: input is f_icode
bool need_regids = f_icode in { IRRMOVQ, IOPQ, IPUSHQ, IPOPQ, IIRMOVQ, IRMMOVQ, IMRMOVQ };



//needValC method: input is f_icode
bool need_valC = f_icode in { IIRMOVQ, IRMMOVQ, IMRMOVQ, IJXX, ICALL };

//predictPC method: inputs are f_icode, f_valC, f_valP
word f_predPC = [
    f_icode in { IJXX, ICALL } : f_valC;
    1: f_valP;
];

*/


