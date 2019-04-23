
BUILD_DIR   = ./build
BIN_DIR     = ./bin
INCLUDE_DIR = ./include
SRC_DIR = ./src

CC          = gcc -I $(INCLUDE_DIR)
LD          = gcc -I $(INCLUDE_DIR)

SEED_PROGRAM = seed
CLIENT_PROGRAM = client
SERVER_PROGRAM = server

SOURCES = $(wildcard $(SRC_DIR)/*.c)

CFLAG = -pthread

run: $(BIN_DIR)/$(SEED_PROGRAM) $(BIN_DIR)/$(CLIENT_PROGRAM) $(BIN_DIR)/$(SERVER_PROGRAM)

seed: $(BIN_DIR)/$(SEED_PROGRAM)
	./$(BIN_DIR)/$(SEED_PROGRAM)

client: $(BIN_DIR)/$(CLIENT_PROGRAM)
	./$(BIN_DIR)/$(CLIENT_PROGRAM)

server: $(BIN_DIR)/$(SERVER_PROGRAM)
	./$(BIN_DIR)/$(SERVER_PROGRAM)

$(BIN_DIR)/$(SEED_PROGRAM): $(BUILD_DIR)/seed.o $(BUILD_DIR)/util.o $(BUILD_DIR)/database.o
	$(LD) $(BUILD_DIR)/seed.o $(BUILD_DIR)/util.o $(BUILD_DIR)/database.o -o $(BIN_DIR)/$(SEED_PROGRAM)

$(BIN_DIR)/$(CLIENT_PROGRAM): $(BUILD_DIR)/client.o $(BUILD_DIR)/util.o
	$(LD) $(BUILD_DIR)/client.o $(BUILD_DIR)/util.o -o $(BIN_DIR)/$(CLIENT_PROGRAM)

$(BIN_DIR)/$(SERVER_PROGRAM): $(BUILD_DIR)/server.o $(BUILD_DIR)/util.o $(BUILD_DIR)/database.o
	$(LD) $(CFLAG) $(BUILD_DIR)/server.o $(BUILD_DIR)/util.o $(BUILD_DIR)/database.o -o $(BIN_DIR)/$(SERVER_PROGRAM)


$(BUILD_DIR)/server.o: $(SRC_DIR)/server.c $(INCLUDE_DIR)/util.h $(INCLUDE_DIR)/datastructures.h $(INCLUDE_DIR)/constants.h $(INCLUDE_DIR)/database.h $(INCLUDE_DIR)/server.h
	$(CC) -c $(SRC_DIR)/server.c -o $(BUILD_DIR)/server.o

$(BUILD_DIR)/client.o: $(SRC_DIR)/client.c $(INCLUDE_DIR)/util.h $(INCLUDE_DIR)/datastructures.h $(INCLUDE_DIR)/constants.h  $(INCLUDE_DIR)/client.h
	$(CC) -c $(SRC_DIR)/client.c -o $(BUILD_DIR)/client.o

$(BUILD_DIR)/seed.o: $(SRC_DIR)/seed.c $(INCLUDE_DIR)/util.h $(INCLUDE_DIR)/datastructures.h $(INCLUDE_DIR)/constants.h $(INCLUDE_DIR)/database.h
	$(CC) -c $(SRC_DIR)/seed.c -o $(BUILD_DIR)/seed.o

$(BUILD_DIR)/util.o: $(SRC_DIR)/util.c $(INCLUDE_DIR)/util.h $(INCLUDE_DIR)/datastructures.h $(INCLUDE_DIR)/constants.h
	$(CC) -c $(SRC_DIR)/util.c -o $(BUILD_DIR)/util.o

$(BUILD_DIR)/database.o: $(SRC_DIR)/util.c $(INCLUDE_DIR)/database.h $(INCLUDE_DIR)/util.h $(INCLUDE_DIR)/datastructures.h $(INCLUDE_DIR)/constants.h
	$(CC) -c $(SRC_DIR)/database.c -o $(BUILD_DIR)/database.o

build: $(BIN_DIR)/$(PROG_NAME)

clean:
	rm -f $(BIN_DIR)/$(SEED_PROGRAM) $(BIN_DIR)/$(CLIENT_PROGRAM) $(BIN_DIR)/$(SERVER_PROGRAM) $(BUILD_DIR)/*.o
