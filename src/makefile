#makefile to compile the .C files in this directory
#and write the .o output to the ../obj directory
CC = g++
INC = ../inc
BIN = ../bin
OBJD = ../obj
CFLAGS = -g -c -Wall -std=c++11 -I $(INC)

LAB4SRCOBJS = $(OBJD)/Memory.o $(OBJD)/Tools.o $(OBJD)/RegisterFile.o $(OBJD)/ConditionCodes.o $(OBJD)/yess.o \
	$(OBJD)/String.o $(OBJD)/Loader.o
#rule to match .C with object in another directory
$(OBJD)/%.o: %.C 
	@mkdir -p $(OBJD)
	$(CC) $(CFLAGS) $< -o $(OBJD)/$@

# 1) need a macro for the .o files corresponding to the .C 
#    files in this directory (note they will be stored in the ../obj
#    directory: OBJS = ....
# 2) need a target (with a name that doesn't match a file; for example, all:)
#    that is dependent upon the .o files. With this target, you can type:
#    make all
#    and all of the .C files will be compiled. 
#    Note: all is a phony target.  It doesn't correspond to file.  
#    No executable is created. We need it just to be able to compile 
#    all of the .C files with one make command.
# 3) need a .o target for each .C
#    Remember each of these is dependent upon the .h files included by the .C
# 4) need a clean target to delete the .o files corresponding to
#    the .C files in this directory from the ../obj directory


compile: $(LAB4SRCOBJS)

$(OBJD)/ConditionCodes.o: $(INC)/Tools.h $(INC)/ConditionCodes.h

$(OBJD)/Memory.o: $(INC)/Memory.h

$(OBJD)/RegisterFile.o: $(INC)/RegisterFile.h

$(OBJD)/Tools.o: $(INC)/Tools.h

$(OBJD)/yess.o: $(INC)/Memory.h $(INC)/Loader.h

$(OBJD)/Loader.o: $(INC)/Memory.h $(INC)/String.h $(INC)/Loader.h

$(OBJD)/String.o: $(INC)/String.h

$(OBJD)/PipeReg.o: $(inc)/PipeReg.h

$(OBJD)/PipeRegArray.o: $(inc)/PipeReg.h $(inc)/PipeRegArray.h $(inc)/F.h $(inc)/D.h $(inc)/E.h $(inc)/M.h $(inc)/W.h 

$(OBJD)/PipeRegField.o: $(inc)/PipeRegField.h

$(OBJD)/Stage.o: $(inc)/Memory.h $(inc)/RegisterFile.h $(inc)/ConditionCodes.h $(inc)/Stage.h

$(OBJD)/FetchStage.o: $(inc)/PipeRegArray.h $(inc)/PipeReg.h $(inc)/Memory.h $(inc)/FetchStage.h $(inc)/Instruction.h $(inc)/RegisterFile.h $(inc)/Status.h $(inc)/F.h $(inc)/D.h

$(OBJD)/DecodeStage.o: $(inc)/PipeRegArray.h $(inc)/DecodeStage.h

$(OBJD)/ExecuteStage.o: $(inc)/PipeRegArray.h $(inc)/ExecuteStage.h

$(OBJD)/MemoryStage.o: $(inc)/PipeRegArray.h $(inc)/MemoryStage.h

$(OBJD)/WritebackStage.o: $(inc)/PipeRegArray.h $(inc)/WritebackStage.h

$(OBJD)/CPU.o: $(inc)/CPU.h $(inc)/PipeRegArray.h $(inc)/StageArray.h $(inc)/Memory.h $(inc)/ConditionCodes.h $(inc)/RegisterFile.h

$(OBJD)/F.o: $(inc)/PipeRegField.h $(inc)/F.h 

$(OBJD)/D.o: $(inc)/Instruction.h $(inc)/RegisterFile.h $(inc)/PipeRegField.h $(inc)/D.h $(inc)/Status.h

$(OBJD)/E.o: $(inc)/RegisterFile.h $(inc)/Instruction.h $(inc)/PipeRegField.h $(inc)/E.h $(inc)/Status.h

$(OBJD)/M.o: $(inc)/RegisterFile.h $(inc)/Instruction.h $(inc)/PipeRegField.h $(inc)/M.h $(inc)/Status.h

$(OBJD)/W.o: $(inc)/RegisterFile.h $(inc)/Instruction.h $(inc)/PipeRegField.h $(inc)/W.h $(inc)/Status.h



yess: $(LAB4SRCOBJS)
	$(CC) $(LAB4SRCOBJS) -o $(BIN)/yess

clean:
	rm -f *.o
