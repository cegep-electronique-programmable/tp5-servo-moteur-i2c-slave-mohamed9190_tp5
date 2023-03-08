#include <mbed.h>
// code du slave 
#define ADDRESSE_I2C_PAR_DEFAUT 0x23

#if !DEVICE_I2CSLAVE
#error [NOT_SUPPORTED] I2C Slave is not supported
#endif

// Utiliser la classe I2CSlave pour créer un objet slave.
// Consulter la documentation pour voir les méthodes disponibles.
I2CSlave slave(I2C_SDA, I2C_SCL);    // I2C_SCL = d7 nucleo    = PB_8,  I2C_SDA = d8    = PB_9,

///////////////////////////////////////////
// Créer un objet debug_led à partir de la classe DigitalOut pour vous aider dans le debuggage
///////////////////////////////////////////
DigitalOut myled(LED1); // detecter communication i2c LED1        = PA_5,
void led_i2c(void){
        myled = 1;          // set LED1 pin to high
        printf("myled = %d \n\r", (uint8_t)myled);
        ThisThread::sleep_for(500ms);

        myled.write(0);     // set LED1 pin to low
        printf("myled = %d \n\r", myled.read());
        ThisThread::sleep_for(500ms);
}
///////////////////////////////////////////
// Créer un objet moteur à partir de la classe PwmOut
///////////////////////////////////////////

PwmOut servo(D2); // D2 = = PB_10,, dortie pwm ; 2 R de 10Kohms pour les pull up pour communication i2c 

///////////////////////////////////////////
// Créer une variable pour la machine à état qui gére le moteur (OFF ou Activé)
///////////////////////////////////////////


int main() {
  char read_buffer[10];
  char write_buffer[10];

  slave.address(ADDRESSE_I2C_PAR_DEFAUT << 1);
  servo.period(0.020f);
  while (1) {

        // Attendre une requête du master
       // int i = slave.receive(); // modifier pour un char
        int i = slave.receive(); 
        
        
        
        // Traiter la requête
        switch (i) {

            // Si le master envoie une requête de lecture
            case I2CSlave::ReadAddressed:
                ///////////////////////////////////////////
                // Retourner l etat du moteur (sa position ou OFF sous forme d une chaine de caracteres)
                ///////////////////////////////////////////

                slave.write(write_buffer, strlen(write_buffer) + 1); // Includes null char
                break;

            // Si le master envoie une requête de lecture qui nous est adressée
            case I2CSlave::WriteAddressed:
                slave.read(read_buffer, 10);
                printf("Read A: %s\n", read_buffer);

                int8_t commande_recue = read_buffer[0];
                if(commande_recue == 127){  // valeur envoyer par master
                    led_i2c(); //clignote la led
                    servo.pulsewidth(0.0075); // valeur pwm //fait tourner le servomoteur

                 }
               
                ///////////////////////////////////////////
                // Modifier l'état du moteur en fonction de la commande reçue
                ///////////////////////////////////////////
                else if(commande_recue == 45 ){
                    servo.pulsewidth(0.002f);
                }
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