
#include <os.h>
#include <boot.h>


static char* init_argv[2]={"init","-i"};

/* charge les modules de depart */
static void load_modules(multiboot_info* mbi){
	
	if (mbi->mods_count>0){
		u32 initrd_location = *((u32*)mbi->mods_addr);
		u32 initrd_end = *(u32*)(mbi->mods_addr+4);
		u32	initrd_size=initrd_end-initrd_location;
		io.print(" >load module:  location=%x, size=%d \n",initrd_location,initrd_end-initrd_location);
		int i=0;
		unsigned int adress;
	
		for (i=0;i<(initrd_size/4072)+1;i++){	
				adress=(initrd_location+i*4096);
				vmm.kmap(adress,adress);
		}
		execv_module(initrd_location,1,init_argv);
	}
}


/* le main du noyau */
extern "C" void kmain(multiboot_info* mbi){
	io.clear();
	io.print("%s - %s -- %s %s \n",	KERNEL_NAME,
									KERNEL_VERSION,
									KERNEL_DATE,
									KERNEL_TIME);
	
	arch.init();
	
	io.print("Loading Virtual Memory Management \n");
	vmm.init(mbi->high_mem);
	
	syscall.init();
	io.print("Loading system \n");
	sys.init();
	
	io.print("\n");
    io.print("--------------------------------------------------------\n");
    io.print("  --        --    -- -- --        -- --        -- --    \n");
    io.print("  -- --  -- --    --      --    --     --    --         \n");
    io.print("  --   --   --    --       --  --       --     -- --    \n");
    io.print("  --        --    --      --    --     --          --   \n");
    io.print("  --        --    -- -- --        -- --       -- --     \n");
    io.print("--------------------------------------------------------\n\n");


	io.print("  ==== System is ready (%s - %s) ==== \n\n",KERNEL_DATE,KERNEL_TIME);
	arch.enable_interrupt();
	arch.shutdown();
}

