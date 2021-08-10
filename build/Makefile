include .env

CFLAGS = -std=c++17 -I. -I$(TINYOBJ_PATH)
LDFLAGS = -lglfw -lvulkan -ldl -lpthread -lX11 -lXxf86vm -lXrandr -lXi

# create list of all spv files and set as dependency
vertSources = $(shell find ../shaders -type f -name "*.vert")
vertObjFiles = $(patsubst %.vert, %.vert.spv, $(vertSources))
fragSources = $(shell find ../shaders -type f -name "*.frag")
fragObjFiles = $(patsubst %.frag, %.frag.spv, $(fragSources))

TARGET = VulkanTest
$(TARGET): $(vertObjFiles) $(fragObjFiles)
$(TARGET): ../src/*.cpp ../inc/*.hpp
	g++ $(CFLAGS) -o $(TARGET) ../src/*.cpp $(LDFLAGS)

# make shader targets
%.spv: %
	${GLSLC} $< -o $@

.PHONY: test clean

test: VulkanTest
	./VulkanTest

clean:
	rm -f VulkanTest
	rm -f ../shaders/*.spv
