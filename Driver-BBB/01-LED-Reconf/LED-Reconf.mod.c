#include <linux/build-salt.h>
#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

BUILD_SALT;

MODULE_INFO(vermagic, VERMAGIC_STRING);
MODULE_INFO(name, KBUILD_MODNAME);

__visible struct module __this_module
__section(.gnu.linkonce.this_module) = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

#ifdef CONFIG_RETPOLINE
MODULE_INFO(retpoline, "Y");
#endif

static const struct modversion_info ____versions[]
__used __section(__versions) = {
	{ 0xb71b8d35, "module_layout" },
	{ 0x2fe838bd, "platform_driver_unregister" },
	{ 0x2e5810c6, "__aeabi_unwind_cpp_pr1" },
	{ 0x54203b50, "__platform_driver_register" },
	{ 0x3c3ff9fd, "sprintf" },
	{ 0xdecd0b29, "__stack_chk_fail" },
	{ 0xbcab6ee6, "sscanf" },
	{ 0x8f678b07, "__stack_chk_guard" },
	{ 0xc5b884e8, "sysfs_create_file_ns" },
	{ 0xe8be4cf3, "kobject_create_and_add" },
	{ 0xe97c4103, "ioremap" },
	{ 0x65584ebc, "of_property_read_variable_u32_array" },
	{ 0xbca68f4d, "of_parse_phandle" },
	{ 0x2ce19f6f, "of_get_named_gpio_flags" },
	{ 0xd94fa632, "kmem_cache_alloc_trace" },
	{ 0xc40c2358, "kmalloc_caches" },
	{ 0xefd6cf06, "__aeabi_unwind_cpp_pr0" },
	{ 0x37a0cba, "kfree" },
	{ 0xedc03953, "iounmap" },
	{ 0x2e8e3cff, "kobject_put" },
	{ 0xc5850110, "printk" },
	{ 0xb1ad28e0, "__gnu_mcount_nc" },
};

MODULE_INFO(depends, "");

MODULE_ALIAS("of:N*T*Cgpio_extled");
MODULE_ALIAS("of:N*T*Cgpio_extledC*");
