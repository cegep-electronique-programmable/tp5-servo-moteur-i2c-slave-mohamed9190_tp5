#include <mbed.h>

#define ADDRESSE_I2C_PAR_DEFAUT 0x23

#if !DEVICE_I2CSLAVE
#error [NOT_SUPPORTED] I2C Slave is not supported
#endif

// Utiliser la classe I2CSlave pour créer un objet slave.
// Consulter la documentation pour voir les méthodes disponibles.
I2CSlave slave(I2C_SDA, I2C_SCL);

///////////////////////////////////////////
// Créer un objet debug_led à partir de la classe DigitalOut pour vous aider dans le debuggage
///////////////////////////////////////////


///////////////////////////////////////////
// Créer un objet moteur à partir de la classe PwmOut
///////////////////////////////////////////


///////////////////////////////////////////
// Créer une variable pour la machine à état qui gére le moteur (OFF ou Activé)
///////////////////////////////////////////


int main() {
  char read_buffer[10];
  char write_buffer[10];

  slave.address(ADDRESSE_I2C_PAR_DEFAUT << 1);

  while (1) {

        // Attendre une requête du master
        int i = slave.receive();

        // Traiter la requête
        switch (i) {

            // Si le master envoie une requête de lecture
            case I2CSlave::ReadAddressed:
                ///////////////////////////////////////////
                // Retourner l'état du moteur (sa position ou OFF sous forme d'une chaine de caractères)
                ///////////////////////////////////////////

                slave.write(write_buffer, strlen(write_buffer) + 1); // Includes null char
                break;

            // Si le master envoie une requête de lecture qui nous est adressée
            case I2CSlave::WriteAddressed:
                slave.read(read_buffer, 10);
                printf("Read A: %s\n", read_buffer);

                int8_t commande_recue = read_buffer[0];

                ///////////////////////////////////////////
                // Modifier l'état du moteur en fonction de la commande reçue
                ///////////////////////////////////////////
                break;
        }
        
        // Vider le buffer de lecture
        for (int i = 0; i < sizeof(read_buffer); i++) {
            read_buffer[i] = 0;
        }
        
        // Vider le buffer d'écriture
        for (int i = 0; i < sizeof(write_buffer); i++) {
            write_buffer[i] = 0;
        }
    }
}