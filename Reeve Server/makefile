RC=src
TGT=obj
INCLUDES = -Iinclude
CXXFLAGS = -Wall -std=c++11 -g $(INCLUDES)
SOURCES = $(wildcard src/*.cpp)
OBJS = $(addprefix $(TGT)/, $(notdir $(SOURCES:.cpp=.o)))

$(TGT)/%.o: src/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

reeve: $(OBJS)
	$(CXX) $(LDFLAGS) $(OBJS) -o $@

clean:
	@rm -R obj/* reeve
