#include <iostream>
#include <fstream>
#define line std::cout<<std::endl;

/*
funcion para calcular la potencia de un numero
*/

int pow(int a,int b)
    {
        int power=a;
        if (b==0){return 1;}
        else
        {
        for(int i=1;i<b;i++)
            {
                    power=power*a;
            }
        return power;
        }
    }

/*
funcion que convierte un caracter 'c' en un numero binario de 'n' bits y lo guarda en una cadena de caracteres 'bin'
*/	

void charToBinary(char c,char * bin,int n)
{
    int x = (int) c;
    int y;

    for(int i=n-1;i>=0;i--)
    {
        y = x/(pow(2,i));
        bin[n-1-i] = y + 48; //n-1 ya que un numero de n bits necesita un arreglo de 0 a n-1
        x = x - pow(2,i)*y;
        //cout<<i<<" "<<x<<" "<<y<<" "<<bin[n-1-i]<<endl; usado anteriormente para debug
    }

	/*
	El algoritmo funciona dividiendo el numero entero respectivo del caracter entre la potencia de 2 correspondiente
	a la posicion actual del algoritmo, obteniendo asi el 1 o 0 correspondiente en esa posicion. 
	
	Una vez obtenido ese numero se guarda en la cadena de caracteres el 1 o 0 pero trasladado 48 posiciones para obtener los valores de los numeros en ASCII
	
	Luego se resta del numero entero la cantidad que ya fue representada por el algoritmo.
	*/
}



/*
	funcion que niega un caracter binario (1 o 0) y lo retorna
*/
char notbin(char c)
{
    char xnot;
    if (c=='0'){xnot='1';}
    if (c=='1'){xnot='0';}

    return xnot;
}


/*implementacion vieja de la conversion a binario que requiere de la libreria bitset
std::bitset<8> binary(char c)
{
    std::bitset<8> x(c);
    return x;
}
*/

/*Funcion que convierte un numero binario de n bits en un caracter*/

char binaryToChar(int n, char * string)
{
int x=0;

    for(int i=0;i<n;i++)
    {
        x=((int) (string[i]-48))*pow(2,n-1-i)+x;

        //std::cout<<x;
        //line
    }

return (char) x;
}

/*simplemente imprime un string*/
void printstr(char * str,int n)
{
    for(int i=0;i<n;i++)
    {
        std::cout<<*(str+i);
    }
}

/*procedimiento que pide al usuario el metodo y la semilla por referencia*/
void encrypt_cfg(int &method, int &seed)
{
    line
    /*Parametros de encriptacion*/
        method=0; //metodo de cifrado

        seed=0; //semilla del metodo

        //Se pide al usuario metodo y semilla hasta que coloque numeros validos

            while(method!=1 && method!=2)
            {
            std::cout<<"Ingrese el metodo de encriptacion (1 o 2): "<<std::endl;
            std::cin>>method;
            }

            while (seed < 3)
            {
                std::cout<<"Ingrese semilla de encriptacion (mayor o igual a 3): "<<std::endl;
                //minimo 3 para que siempre se asegure de que se encripte el archivo (por regla 3 del metodo 1)
                std::cin>>seed;
            }
    line
}

/*Convierte el stream ifs (texto) en su representacion binaria en el stream ofsb (binario)*/

void textToBinary (std::fstream * ifs, std::fstream * ofsb)
{
    /*Conversion del archivo de entrada a su representacion binaria*/
    try{
        char * buffer;// buffer del stream ifs, en el se alojaran los caracteres del archivo temporalmente
        buffer = new char [1]; //reservamos dinamicamente 1 solo caracter ya que convertiremos el archivo caracter a caracter


        //obtenemos el tamanyo del archivo
        ifs->seekg(0,ifs->end);
        int length=ifs->tellg();
        ifs->seekg(0,ifs->beg);

        //un caracter es 1 byte, es decir, 8 bits.
        char bin[8];//cadena de caracteres de 8 espacios para guardar temporalmente el caracter convertido a binario
        //depende del lugar donde se encuentra el cursor, el tamanyo del archivo y la semilla
        int last=0; //permite identificar si estamos en el ultimo bloque
        int cursor;
        while(1)
            {
            cursor=ifs->tellg();//identificamos el lugar del cursor
            if(cursor+1==length){last=1;} //vemos si nos encontramos en el ultimo caracter
            ifs->read(buffer,1);//leemos un caracter en el buffer
            //ofsb<<binary(buffer[i]); implementacion vieja
            charToBinary(buffer[0], bin, 8);//convertimos el caracter en binario en la cadena bin de 8 bits
            ofsb->write(bin,8);//escribimos el numero binario de 8 bits obtenido en el stream de conversion binaria
            if(last){break;}
            if(!ifs->good()){throw '1';}
            if(!ofsb->good()){throw '2';}
            }
    }
        /*Excepciones*/
            catch (char c){
                    std::cout<<"Error # "<<c<<": ";
                    if(c=='1'){std::cout<<"Error al abrir el archivo de entrada.\n";}
                    if(c=='2'){std::cout<<"Error al generar archivo de texto binario intermedio no encriptado.\n";}
                    if(c=='3'){std::cout<<"Error al generar archivo de texto binario encriptado.\n";}
                    if(c=='4'){std::cout<<"Error al eliminar el archivo de texto binario no encriptado.\n";}
                    }

            catch(...){ //cualquier otra excepcion
                std::cout << "Error desconocido." <<std::endl;
            }

}

/*encripta el stream ofsb (binario) en el stream ofsc (binario) segun el seed y el metodo*/
void encrypt(std::fstream * ofsb, std::fstream * ofsc, int seed, int method)
{
    /*Fase de Encriptacion*/
    try{
        /*obtenemos el tamanyo del archivo en representacion binaria*/
        ofsb->seekg(0,ofsb->end);
        int lengthb=ofsb->tellg();
        ofsb->seekg(0,ofsb->beg);

        //creamos un buffer con tamanyo dependiente del seed (semilla del tamanyo del bloque)
        char * bufferb;
        bufferb=new char[seed];

        int rule=0;//variable auxiliar que determina en que regla se debe aplicar
        //esto es dado por la diferencia entre los ceros y unos del bloque anterior
        //en el primer bloque se imagina que el bloque anterior dio igual ceros y unos por lo que se inicializa en 0 la diferencia
        int z,o; //los numeros de ceros (zeros) y unos (ones) se guardaran en cada ciclo en estas variables
        int cursorb;//variable que guarda la posicion del archivo
        int last=0;//variable que es una bandera de que estamos en el ultimo bloque
        int howMany;//variable auxiliar que permite saber cuanto debemos leer del archivo, generalmente es del tamanyo del bloque, excepto en el ultimo
                    //ya que ahi puede ser igual o menor
        while(1)
        {
            cursorb=ofsb->tellg();//detectamos la posicion actual en el archivo
            last=0;

            //si el lugar del cursor + un bloque (la semilla) no llega al final del archivo (notese el menor estricto) entonces...
            if(cursorb+seed<lengthb){howMany=seed;}//leo un bloque del tamanyo de la semilla si todavia queda espacio para un bloque mas

            //de lo contrario ya nos encontramos en el ultimo bloque, luego
                else{howMany=lengthb-cursorb;last=1;}//leo lo que quede, que es el tamanyo del archivo menos el lugar actual del cursor

            //guardamos en el buffer cuantos caracterese sea que tengamos que leer (determinado por howMany)
            ofsb->read(bufferb,howMany);

            //ejecutamos la encriptacion segun el metodo escogido por el usuario

                if (method==1)
                {
                    if(rule==0) //si hay igual ceros que unos, negamos todos los bits
                    {
                        for(int i=0;i<howMany;i++)
                        {
                            ofsc->put(notbin(bufferb[i]));
                        }
                    }

                    if(rule>0)//si hay mas ceros que unos, negamos cada dos (el primero no, el segundo si)
                    {
                        for(int i=0;i<howMany;i++)
                        {
                            if(i%2!=0){ofsc->put(notbin(bufferb[i]));} //notamos que indices impares se niegan
                            else{ofsc->put(bufferb[i]);}
                        }
                    }

                    if(rule<0)//si hay mas unos que ceros, negamos cada 3 (primero no, segundo no, tercero si)
                    //para facilitar esto, verificamos si i+1 es multiplo de 3 (si verificamos sobre i es mas complicado)
                    {
                        for(int i=0;i<howMany;i++)
                        {
                            if((i+1)%3==0){ofsc->put(notbin(bufferb[i]));}
                            else{ofsc->put(bufferb[i]);}

                        }
                    }

                    /* determinamos que regla usar en el siguiente ciclo (ya que la regla depende del bloque sin encriptar procesado en el ciclo anterior)*/
                    z=0;//numero de ceros (zeros)
                    o=0;//numero de unos (ones)
                    //notese que operamos sobre el bufferb sin encriptar
                    for(int i=0;i<howMany;i++)
                    {
                        if (bufferb[i]=='0'){z++;}
                        if (bufferb[i]=='1'){o++;}
                    }
                    rule=z-o; //determinamos la regla del ciclo siguiente , dada por la diferencia
                }

                if (method==2)
                {
                    //desplazamos todo los bits uno a la derecha y el ultimo queda de primero
                    //esto es equivalente a colocar en el archivo el caracter i-1 del buffer
                    //excepto en la primera iteracion que colocamos es el ultimo caracter del buffer
                    for(int i=0;i<howMany;i++)
                    {
                        //el primero codificado es el ultimo no codificado
                        if(i==0){ofsc->put(bufferb[howMany-1]);}
                        //en los demas el anterior no codificado es el actual codificado
                        else {ofsc->put(bufferb[i-1]);}
                    }
                }

            if(last){break;}//ultimo bloque

            if(!ofsb->good()){throw '2';}
            if(!ofsc->good()){throw '3';}
        }
    }
        /*Excepciones*/
            catch (char c){
                    std::cout<<"Error # "<<c<<": ";
                    if(c=='1'){std::cout<<"Error al abrir el archivo de entrada.\n";}
                    if(c=='2'){std::cout<<"Error al generar archivo de texto binario intermedio no encriptado.\n";}
                    if(c=='3'){std::cout<<"Error al generar archivo de texto binario encriptado.\n";}
                    if(c=='4'){std::cout<<"Error al eliminar el archivo de texto binario no encriptado.\n";}
                    }

            catch(...){ //cualquier otra excepcion
                std::cout << "Error desconocido." <<std::endl;
            }

}

/*desencripta el stream ifs (binario) en el stream mfs (binario) segun el seed y el metodo*/
void decrypt(std::fstream *ifs, std::fstream * mfs, int seed , int method)
{
    try
    {
    /*Fase de decodificacion*/

    //obtenemos tamanyo del archivo de entrada
    ifs->seekg(0,ifs->end);
    int length=ifs->tellg();
    ifs->seekg(0,ifs->beg);


    char * buffer; //para el archivo de entrada binario codificado
    char * buffer2;//para el archivo intermedio binario descodificado
    buffer=new char[seed];
    buffer2=new char[seed];

    int rule=0;//Inicializamos en cero ya que el primer bloque en el metodo 1 aplica esa regla
    int cursor;
    int howMany;
    int last=0;

    while(ifs->good()&&mfs->good())

    {
        cursor=ifs->tellg();

        //detectamos cuanto hay que leer y si estamos en el ultimo bloque
        if(cursor+seed<length){howMany=seed;}
            else{howMany=length-cursor;last=1;}

        ifs->read(buffer,howMany);

        /*Segun el metodo, descodificamos*/
            if (method==1)
            {
                if(rule==0)
                {
                    for(int i=0;i<howMany;i++)
                    {
                        mfs->put(notbin(buffer[i]));
                        //invertimos todos
                    }
                }

                if(rule>0)
                {
                    for(int i=0;i<howMany;i++)
                    {
                        if(i%2!=0){mfs->put(notbin(buffer[i]));}
                        else{mfs->put(buffer[i]);}
                        //invertimos cada dos
                    }
                }

                if(rule<0)
                {
                    for(int i=0;i<howMany;i++)
                    {
                        if((i+1)%3==0){mfs->put(notbin(buffer[i]));}
                        else{mfs->put(buffer[i]);}
                        //invertimos cada 3

                    }
                }

                /*Para debug*/
                    //line
                    //std::cout<<mfs.good()<<" "<<mfs.tellp()<<mfs.tellg();
                    //line

                /*Correcion del seek del stream mfs*/

                /*
                Cada vez que usamos mfs<< el cursor howMany posiciones y nuestra meta es leer lo que acabamos de descodificar para determinar que regla usar
                Por lo tanto restamos al cursor howMany posiciones
                */
                mfs->seekg(-howMany,mfs->cur);
                mfs->read(buffer2,howMany);

                /*Determinamos que regla se aplica en el siguiente bloque*/
                int z=0;
                int o=0;
                for(int i=0;i<howMany;i++)
                {
                    if (buffer2[i]=='0'){z++;}
                    if (buffer2[i]=='1'){o++;}
                }
                rule=z-o;

            }

            if (method==2)
            {
                for(int i=0;i<howMany;i++)
                {
                    /*El primero del buffer (codificado) es el ultimo descodificado*/
                    if(i==howMany-1){mfs->put(buffer[0]);}
                    /*En los demas el siguiente del buffer (codificado) es el actual descodificado*/
                    else {mfs->put(buffer[i+1]);}
                }
            }

        if(last){break;}//ultimo bloque

        if(!ifs->good()){throw '1';}
        if(!mfs->good()){throw '2';}
     }
    }

    /*Excepciones*/
        catch (char c){
                std::cout<<"Error # "<<c<<": ";
                if(c=='1'){std::cout<<"Error al abrir el archivo de entrada.\n";}
                if(c=='2'){std::cout<<"Error al generar archivo de texto binario intermedio no encriptado.\n";}
                if(c=='3'){std::cout<<"Error al generar archivo de texto decodificado.\n";}
                if(c=='4'){std::cout<<"Error al eliminar el archivo de texto binario no encriptado.\n";}
                }

        catch(...){ //cualquier otra excepcion
            std::cout << "Error desconocido." <<std::endl;
        }

}

/*Convierte el stream ifs (binario) en su representacion textual en el stream ofsb (texto)*/


void binaryToText(std::fstream * mfs, std::fstream * text)
{
    try{
    /*Conversion from binary to text*/
        mfs->seekg(0,mfs->beg);

        //creamos un buffer para el archivo de texto descodificado
        char buffet[8]; //1 byte is 8 bits, therefore 8 chars in binary are 1 char in text
        mfs->read(buffet,8); //leemos los primeros 8 caracteres (que representan bits)

        while(1)
        {
            text->put(binaryToChar(8,buffet));
            //line
            //std::cout<<mfs.eof();line

            mfs->read(buffet,8);//leemos los siguientes 8 bits para la siguiente iteracion
            //de esta manera si ya estabamos en los ultimos 8 entonces al movernos llegaremos a eof y podremos salir
            if(mfs->eof()){break;}

            if(!mfs->good()){throw '2';}
            if(!text->good()){throw '3';}
        }
    }

    /*Excepciones*/
        catch (char c){
                std::cout<<"Error # "<<c<<": ";
                if(c=='1'){std::cout<<"Error al abrir el archivo de entrada.\n";}
                if(c=='2'){std::cout<<"Error al generar archivo de texto binario intermedio no encriptado.\n";}
                if(c=='3'){std::cout<<"Error al generar archivo de texto decodificado.\n";}
                if(c=='4'){std::cout<<"Error al eliminar el archivo de texto binario no encriptado.\n";}
                }

        catch(...){ //cualquier otra excepcion
            std::cout << "Error desconocido." <<std::endl;
        }
}

void viewFile(std::fstream * ifs)
{
    std::cout <<"\n\n\n";
    char c;
    while(1)
    {
        ifs->get(c);
        if(ifs->eof()){std::cout <<"\n\n\n";break;}
        std::cout << c;
    }
}

/*Funcion que compara dos streams en bloques de tamanyo 'word' definido por el usuario*/
void compare_fixed(std::fstream * ifs1,std::fstream * ifs2,int word)
{
    //buffers de los streams
    char * buffer1;
    char * buffer2;

    buffer1= new char[word];
    buffer2= new char[word];

    //obtenemos tamanyo del archivo 1
    ifs1->seekg(0,ifs1->end);
    int length1=ifs1->tellg();
    ifs1->seekg(0,ifs1->beg);

    //obtenemos tamanyo del archivo 1
    ifs2->seekg(0,ifs2->end);
    int length2=ifs2->tellg();
    ifs2->seekg(0,ifs2->beg);



    while(ifs1->good()&&ifs2->good())
    {
        //determinamos cuanto podemos leer del archivo 1
        int carry1=ifs1->tellg();
        int howMany1;
        if(carry1+word<=length1){howMany1=word;}
            else{howMany1=length1-carry1;}

        //determinamos cuanto podemos leer del archivo 2
        int carry2=ifs2->tellg();
        int howMany2;
        if(carry2+word<=length2){howMany2=word;}
            else{howMany2=length2-carry2;}

        //lectura de los archivos en sus respectivos buffers
        ifs1->read(buffer1,howMany1);
        ifs2->read(buffer2,howMany2);


        //comparacion
        bool equal=true;//empezamos suponiendo que son iguales

        if(howMany1!=howMany2){equal=false;}//si los tamanyos son diferentes ya sabemos que son distintos
        else
        {
            //si los tamanyos son iguales verificamos caracter a caracter
            for(int i=0;i<howMany1;i++)
            {
                if(buffer1[i]!=buffer2[i])
                {
                    equal=false;
                }
            }
        }

        //impresion de los resultados

        printstr(buffer1,howMany1);
        std::cout<<" == ";
        printstr(buffer2,howMany2);
        std::cout<<" : "<<equal<<std::endl;

        //salimos del ciclo si estamos en el ultimo bloque
        if(howMany1<word||howMany2<word){break;}//last block

    }

}

/*Funcion que compara dos archivos palabra a palabra.
 * Note que si los dos archivos tienen las mismas palabras pero diferentes espacios o saltos de linea
 * se veran como iguales en la funcion
*/
void compare_word(std::fstream * ifs1,std::fstream * ifs2, int max_word)
{

    line line

    //creacion de los buffers para los archivos
    char * buffer1;
    char * buffer2;

    //sumamos 1 al tamanyo para incluir a final el fin de string (\0)
    buffer1= new char[max_word+1];
    buffer2= new char[max_word+1];

    //caracteres auxiliares que permiten verificar si llegamos a un espacio o salto de linea
    char a,b;

    while(1)
    {
        //contadores para controlar en que posicion del buffer colocamos el caracter actual
        int i,j;
        i=0;j=0; //i para buffer 1 , j para buffer 2

        while(1)
        {
            a = ifs1->peek();//guardamos el siguiente caracter en a
            //std::cout<<a;
            //si el siguiente caracter es espacio, salto de linea o el fin del archivo entonces salimos del ciclo
            if(a == ' '  || a == '\n'||ifs1->eof()){ifs1->get();break;}
            ifs1->get(buffer1[i]);// de lo contrario estamos dentro de una palabra, guardamos el caracter en el buffer
            i++;
        }

        buffer1[i]='\0';//cuando ya salimos de la colocamos un \0 para no tener errores en la impresion

        //similarmente para el archivo 2
        while(1)
        {
            b = ifs2->peek();
            //std::cout<<b;
            if(b == ' ' || b== '\n'||ifs2->eof()){ifs2->get();break;}
            ifs2->get(buffer2[j]);
            j++;
        }

        buffer2[j]='\0';

        //comparamos las palabras

        bool equal=true;//empezamos suponiendo que son iguales
        int m=0;

        //recorremos las palabras obtenidas en el buffer, comparando caracter a caracter
        while(1)
        {
            if(buffer1[m]!=buffer2[m])
            {
                equal=false;
            }

            //si hemos llegado al fin de alguna de las palabras o de alguno de los archivos nos salimos del ciclo
            if(buffer1[m]=='\0' || buffer1[m]=='\0'||ifs1->eof()||ifs2->eof()){break;}
            m++;

        }

        //impresion de los resultados

        std::cout<<buffer1<<"=="<<buffer2<<":"<<equal;
        line

        //si llegamos al fin de alguno de los archivos nos salimos
        if(ifs1->eof()||ifs2->eof()){break;}

    }


}


int str_size(char * str)
{
    int i = 0;
    while(1)
    {
            if(str[i] == '\0'){break;}
            i++;
    }
    return i;
}

void countWordsBiggerThan(std::fstream * ifs, int big, int max_word, int & totalCount, int & bigCount, int &  smallCount)
{
    char * buffer;
    buffer = new char[max_word+1];
    totalCount = 0;
    bigCount = 0;
    smallCount = 0;

    char a;
    line
    std::cout<<"Palabras de longitud minima "<< big+1<<" :"<<std::endl;

while(1)
{
    int i = 0;
    while(1)
    {
        a = ifs->peek();//guardamos el siguiente caracter en a
        //std::cout<<a;
        //si el siguiente caracter es espacio, salto de linea o el fin del archivo entonces salimos del ciclo
        if(a == ' '  || a == '\n'||ifs->eof()){ifs->get();break;}
        ifs->get(buffer[i]);// de lo contrario estamos dentro de una palabra, guardamos el caracter en el buffer
        i++;
    }

    buffer[i]='\0';//cuando ya salimos de la colocamos un \0 para no tener errores en la impresion

    if(str_size(buffer)>big){bigCount++; std::cout<<"\n"<<buffer<<"\n";}
    else{smallCount++;}

    totalCount++;

    //std::cout<<"\n"<<totalCount<<" = "<<bigCount<<" + " << smallCount<<"\n";

    if(ifs->eof()){break;}

}

}

