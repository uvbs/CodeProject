include version

PLATFORM := $(shell uname)
PLATFORM_VERSION := $(shell uname -r| awk -F 'el' '{printf("%d", substr($$2,1,1))}')

all: 
	$(MAKE) -C src

clean:
	$(MAKE) clean -C src

PACKAGE_DIRS=lib include/pluma
PACKAGE_NAME=$(LIB_NAME)-$(LIB_VERSION)

install: all
	@if [ -z "$(LIB_VERSION)" ]; then \
		echo "ERROR: LIB_VERSION is not set"; \
		exit 1; \
	fi
	@if test ! -d packages/$(PACKAGE_NAME); then \
		mkdir -p packages/$(PACKAGE_NAME); \
	else \
		rm -fr packages/$(PACKAGE_NAME); \
	fi
	@for d in $(PACKAGE_DIRS); do \
		if test ! -d packages/$(PACKAGE_NAME)/$$d; then mkdir -p packages/$(PACKAGE_NAME)/$$d; fi \
	done
	/usr/bin/install -m 664 -c include/pluma/*.h packages/$(PACKAGE_NAME)/include/pluma/
	$(MAKE) install -C src PACKAGE_PATH=../packages/$(PACKAGE_NAME)

package: clean install
	rm -fr packages/$(PACKAGE_NAME).tgz
	tar zcf packages/$(LIB_NAME)-el$(PLATFORM_VERSION)-$(LIB_VERSION).tgz -C packages $(PACKAGE_NAME)

qmodule: package
	(cd packages/$(PACKAGE_NAME); qmodule_add $(LIB_NAME) $(LIB_VERSION) .)
	(qmodule_get -m=$(LIB_NAME) -v=$(LIB_VERSION))

.PHONY: all clean install package qmodule

