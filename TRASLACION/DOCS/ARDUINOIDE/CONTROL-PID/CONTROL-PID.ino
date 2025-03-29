// Definición de pines
#define AIN1 18  // Control de dirección del motor
#define AIN2 19  // Control de dirección del motor
#define STBY 21  // Pin de standby para habilitar el driver
#define ENCA 34  // Canal A del encoder
#define ENCB 35  // Canal B del encoder

// Configuración de la señal PWM
const int freq = 1000;  // Frecuencia de PWM a 1kHz
const int pwmChannel = 15; // Canal PWM asignado
const int resolution = 8; // Resolución de 8 bits (0-255)

// Variables del encoder
volatile long encoderCount = 0; // Contador de pulsos del encoder
const float CPR = 11.0 * 210.0; // Pulsos por revolución ajustados para el motor Ocean 210:1
unsigned long lastTime = 0; // Tiempo de la última medición
float lastError = 0.0; // Error previo para la acción derivativa
float setpoint = 10.0; // Velocidad deseada en rad/s
const int maxPWM = 240;  // Límite máximo de PWM

// Constantes PID obtenidas del PID Tuner
const float Kp = 29226.0284;
const float Ki = 0.0;
const float Kd = 12254.6369;

// Variables PID
float error, P_term, I_term, D_term, output;
float velocidadAngular = 0.0;
float voltajeAplicado = 0.0;

// Rutina de interrupción para contar los pulsos del encoder
void IRAM_ATTR encoderISR() {
    if (digitalRead(ENCA) == digitalRead(ENCB)) {
        encoderCount++;
    } else {
        encoderCount--;
    }
}

void setup() {
    Serial.begin(115200);
    
    // Configurar pines como salida o entrada
    pinMode(AIN1, OUTPUT);
    pinMode(AIN2, OUTPUT);
    pinMode(STBY, OUTPUT);
    pinMode(ENCA, INPUT);
    pinMode(ENCB, INPUT);
    
    // Activar el driver del motor
    digitalWrite(STBY, HIGH);
    
    // Configurar PWM en el ESP32
    ledcAttach(pwmChannel, freq, resolution);
    
    // Configurar la interrupción para el encoder
    attachInterrupt(digitalPinToInterrupt(ENCA), encoderISR, CHANGE);
    
    // Imprimir encabezado de los datos en formato CSV
    Serial.println("Voltaje | Velocidad | Error | Output");
}

// Función para controlar la dirección y velocidad del motor
void motorControl(int velocidad, bool direccion) {
    if (velocidad == 0) {
        digitalWrite(AIN1, LOW);
        digitalWrite(AIN2, LOW);
    } else {
        digitalWrite(AIN1, direccion);
        digitalWrite(AIN2, !direccion);
    }
    ledcWrite(pwmChannel, velocidad);
}

// Función para calcular la velocidad angular del motor
void calcularVelocidad() {
    unsigned long currentTime = millis();
    float deltaTime = (currentTime - lastTime) / 1000.0; // Convertir a segundos
    lastTime = currentTime;
    
    // Convertir conteo de pulsos a rad/s
    float rpm = (encoderCount / CPR) * 60.0;
    velocidadAngular = rpm * (2.0 * PI / 60.0);
    encoderCount = 0; // Reiniciar el contador de pulsos
}

// Función para calcular el control PID
void controlPID() {
    // Calcular error
    error = setpoint - velocidadAngular;

    // Calcular términos del PID
    P_term = Kp * error;
    I_term += Ki * error; // Sin acción integral en este caso
    D_term = Kd * (error - lastError);
    lastError = error;

    // Salida del controlador
    output = P_term + I_term + D_term;

    // Convertir salida a PWM
    int velocidadPWM = constrain(output, 0, maxPWM);

    // Controlar el motor con el nuevo valor de PWM
    motorControl(velocidadPWM, true);
    
    // Calcular voltaje aplicado
    voltajeAplicado = (velocidadPWM / 255.0) * 12.0;

    Serial.print(voltajeAplicado, 2); Serial.print(" | ");
    Serial.print(velocidadAngular, 2); Serial.print(" | ");
    Serial.print(error, 2); Serial.print(" | ");
    Serial.println(output, 2);
}

void loop() {
    calcularVelocidad();  // Medir la velocidad angular
    controlPID();  // Aplicar control PID
    delay(100);  // Espera 100 ms antes de la siguiente iteración
}

