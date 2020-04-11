MAKEFILE_BUILDER = make -f makefile_builder.mk --no-print-directory


all: framework_binary module_tcpsocket module_connection


TINYXML2_CPP := tinyxml2/tinyxml2.cpp


FRAMEWORK_SOURCES_CPP := 	framework/framework.cpp \
							framework/parsers/FrameworkConfig.cpp \
							framework/parsers/ModuleConfig.cpp \
							framework/parsers/Edge.cpp \
							framework/automaton/Automaton.cpp \
							framework/automaton/AutomatonNode.cpp \
							framework/automaton/Callable.cpp \
							framework/threads/AutomatonThread.cpp \
							framework/threads/AutomatonManager.cpp \
							framework/logger/Logger.cpp \
							$(TINYXML2_CPP)

framework_binary:
	@$(MAKEFILE_BUILDER) CPP_TO_COMPILE="$(FRAMEWORK_SOURCES_CPP)" EXE_TARGET="framework.exe"


MODULE_COMMON_CPP 	:= common/Interface.cpp
MODULE_TCP_SOURCES_CPP := $(MODULE_COMMON_CPP) $(TINYXML2_CPP) moduleTCPSocket/TCPSocket.cpp
MODULE_CONNECTION_CPP := $(MODULE_COMMON_CPP) $(TINYXML2_CPP) moduleConnection/moduleConnection.cpp

module_tcpsocket:
	@$(MAKEFILE_BUILDER) CPP_TO_COMPILE="$(MODULE_TCP_SOURCES_CPP)" EXE_TARGET="TCPSocket.dll" BUILD_AS_DLL="yes"

module_connection:
	@$(MAKEFILE_BUILDER) CPP_TO_COMPILE="$(MODULE_CONNECTION_CPP)" EXE_TARGET="Connection.dll" BUILD_AS_DLL="yes"
	
clean:
	@$(MAKEFILE_BUILDER) clean
