SUBDIRS        = one two three
SUBDIR_ACTIONS = all clean distclean
SUBDIR_TARGETS = $(foreach act,${SUBDIR_ACTIONS},$(patsubst %,%_do_${act},${SUBDIRS}))

.PHONY: ${SUBDIR_ACTIONS} ${SUBDIR_TARGETS}

${SUBDIR_ACTIONS}: %: do-%

$(SUBDIR_ACTIONS:%=do-%): do-%:
	${MAKE} $(patsubst %,%_do_$*,${SUBDIRS})

${SUBDIR_TARGETS}:
	${MAKE} -C $(subst _do_, ,$@)

all clean distclean: 
	@echo local $@
