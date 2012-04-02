################################################################################
# Global Makefile (both for caravi and for pcavi)
################################################################################

PCAVI_SRC_PATH := ./src/pcavi/
CARAVI_SRC_PATH:= ./src/caravi/

all: pcavi caravi

pcavi:
	cd $(PCAVI_SRC_PATH); make

caravi:
	cd $(CARAVI_SRC_PATH); make

clean:
	cd $(PCAVI_SRC_PATH); make clean
	cd $(CARAVI_SRC_PATH); make clean