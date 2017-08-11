/*iMA6iNEXYZ
//Curso Explorando el universo robótico
Ejemplo Usar sensor ultrasónica
Este ejemplo servirá como ejemplo para la demostración de: 
  % Declaración de variables
  % Sección de configuración en el código
  % Declaración e Implementación de Funciones
  %Uso de pines digitales
  %Comunicación por puerto serial
  %Uso del cuadro de gráficas del IDLE de arduino
  %Uso y conexión de sensores 
*/

// Definición de variables
int trigPin=2; // Pin emisión de la señal
int echoPin=3; // Pin Echo
long duracion, distancia;
//Configuración previa del equipo
// Lo colocado en esta sección, solamente se ejecutará una vez. 

void setup() {

  // Inicialización del puerto serial
  Serial.begin (9600);

  //Definición de los tipos de pines
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

void loop() {
  //Emisión de la señal de Ultrasónico
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2); 
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10); 
  digitalWrite(trigPin, LOW);

  //Adquisión de la señal
  duracion = pulseIn(echoPin, HIGH);
  distancia = (duracion/2) / 29.1;
  if (distancia >= 200 || distancia <= 0){
    //Serial.println("Out of range");
  }
  else {
    Serial.println(distancia);
    //Serial.println(" cm");
  }
  delay(1000);
}




