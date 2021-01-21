define cmd_cp
	$(Q)$(PRINT4) $(CPMSG) $(MODULE_NAME) $< $@
	$(Q2)$(CP) $< $@
endef

define cmd_mkdir
	$(Q)#$(PRINT3) $(MKDIRMSG) $(MODULE_NAME) $1
	$(Q3)$(MKDIR) $1
endef

define cmd_rm
	$(Q2)[ -d $1 ] && $(RM) $1 || exit 0; \
	$(PRINT3) $(RMMSG) $(MODULE_NAME) $1
endef

define cmd_c
	$(Q)$(PRINT4) $(CCMSG) $(MODULE_NAME) $< $@
	$(Q1)$(CC) -MMD -c $(CPPFLAGS) $(CFLAGS) $< -o $@
endef

define cmd_cxx
	$(Q)$(PRINT4) $(CXXMSG) $(MODULE_NAME) $< $@
	$(Q1)$(CXX) -MMD -c $(CPPFLAGS) $(CXXFLAGS) $< -o $@
endef

ifeq ($(BUILD_ENV),debuginfo)
define cmd_debuginfo
	$(Q)$(PRINT4) $(DBGMSG) $(MODULE_NAME) $@ $@.debuginfo
	$(Q1)$(OBJCOPY) --only-keep-debug $@ $@.debuginfo
	$(Q1)$(OBJCOPY) --strip-debug $@
	$(Q1)$(OBJCOPY) --add-gnu-debuglink=$@.debuginfo $@
endef
endif

ifneq ($(BUILD_ENV),debug)
define cmd_strip
	$(Q)$(PRINT4) $(STRIPMSG) $(MODULE_NAME) $@ $@
	$(Q2)$(STRIP) $@
endef
endif

define cmd_bin
	$(Q)$(PRINT3) $(LDMSG) $(MODULE_NAME) $@
	$(Q1)$(CC) -o $@ $^ $(LDFLAGS) $(LOADLIBES) $(LDLIBS)
	$(call cmd_debuginfo)
	$(call cmd_strip)
endef

define cmd_bins
	$(Q)$(PRINT3) $(LDMSG) $(MODULE_NAME) $@
	$(Q1)$(CC) -o $@ $< $(LDFLAGS) $(LOADLIBES) $(LDLIBS)
	$(call cmd_debuginfo)
	$(call cmd_strip)
endef

define cmd_lib
	$(Q)$(PRINT3) $(ARMSG) $(MODULE_NAME) $@
	$(Q1)$(AR) $(ARFLAGS) $@ $^
	$(call cmd_strip)
endef

define cmd_solib
	$(Q)$(PRINT3) $(LDMSG) $(MODULE_NAME) $@
	$(Q1)$(CC) -o $@ $^ -shared $(LDFLAGS)
	$(call cmd_debuginfo)
	$(call cmd_strip)
endef
