CFLAGS += -g -O2
LDFLAGS += -g

#####################

SRCS = step0_repl.c step1_read_print.c step2_eval.c step3_env.c \
       step4_if_fn_do.c step5_tco.c step6_file.c step7_quote.c \
       step8_macros.c step9_try.c stepA_mal.c
OBJS = $(SRCS:%.c=%.o)
BINS = $(OBJS:%.o=%)
OTHER_OBJS = #types.o reader.o printer.o env.o core.o 
OTHER_HDRS = #types.h reader.h printer.h       core.h 

ifeq ($(shell uname -s),Darwin)
CFLAGS +=-DOSX=1
endif

LDFLAGS += -ldl

#####################

all: $(BINS)

dist: mal

mal: $(word $(words $(BINS)),$(BINS))
	cp $< $@

$(OBJS) $(OTHER_OBJS): %.o: %.c  $(OTHER_HDRS)
	gcc $(CFLAGS) -c $(@:%.o=%.c) -o $@

$(patsubst %.o,%,$(filter step%,$(OBJS))): $(OTHER_OBJS)
$(BINS): %: %.o
	gcc $+ -o $@ $(LDFLAGS)

clean:
	rm -f $(OBJS) $(BINS) $(OTHER_OBJS) mal
