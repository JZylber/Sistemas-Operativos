#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/cdev.h>
#include <linux/device.h>

static cdev miDispositivo;
static file_operations misOperaciones;
dev_t major;
char * nombre = "nulo";
static struct class *mi_class;

static struct file_operations mis_operaciones = { .owner = THIS_MODULE,
        .read = mi_operacion_lectura,
        .write = mi_operacion_escritura
};

ssize_t mi_operacion_lectura(struct file *filp, char __user *data, size_t s, loff_t *off) {
return NULL;
}

ssize_t mi_operacion_escritura(struct file *filp, const char __user *data, size_t s, loff_t *off) {
return s;
}

static int __init hello_init(void) {
    cdev_init(*miDispositivo, *file_operations);
    alloc_chrdev_region(*major, 0, 1, nombre);
    cdev_add(*miDispositivo, major, 1);
    mi_class = class_create(THIS_MODULE, nombre);
    device_create(mi_class, NULL, major, NULL, nombre);
    printk(KERN_ALERT "Hola, Sistemas Operativos!\n");
    return 0;
}

static void __exit hello_exit(void) {
    unregister_chrdev_region(major, 1);
    cdev_del(*miDispositivo);
    device_destroy(mi_class, major);
    class_destroy(mi_class);
    printk(KERN_ALERT "Adios, mundo cruel...\n");
}

module_init(hello_init);
module_exit(hello_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("La banda de SO");
MODULE_DESCRIPTION("Una suerte de 'Hola, mundo'");
