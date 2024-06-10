SRC := ./src
BIN := ./bin
OBJS := ./objs
INC := -I ./include
FLAGS := -c $(INC)
LINK := 
CC := gcc

TESTS := ./tests
TEST_TARGETS := 

OBJ := lexer parser LList

lc: $(addprefix $(OBJS)/,$(addsuffix .o,$(OBJ)))
	$(CC) $^ $(SRC)/lc.c -o $(BIN)/$@ $(LINK)

$(OBJS)/%.o: $(SRC)/%.c
	$(CC) $(FLAGS) $< -o $@

debug:
	$(CC) $(INC) $(SRC)/*.c -pthread -g -o $(BIN)/db $(LINK)
	gdb -tui $(BIN)/db


#Testing
test: all_tests
	@for target in $(TEST_TARGETS); do \
		$(TESTS)/bin/$$target; \
	done

all_tests: $(addprefix $(TESTS)/bin/, $(TEST_TARGETS))

$(TESTS)/bin/%_test: ../testLibC/utestC.c $(TESTS)/%_test.c $(OBJ)
	$(CC) $(INC) $^ $(LINK) -g -o $@

clean:
	rm -rf $(OBJS)/*.o $(BIN)/*
	rm -rf $(TESTS)/bin/*

run:
	$(BIN)/lc test.lep

