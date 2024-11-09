// incluimos las librearias necesarias para la creacion de la agenda

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// definimos la estructura para cada Contacto que vamos a crear
typedef struct {
    char nombre[30];
    char apellido[30];
    char cumple[30];
    double numerodecelular;
} Contacto;

// creacion de todas las funciones del menú que vamos a poder utilizar
Contacto crearContacto(FILE *arch);
int mostraragenda(FILE *arch);
void editarContacto(FILE *arch);
void eliminarContacto(FILE *arch, int contactoeliminar);

int main() {
    FILE *arch;
    int estado = 1;
    int opcionelegida = 0;

    // Abre el archivo de Contactos
    arch = fopen("./datos.dat", "r+b");
    if (arch == NULL) {
    arch = fopen("./datos.dat", "w+b");
    }
    // con este while vamos a poder ver todas las opciones del menu y cambiar entre cada una utilizando la funcion switch 
    while (estado == 1) {
        printf("-------------------------------------------------------\n");
        printf("Agenda de contactos\n");
        printf("Porfavor elija alguna de las siguientes opciones: \n");
        printf("[1] - Agregar nuevo Contacto\n");
        printf("[2] - Editar Contacto\n");
        printf("[3] - Remover Contacto\n");
        printf("[4] - Mostrar lista de Contactos\n");
        printf("[5] - Cerrar agenda de Contactos\n");
        printf("-------------------------------------------------------\n");
        scanf("%i", &opcionelegida);
        getchar();
    //usamos un switch para elegir la opcion del menu que ingrese el usuario
        switch (opcionelegida) {
        case 1: {
            crearContacto(arch);
            break;
        }
        case 2: {
            editarContacto(arch);
            break;
        }
        case 3: {
            int contactoaborrar;
            mostraragenda(arch);
            printf("Por favor elija el numero del Contacto que desea eliminar.\n");
            scanf("%i", &contactoaborrar);
            getchar();
            eliminarContacto(arch, contactoaborrar);
            break;
        }
        case 4: {   //con este estado vamos a volver a mostrar 
            mostraragenda(arch);
            break;
        }
        case 5:
            estado = 0; // con este estado cerramos la agenda
            break;
        default:
            printf("Opcion no valida.\n");
            break;
        }
    }

    // Cerrar el archivo al salir
    fclose(arch);
    return 0;
}

Contacto crearContacto(FILE *arch) { //
    Contacto nuevocontacto;

    printf("Nombre: ");
    fgets(nuevocontacto.nombre, sizeof(nuevocontacto.nombre), stdin);
    printf("Apellido: ");
    fgets(nuevocontacto.apellido, sizeof(nuevocontacto.apellido), stdin);
    printf("Fecha de cumple: ");
    fgets(nuevocontacto.cumple, sizeof(nuevocontacto.cumple), stdin);
    printf("Numero de celular: ");
    scanf("%lf", &nuevocontacto.numerodecelular);
    getchar(); 

    fseek(arch, 0, SEEK_END);
    fwrite(&nuevocontacto, sizeof(nuevocontacto), 1, arch);

    printf("-------------------------------------\n");
    printf("Nuevo contacto agregado exitosamente.\n");
    printf("-------------------------------------\n");

    return nuevocontacto;
}

int mostraragenda(FILE *arch) {
    Contacto Contacto;
    int contador = 0;
    int posicion = 0;
    rewind(arch); // nos movemos al inicio del archivo con la funcion rewind
    fread(&Contacto, sizeof(Contacto), 1, arch);

    while (!feof(arch)) {
        posicion++;
        printf("-------------------------------------\n");
        printf("Numero de contacto: %d\n", posicion);
        printf("Nombre: %s", Contacto.nombre);
        printf("Apellido: %s", Contacto.apellido);
        printf("Cumple: %s", Contacto.cumple);
        printf("Numero de celular: %.lf\n", Contacto.numerodecelular);
        printf("-------------------------------------\n");
        contador++;
        fread(&Contacto, sizeof(Contacto), 1, arch);
    }

    return contador;
}

void editarContacto(FILE *arch) {
    int opcionseleccionada, contactoseleccionado;
    int contactostotales = mostraragenda(arch);

    if (contactostotales == 0) {
        printf("No hay Contactos para editar.\n");
        return;
    }

    printf("Porfavor seleccione el numero de contacto que desea editar: ");
    scanf("%i", &contactoseleccionado);

    if (contactoseleccionado < 1 || contactoseleccionado > contactostotales) {
        printf("Numero de contacto invalida.\n");
        return;
    }

    Contacto Contacto;
    fseek(arch, sizeof(Contacto) * (contactoseleccionado - 1), SEEK_SET);
    fread(&Contacto, sizeof(Contacto), 1, arch);

    printf("-------------------------------------\n");
    printf("Contacto seleccionado :\n");
    printf("Nombre: %s", Contacto.nombre);
    printf("Apellido: %s", Contacto.apellido);
    printf("Cumple: %s", Contacto.cumple);
    printf("Numero de celular: %.lf\n", Contacto.numerodecelular);
    printf("-------------------------------------\n");

    printf("Seleccione la parte que desea editar.\n");
    printf("[1] Nombre - [2] Apellido - [3] Cumple - [4] Numero de celular \n");
    scanf("%i", &opcionseleccionada);
    getchar();

    switch (opcionseleccionada) {
    case 1:
        printf("nombre: ");
        fgets(Contacto.nombre, sizeof(Contacto.nombre), stdin);
        break;
    case 2:
        printf("apellido: ");
        fgets(Contacto.apellido, sizeof(Contacto.apellido), stdin);
        break;
    case 3:
        printf("cumple: ");
        fgets(Contacto.cumple, sizeof(Contacto.cumple), stdin);
        break;
    case 4:
        printf("numero de celular: ");
        scanf("%lf", &Contacto.numerodecelular);
        break;
    }

    fseek(arch, sizeof(Contacto) * (contactoseleccionado - 1), SEEK_SET);
    fwrite(&Contacto, sizeof(Contacto), 1, arch);

    printf("Contacto editado con exito\n");
}

void eliminarContacto(FILE *arch, int contactoeliminar) {
    int contactostotales = mostraragenda(arch);
    if (contactoeliminar < 1 || contactoeliminar > contactostotales) {
        printf("Numero de contacto no valido\n");
        return;
    }

    Contacto contactolista[contactostotales];
    Contacto contacto;
    rewind(arch);
    int contador = 0;
    
    // Leer todos los Contactos en un array temporal
    while (fread(&contacto, sizeof(contacto), 1, arch)) {
        contactolista[contador++] = contacto;
    }

    // Sobreescribir el archivo sin el Contacto eliminado
    freopen("./datos.dat", "w+b", arch);
    for (int i = 0; i < contactostotales; i++) {
        if (i != (contactoeliminar - 1)) {
            fwrite(&contactolista[i], sizeof(contactolista[i]), 1, arch);
        }
    }

    printf("Contacto eliminado exitosamente.\n");
}