
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

$(BIN_DIR)/$(SEED_PROGRAM): $(BUILD_DIR)/seed.o
	$(LD) $(BUILD_DIR)/seed.o -o $(BIN_DIR)/$(SEED_PROGRAM)

client: $(BIN_DIR)/$(CLIENT_PROGRAM)
	./$(BIN_DIR)/$(CLIENT_PROGRAM)

$(BIN_DIR)/$(CLIENT_PROGRAM): $(BUILD_DIR)/client.o
	$(LD) $(BUILD_DIR)/client.o -o $(BIN_DIR)/$(CLIENT_PROGRAM)

server: $(BIN_DIR)/$(SERVER_PROGRAM)
	./$(BIN_DIR)/$(SERVER_PROGRAM)

$(BIN_DIR)/$(SERVER_PROGRAM): $(BUILD_DIR)/server.o
	$(LD) $(BUILD_DIR)/server.o -o $(BIN_DIR)/$(SERVER_PROGRAM)

$(BUILD_DIR)/server.o: $(SRC_DIR)/server.c $(INCLUDE_DIR)/util.h $(INCLUDE_DIR)/database.h
	$(CC) -c $(SRC_DIR)/server.c -o $(BUILD_DIR)/server.o

$(BUILD_DIR)/client.o: $(SRC_DIR)/client.c $(INCLUDE_DIR)/util.h
	$(CC) -c $(SRC_DIR)/client.c -o $(BUILD_DIR)/client.o

$(BUILD_DIR)/seed.o: $(SRC_DIR)/seed.c $(INCLUDE_DIR)/util.h $(INCLUDE_DIR)/database.h
	$(CC) -c $(SRC_DIR)/seed.c -o $(BUILD_DIR)/seed.o

build: $(BIN_DIR)/$(PROG_NAME)

$(BIN_DIR)/$(PROG_NAME): $(BUILD_DIR)/turbine.o $(BUILD_DIR)/f16.o $(BUILD_DIR)/planet.o $(BUILD_DIR)/shark.o $(BUILD_DIR)/util/helper.o $(BUILD_DIR)/util/parser_util.o $(BUILD_DIR)/light.o $(BUILD_DIR)/parser.o $(BUILD_DIR)/main.o $(BUILD_DIR)/mesh.o $(BUILD_DIR)/view.o $(BUILD_DIR)/scene_manager.o $(BUILD_DIR)/model.o $(BUILD_DIR)/controller.o $(BUILD_DIR)/platform_mesh.o $(BUILD_DIR)/earth.o $(BUILD_DIR)/texture.o
	$(LD) $(BUILD_DIR)/*.o $(BUILD_DIR)/util/*.o $(CFLAG) -o  $(BIN_DIR)/$(PROG_NAME)

$(BUILD_DIR)/main.o: main.cpp $(INCLUDE_DIR)/$(INCLUDE_DIR)/controller.h $(INCLUDE_DIR)/util/opengl_init.h $(INCLUDE_DIR)/util/helper.h
	$(CC) -c main.cpp -o $(BUILD_DIR)/main.o

clean:
	rm -f $(BIN_DIR)/$(SEED_PROGRAM) $(BIN_DIR)/$(CLIENT_PROGRAM) $(BIN_DIR)/$(SERVER_PROGRAM) $(BUILD_DIR)/*.o
