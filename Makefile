
BUILD_DIR   = ./build
BIN_DIR     = ./bin
INCLUDE_DIR = ./include
SRC_DIR = ./src

CC          = gcc -I $(INCLUDE_DIR)
LD          = gcc -I $(INCLUDE_DIR)

SEED_PROGRAM = seed
CLIENT_PROGRAM = client
SERVER_PROGRAM = server


CFLAG = -pthread

run: $(BIN_DIR)/seed

seed: $(BIN_DIR)/$(SEED_PROGRAM)
	./$(BIN_DIR)/$(SEED_PROGRAM)

$(BIN_DIR)/$(SEED_PROGRAM): $(SRC_DIR)/seed.c  $(INCLUDE_DIR)/util.h $(INCLUDE_DIR)/database.h
	$(CC) $(SRC_DIR)/seed.c -o $(BIN_DIR)/$(SEED_PROGRAM)

client: $(BIN_DIR)/$(CLIENT_PROGRAM)
	./$(BIN_DIR)/$(CLIENT_PROGRAM)

$(BIN_DIR)/$(CLIENT_PROGRAM): $(SRC_DIR)/client.c  $(INCLUDE_DIR)/util.h $(INCLUDE_DIR)/database.h
	$(CC) $(SRC_DIR)/client.c -o $(BIN_DIR)/$(CLIENT_PROGRAM)

server: $(BIN_DIR)/$(SERVER_PROGRAM)
	./$(BIN_DIR)/$(SERVER_PROGRAM)

$(BIN_DIR)/$(SERVER_PROGRAM): $(SRC_DIR)/server.c  $(INCLUDE_DIR)/util.h $(INCLUDE_DIR)/database.h
	$(CC) $(SRC_DIR)/server.c -o $(BIN_DIR)/$(SERVER_PROGRAM)



build: $(BIN_DIR)/$(PROG_NAME)

$(BIN_DIR)/$(PROG_NAME): $(BUILD_DIR)/turbine.o $(BUILD_DIR)/f16.o $(BUILD_DIR)/planet.o $(BUILD_DIR)/shark.o $(BUILD_DIR)/util/helper.o $(BUILD_DIR)/util/parser_util.o $(BUILD_DIR)/light.o $(BUILD_DIR)/parser.o $(BUILD_DIR)/main.o $(BUILD_DIR)/mesh.o $(BUILD_DIR)/view.o $(BUILD_DIR)/scene_manager.o $(BUILD_DIR)/model.o $(BUILD_DIR)/controller.o $(BUILD_DIR)/platform_mesh.o $(BUILD_DIR)/earth.o $(BUILD_DIR)/texture.o
	$(LD) $(BUILD_DIR)/*.o $(BUILD_DIR)/util/*.o $(CFLAG) -o  $(BIN_DIR)/$(PROG_NAME)

$(BUILD_DIR)/main.o: main.cpp $(INCLUDE_DIR)/$(INCLUDE_DIR)/controller.h $(INCLUDE_DIR)/util/opengl_init.h $(INCLUDE_DIR)/util/helper.h
	$(CC) -c main.cpp -o $(BUILD_DIR)/main.o

clean:
	rm -f $(BIN_DIR)/$(SEED_PROGRAM) $(BIN_DIR)/$(CLIENT_PROGRAM) $(BIN_DIR)/$(SERVER_PROGRAM) $(BUILD_DIR)/*.o

$(BUILD_DIR)/util/parser_util.o: util/parser_util.cpp  $(INCLUDE_DIR)/util/parser_util.h
	$(CC) -c util/parser_util.cpp  -o $(BUILD_DIR)/util/parser_util.o

$(BUILD_DIR)/util/helper.o: util/helper.cpp  $(INCLUDE_DIR)/util/helper.h
	$(CC) -c util/helper.cpp  -o $(BUILD_DIR)/util/helper.o

$(BUILD_DIR)/parser.o: parser.cpp $(INCLUDE_DIR)/util/mesh_metadata.h $(INCLUDE_DIR)/util/trace.h $(INCLUDE_DIR)/util/parser_util.h $(INCLUDE_DIR)/parser.h
	$(CC) -c parser.cpp  -o $(BUILD_DIR)/parser.o

$(BUILD_DIR)/view.o: view.cpp $(INCLUDE_DIR)/view.h $(INCLUDE_DIR)/mesh.h $(INCLUDE_DIR)/scene_manager.h $(INCLUDE_DIR)/util/helper.h $(INCLUDE_DIR)/camera.h
	$(CC) -c view.cpp  -o $(BUILD_DIR)/view.o

$(BUILD_DIR)/model.o: model.cpp $(INCLUDE_DIR)/model.h $(INCLUDE_DIR)/mesh.h
	$(CC) -c model.cpp  -o $(BUILD_DIR)/model.o

$(BUILD_DIR)/light.o: light.cpp $(INCLUDE_DIR)/model.h $(INCLUDE_DIR)/light.h
	$(CC) -c light.cpp  -o $(BUILD_DIR)/light.o

$(BUILD_DIR)/earth.o: earth.cpp $(INCLUDE_DIR)/model.h $(INCLUDE_DIR)/earth.h
	$(CC) -c earth.cpp  -o $(BUILD_DIR)/earth.o

$(BUILD_DIR)/planet.o: planet.cpp $(INCLUDE_DIR)/model.h $(INCLUDE_DIR)/planet.h
	$(CC) -c planet.cpp  -o $(BUILD_DIR)/planet.o

$(BUILD_DIR)/f16.o: f16.cpp $(INCLUDE_DIR)/model.h $(INCLUDE_DIR)/f16.h
	$(CC) -c f16.cpp  -o $(BUILD_DIR)/f16.o

$(BUILD_DIR)/shark.o: earth.cpp $(INCLUDE_DIR)/model.h $(INCLUDE_DIR)/shark.h
	$(CC) -c shark.cpp  -o $(BUILD_DIR)/shark.o

$(BUILD_DIR)/turbine.o: turbine.cpp $(INCLUDE_DIR)/model.h $(INCLUDE_DIR)/turbine.h
	$(CC) -c turbine.cpp  -o $(BUILD_DIR)/turbine.o

$(BUILD_DIR)/mesh.o: $(INCLUDE_DIR)/mesh.h mesh.cpp $(INCLUDE_DIR)/parser.h
	$(CC) -c mesh.cpp  -o $(BUILD_DIR)/mesh.o

$(BUILD_DIR)/platform_mesh.o: $(INCLUDE_DIR)/platform_mesh.h platform_mesh.cpp $(INCLUDE_DIR)/parser.h $(INCLUDE_DIR)/mesh.h
	$(CC) -c platform_mesh.cpp  -o $(BUILD_DIR)/platform_mesh.o

$(BUILD_DIR)/texture.o: $(INCLUDE_DIR)/texture.h texture.cpp
	$(CC) -c texture.cpp  -o $(BUILD_DIR)/texture.o

$(BUILD_DIR)/scene_manager.o: $(INCLUDE_DIR)/scene_manager.h scene_manager.cpp $(INCLUDE_DIR)/mesh.h $(INCLUDE_DIR)/model.h $(INCLUDE_DIR)/util/helper.h $(INCLUDE_DIR)/texture.h $(INCLUDE_DIR)/camera.h
	$(CC) -c scene_manager.cpp  -o $(BUILD_DIR)/scene_manager.o

$(BUILD_DIR)/controller.o: $(INCLUDE_DIR)/controller.h controller.cpp $(INCLUDE_DIR)/mesh.h $(INCLUDE_DIR)/model.h $(INCLUDE_DIR)/view.h $(INCLUDE_DIR)/scene_manager.h
	$(CC) -c controller.cpp  -o $(BUILD_DIR)/controller.o
