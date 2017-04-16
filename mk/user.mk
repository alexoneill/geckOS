# This prevents the gcc4 "hurr durr I'm aligning main()'s stack"
CCFLAGS += -mpreferred-stack-boundary=2
LDFLAGS += --entry=_main
