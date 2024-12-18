.PHONY: filedb networkdb

default: filedb networkdb

filedb:
	$(MAKE) -C filedb

networkdb: filedb
	$(MAKE) -C networkdb

clean:
	$(MAKE) -C filedb clean
	$(MAKE) -C networkdb clean
