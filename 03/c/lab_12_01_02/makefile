PWD := .
LIB := lib
SRC := src
LIBNAME := libarray.so
BUILD_TYPE := debug
RUN_STATE_EXT := runstate

.PHONY: run-release
run-release: BUILD_TYPE = release
run-release: | $(PWD)/.release.$(RUN_STATE_EXT) $(LIBNAME)
	export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:.; python3 $(SRC)/main.py

.PHONY: run-debug
run-debug: BUILD_TYPE = debug
run-debug: | $(PWD)/.debug.$(RUN_STATE_EXT) $(LIBNAME)
	export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:.; python3 $(SRC)/main.py

.PHONY: $(LIB)/$(LIBNAME)
$(LIB)/$(LIBNAME):
	cd $(LIB) && make $(BUILD_TYPE)

$(LIBNAME): $(LIB)/$(LIBNAME)
	cp $(LIB)/$@ ./

.PHONY: clean-all
clean-all:
	rm -f $(PWD)/.*.$(RUN_STATE_EXT)
	rm -f *.so
	cd $(LIB) && make clean


$(PWD)/.%.$(RUN_STATE_EXT):
	rm -f $(PWD)/.*.$(RUN_STATE_EXT)
	touch $@
	rm -f *.so