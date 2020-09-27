CORES ?= 6

.PHONY: debug release clear-debug clear-release

debug:
	$(MAKE) -j$(CORES) -f MakeDebug.mk

release:
	$(MAKE) -j$(CORES) -f MakeRelease.mk

clean-debug:
	$(MAKE) -f MakeDebug.mk clean
	$(MAKE) -j$(CORES) -f MakeDebug.mk

clean-release:
	$(MAKE) -f MakeRelease.mk clean
	$(MAKE) -j$(CORES) -f MakeRelease.mk