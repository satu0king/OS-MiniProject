
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

$(BIN_DIR)/$(PROG_NAME): $(BUILD_DIR)/turbine.o $(BUILD_DIR)/f16.o $(BUILD_DIR)/planet.o $(BUILD_DIR)/shark.o $(BUILD_DIR)/util/helper.o $(BUILD_DIR)/util/parser_util.o $(BUILD_DIR)/light.o $(BUILD_DIR)/parser.o $(BUILD_DIR)/main.o $(BUILD_DIR)/mesh.o $(BUILD_DIR)/view.o $(BUILD_DIR)/scene_manager.o $(BUILD_DIR)/model.o $(BUILD_DIR)/controller.o $(BUILD_DIR)/platform_mesh.o $(BUILD_DIR)/earth.o $(BUILD_DIR)/texture.o
	$(LD) $(BUILD_DIR)/*.o $(BUILD_DIR)/util/*.o $(CFLAG) -o  $(BIN_DIR)/$(PROG_NAME)

$(BUILD_DIR)/main.o: main.cpp $(INCLUDE_DIR)/$(INCLUDE_DIR)/controller.h $(INCLUDE_DIR)/util/opengl_init.h $(INCLUDE_DIR)/util/helper.h
	$(CC) -c main.cpp -o $(BUILD_DIR)/main.o

clean:
	rm -f $(BIN_DIR)/$(SEED_PROGRAM) $(BIN_DIR)/$(CLIENT_PROGRAM) $(BIN_DIR)/$(SERVER_PROGRAM) $(BUILD_DIR)/*.o
