#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/slab.h>
#include <linux/random.h>

static struct cdev miDispositivo;
static dev_t major;
char * nombre = "letras123";
static struct class *mi_class;

struct userData {
    char espacios[3];
    bool ocupados[3];
};

static struct userData midata;
midata.ocupados[0] = false;
midata.ocupados[1] = false;
midata.ocupados[2] = false;

ssize_t mi_operacion_lectura(struct file *filp, char __user *data, size_t s, loff_t *off) {

}

//user_data *udata = (user_data *) filp->private_data;
//if(!udata->valid && s > 0){
//udata->valid = true;
//udata->input = dataCopiada;
//}

ssize_t mi_operacion_escritura(struct file *filp, const char __user *data, size_t s, loff_t *off) {

}

static struct file_operations mis_operaciones = { .owner = THIS_MODULE,
        .read = mi_operacion_lectura,
        .write = mi_operacion_escritura
};

static int __init letras_init(void) {
    cdev_init(&miDispositivo, &mis_operaciones);
    alloc_chrdev_region(&major, 0, 1, nombre);
    cdev_add(&miDispositivo, major, 1);
    mi_class = class_create(THIS_MODULE, nombre);
    device_create(mi_class, NULL, major, NULL, nombre);
    printk(KERN_ALERT "Hola, Sistemas Operativos!\n");
    return 0;
}

static void __exit letras_exit(void) {
    unregister_chrdev_region(major, 1);
    cdev_del(&miDispositivo);
    device_destroy(mi_class, major);
    class_destroy(mi_class);
    printk(KERN_ALERT "Adios, mundo cruel...\n");
}

module_init(letras_init);
module_exit(letras_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("La banda de SO");
MODULE_DESCRIPTION("Ejercicio 3 taller drivers");
