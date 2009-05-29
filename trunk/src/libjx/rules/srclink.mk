
all:

include .config
include $(JX_BASE)/rules/arch/$(JX_ARCH).mk

JX_BUILD=$(JX_BASE)/build/$(JX_PREFIX1)-$(JX_ARCH)

direct:
	install -d $(JX_BUILD)/src
	cp -f .config $(JX_BUILD)
	ln -sf $(JX_BASE)/include/*.[chm]* $(JX_BUILD)/src/
	ln -sf $(JX_BASE)/src/core/*.[chm]* $(JX_BUILD)/src/
	ln -sf $(JX_BASE)/src/os/*.[chm]* $(JX_BUILD)/src/
	ln -sf $(JX_BASE)/src/vm/*.[chm]* $(JX_BUILD)/src/
	ln -sf $(JX_BASE)/src/jxon/*.[chm]* $(JX_BUILD)/src/
	ln -sf $(JX_BASE)/src/shell/*.[chm]* $(JX_BUILD)/src/
	ln -sf $(JX_BASE)/src/py/*.[chm]* $(JX_BUILD)/src/
	ln -sf $(JX_BASE)/src/net/*.[chm]* $(JX_BUILD)/src/
	ln -sf $(JX_BASE)/src/main/*.[chm]* $(JX_BUILD)/src/
	ln -sf $(JX_BASE)/src/test/test*.[chm]* $(JX_BUILD)/src/
	cd $(JX_BUILD)/src;/bin/rm -f $(JX_SRC_QUASH)
	ln -sf $(JX_BUILD) $(JX_BASE)/bin

remap: 
	install -d $(JX_BUILD)/src
	cp -f .config $(JX_BUILD)
	cd $(JX_BASE)/include; for i in *.[ch]*; do $(JX_BASE)/rules/remap.sh $$i $(JX_BUILD)/src $(JX_PREFIX1) $(JX_PREFIX2) $(JX_PREFIX3); done
	cd $(JX_BASE)/src/core; for i in *.[ch]*; do $(JX_BASE)/rules/remap.sh $$i $(JX_BUILD)/src $(JX_PREFIX1) $(JX_PREFIX2) $(JX_PREFIX3); done
	cd $(JX_BASE)/src/os; for i in *.[ch]*; do $(JX_BASE)/rules/remap.sh $$i $(JX_BUILD)/src $(JX_PREFIX1) $(JX_PREFIX2) $(JX_PREFIX3); done
	cd $(JX_BASE)/src/vm; for i in *.[ch]*; do $(JX_BASE)/rules/remap.sh $$i $(JX_BUILD)/src $(JX_PREFIX1) $(JX_PREFIX2) $(JX_PREFIX3); done
	cd $(JX_BASE)/src/jxon; for i in *.[ch]*; do $(JX_BASE)/rules/remap.sh $$i $(JX_BUILD)/src $(JX_PREFIX1) $(JX_PREFIX2) $(JX_PREFIX3); done
	cd $(JX_BASE)/src/shell; for i in *.[ch]*; do $(JX_BASE)/rules/remap.sh $$i $(JX_BUILD)/src $(JX_PREFIX1) $(JX_PREFIX2) $(JX_PREFIX3); done
	cd $(JX_BASE)/src/py; for i in *.[ch]*; do $(JX_BASE)/rules/remap.sh $$i $(JX_BUILD)/src $(JX_PREFIX1) $(JX_PREFIX2) $(JX_PREFIX3); done
	cd $(JX_BASE)/src/net; for i in *.[ch]*; do $(JX_BASE)/rules/remap.sh $$i $(JX_BUILD)/src $(JX_PREFIX1) $(JX_PREFIX2) $(JX_PREFIX3); done
	cd $(JX_BASE)/src/main; for i in *.[ch]*; do $(JX_BASE)/rules/remap.sh $$i $(JX_BUILD)/src $(JX_PREFIX1) $(JX_PREFIX2) $(JX_PREFIX3); done
	cd $(JX_BASE)/src/test; for i in *.[ch]*; do $(JX_BASE)/rules/remap.sh $$i $(JX_BUILD)/src $(JX_PREFIX1) $(JX_PREFIX2) $(JX_PREFIX3); done
	cd $(JX_BUILD)/src;/bin/rm -f $(JX_SRC_QUASH) .nothing
	ln -sf $(JX_BUILD) $(JX_BASE)/bin