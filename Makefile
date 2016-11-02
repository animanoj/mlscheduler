OBJS_DIR = .objs

EXENAME = scheduler
OBJS = sjf_scheduler.o vector.o pqueue.o

CC = clang
WARNINGS = -Wall -Wextra -Werror -Wno-error=unused-parameter
CFLAGS = -O2 $(WARNINGS) -g -std=c99 -c -MMD -MP -D_GNU_SOURCE
LD = clang
LDFLAGS =

all : $(EXENAME)

-include $(OBJS_DIR)/*.d

$(OBJS_DIR):
	@mkdir -p $(OBJS_DIR)

$(OBJS_DIR)/%-$(EXENAME).o: %.c | $(OBJS_DIR)
	$(CC) $(CFLAGS) $< -o $@

$(EXENAME): $(OBJS:%.o=$(OBJS_DIR)/%-$(EXENAME).o)
	$(LD) $^ $(LDFLAGS) -o $@

clean :
	rm -rf .objs $(EXENAME)
