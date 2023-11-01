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

$(OBJD)/CPU.o: $(INC)/CPU.h $(INC)/PipeRegArray.h $(INC)/StageArray.h $(INC)/Memory.h\
 $(INC)/ConditionCodes.h $(INC)/RegisterFile.h

$(OBJD)/D.o: $(INC)/D.h $(INC)/Instruction.h $(INC)/RegisterFile.h $(INC)/PipeRegField.h\
 $(INC)/Status.h

$(OBJD)/E.o: $(INC)/E.h $(INC)/Instruction.h $(INC)/RegisterFile.h $(INC)/PipeRegField.h\
 $(INC)/Status.h

$(OBJD)/F.o: $(INC)/F.h $(INC)/PipeRegField.h

$(OBJD)/M.o: $(INC)/M.h $(INC)/Instruction.h $(INC)/RegisterFile.h $(INC)/PipeRegField.h\
 $(INC)/Status.h

$(OBJD)/W.o: $(INC)/W.h $(INC)/Instruction.h $(INC)/RegisterFile.h $(INC)/PipeRegField.h\
 $(INC)/Status.h

$(OBJD)/FetchStage.o: $(INC)/PipeRegArray.h $(INC)/PipeReg.h $(INC)/Memory.h \
 $(INC)/FetchStage.h $(INC)/Instruction.h $(INC)/RegisterFile.h $(INC)/Status.h \
 $(INC)/F.h $(INC)/D.h

$(OBJD)/DecodeStage.o: $(INC)/DecodeStage.h $(INC)/PipeRegArray.h

$(OBJD)/ExecuteStage.o: $(INC)/ExecuteStage.h $(INC)/PipeRegArray.h

$(OBJD)/MemoryStage.o: $(INC)/MemoryStage.h $(INC)/PipeRegArray.h

$(OBJD)/WritebackStage.o: $(INC)/WritebackStage.h $(INC)/PipeRegArray.h

$(OBJD)/PipeRegArray.o: $(INC)/PipeReg.h $(INC)/PipeRegArray.h $(INC)/F.h $(INC)/D.h \
 $(INC)/E.h $(INC)/M.h $(INC)/W.h

$(OBJD)/PipeReg.o: $(INC)/PipeReg.h

$(OBJD)/PipeRegField.o: $(INC)/PipeRegField.h

$(OBJD)/StageArray.o: $(INC)/StageArray.h $(INC)/Stage.h $(INC)/FetchStage.h \
 $(INC)/DecodeStage.h $(INC)/ExecuteStage.h $(INC)/MemoryStage.h $(INC)/WritebackStage.h

$(OBJD)/Stage.o: $(INC)/Memory.h $(INC)/RegisterFile.h $(INC)/ConditionCodes.h \
 $(INC)/Stage.h




yess: $(LAB4SRCOBJS)
	$(CC) $(LAB4SRCOBJS) -o $(BIN)/yess

clean:
	rm -f *.o
