# Program Name
PROGRAM_NAME = ap.dll # lb1mod.dll

# Compiler
CXX = i686-w64-mingw32-g++ -m32 -DASIO_STANDALONE -DWSWRAP_NO_SSL -DWSWRAP_NO_COMPRESSION -DAPCLIENT_DEBUG -DWSWRAP_SEND_EXCEPTIONS

# Directories
SRC_DIR = ./src
BIN_DIR = ./bin
OBJ_DIR = ./obj
DEP_DIR = ./obj
RELEASE_DIR = ./
INCLUDE_DIR = inc
LIBS = -lws2_32
INCLUDES = \
	-Iinc/apclientpp \
	-Iinc/asio/include  \
	-Iinc/websocketpp \
	-Iinc/wswrap/include \
	-Iinc/json/include \
	-Iinc/valijson/include 
# Bools


# Build modes
DEBUG_FLAGS = $(INCLUDES) -shared -static-libgcc -static-libstdc++ -static $(LIBS) #-static-libgcc -static-libstdc++ -Wl,--subsystem,windows #-Wall -Wextra -g -I$(INCLUDE_DIR) -std=c++17 -MMD -MP
RELEASE_FLAGS = -Wall -Wextra -O3 -DNDEBUG -I$(INCLUDE_DIR) -std=c++17 -static -MMD -MP
DLL_FLAGS = -shared


# Find all source files
SRC_FILES = $(wildcard $(SRC_DIR)/*.cpp)
OBJ_FILES = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRC_FILES))
DEP_FILES = $(OBJ_FILES:.o=.d)  # Dependency files

# Default to debug mode
BUILD_TYPE ?= debug

ifeq ($(BUILD_TYPE), release)
    CXXFLAGS = $(RELEASE_FLAGS)
    FINAL_DIR = $(RELEASE_DIR)
else
    CXXFLAGS = $(DEBUG_FLAGS)
    FINAL_DIR = $(BIN_DIR)
endif

EXECUTABLE = $(FINAL_DIR)/$(PROGRAM_NAME)

# Default rule
all: clean_unused $(EXECUTABLE)

# Linking
$(EXECUTABLE): $(OBJ_FILES) | $(BIN_DIR) $(RELEASE_DIR)
	@echo "linking."
	$(CXX) -shared $(OBJ_FILES) -o $(EXECUTABLE) $(LIBS)

# Compilation rule with dependency generation
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	@echo "generating dependencies."
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Include generated dependency files
-include $(DEP_FILES)

# Ensure necessary directories exist
$(BIN_DIR) $(OBJ_DIR) $(RELEASE_DIR):
	@echo "adding folders."
	mkdir -p $@

# Clean rule
clean:
	rm -rf $(OBJ_DIR)/*.o $(OBJ_DIR)/*.d $(BIN_DIR)/${PROGRAM_NAME} $(RELEASE_DIR)/${PROGRAM_NAME}

# Remove orphaned .o files (ones not matching known sources)
clean_unused:
	@for obj in $(OBJ_DIR)/*.o; do \
		src="$(SRC_DIR)/$$(basename $$obj .o).cpp"; \
		if [ ! -f "$$src" ]; then \
			echo "Removing orphaned object file: $$obj"; \
			rm -f "$$obj"; \
		fi; \
	done
	rm -rf $(FINAL_DIR)/${PROGRAM_NAME}.exe

.PHONY: all clean clean_unused