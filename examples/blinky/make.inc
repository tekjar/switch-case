#
# Makefile addon to compile C Binaries and Libraries
#
# Use this in your Makefile by adding 'include make.inc' in the end of file.
# Syntax:-
#   Assign your binaries and libraries to 'bins :=' macro.
#   Example:- bins := bin1 bin2 .. binN libXX1.so libXX2.so .. libXXN.so'
#   Order above based on dependency. Example if binary 'abc' have a dependency
#   on 'libXYZ.so', then order like:- bins := libXYZ.so abc
#
#   Also supports options to pass additional sources (objs), cflags and ldflags
#   for the binary/library. Example:-
#     'binN_sources := a.c b.c c.c d.c' 'binN_cflags := -I./abc/ -O2 -g'
#     'binN_ldflags := -ldl -lncurses'
#     'libXXN.so_sources := x.c y.c z.c' 'libXXN.so_cflags := -O2 -fPIC'
#     'libXXN.so_ldflags := -L./xyz/ -ldl'
#

CFLAGS := -I. -I$(ESP_INCLUDE) -mlongcalls
LDLIBS := -L$(ESP_LIBS) -nostdlib -Wl,--start-group -lmain -lnet80211 -lwpa -llwip -lpp -lphy -Wl,--end-group -lgcc
LDFLAGS := -T$(ESP_OPEN_SDK)/esp_iot_sdk_v1.3.0/ld/eagle.app.v6.ld


ifeq ($(CC),cc)
CC := xtensa-lx106-elf-gcc
endif
STRIP ?= xtensa-lx106-elf-strip
AR ?= xtensa-lx106-elf-ar
RANLIB ?= xtensa-lx106-elf-ranlib
opt_flags ?= -Wall -Wextra -Wshadow -Wpointer-arith -Wcast-qual -Wstrict-prototypes -Wmissing-prototypes -Winit-self -Wno-missing-braces

.PHONY: flash clean

all: $(libs) elf

elf: $(bins)
	esptool.py elf2image $^

$(info ######$(ESP_OPEN_SDK))

flash:
	$(foreach bin, $(bins), '$(ESP_TOOLS)'/esptool.py write_flash 0 $(bin)-0x00000.bin 0x40000 $(bin)-0x40000.bin)


# Generate Object files (.o)
define build_objs
DEPS += $(patsubst %.c,%_$(notdir $(1)).d,$(2))
$(patsubst %.c,%_$(notdir $(1)).o,$(2)): $(2) $($(notdir $(1))_hdrs)
	$(CC) $(filter-out $(3) $(opt_flags),$(CFLAGS) $($(notdir $(1))_cflags)) $(3) $(opt_flags) -MT $$@ -MD -MP -MF $$(patsubst %.o,%.d,$$@) -c -o $$@ $$<
endef

# Generate final Binaries
define build_bins
$(foreach oname,$(if $($(notdir $(1))_sources),$($(notdir $(1))_sources),$(1).c),$(eval $(call build_objs,$(1),$(oname))))
$(1): $(if $($(notdir $(1))_sources),$(patsubst %.c,%_$(notdir $(1)).o,$($(notdir $(1))_sources)),$(1)_$(notdir $(1)).o)
	$(CC) -o $$@ $$^ $(LDFLAGS) $($(notdir $(1))_ldflags) $(if $(opt_strip),$(if $(STRIP),&& $(STRIP) $$@)) $(LDLIBS)
endef

# Generate final Libraries
define build_libs
$(foreach oname,$(if $($(notdir $(1))_sources),$($(notdir $(1))_sources),$(dir $(1))$(patsubst lib%.so,%.c,$(notdir $(1)))),$(eval $(call build_objs,$(1),$(oname),-fPIC)))
$(1): $(if $($(notdir $(1))_sources),$(patsubst %.c,%_$(notdir $(1)).o,$($(notdir $(1))_sources)),$(dir $(1))$(patsubst lib%.so,%_$(notdir $(1)).o,$(notdir $(1))))
	$(CC) -shared -fPIC -Wl,-soname,$(notdir $(1)) -o $$@ $$^ $(LDFLAGS) $($(notdir $(1))_ldflags) $(if $(opt_strip),$(if $(STRIP),&& $(STRIP) $$@))
endef

# Generate final Lib Archives
define build_lib_ars
$(foreach oname,$(if $($(notdir $(1))_sources),$($(notdir $(1))_sources),$(dir $(1))$(patsubst lib%.a,%.c,$(notdir $(1)))),$(eval $(call build_objs,$(1),$(oname),-fPIC)))
$(1): $(if $($(notdir $(1))_sources),$(patsubst %.c,%_$(notdir $(1)).o,$($(notdir $(1))_sources)),$(dir $(1))$(patsubst lib%.a,%_$(notdir $(1)).o,$(notdir $(1))))
	$(AR) cru $$@ $$^ && $(RANLIB) $$@
endef

$(foreach bname,$(bins),$(if $(filter lib%.so,$(notdir $(bname))),$(eval $(call build_libs,$(bname))),$(if $(filter lib%.a,$(notdir $(bname))),$(eval $(call build_lib_ars,$(bname))),$(eval $(call build_bins,$(bname))))))


# Clean operation
clean:
	$(foreach bname,$(bins),$(if $(filter lib%.so,$(notdir $(bname))),\
		rm -f $(if $($(notdir $(bname))_sources),$(patsubst %.c,%_$(notdir $(bname)).o,$($(notdir $(bname))_sources)),$(dir $(bname))$(patsubst lib%.so,%_$(notdir $(bname)).o,$(notdir $(bname)))) $(patsubst %.c,%_$(notdir $(bname)).d,$(if $($(notdir $(bname))_sources),$($(notdir $(bname))_sources),$(dir $(bname))$(patsubst lib%.so,%_$(notdir $(bname)).d,$(notdir $(bname)))));\
	,\
		$(if $(filter lib%.a,$(notdir $(bname))),\
			rm -f $(if $($(notdir $(bname))_sources),$(patsubst %.c,%_$(notdir $(bname)).o,$($(notdir $(bname))_sources)),$(dir $(bname))$(patsubst lib%.a,%_$(notdir $(bname)).o,$(notdir $(bname)))) $(patsubst %.c,%_$(notdir $(bname)).d,$(if $($(notdir $(bname))_sources),$($(notdir $(bname))_sources),$(dir $(bname))$(patsubst lib%.a,%_$(notdir $(bname)).d,$(notdir $(bname)))));\
		,\
			rm -f $(if $($(notdir $(bname))_sources),$(patsubst %.c,%_$(notdir $(bname)).o,$($(notdir $(bname))_sources)),$(bname)_$(notdir $(bname)).o) $(patsubst %.c,%_$(notdir $(bname)).d,$(if $($(notdir $(bname))_sources),$($(notdir $(bname))_sources),$(bname)_$(notdir $(bname)).d));\
		)\
	))
	$(foreach bin, $(bins), rm -rf $(bin)-0x00000.bin;rm -rf $(bin)-0x40000.bin)
	rm -f $(bins)

distclean: clean
	find . -type f -name "*.[o,a,d]" | xargs rm -f
	find . -type f -name "*.so" | xargs rm -f
	rm -rf ipkg-*;rm -f *.tar.*

dist: distclean
	@$(if $(PKG_NAME),,echo "Define a proper package name in PKG_NAME macro!!";exit 1)
	@$(if $(PKG_VERSION),,$(eval PKG_VERSION=$(shell cat VERSION 2>/dev/null)) $(if $(PKG_VERSION),,echo "Define a proper package version in PKG_VERSION macro or add a VERSION file!!";exit 1))
	@rm -rf "$(PKG_NAME)-$(PKG_VERSION)";rm -f "$(PKG_NAME)-$(PKG_VERSION)".tar*;mkdir -p "$(PKG_NAME)-$(PKG_VERSION)";
	@cp -af `find ./ -maxdepth 1|grep -Ev '$(PKG_NAME)-$(PKG_VERSION)|./$$'` "$(PKG_NAME)-$(PKG_VERSION)/" && tar -cvzf "$(PKG_NAME)-$(PKG_VERSION).tar.gz" "$(PKG_NAME)-$(PKG_VERSION)"
	@rm -rf "$(PKG_NAME)-$(PKG_VERSION)";

-include $(DEPS)