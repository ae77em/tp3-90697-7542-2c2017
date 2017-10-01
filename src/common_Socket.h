#ifndef COMMON_SOCKET_H
#define COMMON_SOCKET_H

#include <stddef.h>
#include <sys/types.h>
#include <string>

class Socket {
private:
    int socket;
    
public:
    /*
     * Crea el socket obteniendo un filedescriptor del sistema operativo. Recibe 
     * las características de configuración básicas.
     */
    Socket();
    
    /*
     * Crea el socket con el filedescriptor pasado como parámetro
     */
    explicit Socket(int socket);
    
    /*
     * Apaga y cierra la conexión.
     */
    ~Socket();
    
    /*
     * bind: Enlaza el socket identificado por el filedescriptor con una 
     * dirección y puerto locales.
     *
     * listen: configura el socket para recibir conexiones en la dirección y 
     * puerto previamente determinada mediante bind().
     */    
    void bind_and_listen(unsigned short port);
    
    /*
     * Conecta el socket a la dirección y puerto destino.
     * Determina dirección y puertos locales si no se utilizó bind() 
     * previamente.
     */
    void connect(const char* host_name, unsigned short port);
    
    /*
     * Espera una conexión en el socket previamente configurado con listen().
     */
    void accept(Socket& accepted_socket);
    
    /*
     * Envía datos a través del socket
     */
    void send(const char* buffer, size_t length);
    
    /*
     * Recibe datos a través del socket.
     */
    void receive(char* buffer, size_t length);
    
    /*
     * Se utiliza para cerrar el envío y la recepción de datos en forma 
     * ordenada.
     */
    void shutdown();
    
    /*
     * Cierra la conexión.
     */
    void close();
    
    /*
     * Se utiliza para cerrar el envío de datos en forma ordenada.
     */
    void shutdown_send();
    
    /*
     * Se retorna el fd del socket.
     */
    int get_socket();
    
    /*
     * Se modifica y retorna el fd del socket.
     */
    int set_socket(int s);
};
#endif
