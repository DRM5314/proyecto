#include <stdio.h>
#include <iostream>
#include <string>
#include <cstdlib>
#include <stdlib.h>
#include <math.h>
#include <fstream>

using namespace std;
struct arbolDatos
{
    arbolDatos *hI = NULL;
    arbolDatos *hD = NULL;
    arbolDatos *padre = NULL;
    arbolDatos *raiz = NULL;
    string valor;
    int numeroHI = 0, numeroHD = 0;
};
struct columna
{
    columna *anterior;
    columna *siuiente;
    int numeroColumnas = 5;
    arbolDatos *arregloArboles[5];
    int posicionFila = 0;
    string nombre;
    double factorCarga;
    double factorCargaPermitido = .6;
    int tipo;
};
struct tabla
{
    tabla *anterior, *siguiente;
    string nombre;
    int numeroColumnas;
    columna *columnas = NULL;
};
void agregarColumna(columna *&entrada, string nombreColumna, int tipo)
{
    /*las columnas estran conectadad entre si por punteros que conectan las 
    columnas de cada tabla, guardando asi la relacion de los datos en como se 
    maneja, cada columna tendra un valor de fila, al momneto de insetar un valor
    nuevo en la tabla este guardara una posicion de la fila, este es un valor que 
    se ira incrementando en cada iteracion de ingresar un nuevo elemento a una tabla
    este guardara la posicion de la fila y al momento de hacer una busqueda este se
    relacionara por dicho numero de fila*/
    columna *aux = new columna;
    aux->nombre = nombreColumna;
    aux->tipo = tipo;
    columna *aux1 = entrada;
    columna *aux2;

    while (aux1 != NULL)
    {
        aux2 = aux1;
        aux1 = aux1->siuiente;
    }

    if (entrada == aux1)
    {
        entrada = aux;
        aux->anterior = NULL;
    }
    else
    {
        aux2->siuiente = aux;
        aux->anterior = aux2;
    }
    aux->siuiente = aux1;
}
void agregarTabla(tabla *&entradaTabla, tabla *aux)
{
    /*cada tabla sera independiente de las columnas, pero tendra una relacion de uno
    a uno, cada tabla con las columnas ingresadas, cada tabla apuntara a las columnas
    que le correspondan*/
    tabla *aux1 = entradaTabla, *aux2;
    while (aux1 != NULL)
    {
        aux2 = aux1;
        aux1 = aux1->siguiente;
    }

    if (entradaTabla == aux1)
    {
        entradaTabla = aux;
        aux->anterior = NULL;
    }
    else
    {
        aux2->siguiente = aux;
        aux->anterior = aux2;
    }
    aux->siguiente = aux1;
}
void mostrarTipoDatos(int i)
{
    cout << "\n1.Entero 2.Decimal 3.Cadena 4.Caracter\nTipo de dato de columnas #: " << i << " __: ";
}
void crearTabla(tabla *&tablaEntrada)
{
    tabla *nuevaTabla = new tabla;
    cout << "\tIngrese nombre de la tabla\n";
    cin >> nuevaTabla->nombre;
    cout << "\tIngrese el numero de columnas\n";
    cin >> nuevaTabla->numeroColumnas;
    for (int i = 0; i < nuevaTabla->numeroColumnas; i++)
    {
        /*en caso que los datos sean manejados en un futuro entonces los datos seran
        de distintos tipòs para poder realizar operaciones con dichos datos, en las 
        siguisntes lineas se invoca a una funcion que muestra los tipos de datos que 
        pueden ser almacenados en las tablas, los datos seran 4, la estructura
        columna guarda los tipos de datos que son ingreados, asi mismo en un archivo
        que se adjunto a dicha carpeta que contiene este codigo se encuentra un 
        archivo que se llama estructura.txt el cual detalla para poder ingresar los 
        4 tipos de datos que se pueden ingresar en las columnas*/
        //int tipoDato = -1;
        //mostrarTipoDatos(i);
        //cin >> tipoDato;
        string nombre;
        cout << "\ningrese nombre de columnas #: " << i << "\n";
        cin >> nombre;
        agregarColumna(nuevaTabla->columnas, nombre, 0);
    }
    cout << "\ncreo un total de " << nuevaTabla->numeroColumnas << " columnas y son las siguientes\n";
    columna *aux = nuevaTabla->columnas;
    while (aux != NULL)
    {
        cout << "\t" << aux->nombre << " tipo: " << aux->tipo << endl;
        aux = aux->siuiente;
    }
    agregarTabla(tablaEntrada, nuevaTabla);
}
void menuSql()
{
    cout << "\n\t1:Seleccionar \n\t2:Insertar\n\t3:Salir\n";
}
string mostrarTablas(tabla *tablasEntrada)
{
    string retorno = "";
    if (tablasEntrada == NULL)
    {
        cout << "\nBase sin datos\n";
        retorno = "null";
    }
    else
    {
        cout << "\nTablas disponibles: \n";
        retorno = "datos";
    }
    while (tablasEntrada != NULL)
    {
        cout << "\t " << tablasEntrada->nombre << endl;
        tablasEntrada = tablasEntrada->siguiente;
    }
    return retorno;
}
void mostrarArbol(arbolDatos *entrada, int contador)
{
    if (entrada == NULL)
    {
        return;
    }
    else
    {
        mostrarArbol(entrada->hD, contador + 1);
        for (int i = 0; i < contador; i++)
        {
            cout << "  ";
        }
        cout << entrada->valor << endl;
        mostrarArbol(entrada->hI, contador + 1);
    }
}
string auxDatos = "";
void escribirHijos(arbolDatos *aux, int entradas)
{
    if (aux == NULL)
        return;
    else
    {
        escribirHijos(aux->hD, entradas + 1);
        auxDatos += aux->valor;
        cout << " contador va en " << entradas << endl;
        int auxEntradas = entradas % 2;
        cout << " el residuo es de " << auxEntradas << endl;
        if (entradas > 0 && auxEntradas == 0)
            auxDatos += ";\n";
        else
            auxDatos += "->";
        escribirHijos(aux->hI, entradas + 1);
    }
}
void consulta(tabla *entrada)
{
    ofstream file;
    file.open("./datos.dot");
    file << "{\n";
    if (entrada != NULL)
    {
        file << "Datos de tabla " << entrada->nombre << endl;
        columna *aux = entrada->columnas;
        while (aux != NULL)
        {
            if (aux->arregloArboles != NULL)
            {
                file << "\ndatos de columna " << aux->nombre << endl;
                for (int i = 0; i < aux->numeroColumnas; i++)
                {
                    string padre = "", hijoI = "", hijoD = "";
                    arbolDatos *auxDatos = aux->arregloArboles[i];
                    while (auxDatos != NULL)
                    {
                        if (auxDatos->padre != NULL)
                            padre = auxDatos->padre->valor;
                        if (auxDatos->hD != NULL)
                            hijoD = auxDatos->hD->valor;
                        if (auxDatos->hI != NULL)
                            hijoI = auxDatos->hI->valor;
                    }
                    if (padre != "")
                        file << padre << endl;
                    if (hijoI != "")
                        file << padre << "->" << hijoI << endl;
                    if (hijoD != "")
                        file << padre << "->" << hijoD << endl;
                    delete auxDatos;
                }
            }
            aux = aux->siuiente;
        }
        delete aux;
    }
    file << "}";
    file.close();
    system("dot -Tpng datos.dot -o imagenDatos.png");
    system("nohup display imagenDatos.png &");
}
void mostrarDatosTotales(tabla *entrada, string nombre)
{
    while (entrada != NULL)
    {
        if (entrada->nombre == nombre)
        {
            columna *auxiliarColumnas = entrada->columnas;
            if (auxiliarColumnas != NULL && entrada->columnas->arregloArboles != NULL)
            {
                cout << "Datos de tabla: " << entrada->nombre << endl;
                cout << "Columna :" << auxiliarColumnas->nombre << endl;
                for (int i = 0; i < auxiliarColumnas->numeroColumnas; i++)
                {
                    arbolDatos *auxiliarDatos = auxiliarColumnas->arregloArboles[i];
                    if (auxiliarDatos != NULL)
                    {
                        mostrarArbol(auxiliarDatos, 0);
                        //escribirHijos(auxiliarDatos,0);
                        if (auxDatos != "")
                        {
                            cout << auxDatos;
                            auxDatos = "";
                        }
                    }
                    if (auxiliarColumnas != NULL && auxiliarColumnas->siuiente != NULL)
                        auxiliarColumnas = auxiliarColumnas->siuiente;
                }
            }
        }
        entrada = entrada->siguiente;
    }
}
void busquedaDatos(tabla *entrada, string nombreTabla)
{
    int opcion;
    cout << "\n1.Seleccionar todos los datos 2.Seleccionar columnas\n";
    cin >> opcion;
    switch (opcion)
    {
    case 1:
        mostrarDatosTotales(entrada, nombreTabla);
        break;
    case 2:
        cout<<"\n\n\t lamentamos el inconveniente, funcion aun no desarrollada\n";
        break;
    default:
        break;
    }
}

void reporteTabla(tabla *tablaEntrada)
{
    if (tablaEntrada != NULL)
    {
        columna *columnaAuxiliar = tablaEntrada->columnas;
        cout << "En la tabla " << tablaEntrada->nombre << " estan las siguientes columnas:\n";
        for (int i = 0; i < tablaEntrada->numeroColumnas; i++)
        {
            cout << "\t Columna #" << i << " :";
            cout << columnaAuxiliar->nombre << endl;
        }
        cout << "Datos totales            : " << columnaAuxiliar->posicionFila << endl;
        cout << "Factor carga (Actual)    : " << columnaAuxiliar->factorCarga << endl;
        cout << "Factor carga (Permitido) : " << columnaAuxiliar->factorCargaPermitido << endl;
        cout << "Numero de columnas       : " << tablaEntrada->numeroColumnas << endl;
    }
    else
        cout << "\n\tNo existen tablas aun\n";
}

void rd(arbolDatos *&nodo)
{
    if (nodo->hI != NULL)
    {
        //cout << "entro a rotacion derecha" << endl;
        //cout << "el padre es " << nodo->padre->valor << endl;
        int diferenciaEntreNumeroHijos = nodo->numeroHI - nodo->numeroHD;
        if (diferenciaEntreNumeroHijos > 0)
        {
            if (nodo->padre != NULL)
                nodo->padre->hI--;
            if (nodo->hI->hD != NULL)
            {
                //cout << "balanceo con hijos" << endl;
                arbolDatos *aux1 = nodo->hI->hD;
                arbolDatos *aux2 = nodo;
                nodo = nodo->hI;
                aux2->hI = NULL;
                nodo->numeroHD = aux2->numeroHI;
                aux2->numeroHI = nodo->numeroHD;
                nodo->hD = aux2;
                nodo->hD->hI = aux1;
            }
            else
            {
                //cout << "balanceo sin hijos" << endl;
                arbolDatos *aux1 = nodo;
                nodo = nodo->hI;
                aux1->hI = NULL;
                nodo->padre = aux1->padre;
                nodo->hD = aux1;
                aux1->padre = nodo;
                //cout << "el padre es " << nodo->padre->valor << " tiene a su derecha " << nodo->padre->hD->valor << endl;
                mostrarArbol(nodo, 0);
            }
        }
    }
    else
        cout << "la entrada viene nula" << endl;
}
int comparacion(string dato){
    int retorno = 0;
    for (int i = 0 ; i < dato.length(); i++)
    {
        retorno += int (dato[i]);
    }
    return retorno;
    
}
void mostrarNumeroHijos(arbolDatos *&entradaAvl)
{
    arbolDatos *auxiliar = entradaAvl;
    while (auxiliar != NULL)
    {
        //cout << "\nEl padre es" << auxiliar->valor << " y tiene a su izquierda " << auxiliar->numeroHI << " y a su derecha " << auxiliar->numeroHD << endl;
        int diferenciaEntreHijos = auxiliar->numeroHI - auxiliar->numeroHD;
        //cout << "la diferencia es de " << diferenciaEntreHijos << endl;
        if (diferenciaEntreHijos > 1)
        {
            cout << "va a balancear " << auxiliar->valor << endl;
            //rd(auxiliar);
            return;
        }
        auxiliar = auxiliar->padre;
    }
}
void insertarArbolEntero(arbolDatos *&entrada, string valorEntrada)
{
    arbolDatos *avlAuxiliar = entrada;
    if (entrada == NULL)
    {
        entrada = new arbolDatos;
        entrada->padre = NULL;
        entrada->valor = valorEntrada;
        entrada->raiz = entrada;
        cout << "\nInserto raiz\n";
    }
    while (avlAuxiliar != NULL)
    {
        if (comparacion(valorEntrada) > comparacion(avlAuxiliar->valor))
        {
            //cout << valorEntrada << " es mayor que " << avlAuxiliar->valor << endl;
            avlAuxiliar->numeroHD++;
            if (avlAuxiliar->hD == NULL)
            {
                arbolDatos *nuevoNodo = new arbolDatos;
                nuevoNodo->padre = entrada;
                nuevoNodo->valor = valorEntrada;
                nuevoNodo->raiz = entrada;
                avlAuxiliar->hD = nuevoNodo;
                //cout << "\n inserto valor " << valorEntrada << " en el hijo derecho de " << avlAuxiliar->valor;
                mostrarNumeroHijos(nuevoNodo);
                return;
            }
            else
                avlAuxiliar = avlAuxiliar->hD;
        }
        if (comparacion(valorEntrada) < comparacion(avlAuxiliar->valor))
        {
            //cout << valorEntrada << " es menor que " << avlAuxiliar->valor << endl;
            avlAuxiliar->numeroHI++;
            if (avlAuxiliar->hI == NULL)
            {
                arbolDatos *nuevoNodo = new arbolDatos;
                nuevoNodo->padre = entrada;
                //cout<<"El padre del nuevo nodo es "<<nuevoNodo->padre->valor<<endl;
                nuevoNodo->valor = valorEntrada;
                nuevoNodo->raiz = entrada;
                avlAuxiliar->hI = nuevoNodo;
                // cout << "\n inserto valor " << valorEntrada << " en el hijo izquierdo de " << avlAuxiliar->valor;
                mostrarNumeroHijos(nuevoNodo);
                return;
            }
            else
                avlAuxiliar = avlAuxiliar->hI;
        }
    }
}
int funcionHash(string entrada, int hola)
{
    /*
    la funcion hash esta diseñada para datos generales en este programa, si desea implementear 
    datos especificos para un manejo posterior de dichos debera crear mas funciones hash, en esta
    ocacion se realizo una suma de los valores ascii de cada entrada evaluando cada entrada en sus 
    caracteres y sumando el valor ascii de cada caracter y obtendra el valor del residuo entre los
    espacios disponibles para la posiscion la cual apunta al arbol que contiene esos datos
    */
    int ascii = 0;
    int retorno = 0;
    for (int d = 0; d < entrada.length(); d++)
    {
        char aux = entrada[d];
        //cout << aux << " ";
        ascii += (int)aux;
    }
    retorno = ascii % hola;
    cout << " la posicion donde debe insertar es en " << retorno << endl;
    if (retorno < 0)
        retorno = retorno * -1;
    return retorno;
}

void rehashing()
{
    cout << "\n\tExedio el factor de carga y debe volver a incrementar el tamaño\n";
}
void mostrarColumnasInsertar(columna *&aux)
{
    columna *auxiliarTabla = aux;
    while (auxiliarTabla != NULL)
    {
        string auxEntrada;
        auxiliarTabla->posicionFila++;
        cout << "ingrese el dato de la columna " << auxiliarTabla->nombre << endl;
        cin >> auxEntrada;

        int posicion = funcionHash(auxEntrada, auxiliarTabla->numeroColumnas);
        if (auxiliarTabla->factorCarga > auxiliarTabla->factorCargaPermitido)
            rehashing();
        insertarArbolEntero(auxiliarTabla->arregloArboles[posicion], auxEntrada);
        mostrarArbol(auxiliarTabla->arregloArboles[posicion], 0);
        auxiliarTabla->factorCarga = (float)auxiliarTabla->posicionFila / (float)auxiliarTabla->numeroColumnas;
        auxiliarTabla = auxiliarTabla->siuiente;
    }
}
void insertarDatoEnColumna(tabla *&tablaEntrada, string nomreColumna)
{
    tabla *auxTabla = tablaEntrada;
    columna *aux = NULL;
    while (auxTabla != NULL)
    {
        if (auxTabla->nombre == nomreColumna)
        {
            cout << "Encontro la tabla buscada en la tabla" << endl;
            aux = auxTabla->columnas;
            break;
        }
        auxTabla = auxTabla->siguiente;
    }

    if (aux != NULL)
    {
        string auxEntrada;
        /*aqui se insertan los case segun los tipos de datos 
        no se manejaron de manera especifica ya que no se manejaran 
        de manera que se necesite que sea un tipo de dato epecifico
        en archivo estructuraDatos.txt estan las estructuras de cada tipo
        para un mejor manejo, segun los datos los tipos son
        0 entero, 1 decimal, 2 cadena, 3 caracter, todos fueron manejados
        de tipo string para hacer mas facil el manejo de datos en el codigo
        y de manera que no se realizaran operaciones entre los datos mas que
        operaciones relacionales y el tipo strin maneja ese tipo de operaciones
        con los datos.
        */
        /*int opcion = aux->tipo; //esta variable se usara para el tipo de dato que 
        se va a ingresar lo pedira segun sea y apartii de esto se debe utilizar un case para
        solicitar el dato correspondiennte */
        mostrarColumnasInsertar(aux);
    }
    else
        cout << "no existe la tabla";
}
void consultasSql(tabla *&entradaTablas)
{
    int opcion = -1;
    while (opcion != 3)
    {
        menuSql();
        cin >> opcion;
        string nombreTablaBuscar;
        switch (opcion)
        {
        case 1:
            if (mostrarTablas(entradaTablas) == "datos")
            {
                cout << "Ingrese nombre de tabla que desea buscar\n";
                cin >> nombreTablaBuscar;
                busquedaDatos(entradaTablas, nombreTablaBuscar);
            }
            break;
        case 2:
            if (mostrarTablas(entradaTablas) == "datos")
            {
                cout << "Ingrese nombre de tabla en que desea insertar\n";
                cin >> nombreTablaBuscar;
                insertarDatoEnColumna(entradaTablas, nombreTablaBuscar);
            }
            break;
        default:
            break;
        }
    }
}
void reportes(tabla *entrada)
{
    if (entrada != NULL)
    {
        while (entrada != NULL)
        {
            if (entrada->columnas != NULL)
            {
                columna *auxiliarColumna = entrada->columnas;
                reporteTabla(entrada);
            }
            entrada = entrada->siguiente;
        }
    }
    else
        cout << "Sin datos que ingresar en los reportes\n";
}
void liberarArbol(arbolDatos *entrada)
{
    {
        if (entrada == NULL)
        {
            return;
        }
        else
        {
            liberarArbol(entrada->hD);
            liberarArbol(entrada->hI);
            cout<<"borro dato"<<entrada->valor<<endl;
            delete entrada;
        }
    }
}
void liberarMemoria(tabla *entrada)
{
    while (entrada != NULL)
    {
        columna *auxLiberar = entrada->columnas;
        while (auxLiberar != NULL)
        {
            for (int i = 0; i < auxLiberar->numeroColumnas; i++)
            {
                liberarArbol(auxLiberar->arregloArboles[i]);
            }
            columna *auxBorrarColumna = auxLiberar;
            auxLiberar = auxLiberar->siuiente;
            cout<<"Borro columna "<<auxBorrarColumna->nombre<<endl;            
            delete auxBorrarColumna;
        }
        tabla *auxBorrarTabla = entrada;
        entrada = entrada->siguiente;
        cout<<"Borro la tabla "<<auxBorrarTabla->nombre<<endl;
        delete auxBorrarTabla;
    }
}
void menu()
{
    tabla *tablas = NULL;
    int opcion = -1;
    while (opcion != 4)
    {
        cout << "\n1. Crear tabla\n2. Consultas Sql\n3. Reportes\n4. Salir\n";
        cin >> opcion;
        switch (opcion)
        {
        case 1:
            crearTabla(tablas);
            break;
        case 2:
            consultasSql(tablas);
            break;
        case 3:
            reporteTabla(tablas);
            break;
        case 4:
            cout << "Hasta la proxima" << endl;
            cout << "________liberando  memoria" << endl;
            liberarMemoria(tablas);
        default:
            break;
        }
    }
}

int main()
{
    menu();
    return 0;
}