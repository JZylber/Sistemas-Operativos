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
char * nombre = "azar";
static struct class *mi_class;
static int azar = -1;

ssize_t mi_operacion_lectura(struct file *filp, char __user *data, size_t s, loff_t *off) {
    if(azar == -1){
        return -EPERM;
    }
    unsigned int numero;
    get_random_bytes(&numero,sizeof (unsigned int));
    numero = numero % azar;
    char * buffer = kmalloc(sizeof (unsigned int) + 1,0);
    snprintf(buffer,sizeof (unsigned int) + 1,"%d\n",numero);
    copy_to_user(data,buffer,sizeof (unsigned int) + 1);
    kfree(buffer);
    return 5;
}

//user_data *udata = (user_data *) filp->private_data;
//if(!udata->valid && s > 0){
//udata->valid = true;
//udata->input = dataCopiada;
//}

ssize_t mi_operacion_escritura(struct file *filp, const char __user *data, size_t s, loff_t *off) {
    char * dataCopiada =  kmalloc(s + 1,GFP_KERNEL);
    copy_from_user(dataCopiada,data,s);
    dataCopiada[s] = 0;

    if (kstrtoint(dataCopiada,10,&azar) != 0){
        kfree(dataCopiada);
        return -EPERM;
    }
    kfree(dataCopiada);
    if(azar >= 0){
      return s;
    }
    return -EPERM;
}

static struct file_operations mis_operaciones = { .owner = THIS_MODULE,
        .read = mi_operacion_lectura,
        .write = mi_operacion_escritura
};

static int __init hello_init(void) {
    cdev_init(&miDispositivo, &mis_operaciones);
    alloc_chrdev_region(&major, 0, 1, nombre);
    cdev_add(&miDispositivo, major, 1);
    mi_class = class_create(THIS_MODULE, nombre);
    device_create(mi_class, NULL, major, NULL, nombre);
    printk(KERN_ALERT "Hola, Sistemas Operativos!\n");
    return 0;
}

static void __exit hello_exit(void) {
    unregister_chrdev_region(major, 1);
    cdev_del(&miDispositivo);
    device_destroy(mi_class, major);
    class_destroy(mi_class);
    printk(KERN_ALERT "Adios, mundo cruel...\n");
}

module_init(hello_init);
module_exit(hello_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("La banda de SO");
MODULE_DESCRIPTION("Ejercicio 2 taller drivers");
