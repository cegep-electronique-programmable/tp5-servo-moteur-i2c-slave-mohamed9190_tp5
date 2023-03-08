#include <mbed.h>
// code du slave
#define ADDRESSE_I2C_PAR_DEFAUT 0x24

#if !DEVICE_I2CSLAVE
#error [NOT_SUPPORTED] I2C Slave is not supported
#endif

// Utiliser la classe I2CSlave pour créer un objet slave.
// Consulter la documentation pour voir les méthodes disponibles.
I2CSlave slave(I2C_SDA, I2C_SCL);    // I2C_SCL = d7 nucleo    = PB_8,  I2C_SDA = d8    = PB_9,

///////////////////////////////////////////
// Créer un objet debug_led à partir de la classe DigitalOut pour vous aider dans le debuggage
///////////////////////////////////////////
DigitalOut myled1(LED1); // detecter communication i2c LED1        = PA_5,
DigitalOut myled2(LED2); // detecter communication i2c LED1        = PA_5,
void led_i2c(void){
        myled1 = 1;          // set LED1 pin to high
        printf("myled = %d \n\r", (uint8_t)myled1);
        ThisThread::sleep_for(5ms);

        myled1.write(0);     // set LED1 pin to low
        printf("myled = %d \n\r", myled1.read());
        ThisThread::sleep_for(5ms);
}
///////////////////////////////////////////
// Créer un objet moteur à partir de la classe PwmOut
///////////////////////////////////////////

PwmOut servo(D5); //  dortie pwm ; 2 R de 10Kohms pour les pull up pour communication i2c 
float variable;
///////////////////////////////////////////
// Créer une variable pour la machine à état qui gére le moteur (OFF ou Activé)
///////////////////////////////////////////


int main() {
  char read_buffer[10];
  char write_buffer[10];
    write_buffer[0] = 0x00;
  slave.address(ADDRESSE_I2C_PAR_DEFAUT);
  servo.period(0.02f);      // 20 milli-second period
  servo.write(0.075f);      // 7.5% duty cycle, relative to period
  while (1) {

        // Attendre une requête du master
       // int i = slave.receive(); // modifier pour un char
        int i = slave.receive(); 
        
        myled2 = 1;
        
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
                  
                    variable = (commande_recue *0.002)/100.0;
                if(commande_recue == 127){  // valeur envoyer par master
                    led_i2c(); //clignote la led
                           myled2 = 0;
                     servo.write(variable); 
                 }
               
                ///////////////////////////////////////////
                // Modifier l'état du moteur en fonction de la commande reçue
                ///////////////////////////////////////////
                else if(commande_recue == 45 ){
                    servo.pulsewidth(0.025f);
                     led_i2c(); //clignote la led
                }

                else if(commande_recue == 90 ){
                    servo.write(variable);
                     led_i2c(); //clignote la led
                }

                else if(commande_recue == 180 ){
                    //servo.pulsewidth(0.002f);
                     servo.write(0.028f); 
                     led_i2c(); //clignote la led
                }
                break;
        }
        
        // Vider le buffer de lecture
        for (int i = 0; i < sizeof(read_buffer); i++) {
            read_buffer[i] = 0;
            ThisThread::sleep_for(100ms); // delai entre les comunication
        }
        
        // Vider le buffer d'écriture
        for (int i = 0; i < sizeof(write_buffer); i++) {
            write_buffer[i] = 0;
             ThisThread::sleep_for(100ms);
        }
    }
}