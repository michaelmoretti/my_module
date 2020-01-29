CONFIG_MODULE_SIG=n
WARN := -W -Wall -Wstrict-prototypes -Wmissing-prototypes
ccflags-y := -O2

obj-m += my_module.o
my_module-objs := mymodule.o 


all:
	make -C /lib/modules/$(shell uname -r)/build M=$(shell pwd) modules 

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(shell pwd) clean
